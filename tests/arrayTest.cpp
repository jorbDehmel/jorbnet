#include "../headers/array.h"

#include <iostream>
using namespace std;

int main() {
    srand(time(NULL));

    Matrix<double> a(5, 5);
    Matrix<double> b(5, 5);

    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            a.set(i, j, rand() % 100);
        }
    }

    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            b.set(i, j, i == j ? 1 : 0);
        }
    }

    cout << "a:\n";
    a.print(cout);
    cout << '\n';

    cout << "b:\n";
    b.print(cout);
    cout << '\n';

    cout << "Dot product (a * b):\n";
    (a * b)->print(cout);
    cout << '\n';

    cout << "Dot procutt (a * b) == a (ergo b == i4):\n";
    cout << (*(a * b) == a) << '\n';
    cout << '\n';

    cout << "Scalar product (a * 4):\n";
    (a * 4)->print(cout);
    cout << '\n';

    cout << "Scalar minus (a - 4):\n";
    (a - 4)->print(cout);
    cout << '\n';

    cout << "Scalar add (a + 4):\n";
    (a + 4)->print(cout);
    cout << '\n';

    cout << "Test completed successfully.\n";

    return 0;
}
