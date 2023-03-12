#ifndef NETWORKSAVE_HPP
#define NETWORKSAVE_HPP

#include <string>
#include <fstream>
#include "resources.hpp"
using namespace std;

void saveNetwork(const string &Filename, network &From);
network loadNetwork(const string &Filename);

#endif
