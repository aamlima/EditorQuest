#ifndef _GLOBALDEF_H_
#define _GLOBALDEF_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	#define rmask 0xff000000
	#define gmask 0x00ff0000
	#define bmask 0x0000ff00
	#define amask 0x000000ff
#else
	#define rmask 0x000000ff
	#define gmask 0x0000ff00
	#define bmask 0x00ff0000
	#define amask 0xff000000
#endif

//#define DEBUG

#endif