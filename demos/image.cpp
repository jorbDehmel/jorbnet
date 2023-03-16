/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <jorbnet/jorbNet.hpp>
using namespace std;

int main(const int argc, const char *argv[])
{
    dataset img = loadBMP("test.bmp", 64, 64, {1, 2, 3}, BW_char);

    for (auto p : img.inputs)
    {
        cout << p << '\n';
    }
    cout << "Size: " << img.inputs.size() << '\n';

    saveBMP("bwscaled.bmp", 64, 64, img.inputs, BW_char);

    return 0;
}
