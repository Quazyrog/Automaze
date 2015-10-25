#ifndef GLMATRIX_HPP
#define GLMATRIX_HPP

#include <GL/gl.h>
#include <cstring>
#include <iostream>
#include <initializer_list>
#include <cmath>
#include <iomanip>

class GLMatrix
{
    friend class RowProxy;
    class RowProxy 
    {
        friend class GLMatrix;
        GLMatrix *parent_;
        int row_;
        
        RowProxy(GLMatrix *parent, int row);
    public:
        GLfloat &operator[](int index);
    };
    
    
    friend class RowProxyConst;
    class RowProxyConst
    {
        friend class GLMatrix;
        const GLMatrix *parent_;
        int row_;
        
        RowProxyConst(const GLMatrix *parent, int row);
    public:
        GLfloat operator[](int index) const;
    };
    
    GLfloat *data_ = nullptr;
    
    static constexpr int index_(int r, int c) {
        return c * 4 + r;
    }
    
public:
    GLMatrix() {
        data_ = new GLfloat[16];
        std::memset(data_, 0, 16 * sizeof(GLfloat));
    }
    
    GLMatrix(const GLMatrix &other) {
        data_ = new GLfloat[16];
        std::memcpy(data_, other.data_, 16 * sizeof(GLfloat));
    }
    
    GLMatrix(GLMatrix &&other) {
        delete [] data_;
        data_ = other.data_;
        other.data_ = nullptr;
    }
    
    GLMatrix(const std::initializer_list<GLfloat> &values);
    
    
    ~GLMatrix() {
        delete [] data_;
    }
    
    
    static GLMatrix Ones();
    static GLMatrix Identity();
    static GLMatrix Zeros();
    static GLMatrix Rotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    static GLMatrix Scale(GLfloat ks, GLfloat ky, GLfloat kz, GLfloat kw = 1);
    static GLMatrix Translation(GLfloat rx, GLfloat ry, GLfloat rz);
    static GLMatrix Projection(GLfloat width, GLfloat height, GLfloat near, GLfloat far);
    
    
    friend std::ostream &operator<<(std::ostream &stream, const GLMatrix &matrix);
    
    
    friend GLMatrix operator*(const GLint &k, const GLMatrix &a) {
        return GLfloat(k) * a;
    }
    
    GLMatrix &operator=(const GLMatrix &other) {
        std::memcpy(data_, other.data_, 16 * sizeof(GLfloat));
        return *this;
    }
    
    GLMatrix &operator=(GLMatrix &&other) {
        std::memcpy(data_, other.data_, 16 * sizeof(GLfloat));
        return *this;
    }
    
    
    friend GLMatrix operator*(const GLfloat &k, const GLMatrix &a);
    GLMatrix operator*(const GLfloat k) const;
    GLMatrix operator*(const GLMatrix &other) const;
    void operator*=(const GLfloat k);
    void operator*=(const GLMatrix &other);
    
    RowProxy operator[](int index);
    RowProxyConst operator[](int index) const;
    
    const GLfloat *data() const {
        return data_;
    }
};

#endif
