/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

// IN PROGRESS

#include "ntransfer.hpp"

nparent::nparent()
{
}

nparent::~nparent()
{
}

void nparent::sendCommand(commandMessage &ToSend)
{
    stringstream temp;
    temp << ToSend.todo << '\t' << ToSend.num << '\t';
    saveNetwork(temp, *ToSend.net);
    server.sendStr(temp.str());
    return;
}

void nparent::sendCommand(network *Net, command Todo, double Num)
{
    stringstream temp;
    temp << Todo << '\t' << Num << '\t';
    saveNetwork(temp, *Net);
    server.sendStr(temp.str());
    return;
}

reply nparent::popReceived()
{
}

bool nparent::hasMessage()
{
}

nchild::nchild()
{
}

nchild::~nchild()
{
}

void nchild::sendReply(network *ToSend)
{
    stringstream temp;
    saveNetwork(temp, *ToSend);
    sock.sendStr(temp.str());
    return;
}

command nchild::popCommand()
{
    if (commands.is_empty())
    {
        throw runtime_error("Cannot pop from an empty queue");
    }
    return commands.pop();
}

bool nchild::hasCommand()
{
    return !commands.is_empty();
}
