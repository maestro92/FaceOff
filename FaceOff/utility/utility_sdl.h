#ifndef UTILITY_SDL_H_
#define UTILITY_SDL_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "SDL.h"
#include "SDL_image.h"

#define DEBUG_FLAG 1

using namespace std; 

namespace utl
{
	/// utl_SDL.cpp
	void initSDL(int w, int h, SDL_Surface* & m_displaySurface);

	void exitSDL(SDL_Surface* & m_displaySurface);

	SDL_Surface* loadRawImage(string filename);

	SDL_Surface* loadSDLImage(string filename);
	
	Uint32 getpixel(SDL_Surface *surface, int x, int y);

};

#endif

