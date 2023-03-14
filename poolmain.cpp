#include "jgraph/jgraph.hpp"
#include "npool.hpp"
using namespace std;

int main()
{
    network n = loadNetwork("hi.nn");
    npool p(n, 10, 7, 10);
    p.train(200);
    graphNetworkError(p.best());

    return 0;
}
