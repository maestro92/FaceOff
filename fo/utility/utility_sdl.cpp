#include "utility.h"



void Utility::initSDL(int w, int h, SDL_Surface* & m_displaySurface)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    m_displaySurface = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE|SDL_OPENGL);
    //error check
    if (m_displaySurface == NULL)
    {
        //report error
        cerr << "Could not set up display surface!" << SDL_GetError() << endl;
        exit(1);
    }
}

void Utility::exitSDL(SDL_Surface* & m_displaySurface)
{
    SDL_FreeSurface(m_displaySurface);
    SDL_Quit();
}



SDL_Surface* Utility::loadRawImage(string filename)
{
    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << filename << " not found" << std::endl;
        return NULL;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << filename << " Load image failed" << endl;
        return NULL;
    }

    return img;
}


SDL_Surface* Utility::loadSDLImage(string filename)
{
    SDL_Surface* img = loadRawImage(filename);

    SDL_PixelFormat pixel_format = {NULL,
                                    32,             // converting to 32 bit pixel
                                    4,              // number of bytes
                                    0,0,0,0,        // byte lost
                                    0,0,0,0,        // byte shift
                                    0xff000000,     // red mask
                                    0x00ff0000,     // green mask
                                    0x0000ff00,     // blue mask
                                    0x000000ff,     // alpha mask
                                    0,              // color code
                                    255};           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img not converted to SDL img " << filename << endl;
        return NULL;
    }

    SDL_FreeSurface(img);
    return img2;
}

