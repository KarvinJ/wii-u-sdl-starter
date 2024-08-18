#include "sdl_starter.h"
#include "sdl_assets_loader.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_GameController *controller = nullptr;

bool isGamePaused;

const int PLAYER_SPEED = 600;

Sprite playerSprite;

Mix_Music *music = nullptr;
Mix_Chunk *sound = nullptr;

SDL_Texture *pauseGameTexture = nullptr;
SDL_Rect pauseGameBounds;

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
                Mix_PlayChannel(-1, sound, 0);
            }
        }
    }
}

void update(float deltaTime)
{
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) && playerSprite.textureBounds.y > 0)
    {
        playerSprite.textureBounds.y -= PLAYER_SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) && playerSprite.textureBounds.y < SCREEN_HEIGHT - playerSprite.textureBounds.h)
    {
        playerSprite.textureBounds.y += PLAYER_SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) && playerSprite.textureBounds.x > 0)
    {
        playerSprite.textureBounds.x -= PLAYER_SPEED * deltaTime;
    }

    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && playerSprite.textureBounds.x < SCREEN_WIDTH - playerSprite.textureBounds.w)
    {
        playerSprite.textureBounds.x += PLAYER_SPEED * deltaTime;
    }
}

void renderSprite(Sprite sprite)
{
    SDL_RenderCopy(renderer, sprite.texture, NULL, &sprite.textureBounds);
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    renderSprite(playerSprite);

    if (isGamePaused)
    {
        SDL_RenderCopy(renderer, pauseGameTexture, NULL, &pauseGameBounds);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
    window = SDL_CreateWindow("Wii U SDL Starter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (startSDL(window, renderer) > 0)
    {
        return 1;
    }

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

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

    playerSprite = loadSprite(renderer, "sprites/alien_1.png", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);

    TTF_Font *font = TTF_OpenFont("fonts/LeroyLetteringLightBeta01.ttf", 36);

    // render text as texture
    updateTextureText(pauseGameTexture, "Game Paused", font, renderer);

    SDL_QueryTexture(pauseGameTexture, NULL, NULL, &pauseGameBounds.w, &pauseGameBounds.h);
    pauseGameBounds.x = SCREEN_WIDTH / 2 - pauseGameBounds.w / 2;
    pauseGameBounds.y = 200;

    // no need to keep the font loaded
    TTF_CloseFont(font);

    // load music and sounds from files
    sound = loadSound("sounds/pop1.wav");

    music = loadMusic("music/background.ogg");

    Mix_PlayMusic(music, -1);

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

        if (!isGamePaused)
        {
            update(deltaTime);
        }
        
        render();
    }

    quitGame();
}
