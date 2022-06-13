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
  truco_card deck[truco_card_num];
};

static
void truco_deal_hands(truco_state* state) {
  truco_shuffle(truco_card_num, (int*)state->deck);

  for (size_t i = 0; i < state->players * TRUCO_HAND_SIZE; i++) {
    size_t player = i % state->players;
    size_t card = i / state->players;
    state->round.hands[player][card] = (truco_playable_card){
      .card = state->deck[i],
      .played = false
    };
  }
}

static
void truco_start_round(truco_state* state) {
  truco_deal_hands(state);
  state->choices = (truco_choices){
    .size = 6,
    .values = {play_first, play_second, play_third, call_envido, call_truco, surrender},
  };
}

static
void truco_start_game(truco_state* state, size_t const players) {
  *state = (truco_state){.players = players};
  for (size_t i = 0; i < truco_card_num; i++) {
    state->deck[i] = i;
  }
  truco_start_round(state);
}

static
void truco_advance_round(truco_state* state) {
  state->current_round++;
  state->current_player = state->current_round % state->players;
  state->round = (truco_round){0};
  truco_start_round(state);
}

void truco_dump(truco_state* state) {
  printf("{\n");
  printf(" Players: %lu\n", state->players);
  printf(" Score: %lu - %lu\n", state->score[0], state->score[1]);
  printf(" Current player: %lu\n", state->current_player);
  printf(" Current round: %lu {\n", state->current_round);

  truco_round round = state->round;

  printf("  Score: %lu - %lu\n", round.score[0], round.score[1]);

  for (size_t i = 0; i < state->players; i++) {
    truco_playable_card* hand = round.hands[i];
    printf("  Player %lu: [ ", i);

    for (size_t j = 0; j < TRUCO_HAND_SIZE; j++) {
      truco_playable_card playable = hand[j];
      truco_card_def def = truco_cards[playable.card];
      printf("%s ", def.name);
    }

    printf("]\n");
  }

  printf("  Stack: [ ");
  for (size_t i = 0; i < round.stack_size; i++) {
    truco_card_def def = truco_cards[round.stack[i]];
    printf("%s ", def.name);
  }
  printf("]\n");

  printf(" }\n");
  printf("}\n");
};

void truco_dispatch(truco_state* state, truco_command const command) {
  if (!state) return;

  switch (command) {
    case start_game_two:
    case start_game_four:
    case start_game_six:
      truco_start_game(state, 2 * (command + 1));
      break;

    case advance_round:
      truco_advance_round(state);
      break;
  }
}
