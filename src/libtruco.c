#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "libtruco.h"
#include "card.c"
#include "util.c"

#define TRUCO_MAX_TEAMS 2
#define TRUCO_MAX_PLAYERS 6
#define TRUCO_HAND_SIZE 3
#define TRUCO_MAX_CHOICES 128
#define TRUCO_MAX_ROUNDS 128

typedef struct truco_lap truco_lap;
struct truco_lap {
  size_t size;
  truco_card const* strongest_card[TRUCO_MAX_TEAMS];
  truco_card const* stack[TRUCO_MAX_PLAYERS];
};

typedef struct truco_round truco_round;
struct truco_round {
  size_t score[TRUCO_MAX_TEAMS];
  truco_card const* hands[TRUCO_MAX_PLAYERS][TRUCO_HAND_SIZE];
  size_t laps_size;
  truco_lap laps[TRUCO_HAND_SIZE];
  bool drew_first;
  size_t bonus;
};

typedef struct truco_choices truco_choices;
struct truco_choices {
  size_t size;
  truco_command values[TRUCO_MAX_CHOICES];
};

struct truco_state {
  size_t players;
  size_t current_player;
  size_t score[TRUCO_MAX_TEAMS];
  size_t rounds_size;
  truco_round rounds[TRUCO_MAX_ROUNDS];
  truco_choices choices;
  truco_card const* deck[truco_card_num];
};

static
void truco_deal_hands(truco_state* state) {
  truco_round* round = &(state->rounds[state->rounds_size - 1]);

  truco_shuffle(truco_card_num, state->deck);

  for (size_t i = 0; i < state->players * TRUCO_HAND_SIZE; i++) {
    size_t player = i % state->players;
    size_t card = i / state->players;
    round->hands[player][card] = state->deck[i];
  }
}

static
void truco_start_round(truco_state* state) {
  state->rounds_size++;
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
    state->deck[i] = &(truco_cards[i]);
  }
  truco_start_round(state);
}

static
void truco_advance_round(truco_state* state) {
  state->current_player = state->rounds_size % state->players;
  truco_start_round(state);
}

static
inline
size_t truco_get_team(size_t player) {
  return player % 2;
}

static
void truco_compute_round_end(truco_state* state) {
  truco_round* round = &(state->rounds[state->rounds_size - 1]);

  if (round->score[0] > round->score[1]) {
    state->score[0] += round->bonus;
  } else if (round->score[1] > round->score[0]) {
    state->score[1] += round->bonus;
  }
}

static
void truco_play_card(truco_state* state, size_t i) {
  size_t current_player = state->current_player;
  truco_round* round = &(state->rounds[state->rounds_size - 1]);
  truco_lap* lap = &(round->laps[round->laps_size - 1]);
  truco_card const** hand = round->hands[current_player];
  truco_card const* card = hand[i];
  size_t team = truco_get_team(current_player);

  assert(card);

  if (!lap->strongest_card[team] || card->power > lap->strongest_card[team]->power) {
    lap->strongest_card[team] = card;
  }

  hand[i] = 0;
  lap->stack[lap->size++] = card;

  if (lap->size == state->players) {
    size_t strongest_0 = lap->strongest_card[0]->power;
    size_t strongest_1 = lap->strongest_card[1]->power;

    if (strongest_0 > strongest_1) {
      round->score[0]++;
    } else if (strongest_1 > strongest_0) {
      round->score[1]++;
    } else if (round->laps_size == 1) {
      round->drew_first = true;
    }

    size_t diff = abs((int)(round->score[0] - round->score[1]));

    if ((round->drew_first && diff > 0) || diff > 1 || round->laps_size == 3) {
      truco_compute_round_end(state);
    } else {
      round->laps_size++;
    }
  }
}

void truco_dump(truco_state* state) {
  printf("{\n");
  printf(" Players: %lu\n", state->players);
  printf(" Score: %lu - %lu\n", state->score[0], state->score[1]);
  printf(" Current player: %lu\n", state->current_player);
  printf(" Current round: %lu {\n", state->rounds_size - 1);

  truco_round* round = &(state->rounds[state->rounds_size - 1]);

  printf("  Score: %lu - %lu\n", round->score[0], round->score[1]);
  printf("  Bonus: %lu\n", round->bonus);
  printf("  Drew first: %u\n", round->drew_first);

  for (size_t i = 0; i < state->players; i++) {
    truco_card const** hand = round->hands[i];
    printf("  Player %lu: [ ", i);

    for (size_t j = 0; j < TRUCO_HAND_SIZE; j++) {
      truco_card const* card = hand[j];
      printf("%s ", card ? card->name : "X");
    }

    printf("]\n");
  }

  printf("  Laps: [ ");
  for (size_t i = 0; i < round->laps_size; i++) {
    truco_lap* lap = &(round->laps[i]);
    printf("   {\n");
    printf("    Size: %lu\n", lap->size);
    printf("    Strongest card: %s - %s\n", lap->strongest_card[0]->name, lap->strongest_card[1]->name);
    printf("   }\n");
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

    case play_first:
    case play_second:
    case play_third:
      truco_play_card(state, command - play_first);
      break;
  }
}
