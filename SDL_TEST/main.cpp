#include <iostream>
#include <SDL2/SDL.h>

const int WIDTH = 800, HEIGHT = 600;

int main( int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("SDL Hello World",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL){
        std::cout << "Could now create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    int a = 0;
    SDL_Event windowEvent;
    while (true)
    {
        a++;
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
        }


        // Background Color
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Rect rect{
            .x = a,
            .y = 0,
            .w = 100,
            .h = 80
        };

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}