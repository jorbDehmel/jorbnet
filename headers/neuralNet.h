#ifndef NN_H
#define NN_H

#include "layer.h"

// Multi Layer Perceptron, not my little pony
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
        throw runtime_error("Invalid dimensions for a network");
    }

    layers = l;
    dimensions = dims;
    network = new Layer*[layers - 1];
    for (int d = 0; d < layers; d++) {
        if (dimensions[d] <= 0) {
            throw runtime_error("Invalid dimensions for a network");
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

}

#endif
