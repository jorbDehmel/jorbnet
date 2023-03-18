/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef IMAGEHANDLING_HPP
#define IMAGEHANDLING_HPP

#include "resources.hpp"
#include <SDL2/SDL.h>
#include <string>
using namespace std;

enum Mode
{
    BW_double,
    BW_char,
    RGBA,
    RGB,
};

dataset loadBMP(const string &Filepath, const int &W, const int &H,
                const vector<double> &Expected, const Mode M = BW_double);
void saveBMP(const string &Filepath, const int &W, const int &H,
             const vector<double> &What, const Mode M);

vector<dataset> addNoise(const dataset &To, const double &Amount, const int &NumOutputs);

#endif
