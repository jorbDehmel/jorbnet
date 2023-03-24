/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "npool.hpp"

NPool::NPool(Network &Starter, const int &Num, const int &Cull, const int &CullInterval)
{
    cull = Cull;
    cullInterval = CullInterval;
    passes = 0;
    num = Num;

    networks = SafeArray<Network *>(num);

    networks[0] = new Network(Starter);
    assert(networks[0] != nullptr);

    for (int i = 1; i < num; i++)
    {
        networks[i] = new Network(Starter);
        assert(networks[i] != nullptr);

        for (auto w : networks[i]->getWeights())
        {
            *w += drand(-Starter.weightVariance, Starter.weightVariance);
        }
    }

    return;
}

NPool::~NPool()
{
    for (int i = 0; i < num; i++)
    {
        delete networks[i];
    }
    return;
}

void __threadTrain(Network *ToCall, const int &TrainFor)
{
    ToCall->train(TrainFor);
    return;
}

void NPool::train(const int &NumTimes, const bool &PrintUpdates)
{
    for (int i = 0; i < NumTimes; i += cullInterval)
    {
        if (PrintUpdates)
        {
            cout << "Network pool is on pass #" << i << '\n';
        }

        // Start training
        SafeArray<thread *> threads(num);

        for (int n = 0; n < num; n++)
        {
            threads[n] = new thread(__threadTrain, networks[n], cullInterval);
            assert(threads[n] != nullptr);
        }

        // Join threads
        for (int n = 0; n < num; n++)
        {
            threads[n]->join();
            delete threads[n];
        }

        // Cull
        int size = num;
        while (size > num - cull)
        {
            int indexOfLeastAccurate = 0;
            for (int j = 1; j < num; j++)
            {
                if (networks[j] == nullptr)
                {
                    continue;
                }
                else if (networks[indexOfLeastAccurate] == nullptr)
                {
                    indexOfLeastAccurate = j;
                }
                else if (networks[j]->getError() > networks[indexOfLeastAccurate]->getError())
                {
                    indexOfLeastAccurate = j;
                }
            }
            assert(networks[indexOfLeastAccurate] != nullptr);

            delete networks[indexOfLeastAccurate];
            networks[indexOfLeastAccurate] = nullptr;

            size--;
        }

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
            else if (networks[j]->getError() < networks[indexOfMostAccurate]->getError())
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
                networks[j] = new Network(*networks[indexOfMostAccurate]);
                assert(networks[j] != nullptr);

                for (auto w : networks[j]->getWeights())
                {
                    *w += drand(-networks[indexOfMostAccurate]->weightVariance, networks[indexOfMostAccurate]->weightVariance);
                }
            }
        }
    }
    return;
}

Network NPool::best()
{
    int indexOfBest = 0;
    for (int i = 1; i < num; i++)
    {
        if (networks[i] == nullptr)
        {
            continue;
        }
        else if (networks[indexOfBest] == nullptr)
        {
            indexOfBest = i;
        }
        else if (networks[i]->getError() < networks[indexOfBest]->getError())
        {
            indexOfBest = i;
        }
    }
    assert(networks[indexOfBest] != nullptr);
    return *networks[indexOfBest];
}
