#include "snake.h"

int main() {
    Game game;
    Snake snake;
    Food food;
    
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    
    // Configurer le terminal
    setup_terminal();
    
    // Initialiser le jeu
    init_game(&game, &snake, &food);
    
    printf("=== SNAKE GAME ===\n");
    printf("Utilisez WASD pour vous déplacer\n");
    printf("Q pour quitter\n");
    printf("Appuyez sur une touche pour commencer...\n");
    getchar();
    
    // Boucle principale du jeu
    while (!game.game_over) {
        // Gestion des entrées
        handle_input(&snake, &game);
        
        // Mise à jour du serpent
        update_snake(&snake);
        
        // Vérification des collisions
        if (check_collision(&snake, game.width, game.height)) {
            game.game_over = 1;
            continue;
        }
        
        // Vérification si le serpent mange la nourriture
        if (check_food_collision(&snake, &food)) {
            grow_snake(&snake);
            game.score += 10;
            food.active = 0;
            spawn_food(&food, &snake, game.width, game.height);
        }
        
        // Affichage
        clear_screen();
        draw_game(&game, &snake, &food);
        
        // Contrôle de la vitesse du jeu
        msleep(150); // 150ms
    }
    
    // Écran de fin
    clear_screen();
    printf("=== GAME OVER ===\n");
    printf("Score final: %d\n", game.score);
    printf("Longueur du serpent: %d\n", snake.length);
    printf("Merci d'avoir joué !\n");
    
    // Nettoyage
    restore_terminal();
    free_snake(&snake);
    
    return 0;
}