#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

void truco_shuffle(size_t size, truco_card const* deck[static size]) {
  static bool seeded = false;

  if (!seeded) {
    srand(time(0));
    seeded = true;
  }

  for (size_t i = 0; i < size; i++) {
    truco_card const* temp = deck[i];
    int j = rand() % size;
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

typedef struct truco_command_def truco_command_def;
struct truco_command_def {
  char const*const name;
};

#define TRUCO_COMMAND(ID) [(ID)+0] = { .name = "" #ID "" }

static truco_command_def const truco_command_defs[] = {
  TRUCO_COMMAND(start_game_two),
  TRUCO_COMMAND(start_game_four),
  TRUCO_COMMAND(start_game_six),
  TRUCO_COMMAND(play_first),
  TRUCO_COMMAND(play_second),
  TRUCO_COMMAND(play_third),
  TRUCO_COMMAND(call_envido),
  TRUCO_COMMAND(call_real_envido),
  TRUCO_COMMAND(call_falta_envido),
  TRUCO_COMMAND(reject_envido),
  TRUCO_COMMAND(call_truco),
  TRUCO_COMMAND(call_retruco),
  TRUCO_COMMAND(call_vale_cuatro),
  TRUCO_COMMAND(reject_truco),
  TRUCO_COMMAND(surrender),
};
static_assert(sizeof(truco_command_defs) / sizeof(truco_command_defs[0]) == truco_command_num);

char const*const truco_command_name(truco_command command) {
  return truco_command_defs[command].name;
}
