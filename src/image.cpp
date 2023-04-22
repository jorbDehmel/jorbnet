/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "image.hpp"

dataset loadBMP(const string &Filepath, const int &W, const int &H,
                const SafeArray<double> &Expected, const Mode M)
{
    // Load image from file into a SDL surface
    SDL_Surface *original = SDL_LoadBMP(Filepath.c_str());

    // Scale the raw file data into the desired size
    SDL_Surface *scaled = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
    SDL_BlitScaled(original, NULL, scaled, NULL);
    unsigned int *pixels = (unsigned int *)scaled->pixels;

    // Transfer from SDL surface to dataset depending on mode
    unsigned char r, g, b, a;
    vector<double> inputTemp;
    for (int i = 0; i < W * H; i++)
    {
        SDL_GetRGBA(pixels[i], scaled->format, &r, &g, &b, &a);

        switch (M)
        {
        case RGBA:
            inputTemp.push_back(r);
            inputTemp.push_back(g);
            inputTemp.push_back(b);
            inputTemp.push_back(a);
            break;
        case RGB:
            inputTemp.push_back(r);
            inputTemp.push_back(g);
            inputTemp.push_back(b);
            break;
        case BW_double:
            inputTemp.push_back((r + g + b) / 3.0);
            break;
        case BW_char:
            inputTemp.push_back((unsigned char)((r + g + b) / 3.0));
            break;
        default:
            throw runtime_error("Invalid mode selection; Cannot open .bmp file.");
        }
    }

    SDL_FreeSurface(scaled);
    SDL_FreeSurface(original);

    vector<double> outputTemp;

    for (int i = 0; i < Expected.size(); i++)
    {
        outputTemp.push_back(Expected[i]);
    }

    dataset out;

    out.input = SafeArray<double>(inputTemp.size());
    for (int i = 0; i < inputTemp.size(); i++)
    {
        out.input[i] = inputTemp[i];
    }

    out.output = SafeArray<double>(outputTemp.size());
    for (int i = 0; i < outputTemp.size(); i++)
    {
        out.output[i] = outputTemp[i];
    }

    return out;
}

void saveBMP(const string &Filepath, const int &W, const int &H,
             const SafeArray<double> &What, const Mode M)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
    SDL_Renderer *rend = SDL_CreateSoftwareRenderer(surface);

    int i = 0;
    switch (M)
    {
    case RGBA:
        assert(What.size() >= W * H * 4);
        for (i = 0; i < W * H * 4; i += 4)
        {
            SDL_SetRenderDrawColor(rend, What[i], What[i + 1], What[i + 2], What[i + 3]);
            SDL_RenderDrawPoint(rend, (i / 4) % W, (i / 4) / W);
        }
        break;
    case RGB:
        assert(What.size() >= W * H * 3);
        for (i = 0; i < W * H * 3; i += 3)
        {
            SDL_SetRenderDrawColor(rend, What[i], What[i + 1], What[i + 2], 255);
            SDL_RenderDrawPoint(rend, (i / 3) % W, (i / 3) / W);
        }
        break;
    case BW_double:
    case BW_char:
        assert(What.size() >= W * H);
        for (int i = 0; i < What.size(); i++)
        {
            SDL_SetRenderDrawColor(rend, What[i], What[i], What[i], What[i]);
            SDL_RenderDrawPoint(rend, i % W, i / W);
        }

        break;
    default:
        throw runtime_error("Invalid mode selection; Cannot save .bmp file.");
    }

    SDL_SaveBMP(surface, Filepath.c_str());
    SDL_FreeSurface(surface);

    return;
}

vector<dataset> addNoise(const dataset &To, const double &Amount, const int &NumOutputs)
{
    vector<dataset> out;
    for (int i = 0; i < NumOutputs; i++)
    {
        dataset toAdd = To;
        for (int j = 0; j < toAdd.input.getSize(); j++)
        {
            toAdd.input[j] += drand(-Amount, Amount);
        }
        out.push_back(toAdd);
    }
    return out;
}
