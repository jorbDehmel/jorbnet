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
#include "network.hpp"
using namespace std;

void saveNetwork(ostream &Stream, network &From);
void saveNetwork(const string &Filename, network &From);

network loadNetwork(istream &Stream);
network loadNetwork(const string &Filename);

#endif
