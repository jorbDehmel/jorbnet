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
ostream& operator << (ostream &stream, const Matrix<T> &m) {
	for (int i = 1; i <= m.height(); i++) {
		for (int j = 1; j <= m.width(); j++) {
			stream << m.get(i, j) << '\t';
		}
		stream << '\n';
	}
	return stream;
}

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
		cout << h << ' ' << w << '\n';
		throw runtime_error("Invalid indices. Cannot get");
	}
}

template <class T>
Matrix<T> *Matrix<T>::operator * (const Matrix<T> &other) const {
	if (this->width_var != other.height()) {
		throw runtime_error("Dimensional mismatch. Cannot dot product");
	} else {
		cout << height_var << "x" << width_var << " matrix dot " << other.height() << 'x' << other.width() << '\n';
	}

	Matrix<T> *out = new Matrix<T>(this->height_var, other.width());
	T temp;

	// for row in out
	for (int row_num = 1; row_num <= height_var; row_num++) {
		// for col in out
		for (int col_num = 1; col_num <= width_var; col_num++) {
			cout << row_num << ' ' << col_num << '\n';
			
			out->arr[row_num - 1][col_num - 1] = 0;
			for (int i = 1; i <= width_var; i++) {
				out->arr[row_num - 1][col_num - 1] += arr[row_num - 1][i - 1] * other.arr[i - 1][col_num - 1];
			}
		}
	}
	return out;;
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
