#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_COMETS 20
#define MAX_STARS 100
#define MAX_PARTICLES 200
#define M_PI 3.14159265358979323846


typedef struct {
    float x, y;
    float vx, vy;
    int size;
    int active;
} Comet;

typedef struct {
    float x, y;
    float speed;
    int brightness;
} Star;

typedef struct {
    float x, y;
    float vx, vy;
    int life;
    Uint8 r, g, b;
} Particle;

typedef struct {
    float x, y;
    float vx, vy;
    int width, height;
    int lives;
    int invulnerable;
} Rocket;

// Globals
Comet comets[MAX_COMETS];
Star stars[MAX_STARS];
Particle particles[MAX_PARTICLES];
Rocket rocket;
int score = 0;
int game_over = 0;

void init_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x = rand() % WIDTH;
        stars[i].y = rand() % HEIGHT;
        stars[i].speed = 0.5f + (rand() % 20) / 10.0f;
        stars[i].brightness = 100 + rand() % 156;
    }
}

void update_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].y += stars[i].speed;
        if (stars[i].y > HEIGHT) {
            stars[i].y = 0;
            stars[i].x = rand() % WIDTH;
        }
    }
}

void draw_stars(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_STARS; i++) {
        SDL_SetRenderDrawColor(renderer, stars[i].brightness, stars[i].brightness, stars[i].brightness, 255);
        SDL_RenderDrawPoint(renderer, (int)stars[i].x, (int)stars[i].y);
    }
}

void spawn_particle(float x, float y, Uint8 r, Uint8 g, Uint8 b) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life <= 0) {
            particles[i].x = x;
            particles[i].y = y;
            float angle = (rand() % 360) * M_PI / 180.0f;
            float speed = 1.0f + (rand() % 30) / 10.0f;
            particles[i].vx = cos(angle) * speed;
            particles[i].vy = sin(angle) * speed;
            particles[i].life = 30 + rand() % 30;
            particles[i].r = r;
            particles[i].g = g;
            particles[i].b = b;
            break;
        }
    }
}

void update_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].life--;
        }
    }
}

void draw_particles(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            Uint8 alpha = (particles[i].life * 255) / 60;
            SDL_SetRenderDrawColor(renderer, particles[i].r, particles[i].g, particles[i].b, alpha);
            SDL_Rect rect = {(int)particles[i].x - 2, (int)particles[i].y - 2, 4, 4};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void init_rocket() {
    rocket.x = WIDTH / 2;
    rocket.y = HEIGHT - 100;
    rocket.vx = 0;
    rocket.vy = 0;
    rocket.width = 30;
    rocket.height = 40;
    rocket.lives = 3;
    rocket.invulnerable = 0;
}

void draw_rocket(SDL_Renderer* renderer) {
    if (rocket.invulnerable > 0 && (rocket.invulnerable / 10) % 2 == 0) {
        return; // Clignotement
    }
    
    // Corps de la fusée
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect body = {(int)rocket.x - 10, (int)rocket.y - 15, 20, 30};
    SDL_RenderFillRect(renderer, &body);
    
    // Nez de la fusée
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_Point nose[3] = {
        {(int)rocket.x, (int)rocket.y - 25},
        {(int)rocket.x - 10, (int)rocket.y - 15},
        {(int)rocket.x + 10, (int)rocket.y - 15}
    };
    SDL_RenderDrawLines(renderer, nose, 3);
    
    // Flammes
    if (abs((int)rocket.vx) > 0.1f || abs((int)rocket.vy) > 0.1f) {
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
        int flame_size = rand() % 5 + 5;
        SDL_Rect flame = {(int)rocket.x - 5, (int)rocket.y + 15, 10, flame_size};
        SDL_RenderFillRect(renderer, &flame);
        
        spawn_particle(rocket.x, rocket.y + 20, 255, 150, 0);
    }
    
    // Ailerons
    SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
    SDL_Rect left_wing = {(int)rocket.x - 15, (int)rocket.y, 5, 10};
    SDL_Rect right_wing = {(int)rocket.x + 10, (int)rocket.y, 5, 10};
    SDL_RenderFillRect(renderer, &left_wing);
    SDL_RenderFillRect(renderer, &right_wing);
}

void init_comets() {
    for (int i = 0; i < MAX_COMETS; i++) {
        comets[i].active = 0;
    }
}

void spawn_comet() {
    for (int i = 0; i < MAX_COMETS; i++) {
        if (!comets[i].active) {
            comets[i].x = rand() % WIDTH;
            comets[i].y = -50;
            comets[i].vx = (rand() % 30 - 15) / 10.0f;
            comets[i].vy = 2.0f + (score / 500.0f); // Difficulté progressive
            comets[i].size = 15 + rand() % 25;
            comets[i].active = 1;
            break;
        }
    }
}

void update_comets() {
    for (int i = 0; i < MAX_COMETS; i++) {
        if (comets[i].active) {
            comets[i].x += comets[i].vx;
            comets[i].y += comets[i].vy;
            
            if (comets[i].y > HEIGHT + 50) {
                comets[i].active = 0;
                score += 10;
            }
        }
    }
}

void draw_comets(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_COMETS; i++) {
        if (comets[i].active) {
            // Comète principale
            SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
            for (int j = 0; j < comets[i].size / 2; j++) {
                int radius = comets[i].size - j * 2;
                for (int angle = 0; angle < 360; angle += 30) {
                    int px = (int)(comets[i].x + cos(angle * M_PI / 180.0) * radius);
                    int py = (int)(comets[i].y + sin(angle * M_PI / 180.0) * radius);
                    SDL_RenderDrawPoint(renderer, px, py);
                }
            }
            
            // Traînée
            SDL_SetRenderDrawColor(renderer, 255, 150, 50, 100);
            for (int t = 0; t < 3; t++) {
                SDL_Rect trail = {
                    (int)comets[i].x - 5 + rand() % 10,
                    (int)comets[i].y - 20 - t * 10,
                    5,
                    10
                };
                SDL_RenderFillRect(renderer, &trail);
            }
        }
    }
}

int check_collision() {
    if (rocket.invulnerable > 0) return 0;
    
    for (int i = 0; i < MAX_COMETS; i++) {
        if (comets[i].active) {
            float dx = rocket.x - comets[i].x;
            float dy = rocket.y - comets[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            
            if (distance < (rocket.width / 2 + comets[i].size / 2)) {
                comets[i].active = 0;
                
                // Explosion
                for (int p = 0; p < 20; p++) {
                    spawn_particle(comets[i].x, comets[i].y, 255, 100, 0);
                }
                
                return 1;
            }
        }
    }
    return 0;
}

void draw_hud(SDL_Renderer* renderer) {
    // Vies
    for (int i = 0; i < rocket.lives; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_Rect heart = {10 + i * 35, 10, 30, 30};
        SDL_RenderFillRect(renderer, &heart);
    }
    
    // Score (simple representation)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int bar_width = (score / 10) % WIDTH;
    SDL_Rect score_bar = {0, 0, bar_width, 3};
    SDL_RenderFillRect(renderer, &score_bar);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Space Rocket", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    init_rocket();
    init_comets();
    init_stars();
    
    int running = 1;
    int spawn_timer = 0;
    Uint32 last_time = SDL_GetTicks();
    
    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && game_over) {
                // Restart
                init_rocket();
                init_comets();
                score = 0;
                game_over = 0;
            }
        }
        
        if (!game_over) {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            
            // Contrôles
            rocket.vx *= 0.9f; // Friction
            rocket.vy *= 0.9f;
            
            if (keys[SDL_SCANCODE_LEFT]) rocket.vx -= 0.5f;
            if (keys[SDL_SCANCODE_RIGHT]) rocket.vx += 0.5f;
            if (keys[SDL_SCANCODE_UP]) rocket.vy -= 0.5f;
            if (keys[SDL_SCANCODE_DOWN]) rocket.vy += 0.5f;
            
            rocket.x += rocket.vx;
            rocket.y += rocket.vy;
            
            // Limites
            if (rocket.x < 20) rocket.x = 20;
            if (rocket.x > WIDTH - 20) rocket.x = WIDTH - 20;
            if (rocket.y < 30) rocket.y = 30;
            if (rocket.y > HEIGHT - 30) rocket.y = HEIGHT - 30;
            
            // Spawn comètes
            spawn_timer++;
            if (spawn_timer > 60 - (score / 200)) {
                spawn_comet();
                spawn_timer = 0;
            }
            
            update_comets();
            update_stars();
            update_particles();
            
            if (rocket.invulnerable > 0) rocket.invulnerable--;
            
            if (check_collision()) {
                rocket.lives--;
                rocket.invulnerable = 120;
                if (rocket.lives <= 0) {
                    game_over = 1;
                }
            }
            
            score++;
        }
        
        // Rendu
        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
        SDL_RenderClear(renderer);
        
        draw_stars(renderer);
        draw_particles(renderer);
        draw_comets(renderer);
        draw_rocket(renderer);
        draw_hud(renderer);
        
        if (game_over) {
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            SDL_Rect game_over_bg = {WIDTH/2 - 150, HEIGHT/2 - 50, 300, 100};
            SDL_RenderFillRect(renderer, &game_over_bg);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}