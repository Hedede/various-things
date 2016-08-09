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

#define AW_ALL_SDL_EVENTS \
X(SDL_AUDIODEVICEADDED) \
X(SDL_AUDIODEVICEREMOVED) \
X(SDL_CONTROLLERAXISMOTION) \
X(SDL_CONTROLLERBUTTONDOWN) \
X(SDL_CONTROLLERBUTTONUP) \
X(SDL_CONTROLLERDEVICEADDED) \
X(SDL_CONTROLLERDEVICEREMOVED) \
X(SDL_CONTROLLERDEVICEREMAPPED) \
X(SDL_DOLLARGESTURE) \
X(SDL_DOLLARRECORD) \
X(SDL_DROPFILE) \
X(SDL_FINGERMOTION) \
X(SDL_FINGERDOWN) \
X(SDL_FINGERUP) \
X(SDL_KEYDOWN) \
X(SDL_KEYUP) \
X(SDL_JOYAXISMOTION) \
X(SDL_JOYBALLMOTION) \
X(SDL_JOYHATMOTION) \
X(SDL_JOYBUTTONDOWN) \
X(SDL_JOYBUTTONUP) \
X(SDL_JOYDEVICEADDED) \
X(SDL_JOYDEVICEREMOVED) \
X(SDL_MOUSEMOTION) \
X(SDL_MOUSEBUTTONDOWN) \
X(SDL_MOUSEBUTTONUP) \
X(SDL_MOUSEWHEEL) \
X(SDL_MULTIGESTURE) \
X(SDL_QUIT) \
X(SDL_SYSWMEVENT) \
X(SDL_TEXTEDITING) \
X(SDL_TEXTINPUT) \
X(SDL_USEREVENT) \
X(SDL_WINDOWEVENT)

char const* EventString(unsigned type)
{
#define X(x) case x: return #x;
	switch (type) {
	AW_ALL_SDL_EVENTS
	}
#undef X
}

void PollEvents()
{
	SDL_StartTextInput();
	SDL_Event evt;
	while( SDL_PollEvent( &evt ) ) {
		std::cout << "Event " << EventString(evt.type) << "\n";
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
