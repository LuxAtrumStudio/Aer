#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include "SDL Headers.h"
#include "../Conscientia Files/Conscientia Headers.h";
#include "Aequus Headers.h"
using namespace std;

WINDOW::WINDOW()
{
	windowPointer = NULL;
	rendererPointer = NULL;
	mouseFocus = false;
	keyboardFocus = false;
	fullScreen = false;
	shown = false;
	windowID = -1;
	windowWidth = 0;
	windowHeight = 0;
}

WINDOW::~WINDOW()
{
}

void WINDOW::GenorateWindow(int posX, int posY, int width, int height)
{
	windowPointer = SDL_CreateWindow(windowName.c_str(), posX, posY, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (windowPointer == NULL) {
		LOGGING::LogError("Unable to genorate window " + windowName, "Window.cpp/WINDOW/GenorateWindow");
	}
	else {
		LOGGING::LogSuccess("Genorated window " + windowName, "Window.cpp/WINDOW/GenorateWindow");
		mouseFocus = true;
		keyboardFocus = true;
		windowWidth = width;
		windowHeight = height;
		rendererPointer = SDL_CreateRenderer(windowPointer, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (rendererPointer == NULL) {
			LOGGING::LogError("Could not create renderer for " + windowName, "Windows.cpp/WINDOW/GenorateWindow");
			SDL_DestroyWindow(windowPointer);
			windowPointer = NULL;
		}
		else {
			LOGGING::LogSuccess("Created renderer for " + windowName, "Window.cpp/WINDOW/GenorateWindow");
			SDL_SetRenderDrawColor(rendererPointer, 0xFF, 0xFF, 0xFF, 0xFF);
			windowID = SDL_GetWindowID(windowPointer);
			shown = true;
		}
	}
}

vector<TextureReturn> WINDOW::HandleEvent(SDL_Event & SDLEvent)
{
	vector<TextureReturn> events;
	if (SDLEvent.type == SDL_WINDOWEVENT && SDLEvent.window.windowID == windowID) {
		if (SDLEvent.window.event == SDL_WINDOWEVENT_SHOWN) {
			shown = true;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_HIDDEN) {
			shown = false;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			windowWidth = SDLEvent.window.data1;
			windowHeight = SDLEvent.window.data2;
			SDL_RenderPresent(rendererPointer);
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_EXPOSED) {
			SDL_RenderPresent(rendererPointer);
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_ENTER) {
			mouseFocus = true;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_LEAVE) {
			mouseFocus = false;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			keyboardFocus = true;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			keyboardFocus = false;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_MINIMIZED) {
			minimized = true;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
			minimized = false;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_RESTORED) {
			minimized = false;
		}
		if (SDLEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
			SDL_HideWindow(windowPointer);
		}
	}
	if (SDLEvent.type == SDL_KEYDOWN || SDLEvent.type == SDL_KEYUP) {
	}

	if (SDLEvent.type == SDL_MOUSEMOTION || SDLEvent.type == SDL_MOUSEBUTTONDOWN || SDLEvent.type == SDL_MOUSEBUTTONUP) {
		for (unsigned a = 0; a < textures.size(); a++) {
			if (textures[a].objectType == 3) {
				if (textures[a].CheckButton(&SDLEvent) == true) {
					TextureReturn newEvent;
					newEvent.TexturePointer = a;
					newEvent.value = 1;
					events.push_back(newEvent);
				}
			}
		}
	}
	return(events);
}

void WINDOW::Focus()
{
	if (shown == false) {
		SDL_ShowWindow(windowPointer);
	}
	SDL_RaiseWindow(windowPointer);
}

void WINDOW::Render()
{
	if (minimized != true) {
		//SDL_SetRenderDrawColor(rendererPointer, 255, 255, 255, 255);
		SDL_RenderClear(rendererPointer);
		for (unsigned int a = 0; a < textures.size(); a++) {
			textures[a].Render();
		}
		SDL_RenderPresent(rendererPointer);
	}
}

void WINDOW::TerminateWindow()
{
	if (windowPointer != NULL) {
		SDL_DestroyWindow(windowPointer);
		LOGGING::LogSuccess("Terminated " + windowName, "Windows.cpp/WINDOW/TerminateWindow");
	}
	windowPointer = NULL;
	mouseFocus = false;
	keyboardFocus = false;
	windowWidth = 0;
	windowHeight = 0;
}

void WINDOW::GetSize(int & width, int & height)
{
	width = windowWidth;
	height = windowHeight;
}

bool WINDOW::GetBool(int value)
{
	if (value == 1) {
		return(mouseFocus);
	}
	else if (value == 2) {
		return(keyboardFocus);
	}
	else if (value == 3) {
		return(fullScreen);
	}
	else if (value == 4) {
		return(minimized);
	}
	else if (value == 5) {
		return(shown);
	}
	else {
		return(false);
	}
}