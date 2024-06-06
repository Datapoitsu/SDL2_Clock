#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <ctime>
#define PI 3.141592654

const int WIDTH = 300, HEIGHT = 300;

int main( int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("SDL Clock",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL){
        std::cout << "Could now create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    double a = 0;
    SDL_Event windowEvent;
    while (true)
    {
        //time_t result = time(nullptr);
        const time_t now = time(nullptr);
        const tm calendar_time = *localtime(std::addressof(now));

        
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
        }


        // Background Color
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        float Seconds = (float)calendar_time.tm_sec;
        float Minutes = (float)calendar_time.tm_min + Seconds / 60;
        float Hours = (float)calendar_time.tm_hour + Minutes / 60;

        //Merkit
        for (int i = 0; i < 60; i ++){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if(i % 5 == 0){
                SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
            }
            float angle = fmod(PI * 2.0 / 60 * i, PI * 2.0);
            SDL_RenderDrawLine(renderer, WIDTH / 2.0 + cos(angle) * 105.0, HEIGHT / 2.0 + sin(angle) * 105.0, WIDTH / 2.0 + cos(angle) * 110.0, HEIGHT / 2.0 + sin(angle) * 110.0);
        }

        // Viisarit
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Hours + 18.0) / 24.0 * PI * 2.0, PI * 2.0)) * 60.0, HEIGHT / 2.0 + sin(fmod((Hours + 18) / 24.0 * PI * 2.0, PI * 2.0)) * 60.0);
        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 80.0, HEIGHT / 2.0 + sin(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 80.0);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 100.0, HEIGHT / 2.0 + sin(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 100.0);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}