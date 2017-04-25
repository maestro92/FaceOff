#include "utility_sdl.h"




void utl::initSDL(int w, int h, SDL_Surface* & m_displaySurface)
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

void utl::exitSDL(SDL_Surface* & m_displaySurface)
{
    SDL_FreeSurface(m_displaySurface);
    SDL_Quit();
}



SDL_Surface* utl::loadRawImage(string filename)
{
    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
#if DEBUG_FLAG == 1
		cout << "ERROR: " << filename << " not found" << std::endl;
#endif
		return NULL;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
#if DEBUG_FLAG == 1
		cout << "ERROR: " << filename << " Load image failed" << endl;
#endif
		return NULL;
    }

    return img;
}


SDL_Surface* utl::loadSDLImage(string filename)
{
    SDL_Surface* img = loadRawImage(filename);

	if (img == NULL)
	{
		cout << "ERROR: " << filename << " not found" << std::endl;
		return NULL;
	}

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
        cout << "ERROR: " << filename << " img not converted to SDL img " << filename << endl;
        return NULL;
    }

    SDL_FreeSurface(img);
    return img2;
}


Uint32 utl::getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) 
	{
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}