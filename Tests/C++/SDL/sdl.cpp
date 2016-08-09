#include <SDL2/SDL.h>
#include <iostream>


void InitSDL()
{
	if (auto err = SDL_Init( SDL_INIT_EVERYTHING )) {
		std::cerr << "SDL_Init fucking failed: " << err << "\n";
		exit(EXIT_FAILURE);
	}

	int width  = 1280;
	int height = 720;

	int screen = 0;
	int posX = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);
	int posY = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);

	SDL_Window* sdl_window = SDL_CreateWindow(
		"SDL TEST",
		posX,
		posY,
		width,
		height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
}

namespace {
bool quit = false;
std::string input;
}

void PollEvents()
{
	SDL_StartTextInput();
	SDL_Event evt;
	while( SDL_PollEvent( &evt ) ) {
		std::cout << "Event " << evt.type << "\n";
		switch( evt.type ) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			break;
		case SDL_TEXTINPUT:
			input += evt.text.text;
			std::cout << evt.text.text << "\n";
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEWHEEL:
			std::cout << "Wheel is working here, why it doesn't in Havogrek?\n";
			std::cout << "x: " << evt.wheel.x << " y: " << evt.wheel.y << "\n";
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_WINDOWEVENT:
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
	}
	SDL_StopTextInput();
}

int main()
{
	InitSDL();

	while (!quit)
		PollEvents();

	std::cout << "\n\n" << std::string(12u,'-') << "\n" << input << "\n";
	return EXIT_SUCCESS;
}
