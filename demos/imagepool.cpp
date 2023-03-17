#include <jorbnet/jorbNet.hpp>
using namespace std;

int main()
{
    srand(time(NULL));

    dataset imgrgb = loadBMP("test.bmp", 8, 8, {1}, BW_char);

    dataset blank;
    blank.expected = {0};
    for (int i = 0; i < imgrgb.inputs.size(); i++)
    {
        blank.inputs.push_back(0);
    }

    const int num = 10;

    vector<dataset> fuzzedImage = addNoise(imgrgb, 5, num);
    vector<dataset> fuzzedBlank = addNoise(blank, 5, num);

    network n({(int)fuzzedImage[0].inputs.size(), 20, 1});
    for (auto a : fuzzedImage)
    {
        n.trainingData.push_back(a);
    }
    for (auto a : fuzzedBlank)
    {
        n.trainingData.push_back(a);
    }

    npool p(n, 10, 7, 2);

    n.train(1);

    auto start = chrono_now();
    p.train(10, true);
    auto end = chrono_now();
    cout << "Trained. Time: " << toTime(chrono_ns(end - start)) << '\n';

    network best = p.best();
    best.train(1);
    cout << "Error: " << best.errors.back() << '\n';
    cout << "Initial error: " << n.errors.back() << '\n';

    saveNetwork("bestImagePool.nn", best);
}
