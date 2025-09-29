#include "snake.h"

// Variables globales pour la configuration du terminal
struct termios original_termios;

void setup_terminal() {
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios term = original_termios;
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

char get_key() {
    char ch;
    if (read(STDIN_FILENO, &ch, 1) == 1) {
        return ch;
    }
    return 0;
}

void clear_screen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void init_game(Game *game, Snake *snake, Food *food) {
    game->width = BOARD_WIDTH;
    game->height = BOARD_HEIGHT;
    game->score = 0;
    game->game_over = 0;
    
    init_snake(snake, game->width / 2, game->height / 2);
    spawn_food(food, snake, game->width, game->height);
}

void init_snake(Snake *snake, int start_x, int start_y) {
    snake->body = malloc(INITIAL_CAPACITY * sizeof(Point));
    snake->capacity = INITIAL_CAPACITY;
    snake->length = 3;
    snake->direction = RIGHT;
    
    // Initialiser le serpent avec 3 segments
    snake->body[0].x = start_x;     snake->body[0].y = start_y;     // Tête
    snake->body[1].x = start_x - 1; snake->body[1].y = start_y;     // Corps
    snake->body[2].x = start_x - 2; snake->body[2].y = start_y;     // Queue
}

void spawn_food(Food *food, Snake *snake, int width, int height) {
    int valid_position = 0;
    
    while (!valid_position) {
        food->position.x = rand() % (width - 2) + 1;  // Éviter les bordures
        food->position.y = rand() % (height - 2) + 1;
        
        valid_position = 1;
        // Vérifier que la nourriture ne spawne pas sur le serpent
        for (int i = 0; i < snake->length; i++) {
            if (snake->body[i].x == food->position.x && 
                snake->body[i].y == food->position.y) {
                valid_position = 0;
                break;
            }
        }
    }
    food->active = 1;
}

void update_snake(Snake *snake) {
    // Calculer la nouvelle position de la tête
    Point new_head = snake->body[0];
    
    switch (snake->direction) {
        case UP:    new_head.y--; break;
        case DOWN:  new_head.y++; break;
        case LEFT:  new_head.x--; break;
        case RIGHT: new_head.x++; break;
    }
    
    // Déplacer le corps (de la queue vers la tête)
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    
    // Placer la nouvelle tête
    snake->body[0] = new_head;
}

int check_collision(Snake *snake, int width, int height) {
    Point head = snake->body[0];
    
    // Collision avec les murs
    if (head.x <= 0 || head.x >= width - 1 || 
        head.y <= 0 || head.y >= height - 1) {
        return 1;
    }
    
    // Auto-collision
    for (int i = 1; i < snake->length; i++) {
        if (head.x == snake->body[i].x && head.y == snake->body[i].y) {
            return 1;
        }
    }
    
    return 0;
}

int check_food_collision(Snake *snake, Food *food) {
    if (!food->active) return 0;
    
    Point head = snake->body[0];
    return (head.x == food->position.x && head.y == food->position.y);
}

void grow_snake(Snake *snake) {
    // Vérifier si on a besoin d'agrandir le tableau
    if (snake->length >= snake->capacity) {
        snake->capacity *= 2;
        snake->body = realloc(snake->body, snake->capacity * sizeof(Point));
    }
    
    // Ajouter un nouveau segment à la queue
    Point tail = snake->body[snake->length - 1];
    snake->body[snake->length] = tail;
    snake->length++;
}

void change_direction(Snake *snake, int new_direction) {
    // Empêcher le serpent de faire demi-tour
    if ((snake->direction == UP && new_direction == DOWN) ||
        (snake->direction == DOWN && new_direction == UP) ||
        (snake->direction == LEFT && new_direction == RIGHT) ||
        (snake->direction == RIGHT && new_direction == LEFT)) {
        return;
    }
    snake->direction = new_direction;
}

void draw_game(Game *game, Snake *snake, Food *food) {
    // Créer un tableau pour représenter l'écran
    char screen[BOARD_HEIGHT][BOARD_WIDTH];
    
    // Initialiser l'écran
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            if (x == 0 || x == game->width - 1 || 
                y == 0 || y == game->height - 1) {
                screen[y][x] = '#';  // Bordures
            } else {
                screen[y][x] = ' ';  // Espaces vides
            }
        }
    }
    
    // Dessiner la nourriture
    if (food->active) {
        screen[food->position.y][food->position.x] = '*';
    }
    
    // Dessiner le serpent
    for (int i = 0; i < snake->length; i++) {
        int x = snake->body[i].x;
        int y = snake->body[i].y;
        if (x >= 0 && x < game->width && y >= 0 && y < game->height) {
            screen[y][x] = (i == 0) ? 'O' : 'o';  // Tête et corps
        }
    }
    
    // Afficher l'écran
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
    
    // Afficher le score
    printf("\nScore: %d\n", game->score);
    printf("Utilisez WASD pour vous déplacer, Q pour quitter\n");
}

void handle_input(Snake *snake, Game *game) {
    char key = get_key();
    
    switch (key) {
        case 'z': case 'Z':
            change_direction(snake, UP);
            break;
        case 's': case 'S':
            change_direction(snake, DOWN);
            break;
        case 'q': case 'Q':
            change_direction(snake, LEFT);
            break;
        case 'd': case 'D':
            change_direction(snake, RIGHT);
            break;
        case 'e': case 'E':
            game->game_over = 1;
            break;
    }
}

void free_snake(Snake *snake) {
    free(snake->body);
    snake->body = NULL;
    snake->length = 0;
    snake->capacity = 0;
}

void msleep(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}