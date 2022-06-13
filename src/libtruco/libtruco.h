typedef struct truco_state truco_state;
typedef enum truco_command truco_command;

enum truco_command {
  init_two,
  init_four,
  init_six,
  start_round,
  play_first,
  play_second,
  play_third,
  call_envido,
  call_real_envido,
  call_falta_envido,
  reject_envido,
  call_truco,
  call_retruco,
  call_vale_cuatro,
  reject_truco,
  surrender,
  truco_command_size,
};

void truco_dispatch(truco_state*, truco_command const);
