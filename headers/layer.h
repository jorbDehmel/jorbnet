#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"
#include "vector.h"
#include <cmath>

constexpr int WEIGHT_RANGE = 50;

class Layer {
public:
    Layer(const int input_s, const int s);
    
    Matrix<double> *run(const Matrix<double> &input);
    inline Matrix<double> *getActivation() { return activation; }
    inline Matrix<double> *getWeights() const { return weights; }
protected:
    int input_size;

    Matrix<double> *weights;
    Matrix<double> *activation;

    void biasedDotProduct(const Matrix<double> &input);
};

// Initialize dynamically allocated stuff, randomize weights
Layer::Layer(const int input_s, const int s) {
    input_size = input_s;

    activation = new Matrix<double>(s, 1);
    weights = new Matrix<double>(s, input_s);

    for (int i = 1; i <= s; i++) {
        for (int j = 1; j <= input_s; j++) {
            weights->set(i, j, (rand() % (2 * WEIGHT_RANGE)) - WEIGHT_RANGE);
        }
    }

    return;
}

// Update the activation given input vector
Matrix<double> *Layer::run(const Matrix<double> &input) {
    if (input.height() < weights->width_var) {
        biasedDotProduct(input);
    } else {
        activation = *weights * input;
    }
    return activation;
}

void Layer::biasedDotProduct(const Matrix<double> &input) {
    if (weights->width_var != (input.height() + 1)) {
		throw runtime_error("Dimensional mismatch. Cannot dot product");
	}
	
	for (int row_num = 0; row_num < activation->height(); row_num++) {
		activation->arr[row_num][0] = 0;
        for (int i = 0; i < weights->width_var - 1; i++) {
            activation->arr[row_num][0] += weights->arr[row_num][i] * input.arr[i][0];
        }

        // Bias
        activation->arr[row_num][0] += weights->arr[row_num][weights->width_var - 1];
	}
    
}

#endif
