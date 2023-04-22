#include <jorbnet/jorbNet.hpp>
using namespace std;

int main()
{
    srand(time(NULL));

    dataset imgrgb = loadBMP("test.bmp", 128, 128, {1}, BW_char);

    saveBMP("trainon.bmp", 128, 128, imgrgb.input, BW_char);

    dataset blank;
    blank.output = {0};
    blank.input = SafeArray<double>(imgrgb.input.getSize());
    for (int i = 0; i < imgrgb.input.getSize(); i++)
    {
        blank.input[i] = 0;
    }

    const int num = 10;

    vector<dataset> fuzzedImage = addNoise(imgrgb, 5, num);
    vector<dataset> fuzzedBlank = addNoise(blank, 5, num);

    Network n({(int)fuzzedImage[0].input.getSize(), 20, 1});
    for (auto a : fuzzedImage)
    {
        n.trainingData.push_back(a);
    }
    for (auto a : fuzzedBlank)
    {
        n.trainingData.push_back(a);
    }

    NPool p(n, 10, 7, 5);

    n.train(1);

    auto start = chrono_now();
    p.train(20, true);
    auto end = chrono_now();
    cout << "Trained. Time: " << toTime(chrono_ns(end - start)) << '\n';

    Network best = p.best();
    best.train(1);
    cout << "Error: " << best.getError() << '\n';
    cout << "Initial error: " << n.getError() << '\n';

    saveNetwork("bestImagePool.nn", best);
}
