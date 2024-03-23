#ifndef __LIST_H__
#define __LIST_H__

#include <assert.h>
#include <stdlib.h>

namespace CG {

template <class type>
class List {
public:
    typedef int cmp_t(const type *, const type *);

    List(int newgranularity = 16);
    ~List<type>();

    void			Clear();
    int				Num() const;
        
    const type		&operator[](int index) const;
    type			&operator[](int index);

    List<type>      &operator=(const List<type> &other);

    int				Append(const type &obj);
    void			Resize(int newsize);

    int				FindNull() const;

    void            Sort(cmp_t *compare);
private:
    int				num;
    int				size;
    int				granularity;
    type			*list;
};

template<class type>
inline List<type>::List(int newgranularity) {
    assert(newgranularity > 0);
    list = NULL;
    granularity = newgranularity;
    Clear();
}

template<class type>
inline List<type>::~List() {
    Clear();
}

template<class type>
inline void List<type>::Clear() {
    if (list) {
        delete[] list;
    }

    list = NULL;
    num = 0;
    size = 0;
}

template<class type>
inline int List<type>::Num() const {
    return num;
}

template<class type>
inline const type &List<type>::operator[](int index) const {
    assert(index >= 0);
    assert(index < num);

    return list[index];
}

template<class type>
inline type &List<type>::operator[](int index) {
    assert(index >= 0);
    assert(index < num);

    return list[index];
}

template <class type> 
inline List<type> &List<type>::operator=(const List<type> &other) {
    Clear();

    num = other.num;
    size = other.size;
    granularity = other.granularity;

    if (size > 0) {
        list = new type[size];

        for (int i = 0; i < num; i++) {
            list[i] = other.list[i];
        }
    }

    return *this;
}

template<class type>
inline int List<type>::Append(const type &obj) {
    if (!list) {
        Resize(granularity);
    }

    if (size == num) {
        if (granularity == 0) {
            granularity = 16;
        }

        int newsize = size + granularity;
        Resize(newsize - newsize % granularity);
    }

    list[num] = obj;
    num++;

    return num - 1;
}

template<class type>
inline void List<type>::Resize(int newsize) {
    assert(newsize >= 0);

    if (newsize == size) {
        return;
    }

    type *temp = list;
    size = newsize;

    if (size < num) {
        num = size;
    }

    list = new type[size];
    for (int i = 0; i < num; i++) {
        list[i] = temp[i];
    }

    if (temp) {
        delete[] temp;
    }
}

template<class type>
inline int List<type>::FindNull() const {
    for (int i = 0; i < size; i++) {
        if (list[i] == NULL) {
            return i;
        }
    }

    return -1;
}

template <class type>
inline void List<type>::Sort(cmp_t *compare) {
    if (!list) {
        return;
    }

    typedef int cmp_c(const void *, const void *);
    cmp_c *scompare = (cmp_c *)compare;
    qsort(list, num, sizeof(type), scompare);
}


}

#endif