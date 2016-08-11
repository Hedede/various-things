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
	return "OTHER";
}

#define AW_ALL_SDL_WEVENTS \
X(SDL_WINDOWEVENT_SHOWN) \
X(SDL_WINDOWEVENT_HIDDEN) \
X(SDL_WINDOWEVENT_EXPOSED) \
X(SDL_WINDOWEVENT_MOVED) \
X(SDL_WINDOWEVENT_RESIZED) \
X(SDL_WINDOWEVENT_SIZE_CHANGED) \
X(SDL_WINDOWEVENT_MINIMIZED) \
X(SDL_WINDOWEVENT_MAXIMIZED) \
X(SDL_WINDOWEVENT_RESTORED) \
X(SDL_WINDOWEVENT_ENTER) \
X(SDL_WINDOWEVENT_LEAVE) \
X(SDL_WINDOWEVENT_FOCUS_GAINED) \
X(SDL_WINDOWEVENT_FOCUS_LOST) \
X(SDL_WINDOWEVENT_CLOSE)

char const* WEventString(unsigned type)
{
#define X(x) case x: return #x;
	switch (type) {
	AW_ALL_SDL_WEVENTS
	}
#undef X
	return "OTHER";
}

void PollEvents()
{
	SDL_Event evt;
	while( SDL_PollEvent( &evt ) ) {
		std::cout << "Event " << EventString(evt.type);
		switch( evt.type ) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			std::cout << " key: " << SDL_GetKeyName(evt.key.keysym.sym);
			break;
		case SDL_TEXTINPUT:
			input += evt.text.text;
			std::cout << " text: " << evt.text.text;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEWHEEL:
			std::cout << " x: " << evt.wheel.x << " y: " << evt.wheel.y;
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_WINDOWEVENT:
			std::cout << " type: " << WEventString(evt.window.event);
			std::cout << " x: " << evt.window.data1;
			std::cout << " y: " << evt.window.data2;
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
		std::cout << '\n';
	}
}

int main()
{
	InitSDL();

	//SDL_StartTextInput();
	while (!quit)
		PollEvents();

	//SDL_StopTextInput();

	std::cout << "\n\n" << std::string(12u,'-') << "\n" << input << "\n";
	return EXIT_SUCCESS;
}
