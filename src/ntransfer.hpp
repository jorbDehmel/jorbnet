/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NTRANSFER_HPP
#define NTRANSFER_HPP

#include "misc/sockstream.hpp"
#include "network.hpp"
#include "networkSave.hpp"
#include "resources.hpp"
#include "npool.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <queue>
using namespace std;

/*
This file defines protocols for multi-computer network training
*/

struct reply
{
    network *net;
    int from;
};

enum command
{
    train,
    poolTrain,
    fuzzData,
};

struct commandMessage
{
    network *net;
    command todo;
    double num;
};

// Hosts the operations of the networks
class nparent
{
public:
    nparent();
    ~nparent();

    void sendCommand(commandMessage &ToSend);
    void sendCommand(network *Net, command Todo, double Num);
    reply popReceived();
    bool hasMessage();

protected:
    queue<reply> received;
    socketstreamserver server;
};

// Takes commands from a nparent unit
class nchild
{
public:
    nchild();
    ~nchild();

    void sendReply(network *ToSend);
    command popCommand();
    bool hasCommand();

protected:
    queue<commandMessage> commands;
    sockstream sock;
};

#endif
