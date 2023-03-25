/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "jorbnet/jorbNet.hpp"
#include "jorbnet/tags.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main(const int argc, const char *argv[])
{
    string inPath, outPath;

    cout << tags::green_bold
         << "Neural Network File Stripper\n"
         << "(Removes all training data, finalizing the network)\n"
         << "Jorb Dehmel, 2022, jdehmel@outlook.com\n"
         << tags::reset;

    if (argc == 3)
    {
        inPath = argv[1];
        outPath = argv[2];
    }
    else
    {
        cout << "\nInput Filepath: ";
        cin >> inPath;

        cout << "Output Filepath: ";
        cin >> outPath;
    }
    cout << '\n';

    cout << "Polling initial size...\n";
    ifstream fileIn(inPath, ios::binary | ios::ate);
    if (!fileIn.is_open())
    {
        cout << tags::red_bold
             << "Error: Could not open input file.\n"
             << tags::reset;

        return 1;
    }
    long long oldSize = fileIn.tellg();
    cout << "Old size: " << oldSize << '\n';
    fileIn.close();

    cout << "Loading network...\n";
    Network n = loadNetwork(inPath);

    cout << "Stripping training data...\n";
    n.trainingData.clear();

    cout << "Saving...\n";
    saveNetwork(outPath, n);

    cout << "Polling output size...\n";
    fileIn.open(outPath, ios::binary | ios::ate);
    if (!fileIn.is_open())
    {
        cout << tags::red_bold
             << "Error: Could not open output file.\n"
             << tags::reset;

        return 2;
    }
    long long newSize = fileIn.tellg();
    cout << "New size: " << newSize << '\n';
    fileIn.close();

    cout << tags::green_bold << "Done.\n\n"
         << tags::reset
         << "Percent size reduction: " << 100 * (oldSize - newSize) / (double)oldSize << "%\n"
         << "(Note: Network can still run, but can no longer train.\n";

    return 0;
}
