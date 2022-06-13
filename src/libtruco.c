#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "libtruco.h"
#include "card.c"
#include "util.c"

#define TRUCO_MAX_TEAMS 2
#define TRUCO_MAX_PLAYERS 6
#define TRUCO_HAND_SIZE 3
#define TRUCO_MAX_CHOICES 128

typedef struct truco_round truco_round;
typedef struct truco_choices truco_choices;

struct truco_round {
  size_t score[TRUCO_MAX_TEAMS];
  truco_playable_card hands[TRUCO_MAX_PLAYERS][TRUCO_HAND_SIZE];
  size_t stack_size;
  truco_card stack[TRUCO_MAX_PLAYERS * TRUCO_HAND_SIZE]; 
};

struct truco_choices {
  size_t size;
  truco_command values[TRUCO_MAX_CHOICES];
};

struct truco_state {
  size_t players;
  size_t current_player;
  size_t score[TRUCO_MAX_TEAMS];
  size_t current_round;
  truco_round round;
  truco_choices choices;
};

static
void truco_init(truco_state* state, size_t const players) {
  *state = (truco_state){
    .players = players,
    .choices = (truco_choices){
      .values = {start_round},
      .size = 1,
    }
  };
}

static
void truco_deal_hands(truco_state* state) {
  int deck[truco_card_num];

  for (size_t i = 0; i < truco_card_num; i++) {
    deck[i] = i;
  }

  truco_shuffle(truco_card_num, deck);

  for (size_t i = 0; i < state->players * TRUCO_HAND_SIZE; i++) {
    size_t player = i % state->players;
    size_t card = i / state->players;
    state->round.hands[player][card] = (truco_playable_card){
      .card = deck[i],
      .played = false
    };
  }
}

static
void truco_start_round(truco_state* state) {
  state->current_player = state->current_round % state->players;
  state->current_round++;
  state->round = (truco_round){0};
  truco_deal_hands(state);
  state->choices = (truco_choices){
    .values = {play_first, play_second, play_third, call_envido, call_truco, surrender},
    .size = 6
  };
}

void truco_dump(truco_state* state) {
  printf("{\n"
      "Players: %lu\n"
      "Score: %lu - %lu\n"
      "Current player: %lu\n"
      "Current round: %lu\n"
      "}\n",
      state->players,
      state->score[0], state->score[1],
      state->current_player,
      state->current_round);
};

void truco_dispatch(truco_state* state, truco_command const command) {
  if (!state) return;

  switch (command) {
    case init_two:
    case init_four:
    case init_six:
      truco_init(state, 2 * (command + 1));
      break;

    case start_round:
      truco_start_round(state);
      break;
  }
}
