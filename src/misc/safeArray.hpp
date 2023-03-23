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

#define FATAL true

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

            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Failed to allocate array.");
            }
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
        if (!freed)
        {
            free();
        }

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
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Double free attempted!");
            }
        }
        return;
    }

    T &operator[](const int &I) const
    {
        if (freed)
        {
            cout << "SafeArray error: Cannot access freed data.\n";
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Freed access attempted!");
            }
        }
        else if (I >= 0 && I >= size)
        {
            cout << "SafeArray error: Access denied to illegal element " << I
                 << ". Size: " << size << ".\n";
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Illegal access attempted!");
            }
        }
        else if (I < 0 && I < -size)
        {
            cout << "SafeArray error: Access denied to illegal element " << I
                 << ". Size: " << size << ".\n";
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Illegal access attempted!");
            }
        }

        if (I >= 0)
        {
            return data[I];
        }

        // Creature comfort
        else
        {
            return data[size + I];
        }
    }

    int getSize() const
    {
        return size;
    }

    // HIGHLY UNSAFE! (But pointer casting is my favorite hobby)
    void *bytes() const
    {
        return data;
    }

protected:
    int size;
    bool freed;
    T *data;
};

#endif
