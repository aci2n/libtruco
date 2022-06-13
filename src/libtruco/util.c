#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void truco_shuffle(size_t size, int arr[static size]) {
  static bool seeded = false;

  if (!seeded) {
    srand(time(0));
    seeded = true;
  }

  for (size_t i = 0; i < size; i++) {
    int t = arr[i];
    int r = rand() % size;
    arr[i] = arr[r];
    arr[r] = t;
  }
}
