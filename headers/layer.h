#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"
#include "vector.h"
#include <cmath>

constexpr int WEIGHT_RANGE = 50;

// Wx = a
// W: weight matrix
// x: input vector
// a: activation vector

class Layer {
public:
    Layer(const int input_s, const int s);
    ~Layer();

    Matrix<double> *run(const Matrix<double> &input);
    inline Matrix<double> getActivation() { return *activation; }
protected:
    int input_size;

    Matrix<double> *weights;
    Matrix<double> *activation;

    void sigmoid(Matrix<double> &to_modify);
};

// Initialize dynamically allocated stuff, randomize weights
Layer::Layer(const int input_s, const int s) {
    input_size = input_s;

    activation = new Matrix<double>(s, 1);
    weights = new Matrix<double>(s, input_s);

    srand(time(NULL));
    for (int i = 1; i <= s; i++) {
        for (int j = 1; j <= input_s; j++) {
            weights->set(i, j, (rand() % (2 * WEIGHT_RANGE)) - WEIGHT_RANGE);
        }
    }

    return;
}

// Free memory obvs
Layer::~Layer() {
    delete weights;
    delete activation;
}

// Update the activation given input vector
Matrix<double> *Layer::run(const Matrix<double> &input) {
    activation = *weights * input;
    return activation;
}

//x = 1 / (1 + e^-x)
void sigmoid(Matrix<double> &to_modify) {
    double x;
    for (int i = 1; i <= to_modify.height(); i++) {
        x = to_modify.get(i, 1);
        x = 1 / (1 + pow(2.721828, -x));
        to_modify.set(i, 1, x);
    }
    return;
}

#endif
