#include "../headers/layer.h"

#include <iostream>
using namespace std;

int main() {
    srand(420);

    Layer l(3, 3);

    Matrix<double> input(2, 1);
    for (int i = 1; i <= input.height(); i++) {
        input.set(i, 1, rand() % 10);
    }

    cout << input << '\n';
    cout << *l.getWeights() << '\n';

    cout << "Output:\n";
    l.run(input);
    cout << *l.getActivation();

    return 0;
}
