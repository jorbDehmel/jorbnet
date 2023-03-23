/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef SAFEARRAY_HPP
#define SAFEARRAY_HPP

#include <iostream>
#include <string>
using namespace std;

class safearray_error : public runtime_error
{
public:
    safearray_error(const string &What) : runtime_error(What) {}
};

// double *a = new double[34];
// SafeArray<double> a(34);
template <class T>
class SafeArray
{
public:
    SafeArray()
    {
        freed = true;
        size = 0;
        data = nullptr;
        return;
    }

    SafeArray(const int &Size)
    {
        try
        {
            freed = false;
            size = Size;
            data = new T[size];
        }
        catch (...)
        {
            cout << "SafeArray error: Failed to allocate array of size " << Size << '\n';
            throw safearray_error("Failed to allocate array.");
        }
        return;
    }

    // Steal another safearray's identity
    SafeArray(SafeArray<T> &Other)
    {
        freed = false;
        size = Other.size;
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = Other[i];
        }
        return;
    }

    ~SafeArray()
    {
        if (!freed)
        {
            delete[] data;
            data = nullptr;
            freed = true;
        }
        return;
    }

    void operator=(const SafeArray<T> &Other)
    {
        freed = false;
        size = Other.size;
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = Other[i];
        }
        return;
    }

    void free()
    {
        if (!freed)
        {
            delete[] data;
            data = nullptr;
            freed = true;
        }
        else
        {
            cout << "SafeArray error: Double free attempted.\n";
            throw safearray_error("Double free attempted!");
        }
        return;
    }

    T &operator[](const int &I) const
    {
        if (freed)
        {
            cout << "SafeArray error: Cannot access freed data.\n";
            throw safearray_error("Freed access attempted!");
        }
        else if (I < 0 || I >= size)
        {
            cout << "SafeArray error: Access denied to illegal element " << I
                 << ". Size: " << size << ".\n";
            throw safearray_error("Illegal access attempted!");
        }

        return data[I];
    }

    int getSize() const
    {
        return size;
    }

protected:
    T *data;
    int size;
    bool freed;
};

#endif
