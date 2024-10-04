#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#ifdef WIN64
  #include <windows.h>
#else
  #include <sys/time.h>
#endif

int quit = 0;
int moves_to_go = 30;
int move_time = -1;
int time = -1;
int inc_time = 0;
int start_time = 0;
int stop_time = 0;
int time_set = 0;
int stopped = 0;

int get_time_ms()
{
  #ifdef WIN64
    return GetTickCount();
  #else
    struct timeval time_value;
    gettimeofday(&time_value, NULL);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
  #endif
}

void reset_tc()
{
  quit = 0;
  moves_to_go = 30;
  move_time = -1;
  time = -1;
  inc_time = 0;
  start_time = 0;
  stop_time = 0;
  time_set = 0;
  stopped = 0;
}

int input_waiting()
{
  #ifdef WIN64
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;

    if (!init)
    {
      init = 1;
      inh = GetStdHandle(STD_INPUT_HANDLE);
      pipe = !GetConsoleMode(inh, &dw);

      if (!pipe)
      {
        SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
        FlushConsoleInputBuffer(inh);
      }
    }

    if (pipe)
    {
      if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
      {
        return 1;
      }

      return dw;
    }

    else
    {
      GetNumberOfConsoleInputEvents(inh, &dw);
      return dw <= 1 ? 0 : dw;
    }
  #else
    fd_set readfds;
    struct timeval tv;
    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec=0; tv.tv_usec=0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
  #endif
}

void read_input()
{
  int bytes;

  char input[256] = "", *endc;

  if (input_waiting())
  {
    stopped = 1;

    do
    {
      bytes=read(fileno(stdin), input, 256);
    }

    while (bytes < 0);

    endc = strchr(input,'\n');

    if (endc)
    {
      *endc=0;
    }

    if (strlen(input) > 0)
    {
      if (!strncmp(input, "quit", 4))
      {
        quit = 1;
      }

      else if (!strncmp(input, "stop", 4))
      {
        quit = 1;
      }
    }
  }
}

void check_up()
{
  if (time_set == 1 && get_time_ms() > stop_time)
  {
		stopped = 1;
	}

	read_input();
}
