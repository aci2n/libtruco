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

typedef struct truco_lap truco_lap;
typedef struct truco_round truco_round;
typedef struct truco_choices truco_choices;

struct truco_lap {
  size_t size;
  truco_card strongest_card[TRUCO_MAX_TEAMS];
  truco_card stack[TRUCO_MAX_PLAYERS];
};

struct truco_round {
  size_t score[TRUCO_MAX_TEAMS];
  truco_playable_card hands[TRUCO_MAX_PLAYERS][TRUCO_HAND_SIZE];
  size_t current_lap;
  truco_lap laps[TRUCO_HAND_SIZE];
  bool drew_first;
  size_t bonus;
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

static
inline
size_t truco_get_team(size_t player) {
  return player % 2;
}

static
void truco_compute_round_end(truco_state* state) {
  truco_round* round = &(state->round);

  if (round->score[0] > round->score[1]) {
    state->score[0] += round->bonus;
  } else if (round->score[1] > round->score[0]) {
    state->score[1] += round->bonus;
  }
}

static
void truco_play_card(truco_state* state, size_t i) {
  truco_round* round = &(state->round);
  truco_lap* lap = &(round->laps[round->current_lap]);
  truco_playable_card* playable = &(round->hands[state->current_player][i]);
  size_t team = truco_get_team(state->current_player);

  assert(!playable->played);
  
  if (truco_cards[playable->card].power > truco_cards[lap->strongest_card[team]].power) {
    lap->strongest_card[team] = playable->card;
  }

  playable->played = true;
  lap->stack[lap->size++] = playable->card;

  if (lap->size == state->players) {
    size_t strongest_0 = truco_cards[lap->strongest_card[0]].power;
    size_t strongest_1 = truco_cards[lap->strongest_card[1]].power;

    if (strongest_0 > strongest_1) {
      round->score[0]++;
    } else if (strongest_1 > strongest_0) {
      round->score[1]++;
    } else if (round->current_lap == 0) {
      round->drew_first = true;
    }

    size_t diff = abs((int)(round->score[0] - round->score[1]));

    if ((round->drew_first && diff > 0) || diff > 1 || round->current_lap == 2) {
      truco_compute_round_end(state);
    } else {
      round->current_lap++;
    }
  }
}

void truco_dump(truco_state* state) {
  printf("{\n");
  printf(" Players: %lu\n", state->players);
  printf(" Score: %lu - %lu\n", state->score[0], state->score[1]);
  printf(" Current player: %lu\n", state->current_player);
  printf(" Current round: %lu {\n", state->current_round);

  truco_round round = state->round;

  printf("  Score: %lu - %lu\n", round.score[0], round.score[1]);
  printf("  Bonus: %lu\n", round.bonus);
  printf("  Drew first: %u\n", round.drew_first);

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

  printf("  Laps: [ ");
  for (size_t i = 0; i < round.current_lap; i++) {
    truco_lap* lap = &(round.laps[i]);
    printf("   {\n");
    printf("    Size: %lu\n", lap->size);
    printf("    Strongest card: %s - %s\n", 
        truco_cards[lap->strongest_card[0]].name,
        truco_cards[lap->strongest_card[1]].name);
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
