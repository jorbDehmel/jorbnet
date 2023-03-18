/*
Jordan Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
MIT licence via mit-license.org held by author
*/

#include "sockstream.hpp"

// Set n bytes of s to c
void *memset(void *s, const unsigned char c, const unsigned int n)
{
    for (int pos = 0; pos < n; pos++)
    {
        ((unsigned char *)s)[pos] = c;
    }
    return s;
}

// Internals for OSockStream
///////////////////////////////////////////

// Dummy function to use in threadPools
void _call_sockstream_scan(socketstreamserver *what)
{
    while (what->running)
    {
        what->scan();
    }

    return;
}

// OSockStream
///////////////////////////////////////////

socketstreamserver::socketstreamserver(string Port) : fds(set<int>()), threads(1, _call_sockstream_scan, this)
{
    port = Port;

    // Clear hints
    memset(&hints, 0, sizeof(hints));

    // Set hints
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Translate to actual address data
    struct addrinfo *servinfo;
    if (getaddrinfo(NULL, port.c_str(), &hints, &servinfo) != 0)
    {
        throw runtime_error("Failure in getaddrinfo");
    }

    // Iterate through linked list
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // Failed to assign socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            cerr << "Failed to connect.\n";
            continue;
        }

        // Set socket information
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            throw runtime_error("Failed to set socket options");
        }

        // Bind object to socket
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);

            cerr << "Failed to bind socket.\n";

            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        throw runtime_error("Failed to bind to socket.");
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        throw runtime_error("Failed to prepare socket for connection.");
    }

    return;
}

socketstreamserver::~socketstreamserver()
{
    running = false;

    sendStr("\0");

    threads.joinAll();
}

void socketstreamserver::scan()
{
    // Wait until a connection arrives
    sin_size = sizeof(their_addr);

    // Accept connection
    new_fd = accept(sockfd, (sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1)
    {
        return;
    }

    // Add new_fd to list of fd's
    {
        set<int> *fdsSet;
        while (!fds.lock(fdsSet))
        {
            // Hang until mutex is secured
        }
        fdsSet->insert(new_fd);
        fds.unlock(fdsSet);
    }

    return;
}

void socketstreamserver::sendStr(string what)
{
    // Get mutex lock
    set<int> copiedSet;

    {
        set<int> *s;
        while (!fds.lock(s))
        {
            // Hang until mutex is secured
        }

        for (auto i : *s)
        {
            copiedSet.insert(i);
        }

        fds.unlock(s);
    }

    if (copiedSet.empty())
    {
        cout << "Did not send: No receivers left.\n";
    }
    else
    {
        for (auto cur_fd : copiedSet)
        {
            unsigned char buf[1];
            buf[0] = 0x00;

            auto begin = chrono::high_resolution_clock::now();

            // Send
            send(cur_fd, what.c_str(), what.size(), 0);
            recv(cur_fd, buf, 1, 0);

            auto end = chrono::high_resolution_clock::now();
            int ellapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

            cout << "Ping (ns): " << ellapsed << '\n';

            if (buf[0] == 0x00)
            {
                // kill responce
                {
                    // Remove from mutex
                    set<int> *s;
                    while (!fds.lock(s))
                    {
                        // Hang while the mutex is secured
                    }

                    s->erase(cur_fd);
                    fds.unlock(s);
                }
            }
        }
    }

    return;
}

map<int, string> socketstreamserver::getStr(const unsigned int Bytes)
{
    map<int, string> out;

    // Get mutex data
    set<int> copiedFDs;

    {
        set<int> *data;
        while (!fds.lock(data))
        {
            // Hang until lock is secured
        }
        for (auto i : *data)
        {
            copiedFDs.insert(i);
        }
        fds.unlock(data);
    }

    for (auto i : copiedFDs)
    {
        char buf[MAXDATASIZE];
        memset(buf, 0, MAXDATASIZE);
        recv(i, buf, Bytes, 0);

        out[i] = string(buf);
    }

    return out;
}

string socketstreamserver::getStr(const int FD, const unsigned int Bytes)
{
    char buf[MAXDATASIZE];
    memset(buf, 0, MAXDATASIZE);
    recv(FD, buf, Bytes, 0);

    return string(buf);
}

// Stream operators for OSockStream
///////////////////////////////////////////

socketstreamserver &operator<<(socketstreamserver &strm, string &what)
{
    if (what == "")
    {
        return strm;
    }

    strm.sendStr(what);
    return strm;
}

socketstreamserver &operator<<(socketstreamserver &strm, char *&what)
{
    strm.sendStr(string(what));
    return strm;
}

socketstreamserver &operator<<(socketstreamserver &strm, char what)
{
    strm.sendStr(string(&what));
    return strm;
}

socketstreamserver &operator<<(socketstreamserver &strm, int &what)
{
    strm.sendStr(to_string(what));
    return strm;
}

socketstreamserver &operator<<(socketstreamserver &strm, double &what)
{
    strm.sendStr(to_string(what));
    return strm;
}

// Internals for ISockStream
///////////////////////////////////////////

sockstream::sockstream(string IP, string Port)
{
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(IP.c_str(), Port.c_str(), &hints, &servinfo) != 0)
    {
        throw runtime_error("Getaddrinfo error");
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            cerr << "Client failed to get socket.\n";
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            cerr << "Client failed to connect.\n";
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        throw runtime_error("Failed to connect");
    }

    freeaddrinfo(servinfo);
}

sockstream::~sockstream()
{
    close(sockfd);
    return;
}

void sockstream::sendStr(string What)
{
    send(sockfd, What.c_str(), What.size(), 0);
}

string sockstream::getStr()
{
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
    {
        throw runtime_error("recv error");
    }

    send(sockfd, &PING_MESSAGE, 1, 0);

    buf[numbytes] = '\0';

    string out(buf);

    return out;
}

sockstream &operator>>(sockstream &strm, string &what)
{
    what = strm.getStr();
    return strm;
}

sockstream &operator>>(sockstream &strm, int &what)
{
    what = stoi(strm.getStr());
    return strm;
}

sockstream &operator>>(sockstream &strm, double &what)
{
    what = stod(strm.getStr());
    return strm;
}

sockstream &operator>>(sockstream &istrm, socketstreamserver &ostrm)
{
    string temp;
    istrm >> temp;
    ostrm << temp;
    return istrm;
}

socketstreamserver &operator<<(socketstreamserver &ostrm, sockstream &istrm)
{
    string temp;
    istrm >> temp;
    ostrm << temp;
    return ostrm;
}
