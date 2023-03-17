/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NPOOL_HPP
#define NPOOL_HPP

#include "resources.hpp"
#include "networkSave.hpp"
#include <thread>
using namespace std;

class npool
{
public:
    npool(network &Starter, const int &Num, const int &Cull, const int &CullInterval);
    ~npool();

    int cull, cullInterval, passes, num;
    double minVar = 1;
    network **networks;

    void train(const int &Num, const bool &PrintUpdates = false);
    network best();
};

#endif
