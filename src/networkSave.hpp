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
#include "network.hpp"
using namespace std;

void saveNetwork(ostream &Stream, Network &From);
void saveNetwork(const string &Filename, Network &From);

Network loadNetwork(istream &Stream);
Network loadNetwork(const string &Filename);

#endif
