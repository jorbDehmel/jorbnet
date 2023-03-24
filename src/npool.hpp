/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NPOOL_HPP
#define NPOOL_HPP

#include "networkSave.hpp"
#include "network.hpp"
#include <thread>
using namespace std;

class NPool
{
public:
    NPool(Network &Starter, const int &Num, const int &Cull, const int &CullInterval);
    ~NPool();

    int cull, cullInterval, passes, num;
    double minVar = 1;

    SafeArray<Network *> networks;
    // Network **networks;

    void train(const int &Num, const bool &PrintUpdates = false);
    Network best();
};

#endif
