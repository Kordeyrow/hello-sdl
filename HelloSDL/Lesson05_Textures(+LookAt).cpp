#include <iostream>
#include <SDL.h>
#include <cmath>

namespace Lesson05_LookAt
{
	using namespace std;

	typedef enum {
		FLIP_NONE = 0,
		FLIP_X_WHEN_LEFT = 1,
		FLIP_X_WHEN_RIGHT = 2,
		FLIP_X_WHEN_UP = 3,
		FLIP_X_WHEN_DOWN = 4,
		FLIP_Y_WHEN_LEFT = 5,
		FLIP_Y_WHEN_RIGHT = 6,
		FLIP_Y_WHEN_UP = 7,
		FLIP_Y_WHEN_DOWN = 8,
	} IMAGE_FLIP_DIRECTION;

	static bool IsLookingLeft(double rotation)		{ return rotation < -90 || rotation > 90; }
	static bool IsLookingRight(double rotation)		{ return IsLookingLeft(rotation) == false; }
	static bool IsLookingUp(double rotation)		{ return rotation < 0; }
	static bool IsLookingDown(double rotation)		{ return IsLookingUp(rotation) == false; }

	struct Vector2 {
		float x = 0;
		float y = 0;
		Vector2() 
			{}
		Vector2(float _x, float _y) 
			: x(_x), y(_y) {}
		Vector2(SDL_Rect dest) 
			: x(dest.x + dest.w/2), y(dest.y + dest.h / 2) {}
	};

	static bool init();
	static void kill();
	static bool loop();
	static double LookAtAngle(Vector2 orig, Vector2 target);

	// Pointers to our window, renderer, and texture
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static SDL_Texture* texture;

	static int main(int argc, char** args) 
	{
		if (!init()) return 1;

		while (loop()) {
			// wait before processing the next frame
			SDL_Delay(10);
		}

		kill();
		return 0;
	}

	bool loop() 
	{
		static const unsigned char* keys = SDL_GetKeyboardState(NULL);

		SDL_Event e;
		SDL_Rect dest;

		static int mx = -1, my = -1;
		static double rot = 0;

		// Clear the window to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		// Event loop
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEMOTION:
				mx = e.motion.x;
				my = e.motion.y;
				break;
			}
		}

		// Render texture pos, scale
		dest.x = 100;
		dest.y = 280;
		dest.w = 160;
		dest.h = 120;

		static Vector2 orig{};

		static SDL_RendererFlip flip = SDL_FLIP_NONE;

		if (mx != -1) {
			// Distance across window
			float wpercent = mx / 640.0f;
			float hpercent = my / 480.0f;

			// Color
			unsigned char r = round(wpercent * 255);
			unsigned char g = round(hpercent * 255);

			// Color mod (b will always be zero)
			SDL_SetTextureColorMod(texture, r, g, 0);

			orig = Vector2(dest);
			Vector2 target = Vector2(mx, my);
			bool shouldFlip = false;
			rot = LookAtAngle(orig, target);
			cout << rot << endl;

			IMAGE_FLIP_DIRECTION flipDirection = FLIP_Y_WHEN_LEFT;
			switch (flipDirection) {
				case FLIP_X_WHEN_LEFT:
					flip = IsLookingLeft(rot) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
					break;
				case FLIP_X_WHEN_RIGHT:
					flip = IsLookingRight(rot) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
					break;
				case FLIP_X_WHEN_UP:
					flip = IsLookingUp(rot) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
					break;
				case FLIP_X_WHEN_DOWN:
					flip = IsLookingDown(rot) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
					break;
				case FLIP_Y_WHEN_LEFT:
					flip = IsLookingLeft(rot) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
					break;
				case FLIP_Y_WHEN_RIGHT:
					flip = IsLookingRight(rot) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
					break;
				case FLIP_Y_WHEN_UP:
					flip = IsLookingUp(rot) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
					break;
				case FLIP_Y_WHEN_DOWN:
					flip = IsLookingDown(rot) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
					break;
			}
		}

		mx = my = -1;

		SDL_RenderCopyEx(renderer, texture, NULL, &dest, rot, NULL, flip);

		SDL_Rect textureCenterPoint{};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		textureCenterPoint.x = orig.x;
		textureCenterPoint.y = orig.y;
		textureCenterPoint.w = 5;
		textureCenterPoint.h = 5;
		SDL_RenderFillRect(renderer, &textureCenterPoint);

		// Update window
		SDL_RenderPresent(renderer);

		return true;
	}

	double LookAtAngle(Vector2 orig, Vector2 target)
	{
		float angleRadians = atan2(target.y - orig.y, target.x - orig.x);
		float convertRadiansToDegrees = (180.0f / 3.14f);
		return angleRadians * convertRadiansToDegrees;
	}

	bool init() 
	{
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

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			cout << "Error creating renderer: " << SDL_GetError() << endl;
			return false;
		}

		// Load bitmap into surface
		SDL_Surface* buffer = SDL_LoadBMP("../src/bradock_disc_inv.bmp");
		if (!buffer) {
			cout << "Error loading image test.bmp: " << SDL_GetError() << endl;
			return false;
		}

		// Create texture
		texture = SDL_CreateTextureFromSurface(renderer, buffer);
		// Free surface as it's no longer needed
		SDL_FreeSurface(buffer);
		buffer = NULL;
		if (!texture) {
			cout << "Error creating texture: " << SDL_GetError() << endl;
			return false;
		}

		return true;
	}

	void kill() 
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		texture = NULL;
		window = NULL;
		renderer = NULL;
		SDL_Quit();
	}
}