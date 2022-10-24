#include "../headers/layer.h"

#include <iostream>
using namespace std;

int main() {
    srand(time(NULL));

    Layer l(10, 5); // 10 in, 5 out

    Matrix<double> input(10, 1);
    for (int i = 1; i <= 10; i++) {
        input.set(i, 1, rand() % 10);
    }

    cout << l.run(input) << '\n';
    
    return 0;
}
