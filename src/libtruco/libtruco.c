#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "libtruco.h"
#include "card.c"

#define TRUCO_MAX_TEAMS 2
#define TRUCO_MAX_PLAYERS 6
#define TRUCO_HAND_SIZE 3
#define TRUCO_MAX_CHOICES 128

typedef struct truco_round truco_round;

struct truco_round {
  size_t score[TRUCO_MAX_TEAMS];

  truco_playable_card hands[TRUCO_MAX_PLAYERS][TRUCO_HAND_SIZE];

  size_t stack_size;
  truco_card stack[TRUCO_MAX_PLAYERS * TRUCO_HAND_SIZE]; 
};

struct truco_state {
  size_t players;
  size_t current_player;
  size_t score[TRUCO_MAX_TEAMS];

  size_t current_round;
  truco_round round;

  truco_command choices[TRUCO_MAX_CHOICES];
  size_t choices_size;
};

static
void truco_init(truco_state* state, size_t const players) {
  *state = (truco_state){
    .players = players,
    .choices = {start_round},
    .choices_size = 1,
  };
}

static
void truco_start_round(truco_state* state) {
  state->current_player = state->current_round % state->players;
  state->current_round++;
  state->round = (truco_round){0};
}

void truco_dispatch(truco_state* state, truco_command const command) {
  if (!state) return;

  size_t players = 2;

  switch (command) {
    case init_six: players = 6;
    case init_four: players = 4;
    case init_two:
      truco_init(state, players);
      break;

    case start_round:
      truco_start_round(state);
      break;
  }
}
