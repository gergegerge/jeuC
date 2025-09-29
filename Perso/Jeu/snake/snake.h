#ifndef SNAKE_H
#define SNAKE_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

// Constantes
#define INITIAL_CAPACITY 10
#define BOARD_WIDTH 40
#define BOARD_HEIGHT 20

// Directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// Structures
typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *body;
    int length;
    int capacity;
    int direction;
} Snake;

typedef struct {
    Point position;
    int active;
} Food;

typedef struct {
    int width, height;
    int score;
    int game_over;
} Game;

// Prototypes des fonctions
void init_game(Game *game, Snake *snake, Food *food);
void init_snake(Snake *snake, int start_x, int start_y);
void spawn_food(Food *food, Snake *snake, int width, int height);
void update_snake(Snake *snake);
int check_collision(Snake *snake, int width, int height);
int check_food_collision(Snake *snake, Food *food);
void grow_snake(Snake *snake);
void change_direction(Snake *snake, int new_direction);
void clear_screen();
void draw_game(Game *game, Snake *snake, Food *food);
void setup_terminal();
void restore_terminal();
char get_key();
void free_snake(Snake *snake);
void handle_input(Snake *snake, Game *game);
void msleep(int milliseconds);

#endif