#include <stdlib.h>
#include <stdio.h>

#include "../src/libtruco.c"

int main(int const argc, char const*const argv[static argc + 1]) {
  truco_state* state = malloc(sizeof(truco_state));
  printf("sizeof(truco_state): %lu\n", sizeof(*state));

  truco_dispatch(state, start_game_two); truco_dump(state);
  truco_dispatch(state, play_first); truco_dump(state);

  return EXIT_SUCCESS;
}
