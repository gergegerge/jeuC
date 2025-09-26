#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 10
#define GRID_WIDTH 80
#define GRID_HEIGHT 60
#define WINDOW_WIDTH (CELL_SIZE * GRID_WIDTH)
#define WINDOW_HEIGHT (CELL_SIZE * GRID_HEIGHT)
#define DELAY_MS 100

int grid[GRID_HEIGHT][GRID_WIDTH];
int next_grid[GRID_HEIGHT][GRID_WIDTH];

// Initialise la grille avec des cellules aléatoires
void init_grid() {
    srand(time(NULL));
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = rand() % 2;
        }
    }
}

// Compte les cellules vivantes autour de (x, y)
int count_neighbors(int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

// Applique les règles du jeu pour la prochaine génération
void update_grid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int neighbors = count_neighbors(x, y);
            if (grid[y][x]) {
                next_grid[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                next_grid[y][x] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    // Copier la nouvelle grille dans l’ancienne
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = next_grid[y][x];
        }
    }
}

// Dessine la grille
void draw_grid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // cellules vivantes en vert

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Jeu de la Vie (SDL2)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window) {
        SDL_Log("Erreur création fenêtre: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur création renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    init_grid();

    bool running = true;
    SDL_Event event;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        update_grid();
        draw_grid(renderer);
        SDL_Delay(DELAY_MS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
