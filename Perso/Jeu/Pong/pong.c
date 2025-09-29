#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Constantes du jeu
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 120
#define BALL_SIZE 20
#define PADDLE_SPEED 8
#define INITIAL_BALL_SPEED 6
#define MAX_BALL_SPEED 12

// Structure pour la balle
typedef struct {
    float x, y;
    float dx, dy;
    int size;
} Ball;

// Structure pour les raquettes
typedef struct {
    float x, y;
    int width, height;
    int score;
} Paddle;

// Structure pour les particules d'effet
typedef struct {
    float x, y;
    float dx, dy;
    int life;
    int max_life;
} Particle;

// Variables globales
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Ball ball;
Paddle player1, player2;
Particle particles[50];
int num_particles = 0;

// Couleurs
SDL_Color white = {255, 255, 255, 255};
SDL_Color cyan = {0, 255, 255, 255};
SDL_Color magenta = {255, 0, 255, 255};
SDL_Color yellow = {255, 255, 0, 255};

// Fonctions de base
bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL ne peut pas s'initialiser! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Pong - Style Néon",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             WINDOW_WIDTH,
                             WINDOW_HEIGHT,
                             SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        printf("La fenêtre ne peut pas être créée! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Le renderer ne peut pas être créé! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void initializeGame() {
    // Initialisation de la balle
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.dx = INITIAL_BALL_SPEED * ((rand() % 2) ? 1 : -1);
    ball.dy = INITIAL_BALL_SPEED * ((rand() % 2) ? 1 : -1);
    ball.size = BALL_SIZE;

    // Initialisation des raquettes
    player1.x = 50;
    player1.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    player1.width = PADDLE_WIDTH;
    player1.height = PADDLE_HEIGHT;
    player1.score = 0;

    player2.x = WINDOW_WIDTH - 50 - PADDLE_WIDTH;
    player2.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    player2.width = PADDLE_WIDTH;
    player2.height = PADDLE_HEIGHT;
    player2.score = 0;

    // Initialisation des particules
    num_particles = 0;
}

void addParticle(float x, float y) {
    if (num_particles < 50) {
        particles[num_particles].x = x;
        particles[num_particles].y = y;
        particles[num_particles].dx = (rand() % 21 - 10) * 0.5f;
        particles[num_particles].dy = (rand() % 21 - 10) * 0.5f;
        particles[num_particles].life = 60;
        particles[num_particles].max_life = 60;
        num_particles++;
    }
}

void updateParticles() {
    for (int i = 0; i < num_particles; i++) {
        particles[i].x += particles[i].dx;
        particles[i].y += particles[i].dy;
        particles[i].life--;
        
        if (particles[i].life <= 0) {
            particles[i] = particles[num_particles - 1];
            num_particles--;
            i--;
        }
    }
}

void drawGlowRect(int x, int y, int w, int h, SDL_Color color) {
    // Effet de lueur avec plusieurs rectangles de transparence décroissante
    for (int i = 0; i < 5; i++) {
        int alpha = color.a * (5 - i) / 5 / 3;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
        SDL_Rect rect = {x - i, y - i, w + 2 * i, h + 2 * i};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Rectangle principal
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void drawPixelScore(int x, int y, int digit) {
    // Matrice 7x9 pour chaque chiffre (style LCD)
    int digits[10][9][7] = {
        // 0
        {{1,1,1,1,1,1,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, 
         {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,1,1,1,1,1,1}},
        // 1
        {{0,0,0,1,0,0,0}, {0,0,1,1,0,0,0}, {0,0,0,1,0,0,0}, {0,0,0,1,0,0,0}, 
         {0,0,0,1,0,0,0}, {0,0,0,1,0,0,0}, {0,0,0,1,0,0,0}, {0,0,0,1,0,0,0}, {0,1,1,1,1,1,0}},
        // 2
        {{1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, 
         {1,1,1,1,1,1,1}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, {1,1,1,1,1,1,1}},
        // 3
        {{1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, 
         {1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {1,1,1,1,1,1,1}},
        // 4
        {{1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, 
         {1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}},
        // 5
        {{1,1,1,1,1,1,1}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, 
         {1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {1,1,1,1,1,1,1}},
        // 6
        {{1,1,1,1,1,1,1}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,0}, 
         {1,1,1,1,1,1,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,1,1,1,1,1,1}},
        // 7
        {{1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, 
         {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}},
        // 8
        {{1,1,1,1,1,1,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, 
         {1,1,1,1,1,1,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,1,1,1,1,1,1}},
        // 9
        {{1,1,1,1,1,1,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, {1,0,0,0,0,0,1}, 
         {1,1,1,1,1,1,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {0,0,0,0,0,0,1}, {1,1,1,1,1,1,1}}
    };
    
    if (digit < 0 || digit > 9) return;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 7; col++) {
            if (digits[digit][row][col]) {
                SDL_Rect pixel = {x + col * 8, y + row * 8, 6, 6};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}
void drawGlowCircle(int x, int y, int radius, SDL_Color color) {
    // Dessine un cercle avec effet de lueur
    for (int i = 0; i < 3; i++) {
        int alpha = color.a * (3 - i) / 3 / 2;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
        
        int current_radius = radius + i * 2;
        for (int w = 0; w < current_radius * 2; w++) {
            for (int h = 0; h < current_radius * 2; h++) {
                int dx = current_radius - w;
                int dy = current_radius - h;
                if ((dx * dx + dy * dy) <= (current_radius * current_radius)) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
}

void updateBall() {
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Collision avec le haut et le bas
    if (ball.y <= ball.size / 2 || ball.y >= WINDOW_HEIGHT - ball.size / 2) {
        ball.dy = -ball.dy;
        addParticle(ball.x, ball.y);
    }

    // Collision avec les raquettes
    SDL_Rect ball_rect = {ball.x - ball.size / 2, ball.y - ball.size / 2, ball.size, ball.size};
    SDL_Rect player1_rect = {player1.x, player1.y, player1.width, player1.height};
    SDL_Rect player2_rect = {player2.x, player2.y, player2.width, player2.height};

    if (SDL_HasIntersection(&ball_rect, &player1_rect) && ball.dx < 0) {
        ball.dx = -ball.dx;
        // Effet de spin basé sur la position de collision
        float hit_pos = (ball.y - (player1.y + player1.height / 2)) / (player1.height / 2);
        ball.dy += hit_pos * 2;
        // Augmentation légère de la vitesse
        if (fabs(ball.dx) < MAX_BALL_SPEED) ball.dx *= 1.05f;
        if (fabs(ball.dy) < MAX_BALL_SPEED) ball.dy *= 1.05f;
        addParticle(ball.x, ball.y);
    }

    if (SDL_HasIntersection(&ball_rect, &player2_rect) && ball.dx > 0) {
        ball.dx = -ball.dx;
        float hit_pos = (ball.y - (player2.y + player2.height / 2)) / (player2.height / 2);
        ball.dy += hit_pos * 2;
        if (fabs(ball.dx) < MAX_BALL_SPEED) ball.dx *= 1.05f;
        if (fabs(ball.dy) < MAX_BALL_SPEED) ball.dy *= 1.05f;
        addParticle(ball.x, ball.y);
    }

    // Score et reset
    if (ball.x < 0) {
        player2.score++;
        ball.x = WINDOW_WIDTH / 2;
        ball.y = WINDOW_HEIGHT / 2;
        ball.dx = INITIAL_BALL_SPEED;
        ball.dy = INITIAL_BALL_SPEED * ((rand() % 2) ? 1 : -1);
    } else if (ball.x > WINDOW_WIDTH) {
        player1.score++;
        ball.x = WINDOW_WIDTH / 2;
        ball.y = WINDOW_HEIGHT / 2;
        ball.dx = -INITIAL_BALL_SPEED;
        ball.dy = INITIAL_BALL_SPEED * ((rand() % 2) ? 1 : -1);
    }
}

void updateAI() {
    // IA simple pour le joueur 2
    float ball_center = ball.y;
    float paddle_center = player2.y + player2.height / 2;
    
    if (ball_center < paddle_center - 10) {
        player2.y -= PADDLE_SPEED * 0.8f; // IA légèrement moins rapide
    } else if (ball_center > paddle_center + 10) {
        player2.y += PADDLE_SPEED * 0.8f;
    }
    
    // Limites de l'écran
    if (player2.y < 0) player2.y = 0;
    if (player2.y > WINDOW_HEIGHT - player2.height) player2.y = WINDOW_HEIGHT - player2.height;
}

void render() {
    // Fond noir avec effet de grille
    SDL_SetRenderDrawColor(renderer, 5, 5, 15, 255);
    SDL_RenderClear(renderer);
    
    // Grille de fond
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    for (int i = 0; i < WINDOW_WIDTH; i += 40) {
        SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT);
    }
    for (int i = 0; i < WINDOW_HEIGHT; i += 40) {
        SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
    }
    
    // Ligne centrale
    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
    for (int i = 0; i < WINDOW_HEIGHT; i += 20) {
        SDL_Rect dash = {WINDOW_WIDTH / 2 - 2, i, 4, 10};
        SDL_RenderFillRect(renderer, &dash);
    }

    // Raquettes avec effet de lueur
    drawGlowRect(player1.x, player1.y, player1.width, player1.height, cyan);
    drawGlowRect(player2.x, player2.y, player2.width, player2.height, magenta);

    // Balle avec effet de lueur
    drawGlowCircle(ball.x, ball.y, ball.size / 2, yellow);

    // Particules
    for (int i = 0; i < num_particles; i++) {
        int alpha = 255 * particles[i].life / particles[i].max_life;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
        SDL_Rect particle_rect = {particles[i].x - 2, particles[i].y - 2, 4, 4};
        SDL_RenderFillRect(renderer, &particle_rect);
    }

    // Score avec chiffres pixelisés
    drawPixelScore(WINDOW_WIDTH / 4 - 30, 80, player1.score);
    drawPixelScore(3 * WINDOW_WIDTH / 4 - 30, 80, player2.score);

    SDL_RenderPresent(renderer);
}

void cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initialize()) {
        printf("Échec de l'initialisation!\n");
        return 1;
    }

    initializeGame();

    bool running = true;
    SDL_Event event;
    const Uint8* keystate;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Gestion des entrées clavier
        keystate = SDL_GetKeyboardState(NULL);
        
        // Joueur 1 (flèches ou WASD)
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
            player1.y -= PADDLE_SPEED;
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
            player1.y += PADDLE_SPEED;
        }
        
        // Limites pour le joueur 1
        if (player1.y < 0) player1.y = 0;
        if (player1.y > WINDOW_HEIGHT - player1.height) player1.y = WINDOW_HEIGHT - player1.height;

        // Mise à jour du jeu
        updateBall();
        updateAI();
        updateParticles();

        // Rendu
        render();
        
        SDL_Delay(16); // ~60 FPS
    }

    cleanup();
    return 0;
}