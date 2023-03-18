/*
Jordan Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
MIT licence via mit-license.org held by author
*/

#ifndef SOCKSTREAM_HPP
#define SOCKSTREAM_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include <string>
#include <iostream>
#include <set>
#include <map>
#include <chrono>

#include "mutex.hpp"
using namespace std;

// Number of connections to keep in queue
#define BACKLOG 10

// Chunk size for receiving data (bytes)
#define MAXDATASIZE 128

// Message to send upon message receive
const unsigned char PING_MESSAGE = 0xFF;

// Client socket stream
class sockstream
{
public:
    sockstream(string IP, string Port);
    ~sockstream();

    void sendStr(string What);
    string getStr();

protected:
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];
};

// Basic stream definitions
sockstream &operator>>(sockstream &strm, string &what);
sockstream &operator>>(sockstream &strm, int &what);
sockstream &operator>>(sockstream &strm, double &what);

// Server socket stream
class socketstreamserver
{
public:
    socketstreamserver(string Port);
    ~socketstreamserver();

    void sendStr(string what);
    map<int, string> getStr(const unsigned int Bytes);
    string getStr(const int FD, const unsigned int Bytes);

    void scan();

    bool running = true;

protected:
    string port;

    int sockfd, new_fd;
    struct addrinfo hints, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes = 1;

    Mutex<set<int>> fds;
    ArgThreadPool<void (*)(socketstreamserver *), socketstreamserver *> threads;
};

// Dummy function for threadPool
void _call_sockstream_scan(socketstreamserver *what);

// Basic stream definitions
socketstreamserver &operator<<(socketstreamserver &strm, string &what);
socketstreamserver &operator<<(socketstreamserver &strm, char *&what);
socketstreamserver &operator<<(socketstreamserver &strm, char what);
socketstreamserver &operator<<(socketstreamserver &strm, int &what);
socketstreamserver &operator<<(socketstreamserver &strm, double &what);

// Universal binary stream operator
template <class T>
socketstreamserver &operator<<(socketstreamserver &strm, T &what)
{
    strm.sendStr(to_string(what));
    return strm;
}

// Sockstream to sockstream communication
sockstream &operator>>(sockstream &istrm, socketstreamserver &ostrm);
socketstreamserver &operator<<(socketstreamserver &ostrm, sockstream &istrm);

#endif
