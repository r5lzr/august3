#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include "util.h"

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
  return GetTickCount();
}

int input_waiting()
{
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
}
