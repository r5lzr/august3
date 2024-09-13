#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "util.h"

int get_time_ms()
{
  return GetTickCount();
}
