#ifndef UTIL_H
#define UTIL_H


extern int quit;
extern int moves_to_go;
extern int move_time;
extern int time;
extern int inc_time;
extern int start_time;
extern int stop_time;
extern int time_set;
extern int stopped;

int get_time_ms();
void reset_tc();
int input_waiting();
void read_input();
void check_up();


#endif
