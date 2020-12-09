#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_PLAYERS 2
#define SHARED 0

struct player {
    int id;
    int score;
};

struct player player_data[NUM_PLAYERS];

sem_t mutex;

void *playerThread(void *data) {
    struct player *my_data = (struct player *) data;
    
    // Critical section
    sem_wait(&mutex);

    my_data->score = rand() % 100 + 1;
    printf("Welcome Player%d :)\nYou scored %d points!\n", my_data->id, my_data->score);

    // Free up critical section
    sem_post(&mutex);
}

void *serverThread(void *player_args) {
    // Make the players wait to join the game
    struct player data[NUM_PLAYERS];
    sleep(1);
    sem_wait(&mutex);

    // Critical section
    printf("Heads up! The game has started.\n");

    // Free up critical section
    sem_post(&mutex);

    // TODO: Figure out how to pass player data to the game
    data = (struct player *) player_args;

    struct player *p1 = (struct player *) data[0];
    struct player *p2 = (struct player *) data[1];

    int scores[NUM_PLAYERS];

    scores[0] = p1->score;
    scores[1] = p2->score;

    printf("Player 1: %d\nPlayer2: %d\n", scores[0], scores[1]);

    if (p1->score > p2->score) {
        printf("Congratulations Player1. You've won the game!\n");
    } else if (p2->score > p1->score) {
        printf("Congratulations Player2. You won the game!\n");
    } else {
        printf("It's a tie! Better luck next time.\n");
    }

    sleep(1);

}

int main(int argc, char *argv[]) {
    // Seed the random number generator
    srand(time(NULL));

    // Initialize the semaphore
    sem_init(&mutex, SHARED, 0);

    pthread_t game_thread;
    pthread_t player1_thread;
    pthread_t player2_thread;

    player_data[0].id = 1;
    player_data[1].id = 2;

    printf("Initializing game...\n");

    // Create the game thread and sleep to enfore order
    pthread_create(&game_thread, NULL, serverThread, (void *)&player_data);
    sleep(1);

    pthread_create(&player1_thread, NULL, playerThread, (void *)&player_data[0]);
    pthread_create(&player2_thread, NULL, playerThread, (void *)&player_data[1]);

    pthread_join(game_thread, NULL);
    pthread_join(player1_thread, NULL);
    pthread_join(player2_thread, NULL);

    printf("Game Over!\n");

    pthread_exit(NULL);
    sem_destroy(&mutex);
    exit(0);
}