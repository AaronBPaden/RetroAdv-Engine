#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#include "app.h"

void log_error(char *prefix) { SDL_Log("%s: %s", prefix, SDL_GetError()); }

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    app_t *state = malloc(sizeof(app_t));
    *appstate = state;

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("%s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = SDL_CreateWindow("Retro ADV", 800, 600, 0);

    if (!state->window) {
        log_error("Error initializing window");
        return SDL_APP_FAILURE;
    }

    state->renderer = SDL_CreateRenderer(state->window, NULL);

    if (!state->renderer) {
        log_error("Error loading renderer");
        return SDL_APP_FAILURE;
    }

    state->player_texture = IMG_LoadTexture(state->renderer, "char_spritesheet.png");

    if (!state->player_texture) {
        log_error("Error getting texture");
        return SDL_APP_FAILURE;
    }    

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    app_t *state = (app_t *)appstate;
    SDL_DestroyWindow(state->window);
    free(appstate);
    return;
}
