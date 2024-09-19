#ifndef UCI_H
#define UCI_H



int parse_move(char *move_string);
void parse_position(char *command);
void parse_go(char *command);
void get_uci_id();
void uci_loop();




#endif
