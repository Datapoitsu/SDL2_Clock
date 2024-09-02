#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <ctime>
#define PI 3.141592654

// -- Preference variables -- //
int width = 400;
int height = 400;

float hourArrowLength = 0.55;
float minuteArrowLength = 0.75;
float secondArrowLength = 0.80;

float markStartLenght = 0.90;
float markEndLenght = 0.95;
float markFiveStartLenght = 0.85;
float markFiveEndLenght = 0.95;

float boardSize = 1.0f;
float emptySize = 0.1f;
float knobSize = 0.025f;
int ClockBoardCorners = 32;

SDL_Color backGroundColor = {125,125,125,255};
SDL_Color boardColor = {255,255,255,255};
SDL_Color hourArrowColor = {0,0,0,255};
SDL_Color minuteArrowColor = {0,0,0,255};
SDL_Color secondArrowColor = {255,0,0,255};
SDL_Color markerColor = {0,0,0,255};
SDL_Color markerFiveColor = {0,0,0,255};
SDL_Color knobColor = {50,50,50,255};


void *configData[] = { &width, &height, &hourArrowLength, &minuteArrowLength, &secondArrowLength, &markStartLenght, &markEndLenght, &markFiveStartLenght, &markFiveEndLenght, &boardSize,&emptySize,&knobSize, &ClockBoardCorners, &backGroundColor, &boardColor, &hourArrowColor, &minuteArrowColor, &secondArrowColor, &markerColor, &markerFiveColor, &knobColor};

// -- Don't touch variables -- //
int sizeMultiplier = 0;
int LastSecond = 0;

int SetClockSize(SDL_Window *window)
{
    int w, h, size;
    SDL_GetWindowSize(window,&w,&h);
    size = ((float)h - (float)h * emptySize) / 2;
    if(w < h){
        size = ((float)w - (float)w * emptySize) / 2;
    }
    return size;
}

void readConfig()
{
    // -- Reading config -- //
    FILE *fptr;
    fptr = fopen("config.txt", "r");

    if(fptr == NULL)
    {
        return;
    }

    char myString[100];
    memset(&myString, 0, 100);
    int index = 0;
    int doubleDotIndex = -1;
    while(fgets(myString, 99, fptr))
    {
        for(int i = 0; i < strlen(myString); i++)
        {
            if(myString[i] == ':')
            {
                doubleDotIndex = i;
            }
            if(doubleDotIndex != -1 && myString[i] == '.')
            {
                *(float*)configData[index] = atof(&myString[doubleDotIndex + 1]);
                break;
            }
            if(doubleDotIndex != -1 && myString[i] == '{')
            {
                i++;
                unsigned char values[4];
                int valueIndex = 0;
                for(int k = i + 1; k < strlen(myString); k++)
                {
                    if(myString[k] == ',' || myString[k] == '}'){
                        char *holder = (char *)malloc(k-i + 1);
                        memcpy(holder,&myString[i],k-i);
                        holder[k - i] = '\0';
                        values[valueIndex] = atoi(holder);
                        i = k + 1;
                        free(holder);
                        valueIndex++;
                    }
                }
                (*(SDL_Color*)configData[index]) = {values[0], values[1], values[2], values[3]};
                break;
            }
            if(myString[i + 1] == '\0' || myString[i + 1] == '\n' || myString[i + 1] == EOF || i == strlen(myString))
            {
                *(int*)configData[index] = atoi(&myString[doubleDotIndex + 1]);
                break;
            }
        }
        index++;
        doubleDotIndex = -1;
        memset(&myString, 0, 100);
    }
    
    // Close the file
    fclose(fptr); 
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    readConfig();

    SDL_Window *window = SDL_CreateWindow("SDL Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cout << "Could now create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    sizeMultiplier = SetClockSize(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event windowEvent;

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
        if(windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
            sizeMultiplier = SetClockSize(window);
            SDL_GetWindowSize(window,&width,&height);
        }
        
        // -- Calculating the time -- //
        const time_t now = time(nullptr);
        const tm calendar_time = *localtime(std::addressof(now));
        float Seconds = (float)calendar_time.tm_sec;
        
        //Skips rendering if data doesn't change.
        if(Seconds == LastSecond)
        {
            continue;
        }
        LastSecond = Seconds;

        float Minutes = (float)calendar_time.tm_min + Seconds / 60.0; // Adds division of the lower unit, so arrays work like in a real analog clock
        float Hours = fmod(((float)calendar_time.tm_hour + Minutes / 60.0), 12.0);

        // Background Color
        SDL_SetRenderDrawColor(renderer, backGroundColor.r, backGroundColor.g, backGroundColor.b, backGroundColor.a);
        SDL_RenderClear(renderer);

        // -- Clock board -- //
        for (int i = 0; i < ClockBoardCorners; i++)
        {
            SDL_Vertex vertex_1 = {{(float)(width / 2.0), (float)(height / 2.0)}, boardColor, {1, 1}}; //Center vertex
            SDL_Vertex vertex_2 = {{(float)(width / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * i)) * boardSize * sizeMultiplier), (float)(height / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * i)) * boardSize * sizeMultiplier)}, boardColor, {1, 1}};
            SDL_Vertex vertex_3 = {{(float)(width / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * (i + 1))) * boardSize * sizeMultiplier), (float)(height / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * (i + 1))) * boardSize * sizeMultiplier)}, boardColor, {1, 1}};
            SDL_Vertex vertices[] = {vertex_1,vertex_2,vertex_3};
            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
        }

        // -- Marks -- //
        for (int i = 0; i < 60; i ++)
        {
            float angle = fmod(PI * 2.0 / 60 * i, PI * 2.0);

            if(i % 5 != 0) //Not five minute marker
            {
                SDL_RenderDrawLine(renderer, width / 2.0 + cos(angle) * markStartLenght * sizeMultiplier, height / 2.0 + sin(angle) * markStartLenght * sizeMultiplier, width / 2.0 + cos(angle) * markEndLenght * sizeMultiplier, height / 2.0 + sin(angle) * markEndLenght * sizeMultiplier);
            }
            else //Five minute marker
            {
                SDL_SetRenderDrawColor(renderer, markerColor.r, markerColor.g, markerColor.b, markerColor.a);
                SDL_RenderDrawLine(renderer, width / 2.0 + cos(angle) * markFiveStartLenght * sizeMultiplier, height / 2.0 + sin(angle) * markFiveStartLenght * sizeMultiplier, width / 2.0 + cos(angle) * markFiveEndLenght * sizeMultiplier, height / 2.0 + sin(angle) * markFiveEndLenght * sizeMultiplier);
                SDL_SetRenderDrawColor(renderer, markerFiveColor.r, markerFiveColor.g, markerFiveColor.b, markerFiveColor.a); //Resetting the color
            }
        }

        // -- Arrows -- //

        //hour
        SDL_SetRenderDrawColor(renderer, hourArrowColor.r, hourArrowColor.g, hourArrowColor.b, hourArrowColor.a);
        SDL_RenderDrawLine(renderer, width / 2, height / 2, width / 2.0 + cos(fmod((Hours + 9.0) / 12.0 * PI * 2.0, PI * 2.0)) * hourArrowLength * sizeMultiplier, height / 2.0 + sin(fmod((Hours + 9.0) / 12.0 * PI * 2.0, PI * 2.0)) * hourArrowLength * sizeMultiplier);
        //minute
        SDL_SetRenderDrawColor(renderer, minuteArrowColor.r, minuteArrowColor.g, minuteArrowColor.b, minuteArrowColor.a);
        SDL_RenderDrawLine(renderer, width / 2, height / 2, width / 2.0 + cos(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * minuteArrowLength * sizeMultiplier, height / 2.0 + sin(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * minuteArrowLength * sizeMultiplier);
        //second
        SDL_SetRenderDrawColor(renderer, secondArrowColor.r, secondArrowColor.g, secondArrowColor.b, secondArrowColor.a);
        SDL_RenderDrawLine(renderer, width / 2, height / 2, width / 2.0 + cos(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * secondArrowLength * sizeMultiplier, height / 2.0 + sin(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * secondArrowLength * sizeMultiplier);
        
        // -- Knob -- //
        for (int i = 0; i < ClockBoardCorners; i++)
        {
            SDL_Vertex vertex_1 = {{(float)(width / 2.0), (float)(height / 2.0)}, knobColor, {1, 1}};
            SDL_Vertex vertex_2 = {{(float)(width / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * i)) * knobSize * sizeMultiplier), (float)(height / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * i)) * knobSize * sizeMultiplier)}, knobColor, {1, 1}};
            SDL_Vertex vertex_3 = {{(float)(width / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * (i + 1))) * knobSize * sizeMultiplier), (float)(height / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * (i + 1))) * knobSize * sizeMultiplier)}, knobColor, {1, 1}};
            SDL_Vertex vertices[] = {vertex_1,vertex_2,vertex_3};
            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}