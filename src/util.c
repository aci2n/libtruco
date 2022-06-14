#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
