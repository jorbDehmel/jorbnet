/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NETWORKSAVE_HPP
#define NETWORKSAVE_HPP

#include <string>
#include <fstream>
#include "resources.hpp"
using namespace std;

void saveNetwork(const string &Filename, network &From);
network loadNetwork(const string &Filename);

#endif
