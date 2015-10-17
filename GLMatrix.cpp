#include "GLMatrix.hpp"

GLMatrix::GLMatrix(const std::initializer_list<GLfloat> &values)
{
    data_ = new GLfloat [16];
    auto iterator = values.begin();
    for (int i = 0; i < 16; ++i)
        data_[i] = *iterator++;
}


std::ostream &operator<<(std::ostream &stream, const GLMatrix &matrix)
{
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) 
            stream << std::setw(8) << std::setprecision(4) << std::fixed << matrix.data_[GLMatrix::index_(r, c)] << "  ";
        stream << "\n";
    }
    return stream;
}


GLMatrix operator*(const GLfloat &k, const GLMatrix &a)
{
    GLMatrix result(a);
    for (int i = 0; i < 16; ++i)
        result.data_[i] *= k;
    return result;
}


GLMatrix GLMatrix::operator*(const GLfloat k) const
{
    GLMatrix result(*this);
    for (int i = 0; i < 16; ++i)
        result.data_[i] *= k;
    return result;
}


GLMatrix GLMatrix::operator*(const GLMatrix& other) const
{
    GLMatrix result;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            result.data_[index_(r, c)] = data_[index_(r, 0)] * other.data_[index_(0, c)]
            + data_[index_(r, 1)] * other.data_[index_(1, c)]
            + data_[index_(r, 2)] * other.data_[index_(2, c)]
            + data_[index_(r, 3)] * other.data_[index_(3, c)];
        }
    }
    return result;
}


void GLMatrix::operator*=(const GLfloat k)
{
    for (int i = 0; i < 16; ++i)
        data_[i] *= k;
}


void GLMatrix::operator*=(const GLMatrix& other)
{
    GLMatrix tmp = other * (*this);
    data_ = tmp.data_;
    tmp.data_ = nullptr;
}


GLMatrix GLMatrix::Ones()
{
    GLMatrix result;
    for (int i = 0; i < 16; ++i)
        result.data_[i] = 1;
    return result;
}


GLMatrix GLMatrix::Identity()
{
    GLMatrix result;
    for (int i = 0; i < 4; ++i)
        result.data_[index_(i, i)] = 1;
    return result;
}


GLMatrix GLMatrix::Scale(GLfloat kx, GLfloat ky, GLfloat kz, GLfloat kw)
{
    GLMatrix result;
    result.data_[0] = kx;
    result.data_[5] = ky;
    result.data_[10] = kz;
    result.data_[15] = kw;
    return result;
}


GLMatrix GLMatrix::Zeros()
{
    return GLMatrix();
}


GLMatrix GLMatrix::Translation(GLfloat rx, GLfloat ry, GLfloat rz)
{
    GLMatrix result = Identity();
    result.data_[12] = rx;
    result.data_[13] = ry;
    result.data_[14] = rz;
    return result;
}


GLMatrix GLMatrix::Rotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    const GLfloat s = std::sin(angle);
    const GLfloat c = std::cos(angle);
    const GLfloat cp = 1.0 - c;
    return GLMatrix({
        c + x * x * cp,      x * y * cp  - z * s,  x * z * cp + y * s,  0,
        y * x * cp + z * s,  c + y * y * cp,       y * z * cp - x * s,  0,
        x * z * cp - y * s,  z * y * cp + x * s,   c + z * z * cp,      0,
        0,                   0,                    0,                   1
    });
}


GLMatrix GLMatrix::Projection(GLfloat width, GLfloat height, GLfloat near, GLfloat far)
{
    GLMatrix projection = Zeros();
    projection.data_[index_(0, 0)] = 2 * near / width;
    projection.data_[index_(1, 1)] = 2 * near / height;
    projection.data_[index_(2, 2)] = (far + near) / (near - far);
    projection.data_[index_(2, 3)] = 2 * far * near / (near - far);
    projection.data_[index_(3, 2)] = -1;
    return projection;
}



GLMatrix::RowProxy GLMatrix::operator[](int index)
{
    return RowProxy(this, index);
}


GLMatrix::RowProxyConst GLMatrix::operator[](int index) const
{
    return RowProxyConst(this, index);
}


GLMatrix::RowProxy::RowProxy(GLMatrix* parent, int row)
{
    parent_ = parent;
    row_ = row;
}


GLfloat& GLMatrix::RowProxy::operator[](int index)
{
    return parent_->data_[GLMatrix::index_(row_, index)];
}


GLMatrix::RowProxyConst::RowProxyConst(const GLMatrix* parent, int row)
{
    parent_ = parent;
    row_ = row;
}


GLfloat GLMatrix::RowProxyConst::operator[](int index) const
{
    return parent_->data_[GLMatrix::index_(row_, index)];
}
