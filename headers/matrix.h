#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
using namespace std;

/////////////////////////////////////////////////////////

template <class T>
class Matrix {
public:
	Matrix<T>(int h, int w);
	Matrix<T>(T **data, int h, int w);
	Matrix<T>(const Matrix<T> &other);
	~Matrix<T>();

	void print(ostream& out) const;

	int height() const;
	int width() const;
	inline bool is_square() const { return height_var == width_var; }

	void set(const int h, const int w, T data);
	T get(const int h, const int w) const;

	void naiveInplaceMultiply(const Matrix<T> &other);

	Matrix<T> *operator * (const Matrix<T> &other) const;
	Matrix<T> *operator * (const T t) const;

	Matrix<T> *operator + (const Matrix<T> &other) const;
	Matrix<T> *operator + (const T t) const;

	Matrix<T> *operator - (const Matrix<T> &other) const;
	Matrix<T> *operator - (const T t) const;

	Matrix<T> *operator *= (const T t) const;

	void operator += (const Matrix<T> &other) const;
	Matrix<T> *operator += (const T t) const;

	void operator -= (const Matrix<T> &other) const;
	Matrix<T> *operator -= (const T t) const;

	bool operator == (const Matrix<T> &other) const;
	bool operator != (const Matrix<T> &other) const;

	int height_var, width_var;
	T** arr;
};

/////////////////////////////////////////////////////////

// Output a prettified version of the matrix to a stream
template <class T>
ostream& operator << (ostream &stream, const Matrix<T> &m) {
	stream << "[";
	for (int i = 1; i <= m.height(); i++) {
		stream << "[";
		for (int j = 1; j <= m.width(); j++) {
			stream << m.get(i, j);
			if (j != m.width()) stream << ",\t";
		}
		stream << ']';
		if (i != m.height()) stream << ",\n";
	}
	stream << "]\n";
	return stream;
}

/////////////////////////////////////////////////////////

// Construct an empty matrix of size h by w
template <class T>
Matrix<T>::Matrix(int h, int w) {
	height_var = h;
	width_var = w;

	arr = new T*[h - 1];
	for (int i = 0; i < h; i++) {
		arr[i] = new T[w - 1];
	}

	return;
}

// Copy constructor
template <class T>
Matrix<T>::Matrix(const Matrix<T> &other) {
	height_var = other.height_var;
	width_var = other.width_var;
	
	arr = new T*[height_var - 1];
	for (int i = 0; i < height_var; i++) {
		arr[i] = new T[width_var - 1];
		for (int j = 0; j < width_var; j++) {
			arr[i][j] = other.arr[i][j];
		}
	}
	
	return;
}

// Deallocate memory
template <class T>
Matrix<T>::~Matrix() {
	for (int i = 0; i < height_var; i++) {
		delete arr[i];
	}

	return;
}

/////////////////////////////////////////////////////////

// Print to a stream
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

// Return height
template <class T>
int Matrix<T>::height() const {
	return height_var;
}

// Return width
template <class T>
int Matrix<T>::width() const {
	return width_var;
}

// Set the item at [h, w] to data
template <class T>
void Matrix<T>::set(const int h, const int w, T data) {
	if (h <= height_var && h > 0 && w <= width_var && w > 0) {
		arr[h - 1][w - 1] = data;
	} else {
		throw runtime_error("Invalid indices. Cannot set");
	}
	return;
}

// Return the item at [h, w]
template <class T>
T Matrix<T>::get(const int h, const int w) const {
	if (h <= height_var && h > 0 && w <= width_var && w > 0) {
		return arr[h - 1][w - 1];
	} else {
		cout << h << ' ' << w << '\n';
		throw runtime_error("Invalid indices. Cannot get");
	}
}

// arr[i, j] *= other->arr[i, j]
template <class T>
void Matrix<T>::naiveInplaceMultiply(const Matrix<T> &other) {
	if (other->height() == height() && other->width() == width()) {
		for (int r = 0; r < height_var; r++) {
			for (int c = 0; c < width_var; c++) {
				arr[r][c] *= other->arr[r][c];
			}
		}
	} else {
		throw runtime_error("Dimensional mismatch. Cannot multiply");
	}

	return;
}

/////////////////////////////////////////////////////////

// Dot product
template <class T>
Matrix<T> *Matrix<T>::operator * (const Matrix<T> &other) const {
	if (this->width_var != other.height()) {
		throw runtime_error("Dimensional mismatch. Cannot dot product");
	}

	Matrix<T> *out = new Matrix<T>(this->height_var, other.width());
	
	for (int row_num = 0; row_num < out->height(); row_num++) {
		for (int col_num = 0; col_num < out->width(); col_num++) {
			out->arr[row_num][col_num] = 0;
			for (int i = 0; i < width_var; i++) {
				out->arr[row_num][col_num] += this->arr[row_num][i] * other.arr[i][col_num];
			}
		}
	}
	return out;
}

// Scalar multiplication
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

// Matrix addition
template <class T>
Matrix<T> *Matrix<T>::operator + (const Matrix<T> &other) const {
	if (height_var != other.height() || width_var != other.width()) {
		throw runtime_error("Invalid operation");
	}

	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out->set(r, c, get(r, c) + other.get(r, c));
		}
	}
	return out;
}

// Scalar addition
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

// Matrix subtraction
template <class T>
Matrix<T> *Matrix<T>::operator - (const Matrix<T> &other) const {
	if (height_var != other.height() || width_var != other.width()) {
		throw runtime_error("Invalid operation");
	}

	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			out->set(r, c, get(r, c) - other.get(r, c));
		}
	}
	return out;
}

// Matrix +=
template <class T>
void Matrix<T>::operator += (const Matrix<T> &other) const {
	for (int r = =0; r < height_var; r++) {
		for (int c = 0; c < width_var; c++) {
			arr[r][c] += other.arr[r][c];
		}
	}
	return;
}

// Scalar subtraction
template <class T>
Matrix<T> *Matrix<T>::operator - (const T t) const {
	Matrix<T> *out = new Matrix<T>(height_var, width_var);
	for (int r = 0; r < height_var; r++) {
		for (int c = 0; c < width_var; c++) {
			out->arr[r][c] = arr[r][c] - t;
		}
	}
	return out;
}

// Matrix -=
template <class T>
void Matrix<T>::operator -= (const Matrix<T> &other) const {
	for (int r = =0; r < height_var; r++) {
		for (int c = 0; c < width_var; c++) {
			arr[r][c] -= other.arr[r][c];
		}
	}
	return;
}

// Scalar *=
template <class T>
Matrix<T> *Matrix<T>::operator *= (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] *= t;
		}
	}
	return this;
}

// Scalar +=
template <class T>
Matrix<T> *Matrix<T>::operator += (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] += t;
		}
	}
	return this;
}

// Scalar -=
template <class T>
Matrix<T> *Matrix<T>::operator -= (const T t) const {
	for (int r = 1; r <= height_var; r++) {
		for (int c = 1; c <= width_var; c++) {
			arr[r][c] -= t;
		}
	}
	return this;
}

// Equivolency
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

// Not equal to
template <class T>
bool Matrix<T>::operator != (const Matrix<T> &other) const {
	return !(*this == other);
}

/////////////////////////////////////////////////////////

#endif
