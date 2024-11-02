#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "ttable.h"
#include "zobrist.h"
#include "search.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"
#include "evaluation.h"

void restore_ttable()
{
  for (int index = 0; index < ttable_size; index++)
  {
    ttable[index].zobrist_key = 0;
    ttable[index].depth = 0;
    ttable[index].flag = 0;
    ttable[index].score = 0;
  }
}
