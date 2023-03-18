# JorbNet
Jordan "Jorb" Dehmel, 2023
github.com/jorbDehmel/jorbnet


## Description

JorbNet is a simple machine learning library for
C++.

## Classes

### Network

This is the base type of machine learning model. It
is a Multi-Layered Perceptron (MLP) model which uses
traditional backpropogation training.

### Network Pool (npool)

This is a faster, but less efficient, way of training
networks. It creates a pool of networks which are all
trained at the same time using multithreading. These
networks are periodically "pruned", leaving only the
best. The best network is then cloned to repopulate
the pool. It uses regular backpropogation training in
addition to evolutionary pruning.

## License

This library is protected by the GPLv3.
