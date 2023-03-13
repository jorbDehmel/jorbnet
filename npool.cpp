/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
MIT licence via mit-license.org held by author
*/

#include "npool.hpp"

npool::npool(network &Starter, const int &Num, const int &Cull, const int &CullInterval)
{
    cull = Cull;
    cullInterval = CullInterval;
    passes = 0;
    num = Num;

    networks = new network *[Num];
    assert(networks != nullptr);

    networks[0] = new network(Starter);
    assert(networks[0] != nullptr);

    for (int i = 1; i < num; i++)
    {
        networks[i] = new network(Starter);
        assert(networks[i] != nullptr);

        for (int layer = 1; layer < networks[i]->sizes.size(); layer++)
        {
            for (int node = 0; node < networks[i]->sizes[layer]; node++)
            {
                for (int weight = 0; weight < networks[i]->nodes[layer][node].links.size(); weight++)
                {
                    networks[i]->nodes[layer][node].weights[weight] += drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
                }
            }
        }
    }
    return;
}

npool::~npool()
{
    for (int i = 0; i < num; i++)
    {
        delete networks[i];
    }
    delete[] networks;
    return;
}

void npool::train(const int &NumTimes)
{
    for (int i = 0; i < NumTimes; i += cullInterval)
    {
        cout << "Training...\n";
        for (int n = 0; n < num; n++)
        {
            assert(networks[n] != nullptr);
            cout << "Training network " << n << '\n';
            networks[n]->train(cullInterval);
        }

        cout << "Culling...\n";

        // Cull
        int size = num;
        while (size > num - cull)
        {
            int indexOfLeastAccurate = -1;
            for (int j = 0; j < num; j++)
            {
                if (networks[j] == nullptr)
                {
                    continue;
                }
                else if (indexOfLeastAccurate == -1)
                {
                    indexOfLeastAccurate = j;
                }
                else if (networks[j]->errors.back() > networks[indexOfLeastAccurate]->errors.back())
                {
                    indexOfLeastAccurate = j;
                }
            }
            assert(indexOfLeastAccurate != -1);
            cout << "Culling network " << indexOfLeastAccurate << "\taddress " << networks[indexOfLeastAccurate] << '\n';

            delete networks[indexOfLeastAccurate];
            networks[indexOfLeastAccurate] = nullptr;

            size--;
        }

        cout << "Culled.\n";

        // Find best
        int indexOfMostAccurate = 0;
        for (int j = 1; j < num; j++)
        {
            if (networks[j] == nullptr)
            {
                continue;
            }
            else if (networks[indexOfMostAccurate] == nullptr)
            {
                indexOfMostAccurate = j;
            }
            else if (networks[j]->errors.back() < networks[indexOfMostAccurate]->errors.back())
            {
                indexOfMostAccurate = j;
            }
        }

        // Repopulate
        for (int j = 0; j < num; j++)
        {
            if (networks[j] != nullptr)
            {
                continue;
            }
            else
            {
                networks[j] = new network(*networks[indexOfMostAccurate]);
                for (auto w : networks[j]->weights)
                {
                    *w += drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
                }
            }
        }
        cout << "Repopulated.\n";
    }
    return;
}
