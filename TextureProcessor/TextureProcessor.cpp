#include <iostream>
#include <fstream>

#include "DDS.h"

#include "SDL_video.h"
#include "SDL_image.h"

using namespace std;

//Hack to stop SDL defining main!
#undef main

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

int main(int argc, char *argv[])
{
	//Check arguments
	if (argc != 3)
	{
		cout << "Texture Processor\n\nUsage: TextureProcessor.exe inputfile outputfile" << endl;
		cout << "Copies textures to the output directory for now." << endl;
		return 0;
	}

	//Read in image from file
	SDL_Init(0);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	SDL_Surface* pSurface = IMG_Load(argv[1]);
	
	if (!pSurface)
	{
		cout << "Error Loading Texture: " << SDL_GetError() << endl;
		return 1;
	}

	//Create DDS file headers
	int pitch = (pSurface->w * pSurface->format->BitsPerPixel + 7) / 8;

	DDS_HEADER header;
	ZeroMemory(&header, sizeof(DDS_HEADER));
	header.dwSize = 124,
	header.dwHeaderFlags = DDS_HEADER_FLAGS_TEXTURE,
	header.dwWidth = pSurface->w;
	header.dwHeight = pSurface->h;
	header.dwDepth = 0;
	header.dwPitchOrLinearSize = pitch;
	header.ddspf = DDSPF_R8G8B8;
	header.dwSurfaceFlags = DDS_SURFACE_FLAGS_TEXTURE;

	//Write DDS file
	std::ofstream fs(argv[2], ios::out|ios::binary|ios::trunc);

	if (!fs.good())
	{
		cout << "Error creating: \"" << argv[2] << "\"" << endl;
		return 1;
	}

	//Magic Number 'DDS' 0x20534444
	DWORD dwMagic = 0x20534444;
	fs.write((char*)&dwMagic, sizeof(DWORD));
	//Write header
	fs.write((char*)&header, sizeof(DDS_HEADER));

	size_t pixelSize = header.ddspf.dwRGBBitCount/8;

	for (DWORD x = 0; x<header.dwWidth; ++x)
		for (DWORD y = 0; y<header.dwHeight; ++y)
		{
			Uint32 pixel = getpixel(pSurface, x, y);
			fs.write((char*)&pixel, pixelSize);
		}

	fs.close();

	return 0;
}