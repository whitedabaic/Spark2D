#include "SDL_Wrappers.h"
#include <iostream>

void SPARK_UTIL::SDL_Destroyer::operator()(SDL_Window* window) const
{
    SDL_DestroyWindow(window);
	std::cout << "SDL_Window destroyed." << std::endl;

}

void SPARK_UTIL::SDL_Destroyer::operator()(SDL_GameController* controller) const
{
    SDL_GameControllerClose(controller);
    controller = nullptr;
    std::cout << "Closed SDL Game Controller!" << std::endl;
}

void SPARK_UTIL::SDL_Destroyer::operator()(Mix_Chunk* chunk) const
{
    Mix_FreeChunk(chunk);
    std::cout << "Freed SDL Mix_Chunk!" << std::endl;
}

void SPARK_UTIL::SDL_Destroyer::operator()(Mix_Music* music) const
{
    Mix_FreeMusic(music);
    std::cout << "Freed SDL Mix_Chunk!" << std::endl;
}

void SPARK_UTIL::SDL_Destroyer::operator()(SDL_Cursor* cursor) const
{
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
    return Cursor();
}
