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
#include <vector>
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
        maxSize = sizeNum = 0;
        data = nullptr;
        return;
    }

    SafeArray(const int &Size)
    {
        try
        {
            freed = false;
            maxSize = sizeNum = Size;
            data = new T[sizeNum];
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
    SafeArray(const SafeArray<T> &Other)
    {
        freed = false;
        maxSize = sizeNum = Other.sizeNum;
        data = new T[sizeNum];
        for (int i = 0; i < sizeNum; i++)
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
        maxSize = sizeNum = Other.sizeNum;
        data = new T[sizeNum];
        for (int i = 0; i < sizeNum; i++)
        {
            data[i] = Other[i];
        }
        return;
    }

    void operator=(const vector<T> &Other)
    {
        if (!freed)
        {
            free();
        }

        freed = false;
        maxSize = sizeNum = Other.size();
        data = new T[sizeNum];
        for (int i = 0; i < sizeNum; i++)
        {
            data[i] = Other[i];
        }
        return;
    }

    void operator=(const initializer_list<T> &What)
    {
        if (!freed)
        {
            free();
        }

        freed = false;
        maxSize = sizeNum = What.size();
        data = new T[sizeNum];

        int i = 0;
        for (auto item : What)
        {
            (*this)[i] = item;
            i++;
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
            maxSize = sizeNum = 0;
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
        else if (I >= 0 && I >= sizeNum)
        {
            cout << "SafeArray error: Access denied to illegal element " << I
                 << ". Size: " << sizeNum << ".\n";
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Illegal access attempted!");
            }
        }
        else if (I < 0 && I < -sizeNum)
        {
            cout << "SafeArray error: Access denied to illegal element " << I
                 << ". Size: " << sizeNum << ".\n";
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
            return data[sizeNum + I];
        }
    }

    int getSize() const
    {
        return sizeNum;
    }

    int size() const
    {
        return sizeNum;
    }

    // HIGHLY UNSAFE! (But pointer casting is my favorite hobby)
    void *bytes() const
    {
        return data;
    }

    void setSize(const int &To)
    {
        if (To > maxSize || To < -maxSize)
        {
            if (FATAL)
            {
                assert(false);
            }
            else
            {
                throw safearray_error("Illegal access attempted!");
            }
        }
        else if (To > 0)
        {
            sizeNum = To;
        }
        else if (To < 0)
        {
            sizeNum += To;
        }

        return;
    }

    T *begin() const
    {
        return &data[0];
    }

    T *end() const
    {
        return &data[sizeNum];
    }

protected:
    int sizeNum;
    int maxSize;
    bool freed;
    T *data;
};

#endif
