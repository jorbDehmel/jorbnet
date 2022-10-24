/*
#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"

template <class T>
class ColVector : public Matrix<T> {
public:
    ColVector(const int h): Matrix(h, 1) {}

    void set(const int h, T data);
	T get(const int h) const;
};

template <class T>
void ColVector<T>::set(const int h, T data) {
    this->arr[h][0] = data;
}

template <class T>
T ColVector<T>::get(const int h) const {
    return this->arr[h][0];
}

template <class T>
class RowVector : public Matrix<T> {
public:
    RowVector(const int w): Matrix(1, w) {}

    void set(const int w, T data);
	T get(const int w) const;
};

template <class T>
void RowVector<T>::set(const int w, T data) {
    this->arr[0][w] = data;
}

template <class T>
T RowVector<T>::get(const int w) const {
    return this->arr[0][w];
}

#endif
*/