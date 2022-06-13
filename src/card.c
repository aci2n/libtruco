#include <stdbool.h>

typedef enum truco_card truco_card;
typedef struct truco_card_def truco_card_def;
typedef struct truco_playable_card truco_playable_card;

enum truco_card {
  swords_1,
  swords_2,
  swords_3,
  swords_4,
  swords_5,
  swords_6,
  swords_7,
  swords_10,
  swords_11,
  swords_12,
  clubs_1,
  clubs_2,
  clubs_3,
  clubs_4,
  clubs_5,
  clubs_6,
  clubs_7,
  clubs_10,
  clubs_11,
  clubs_12,
  coins_1,
  coins_2,
  coins_3,
  coins_4,
  coins_5,
  coins_6,
  coins_7,
  coins_10,
  coins_11,
  coins_12,
  cups_1,
  cups_2,
  cups_3,
  cups_4,
  cups_5,
  cups_6,
  cups_7,
  cups_10,
  cups_11,
  cups_12,
  truco_card_num,
};

struct truco_card_def {
  size_t const power;
  char const*const name;
};

#define TRUCO_CARD_DEF(CARD, POWER) [(CARD)+0] = { .power = (POWER)+0, .name = "" #CARD "" }

truco_card_def const truco_cards[] = {
  TRUCO_CARD_DEF(swords_1, 100),

  TRUCO_CARD_DEF(clubs_1, 99),

  TRUCO_CARD_DEF(swords_7, 98),

  TRUCO_CARD_DEF(coins_7, 97),

  TRUCO_CARD_DEF(swords_3, 96),
  TRUCO_CARD_DEF(clubs_3, 96),
  TRUCO_CARD_DEF(coins_3, 96),
  TRUCO_CARD_DEF(cups_3, 96),

  TRUCO_CARD_DEF(swords_2, 95),
  TRUCO_CARD_DEF(clubs_2, 95),
  TRUCO_CARD_DEF(coins_2, 95),
  TRUCO_CARD_DEF(cups_2, 95),

  TRUCO_CARD_DEF(coins_1, 94),
  TRUCO_CARD_DEF(cups_1, 94),

  TRUCO_CARD_DEF(swords_12, 93),
  TRUCO_CARD_DEF(clubs_12, 93),
  TRUCO_CARD_DEF(coins_12, 93),
  TRUCO_CARD_DEF(cups_12, 93),

  TRUCO_CARD_DEF(swords_11, 92),
  TRUCO_CARD_DEF(clubs_11, 92),
  TRUCO_CARD_DEF(coins_11, 92),
  TRUCO_CARD_DEF(cups_11, 92),

  TRUCO_CARD_DEF(swords_10, 91),
  TRUCO_CARD_DEF(clubs_10, 91),
  TRUCO_CARD_DEF(coins_10, 91),
  TRUCO_CARD_DEF(cups_10, 91),

  TRUCO_CARD_DEF(swords_7, 90),
  TRUCO_CARD_DEF(clubs_7, 90),
  TRUCO_CARD_DEF(coins_7, 90),
  TRUCO_CARD_DEF(cups_7, 90),

  TRUCO_CARD_DEF(swords_6, 89),
  TRUCO_CARD_DEF(clubs_6, 89),
  TRUCO_CARD_DEF(coins_6, 89),
  TRUCO_CARD_DEF(cups_6, 89),

  TRUCO_CARD_DEF(swords_5, 88),
  TRUCO_CARD_DEF(clubs_5, 88),
  TRUCO_CARD_DEF(coins_5, 88),
  TRUCO_CARD_DEF(cups_5, 88),

  TRUCO_CARD_DEF(swords_4, 87),
  TRUCO_CARD_DEF(clubs_4, 87),
  TRUCO_CARD_DEF(coins_4, 87),
  TRUCO_CARD_DEF(cups_4, 87),
};

struct truco_playable_card {
  truco_card card;
  bool played;
};
