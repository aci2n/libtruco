typedef struct truco_state truco_state;
typedef enum truco_command truco_command;

// keep in sync with util.c#truco_command_defs
// be careful about reordering
enum truco_command {
  start_game_two,
  start_game_four,
  start_game_six,

  play_first,
  play_second,
  play_third,

  call_envido,
  call_real_envido,
  call_falta_envido,
  accept_envido,
  reject_envido,

  call_truco,
  call_retruco,
  call_vale_cuatro,
  accept_truco,
  reject_truco,

  go_to_deck,
  surrender,

  truco_command_num,
};

void truco_dispatch(truco_state*, truco_command);
