/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <jorbnet/jorbNet.hpp>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

double getError(const vector<double> &E, const vector<double> &O)
{
    double totalError = 0;
    for (int i = 0; i < E.size(); i++)
    {
        totalError += pow(E[i] - O[i], 2);
    }
    return totalError / E.size();
}

int main(const int argc, const char *argv[])
{
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "          Neural Network  Data Extractor          \n"
         << "(Jorb Dehmel, 2023, github.com/jorbDehmel/jorbNet)\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    string fileName;
    cout << "Name of network to load: ";
    cin >> fileName;

    cout << "Loading...\n";
    auto n = loadNetwork(fileName);
    cout << "Loaded.\n";

    cout << "Network creation time/date: " << ctime(&n.creationTime) << '\n'
         << "Description:\n\t" << n.description << '\n'
         << "Data info ID: " << n.dataInfo << '\n';

    if (n.trainingData.size() == 0)
    {
        cout << "Network has no training data: Process aborted.\n";
        return 1;
    }

    cout << "Image modes:\n"
         << " 0) BW_double\n"
         << " 1) BW_char\n"
         << " 2) RGBA\n"
         << " 3) RGB\n"
         << "(Usually matches data info ID)\n";

    int modeInt;
    cout << "Mode index: ";
    cin >> modeInt;

    Mode mode = (Mode)modeInt;

    string outputFolder;
    cout << "Name of folder to save data: ";
    cin >> outputFolder;
    outputFolder += '/';

    assert(outputFolder != "/" && outputFolder != "~" && outputFolder != "" && outputFolder != "/root");

    char choice;
    cout << "Wipe old folder contents? (Y/n): ";
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        cout << "Are you sure? (Y/n): ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            cout << "Deleting files...\n";

            system(string("rm -rf " + outputFolder + "/*").c_str());
        }
    }

    int w, h;

    cout << "Manually adjust dimensions (default square)? (Y/n): ";
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        cout << "Width (Height will be adjusted accordingly): ";
        cin >> w;

        h = n.trainingData[0].input.size() / w;
        cout << "Calculated height: " << h << '\n';
    }
    else
    {
        w = h = sqrt(n.trainingData[0].input.size());
    }

    cout << "Saving images...\n";

    ofstream errors(outputFolder + "nn_data_" + to_string(time(NULL)) + ".csv");

    if (!errors.is_open())
    {
        cout << "Error: Could not save error file! Process aborted.\n";
        return 2;
    }

    errors << "Image number, Error, ";
    for (int i = 0; i < n.trainingData[0].output.size(); i++)
    {
        errors << "Act " << i << ", ";
    }
    errors << "\n";

    int i = 0;
    for (auto set : n.trainingData)
    {
        cout << '=' << flush;
        if (i + 1 % 40 == 0)
        {
            cout << '\n';
        }

        auto observed = n.prop(set.input);
        double err = getError(set.output, observed);

        errors << i << ", " << err << ", ";
        for (auto a : observed)
        {
            errors << a << ", ";
        }
        errors << '\n';

        saveBMP(outputFolder + to_string(i) + "_" + to_string(time(NULL)) + ".bmp", w, h, set.input, mode);
        i++;
    }
    cout << '\n';

    errors.close();

    cout << "Saved.\n";

    cout << "Save stripped version of network? (Y/n): ";
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        n.trainingData.clear();
        saveNetwork(outputFolder + "stripped_" + fileName, n);
        cout << "Saved stripped network.\n";
    }

    cout << "Done.\n";

    return 0;
}
