#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
using namespace std;

template <class T>
class Matrix {
public:
	Matrix<T>(int h, int w);
	~Matrix<T>();

	void print(ostream& out) const;

	int height() const;
	int width() const;
	inline bool is_square() const { return height_var == width_var; }

	void set(const int h, const int w, T data);
	T get(const int h, const int w) const;

	Matrix<T> *operator * (const Matrix<T> &other) const;
	Matrix<T> *operator * (const T t) const;
	Matrix<T> *operator + (const T t) const;
	Matrix<T> *operator - (const T t) const;

	Matrix<T> *operator *= (const T t) const;
	Matrix<T> *operator += (const T t) const;
	Matrix<T> *operator -= (const T t) const;

	bool operator == (const Matrix<T> &other) const;
	bool operator != (const Matrix<T> &other) const;

	int height_var, width_var;
	T** arr;
};

template <class T>
Matrix<T>::Matrix(int h, int w) {
	height_var = h;
	width_var = w;

	arr = new T*[h];
	for (int i = 0; i < h; i++) {
		arr[i] = new T[w];
	}

	return;
}

template <class T>
Matrix<T>::~Matrix() {
	for (int i = 0; i < height_var; i++) {
		delete [] arr[i];
	}
	delete [] arr;

	return;
}

template <class T>
void Matrix<T>::print(ostream& out) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out << get(r, c) << '\t';
		}
		out << '\n';
	}
	return;
}

template <class T>
int Matrix<T>::height() const {
	return height_var;
}

template <class T>
int Matrix<T>::width() const {
	return width_var;
}

template <class T>
void Matrix<T>::set(const int h, const int w, T data) {
	if (h <= height_var && h > 0 && w <= width_var && w > 0) {
		arr[h - 1][w - 1] = data;
	} else {
		throw runtime_error("Invalid indices. Cannot set");
	}
	return;
}

template <class T>
T Matrix<T>::get(const int h, const int w) const {
	if (h <= height_var && h > 0 && w <= width_var && w > 0) {
		return arr[h - 1][w - 1];
	} else {
		throw runtime_error("Invalid indices. Cannot get");
	}
}

template <class T>
Matrix<T> *Matrix<T>::operator * (const Matrix<T> &other) const {
	if (this->width_var != other.height()) {
		throw runtime_error("Dimensional mismatch. Cannot dot product");
	}

	Matrix<T> *out = new Matrix<T>(this->height_var, other.width());
	T temp;

	// cols in other
	for (int c = 1; c <= this->width_var; c++) {
		// rows in this
		for (int r = 1; r <= this->height_var; r++) {
			
			// dot product of the two
			temp = this->get(r, 1) * other.get(1, c);
			for (int i = 2; i <= this->width_var; i++) {
				temp += this->get(r, i) * other.get(i, c);
			}

			out->set(r, c, temp);
		}
	}

	return out;
}

template <class T>
Matrix<T> *Matrix<T>::operator * (const T t) const {
	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out->set(r, c, get(r, c) * t);
		}
	}
	return out;
}

template <class T>
Matrix<T> *Matrix<T>::operator + (const T t) const {
	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out->set(r, c, get(r, c) + t);
		}
	}
	return out;
}

template <class T>
Matrix<T> *Matrix<T>::operator - (const T t) const {
	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out->set(r, c, get(r, c) - t);
		}
	}
	return out;
}

template <class T>
Matrix<T> *Matrix<T>::operator *= (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] *= t;
		}
	}
	return this;
}

template <class T>
Matrix<T> *Matrix<T>::operator += (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] += t;
		}
	}
	return this;
}

template <class T>
Matrix<T> *Matrix<T>::operator -= (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] -= t;
		}
	}
	return this;
}

template <class T>
bool Matrix<T>::operator == (const Matrix<T> &other) const {
	if (height_var != other.height_var || width_var != other.width_var) {
		return false;
	}

	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			if (get(r, c) != other.get(r, c)) {
				return false;
			}
		}
	}
	return true;
}

template <class T>
bool Matrix<T>::operator != (const Matrix<T> &other) const {
	return !(*this == other);
}

#endif
