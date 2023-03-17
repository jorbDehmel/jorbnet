/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "imageHandling.hpp"

dataset loadBMP(const string &Filepath, const int &W, const int &H,
                const vector<double> &Expected, const Mode M)
{
    // Load image from file into a SDL surface
    SDL_Surface *original = SDL_LoadBMP(Filepath.c_str());

    // Scale the raw file data into the desired size
    SDL_Surface *scaled = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
    SDL_BlitScaled(original, NULL, scaled, NULL);
    unsigned int *pixels = (unsigned int *)scaled->pixels;

    // Transfer from SDL surface to dataset depending on mode
    dataset out;
    unsigned char r, g, b, a;
    for (int i = 0; i < W * H; i++)
    {
        SDL_GetRGBA(pixels[i], scaled->format, &r, &g, &b, &a);

        switch (M)
        {
        case RGBA:
            out.inputs.push_back(r);
            out.inputs.push_back(g);
            out.inputs.push_back(b);
            out.inputs.push_back(a);
            break;
        case RGB:
            out.inputs.push_back(r);
            out.inputs.push_back(g);
            out.inputs.push_back(b);
            break;
        case BW_double:
            out.inputs.push_back((r + g + b) / 3.0);
            break;
        case BW_char:
            out.inputs.push_back((unsigned char)((r + g + b) / 3.0));
            break;
        default:
            throw runtime_error("Invalid mode selection; Cannot open .bmp file.");
        }
    }

    SDL_FreeSurface(scaled);
    SDL_FreeSurface(original);

    for (auto exp : Expected)
    {
        out.expected.push_back(exp);
    }

    return out;
}

void saveBMP(const string &Filepath, const int &W, const int &H,
             const vector<double> &What, const Mode M)
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
        for (auto p : What)
        {
            SDL_SetRenderDrawColor(rend, p, p, p, p);
            SDL_RenderDrawPoint(rend, i % W, i / W);
            i++;
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
        for (int j = 0; j < toAdd.inputs.size(); j++)
        {
            toAdd.inputs[j] += drand(-Amount, Amount);
        }
        out.push_back(toAdd);
    }
    return out;
}
