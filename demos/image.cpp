/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <jorbnet/jorbNet.hpp>
using namespace std;

#define chrono_now() chrono::high_resolution_clock::now()
#define chrono_ns(A) chrono::duration_cast<chrono::nanoseconds>(A).count()

int main(const int argc, const char *argv[])
{
    srand(time(NULL));

    cout << "Load? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'n')
    {
        cout << "Constructing dataset...\n";
        dataset imgrgb = loadBMP("test.bmp", 128, 128, {1}, BW_char);

        dataset blank;
        blank.output = {0};
        for (int i = 0; i < imgrgb.input.size(); i++)
        {
            blank.input.push_back(0);
        }

        const int num = 10;

        vector<dataset> fuzzedImage = addNoise(imgrgb, 5, num);
        vector<dataset> fuzzedBlank = addNoise(blank, 5, num);

        Network n({(int)fuzzedImage[0].input.size(), 20, 1});
        for (auto a : fuzzedImage)
        {
            n.trainingData.push_back(a);
        }
        for (auto a : fuzzedBlank)
        {
            n.trainingData.push_back(a);
        }

        cout << "Training...\n";
        auto start = chrono_now();
        n.train(50);
        auto end = chrono_now();
        cout << "Trained. Nanoseconds: " << chrono_ns(end - start) << '\n';

        cout << "Error: " << n.getError() << '\n';

        cout << "Saving...\n";
        saveNetwork("bestImage.nn", n);
        cout << "Saved.\n";
    }
    else
    {
        cout << "Loading...\n";
        Network n = loadNetwork("bestImage.nn");
        cout << "Loaded.\n";

        double prevError = n.getError();

        // Error hop
        for (auto w : n.getWeights())
        {
            *w += drand(-1, 1);
        }

        cout << "Passes before: " << n.passes << '\n';

        auto start = chrono_now();
        n.train(50);
        auto end = chrono_now();

        cout << "Passes after: " << n.passes << '\n';

        cout << "Trained. Nanoseconds: " << chrono_ns(end - start) << '\n';

        cout << "Error: " << n.getError() << '\n';

        if (n.getError() < prevError)
            saveNetwork("bestImage.nn", n);
        else
            cout << "Did not save.\n";
    }

    return 0;
}
