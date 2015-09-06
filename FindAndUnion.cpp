#include "FindAndUnion.hpp"


unsigned int FindAndUnion::newSet()
{
    Node new_set;
    new_set.parent = data_.size();
    new_set.rank = 0;
    data_.push_back(new_set);
    return new_set.parent;
}


void FindAndUnion::merge(unsigned int a, unsigned int b)
{
    a = representative(a);
    b = representative(b);
    if (data_[a].rank < data_[b].rank) {
        data_[a].parent = b;
    } else if (data_[a].rank > data_[b].rank) {
        data_[b].parent = a;
    } else {
        data_[a].parent = b;
        data_[b].rank++;
    }
}


unsigned int FindAndUnion::representative(unsigned int x) const
{
    if (x >= data_.size())
        throw IndexError();
    if (data_[x].parent == x)
        return x;
    return data_[x].parent = representative(data_[x].parent);
}

