#include <jorbnet/jorbNet.hpp>
using namespace std;

int main()
{
    srand(time(NULL));

    dataset imgrgb = loadBMP("test.bmp", 8, 8, {1}, BW_char);

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
