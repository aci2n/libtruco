#include <stdlib.h>
#include <stdio.h>

#include "../src/libtruco.c"

int main(int const argc, char const*const argv[static argc + 1]) {
  truco_state* state = malloc(sizeof(truco_state));
  printf("sizeof(truco_state): %lu\n", sizeof(*state));

#define DISPATCH(COMMAND) \
  do { \
    printf("\nCommand: " #COMMAND "\n"); \
    printf("@Before:\n"); \
    truco_dump(state); \
    truco_dispatch(state, COMMAND+0); \ 
    printf("@After:\n"); \
    truco_dump(state); \
  } while (false);

  DISPATCH(start_game_two);
  DISPATCH(play_first);
  DISPATCH(play_second);
  DISPATCH(play_second);
  DISPATCH(play_third);

  return EXIT_SUCCESS;
}
