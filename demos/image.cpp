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
    dataset imgrgb = loadBMP("test.bmp", 64, 64, {1, 2, 3}, RGB);
    saveBMP("rgb.bmp", 64, 64, imgrgb.inputs, RGB);

    dataset imgrgba = loadBMP("test.bmp", 64, 64, {1, 2, 3}, RGBA);
    saveBMP("rgba.bmp", 64, 64, imgrgba.inputs, RGBA);

    dataset imgbwd = loadBMP("test.bmp", 64, 64, {1, 2, 3}, BW_double);
    saveBMP("bw_double.bmp", 64, 64, imgbwd.inputs, BW_double);

    dataset imgbwc = loadBMP("test.bmp", 64, 64, {1, 2, 3}, BW_char);
    saveBMP("bw_char.bmp", 64, 64, imgbwc.inputs, BW_char);

    return 0;
}
