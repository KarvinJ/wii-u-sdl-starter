#include <whb/proc.h>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_STICKL 4
#define BUTTON_STICKR 5
#define BUTTON_L 6
#define BUTTON_R 7
#define BUTTON_ZL 8
#define BUTTON_ZR 9
#define BUTTON_PLUS 10
#define BUTTON_MINUS 11
#define BUTTON_LEFT 12
#define BUTTON_UP 13
#define BUTTON_RIGHT 14
#define BUTTON_DOWN 15
#define BUTTON_COUNT 16

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_GameController *controller = nullptr;

bool isGamePaused;

const int SPEED = 600;

SDL_Rect player = {SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 - 64, 64, 64};

bool shouldCloseTheGame;

void quitGame()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    WHBProcShutdown();
}

void handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            shouldCloseTheGame = true;
        }

        if (event.type == SDL_JOYBUTTONDOWN)
        {
            if (event.jbutton.button == BUTTON_MINUS)
            {
                shouldCloseTheGame = true;
            }

            if (event.jbutton.button == BUTTON_PLUS)
            {
                isGamePaused = !isGamePaused;
            }
        }
    }
}

void update(float deltaTime)
{
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) && player.y > 0)
    {
        player.y -= SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) && player.y < SCREEN_HEIGHT - player.h)
    {
        player.y += SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) && player.x > 0)
    {
        player.x -= SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && player.x < SCREEN_WIDTH - player.w)
    {
        player.x += SPEED * deltaTime;
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &player);

    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
    WHBProcInit();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    window = SDL_CreateWindow("Wii U SDL Starter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (SDL_NumJoysticks() < 1)
    {
        printf("No game controllers connected!\n");
        return -1;
    }
    else
    {
        controller = SDL_GameControllerOpen(0);
        if (controller == NULL)
        {
            printf("Unable to open game controller! SDL Error: %s\n", SDL_GetError());
            return -1;
        }
    }

    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    while (!shouldCloseTheGame && WHBProcIsRunning())
    {
        currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
        previousFrameTime = currentFrameTime;

        SDL_GameControllerUpdate();

        handleEvents();
        update(deltaTime);
        render();
    }

    quitGame();
}
