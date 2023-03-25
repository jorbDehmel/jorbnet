# JorbNet
Jordan "Jorb" Dehmel, 2023
github.com/jorbDehmel/jorbnet


## Description

JorbNet is a simple machine learning library for
C++.

## Installation

From this folder, call the command `make install`.
This library relies on SDL2 and clang, which must be
installed beforehand. Instructions on how to install
these based on your Linux distro are below. If you
are using Windows, install WSL/WSL2 with any of the
below distros.

Ubuntu / apt:
`sudo apt-get install libsdl2-dev clang`

Arch / pacman:
`sudo pacman -S sdl2 clang`

Fedora / yum:
`sudo yum install SDL2-devel clang`

After installation, you may delete this folder,
although this may make updating and uninstalling
jorbnet more difficult.

## Updates

To update jorbnet, call the following command
within this folder.

`git pull && make install`

If you no longer have this folder, call the following.

`git clone github.com/jorbDehmel/jorbnet && make -C jorbnet && sudo rm -rf jorbnet`

This will clone the source repo, install jorbnet, and
delete the source code.

## Uninstallation

Simply call `make uninstall` from this folder, or call
the following command if you no longer have the
Makefile.

`sudo rm -rf /usr/include/jorbnet /usr/bin/jorbnet-flags`

Neither method will remove SDL2 or clang.

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

## Activation Functions

Activation functions are used in network propogation.
Jorbnet includes several, namely the Sigmoid
(as __sigmoid and __sigder) and ReLU (as __ReLU and 
__ReLUder) functions. To change an initialized network's
activation function, set the following member variables.
Note that you must change both for it to work
properly.

> Network n({1, 2, 3, 4});
> n.act = __sigmoid;   // Or __ReLU
> n.actder = __sigder; // Or __ReLUder

By default, networks use the Sigmoid activation
function because it's my favorite.

## More Information

For more information, as well as a mediocre derivation,
see the accompianing file `docs/writeup.pdf` (made via
`make docs`), or the sources listed below.

## Sources

https://en.wikipedia.org/wiki/Backpropagation
http://neuralnetworksanddeeplearning.com/chap2.html

## License

This library is protected by the GPLv3.
