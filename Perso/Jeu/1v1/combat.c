#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define MAX_HP 100
#define MAX_STAMINA 100
#define GRAVITY 0.8
#define JUMP_POWER -15
#define MOVE_SPEED 6
#define DASH_SPEED 15

typedef struct {
    char nom[20];
    int hp;
    int stamina;
    float x, y;
    float vx, vy;
    bool onGround;
    bool facingRight;
    SDL_Color couleur;
    int cooldownAttack;
    int cooldownDash;
    int cooldownBlock;
    bool isBlocking;
    bool isDashing;
    int dashTimer;
    int hitStun;
    int combo;
    bool hasDoubleJump;
} Joueur;

typedef struct {
    float x, y;
    float vx, vy;
    int owner;
    bool active;
    int timer;
} Projectile;

#define MAX_PROJECTILES 10
Projectile projectiles[MAX_PROJECTILES];

void initialiserJoueur(Joueur *j, const char *nom, float x, float y, SDL_Color c) {
    snprintf(j->nom, sizeof(j->nom), "%s", nom);
    j->hp = MAX_HP;
    j->stamina = MAX_STAMINA;
    j->x = x;
    j->y = y;
    j->vx = 0;
    j->vy = 0;
    j->onGround = false;
    j->facingRight = true;
    j->couleur = c;
    j->cooldownAttack = 0;
    j->cooldownDash = 0;
    j->cooldownBlock = 0;
    j->isBlocking = false;
    j->isDashing = false;
    j->dashTimer = 0;
    j->hitStun = 0;
    j->combo = 0;
    j->hasDoubleJump = true;
}

void creerProjectile(float x, float y, bool right, int owner) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].vx = right ? 12 : -12;
            projectiles[i].vy = 0;
            projectiles[i].owner = owner;
            projectiles[i].active = true;
            projectiles[i].timer = 100;
            break;
        }
    }
}

void dessinerBarre(SDL_Renderer *renderer, int x, int y, int w, int h, 
                   float ratio, SDL_Color c) {
    SDL_Rect fond = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &fond);
    
    SDL_Rect barre = {x + 2, y + 2, (int)((w - 4) * ratio), h - 4};
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(renderer, &barre);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &fond);
}

void dessinerTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte,
                   int x, int y, SDL_Color couleur) {
    if (!font) return;
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, couleur);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void dessinerJoueur(SDL_Renderer *renderer, Joueur *j) {
    int size = 30;
    
    // Effet de dash
    if (j->isDashing) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, j->couleur.r, j->couleur.g, j->couleur.b, 100);
        SDL_Rect trail = {(int)j->x - size - 10, (int)j->y - size - 5, size * 2 + 20, size * 2 + 10};
        SDL_RenderFillRect(renderer, &trail);
    }
    
    // Corps
    SDL_Rect corps = {(int)j->x - size, (int)j->y - size, size * 2, size * 2};
    SDL_SetRenderDrawColor(renderer, j->couleur.r, j->couleur.g, j->couleur.b, 255);
    SDL_RenderFillRect(renderer, &corps);
    
    // Tête
    SDL_Rect tete = {(int)j->x - size/2, (int)j->y - size - 15, size, size};
    SDL_RenderFillRect(renderer, &tete);
    
    // Bras (direction)
    int brasX = j->facingRight ? size : -size;
    SDL_Rect bras = {(int)j->x + brasX/2, (int)j->y - 5, size, 10};
    SDL_RenderFillRect(renderer, &bras);
    
    // Effet de blocage
    if (j->isBlocking) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 150);
        SDL_Rect shield = {(int)j->x - size - 5, (int)j->y - size - 20, size * 2 + 10, size * 2 + 35};
        SDL_RenderDrawRect(renderer, &shield);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){shield.x+1, shield.y+1, shield.w-2, shield.h-2});
    }
    
    // Indicateur de stun
    if (j->hitStun > 0) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for (int i = 0; i < 3; i++) {
            int angle = i * 120 + (j->hitStun * 10);
            int px = (int)j->x + (int)(cos(angle * 3.14159 / 180) * 40);
            int py = (int)j->y - 40 + (int)(sin(angle * 3.14159 / 180) * 40);
            SDL_Rect star = {px - 3, py - 3, 6, 6};
            SDL_RenderFillRect(renderer, &star);
        }
    }
}

void updateJoueur(Joueur *j, const Uint8 *keys, int up, int down, int left, int right, 
                  int attack, int dash, int block, int special) {
    if (j->hitStun > 0) {
        j->hitStun--;
        return;
    }
    
    // Cooldowns
    if (j->cooldownAttack > 0) j->cooldownAttack--;
    if (j->cooldownDash > 0) j->cooldownDash--;
    if (j->cooldownBlock > 0) j->cooldownBlock--;
    if (j->dashTimer > 0) j->dashTimer--;
    else j->isDashing = false;
    
    // Régénération stamina
    if (j->stamina < MAX_STAMINA && !j->isBlocking) j->stamina++;
    
    // Mouvement horizontal
    if (!j->isDashing) {
        if (keys[left] && !j->isBlocking) {
            j->vx = -MOVE_SPEED;
            j->facingRight = false;
        } else if (keys[right] && !j->isBlocking) {
            j->vx = MOVE_SPEED;
            j->facingRight = true;
        } else {
            j->vx *= 0.8;
        }
    }
    
    // Saut
    if (keys[up] && j->onGround && !j->isBlocking) {
        j->vy = JUMP_POWER;
        j->onGround = false;
        j->hasDoubleJump = true;
    }
    // Double saut
    else if (keys[up] && !j->onGround && j->hasDoubleJump && j->vy > 0) {
        j->vy = JUMP_POWER * 0.8;
        j->hasDoubleJump = false;
    }
    
    // Dash
    if (keys[dash] && j->cooldownDash == 0 && j->stamina >= 20) {
        j->isDashing = true;
        j->dashTimer = 10;
        j->vx = j->facingRight ? DASH_SPEED : -DASH_SPEED;
        j->cooldownDash = 30;
        j->stamina -= 20;
    }
    
    // Blocage
    j->isBlocking = false;
    if (keys[block] && j->onGround && j->stamina >= 1) {
        j->isBlocking = true;
        j->vx = 0;
        j->stamina--;
    }
    
    // Gravité
    if (!j->onGround) {
        j->vy += GRAVITY;
    }
    
    // Limiter vitesse verticale
    if (j->vy > 20) j->vy = 20;
    
    // Mise à jour position
    j->x += j->vx;
    j->y += j->vy;
    
    // Sol
    if (j->y >= 500) {
        j->y = 500;
        j->vy = 0;
        j->onGround = true;
        j->hasDoubleJump = true;
    } else {
        j->onGround = false;
    }
    
    // Limites écran
    if (j->x < 40) j->x = 40;
    if (j->x > WINDOW_WIDTH - 40) j->x = WINDOW_WIDTH - 40;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_Window *window = SDL_CreateWindow("Combat Arena 1v1",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    TTF_Font *bigFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 48);
    
    Joueur j1, j2;
    initialiserJoueur(&j1, "P1", 200, 500, (SDL_Color){255, 80, 80, 255});
    initialiserJoueur(&j2, "P2", 800, 500, (SDL_Color){80, 120, 255, 255});
    
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
    
    bool running = true;
    bool jeuTermine = false;
    char message[100] = "";
    int messageTimer = 0;
    
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            
            if (!jeuTermine && event.type == SDL_KEYDOWN) {
                // Attaque J1 (F)
                if (event.key.keysym.sym == SDLK_f && j1.cooldownAttack == 0) {
                    float dist = fabs(j1.x - j2.x);
                    if (dist < 80) {
                        int degats = 8 + rand() % 7;
                        if (j2.isBlocking) {
                            degats /= 3;
                            snprintf(message, sizeof(message), "BLOQUE!");
                        } else {
                            j2.hitStun = 10;
                            j1.combo++;
                            snprintf(message, sizeof(message), "HIT! x%d", j1.combo);
                        }
                        j2.hp -= degats;
                        messageTimer = 30;
                    }
                    j1.cooldownAttack = 15;
                }
                // Attaque spéciale J1 (G)
                else if (event.key.keysym.sym == SDLK_g && j1.stamina >= 30) {
                    creerProjectile(j1.x, j1.y - 15, j1.facingRight, 1);
                    j1.stamina -= 30;
                    j1.combo = 0;
                }
                
                // Attaque J2 (K)
                if (event.key.keysym.sym == SDLK_k && j2.cooldownAttack == 0) {
                    float dist = fabs(j1.x - j2.x);
                    if (dist < 80) {
                        int degats = 8 + rand() % 7;
                        if (j1.isBlocking) {
                            degats /= 3;
                            snprintf(message, sizeof(message), "BLOQUE!");
                        } else {
                            j1.hitStun = 10;
                            j2.combo++;
                            snprintf(message, sizeof(message), "HIT! x%d", j2.combo);
                        }
                        j1.hp -= degats;
                        messageTimer = 30;
                    }
                    j2.cooldownAttack = 15;
                }
                // Attaque spéciale J2 (L)
                else if (event.key.keysym.sym == SDLK_l && j2.stamina >= 30) {
                    creerProjectile(j2.x, j2.y - 15, j2.facingRight, 2);
                    j2.stamina -= 30;
                    j2.combo = 0;
                }
            }
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
        if (!jeuTermine) {
            // J1: WASD + F(attaque) + G(special) + Shift(dash) + Space(block)
            updateJoueur(&j1, keys, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, 
                        SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_LSHIFT, 
                        SDL_SCANCODE_SPACE, SDL_SCANCODE_G);
            
            // J2: Flèches + K(attaque) + L(special) + RShift(dash) + Enter(block)
            updateJoueur(&j2, keys, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, 
                        SDL_SCANCODE_RIGHT, SDL_SCANCODE_K, SDL_SCANCODE_RSHIFT, 
                        SDL_SCANCODE_RETURN, SDL_SCANCODE_L);
            
            // Reset combo si trop loin
            if (fabs(j1.x - j2.x) > 150) {
                j1.combo = 0;
                j2.combo = 0;
            }
            
            // Projectiles
            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (projectiles[i].active) {
                    projectiles[i].x += projectiles[i].vx;
                    projectiles[i].timer--;
                    
                    if (projectiles[i].timer <= 0 || projectiles[i].x < 0 || projectiles[i].x > WINDOW_WIDTH) {
                        projectiles[i].active = false;
                        continue;
                    }
                    
                    // Collision avec joueurs
                    if (projectiles[i].owner == 1) {
                        if (fabs(projectiles[i].x - j2.x) < 40 && fabs(projectiles[i].y - j2.y) < 40) {
                            if (!j2.isBlocking) {
                                j2.hp -= 15;
                                j2.hitStun = 15;
                                snprintf(message, sizeof(message), "FIREBALL!");
                                messageTimer = 30;
                            }
                            projectiles[i].active = false;
                        }
                    } else {
                        if (fabs(projectiles[i].x - j1.x) < 40 && fabs(projectiles[i].y - j1.y) < 40) {
                            if (!j1.isBlocking) {
                                j1.hp -= 15;
                                j1.hitStun = 15;
                                snprintf(message, sizeof(message), "FIREBALL!");
                                messageTimer = 30;
                            }
                            projectiles[i].active = false;
                        }
                    }
                }
            }
        }
        
        if (messageTimer > 0) messageTimer--;
        
        if (!jeuTermine && (j1.hp <= 0 || j2.hp <= 0)) {
            jeuTermine = true;
            if (j1.hp <= 0) snprintf(message, sizeof(message), "JOUEUR 2 GAGNE!");
            else snprintf(message, sizeof(message), "JOUEUR 1 GAGNE!");
            messageTimer = 300;
        }
        
        // Rendu
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255);
        SDL_RenderClear(renderer);
        
        // Sol
        SDL_SetRenderDrawColor(renderer, 60, 40, 30, 255);
        SDL_Rect sol = {0, 520, WINDOW_WIDTH, 80};
        SDL_RenderFillRect(renderer, &sol);
        
        // Ligne de sol
        SDL_SetRenderDrawColor(renderer, 80, 60, 40, 255);
        SDL_RenderDrawLine(renderer, 0, 520, WINDOW_WIDTH, 520);
        
        // Projectiles
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (projectiles[i].active) {
                SDL_Color c = projectiles[i].owner == 1 ? j1.couleur : j2.couleur;
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
                SDL_Rect proj = {(int)projectiles[i].x - 8, (int)projectiles[i].y - 8, 16, 16};
                SDL_RenderFillRect(renderer, &proj);
                SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
                SDL_RenderDrawRect(renderer, &proj);
            }
        }
        
        dessinerJoueur(renderer, &j1);
        dessinerJoueur(renderer, &j2);
        
        // UI Joueur 1
        dessinerTexte(renderer, font, "JOUEUR 1", 20, 10, (SDL_Color){255, 255, 255, 255});
        dessinerBarre(renderer, 20, 35, 250, 25, (float)j1.hp / MAX_HP, (SDL_Color){255, 60, 60, 255});
        dessinerBarre(renderer, 20, 65, 250, 15, (float)j1.stamina / MAX_STAMINA, (SDL_Color){60, 200, 255, 255});
        
        char combo1[30];
        snprintf(combo1, sizeof(combo1), "Combo: x%d", j1.combo);
        dessinerTexte(renderer, font, combo1, 20, 85, (SDL_Color){255, 200, 0, 255});
        
        // UI Joueur 2
        dessinerTexte(renderer, font, "JOUEUR 2", WINDOW_WIDTH - 270, 10, (SDL_Color){255, 255, 255, 255});
        dessinerBarre(renderer, WINDOW_WIDTH - 270, 35, 250, 25, (float)j2.hp / MAX_HP, (SDL_Color){255, 60, 60, 255});
        dessinerBarre(renderer, WINDOW_WIDTH - 270, 65, 250, 15, (float)j2.stamina / MAX_STAMINA, (SDL_Color){60, 200, 255, 255});
        
        char combo2[30];
        snprintf(combo2, sizeof(combo2), "Combo: x%d", j2.combo);
        dessinerTexte(renderer, font, combo2, WINDOW_WIDTH - 270, 85, (SDL_Color){255, 200, 0, 255});
        
        // Contrôles
        dessinerTexte(renderer, font, "P1: WASD+Space(block)+Shift(dash)+F(hit)+G(special)", 
                     20, WINDOW_HEIGHT - 25, (SDL_Color){150, 150, 150, 255});
        dessinerTexte(renderer, font, "P2: Arrows+Enter(block)+RShift(dash)+K(hit)+L(special)", 
                     WINDOW_WIDTH - 520, WINDOW_HEIGHT - 25, (SDL_Color){150, 150, 150, 255});
        
        // Message
        if (messageTimer > 0 && bigFont) {
            dessinerTexte(renderer, bigFont, message, WINDOW_WIDTH/2 - 200, 250, 
                         (SDL_Color){255, 255, 100, 255});
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    if (font) TTF_CloseFont(font);
    if (bigFont) TTF_CloseFont(bigFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}