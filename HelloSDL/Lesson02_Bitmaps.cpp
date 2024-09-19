#include <iostream>
#include <SDL.h>

namespace Lesson02 
{
	using namespace std;

	static bool init();
	static void kill();
	static bool load();

	// Pointers to our window and surfaces
	static SDL_Window* window;
	static SDL_Surface* winSurface;
	static SDL_Surface* image1;
	static SDL_Surface* image2;

	static int main(int argc, char** args) 
	{
		if (!init()) return 1;

		if (!load()) return 1;

		// Fill the window with a white rectangle
		//SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 11, 211, 191));

		// Blit image to entire window
		SDL_BlitSurface(image1, NULL, winSurface, NULL);
		SDL_Rect dest{};
		dest.x = 0;
		dest.y = 0;
		dest.w = winSurface->w;
		dest.h = winSurface->h;

		// Scale
		// 
		//int result = SDL_BlitScaled(image1, NULL, winSurface, &dest);
		//if (result < 0) {
		//	// blit failed
		//}

		// Blit image to scaled portion of window
		SDL_Rect dest2{};
		dest2.x = 160;
		dest2.y = 120;
		dest2.w = 320;
		dest2.h = 240;
		int result2 = SDL_BlitScaled(image2, NULL, winSurface, &dest2);
		if (result2 < 0) {
			// blit failed
		}

		// Update window
		SDL_UpdateWindowSurface(window);
		system("pause");

		kill();
		return 0;
	}

	bool load() 
	{
		// Temporary surfaces to load images into
		// This should use only 1 temp surface, but for conciseness we use two
		SDL_Surface* temp1, * temp2;

		// Load images
		//temp1 = SDL_LoadBMP("../src/background.bmp");
		temp1 = SDL_LoadBMP("../src/bradock_disc_inv.bmp");
		temp2 = SDL_LoadBMP("../src/bradock_disc_inv.bmp");

		// Make sure loads succeeded
		if (!temp1 || !temp2) {
			cout << "Error loading image: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}

		// Format surfaces
		image1 = SDL_ConvertSurface(temp1, winSurface->format, 0);
		image2 = SDL_ConvertSurface(temp2, winSurface->format, 0);

		// Free temporary surfaces
		SDL_FreeSurface(temp1);
		SDL_FreeSurface(temp2);

		// Make sure format succeeded
		if (!image1 || !image2) {
			cout << "Error converting surface: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}
		return true;
	}

	bool init() 
	{
		// See last example for comments
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			cout << "Error initializing SDL: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}

		window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		if (!window) {
			cout << "Error creating window: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}

		winSurface = SDL_GetWindowSurface(window);
		if (!winSurface) {
			cout << "Error getting surface: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}
		return true;
	}

	void kill() 
	{
		// Free images
		SDL_FreeSurface(image1);
		SDL_FreeSurface(image2);

		// Quit
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}
