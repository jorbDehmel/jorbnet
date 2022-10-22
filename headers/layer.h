#ifndef LAYER_H
#define LAYER_H

#include "array.h"
#include "vector.h"
#include <cmath>

constexpr int WEIGHT_MAX = 100;

// Wx = a
// W: weight matrix
// x: input vector
// a: activation vector

class Layer {
public:
    Layer(const int input_s, const int s);
    ~Layer();

    Matrix<double> *run(const Matrix<double> &input);
protected:
    int input_size;
    Matrix<double> *weights;
    Matrix<double> *activation;

    void sigmoid(ColVector<double> &to_modify);
};

Layer::Layer(const int input_s, const int s) {
    input_size = input_s;

    activation = new Matrix<double>(s, 1);
    weights = new Matrix<double>(s, input_s);

    srand(time(NULL));
    for (int i = 1; i <= s; i++) {
        for (int j = 1; j <= input_s; j++) {
            weights->set(i, j, rand() % WEIGHT_MAX);
        }
    }

    return;
}

Layer::~Layer() {
    delete weights;
    delete activation;
}

Matrix<double> *Layer::run(const Matrix<double> &input) {
    activation = *weights * input;
    return activation;
}

void sigmoid(ColVector<double> &to_modify) {
    double x;
    for (int i = 1; i <= to_modify.height(); i++) {
        x = to_modify.get(i);
        x = 1 / (1 + pow(2.721828, -x));
        to_modify.set(i, x);
    }
    return;
}

#endif
