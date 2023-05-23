#ifndef SPIKE_HPP
#define SPIKE_HPP

#include "network.hpp"
using namespace std;

// A spiking neural network; Marginally more efficient
class SpikeNetwork : public Network
{
public:
    SpikeNetwork(const vector<int> &Sizes) : Network(Sizes) {}

    // For cloning
    SpikeNetwork(const SpikeNetwork &Other) : Network(Other) {}
    SpikeNetwork(const Network &Other) : Network(Other) {}

    SpikeNetwork &operator=(Network &Other);
    SpikeNetwork &operator=(SpikeNetwork &Other);

    // Propagation
    SafeArray<double> spikeProp(const SafeArray<double> &Input);
};

#endif
