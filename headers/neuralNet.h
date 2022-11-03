#ifndef NN_H
#define NN_H

#include "layer.h"

/////////////////////////////////////////////////////////

void sigmoid(Matrix<double> *to_modify) {
    double x;
    for (int i = 1; i <= to_modify->height(); i++) {
        x = to_modify->get(i, 1);
        x = 1 / (1 + pow(2.721828, -x));
        to_modify->set(i, 1, x);
    }
    return;
}

void sigmoidDer(Matrix<double> *to_modify) {
    double x;
    for (int i = 1; i <= to_modify->height(); i++) {
        x = to_modify->get(i, 1);
        x = sigmoidDer(x);
        to_modify->set(i, 1, x);
    }
    return;
}

inline double sigmoid(double x) {
    return 1 / (1 + pow(2.71828, -x));
}

inline double sigmoidDer(double x) {
    x = sigmoid(x);
    return x * (1 - x);
}

/////////////////////////////////////////////////////////

// Multi Layer Perceptron artificial neural network
class MLP {
public:
    MLP(int layers, int *dims);
    ~MLP();

    void run(Matrix<double> &input);
    Matrix<double> *getActivation() const;
    void train(Matrix<double> &correct);
private:
    int layers;
    int *dimensions;
    Layer **network;
};

MLP::MLP(int l, int *dims) {
    if (l <= 0) {
        throw nn_error("Invalid dimensions for a network");
    }

    layers = l;
    dimensions = dims;
    network = new Layer*[layers - 1];
    for (int d = 0; d < layers; d++) {
        if (dimensions[d] <= 0) {
            throw nn_error("Invalid dimensions for a network");
        }

        network[d] = new Layer(dimensions[d], dimensions[d + 1]);
    }
    return;
}

MLP::~MLP() {
    for (int d = 0; d < layers; d++) {
        delete network[d];
    }
    return;
}

/////////////////////////////////////////////////////////

void MLP::run(Matrix<double> &input) {
    network[0]->run(input);
    for (int i = 1; i < layers; i++) {
        network[i]->run(*(network[i - 1]->getActivation()));
    }
    sigmoid(network[layers - 1]->getActivation());
    return;
}

Matrix<double> *MLP::getActivation() const {
    return network[layers - 1]->getActivation();
}

void MLP::train(Matrix<double> &correct) {
    Matrix<double> deriv = *(correct - *getActivation()); // equals error rn

    // Iterate over layers
    for (int layer_num = layers - 1; layer_num >= 0; layer_num--) {
        
        // define deriv
        
        *network[layer_num]->weights -= deriv;
    }

    // d for l (weight of b in prev layer)
    // derror(prev_layer_run(a, b, c)*i + prev_layer_run(a, b, c)*j + prev_layer_run(a, b, c)*k) *
    // derror(previous activation) * dprevious_activation
}

/////////////////////////////////////////////////////////

#endif
