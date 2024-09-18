#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitboard.h"
#include "perft.h"
#include "movegen.h"

ui64 nodes;

void perft_driver(int depth)
{
  if (depth == 0)
  {
    nodes++;
    return;
  }

  moves move_list[1];

  generate_moves(&move_list);

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    copy_board();

    // if king is check, return 0 -> continue to next board
    if (!make_move(move, all_moves))
    {
      continue;
    }

    perft_driver(depth - 1);

    restore_board();
  }
}

void perft_test(int depth)
{
  printf("\nPERFORMANCE TEST:\n\n");

  printf("MOVES  NODES\n\n");

  moves move_list[1];

  generate_moves(&move_list);

  long start = get_time_ms();

  for (int move_count = 0; move_count < move_list->count; move_count++)
  {
    int move = move_list->moves[move_count];

    copy_board();

    // if king is check, return 0 -> continue to next board
    if (!make_move(move, all_moves))
    {
      continue;
    }

    long cumulative_nodes = nodes;

    perft_driver(depth - 1);

    long old_nodes = nodes - cumulative_nodes;

//    printf("Initial make move from %s: board %d\n>>%s to move\n", board.side ? "white" : "black", move_count, !board.side ? "white" : "black");
//    show_board();
//    show_bitboard(side_bitboards[white]);
//    getchar();

    restore_board();
//
//    printf("Restore board from: board %d\n>>%s to move\n", move_count, !board.side ? "white" : "black");
//    show_board();
//    show_bitboard(side_bitboards[white]);

    char promotion = promoted_pieces[get_move_promoted(move)];

    if (promotion)
    {
      printf("%s%s%c: %ld\n",
             square_to_coordinates[get_move_source(move)],
             square_to_coordinates[get_move_target(move)],
             promotion,
             old_nodes);
    }

    else
    {
      printf("%s%s: %ld\n",
           square_to_coordinates[get_move_source(move)],
           square_to_coordinates[get_move_target(move)],
           old_nodes);
    }

  }

  printf("\nDepth: %d\n", depth);
  printf("Nodes: %lld\n", nodes);
  printf("Time: %ld ms\n", get_time_ms() - start);

}