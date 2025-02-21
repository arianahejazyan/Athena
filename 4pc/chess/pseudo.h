#include "definitions.h"

using namespace definitions;

namespace pseudo
{

extern unsigned int king_movement;
extern unsigned int knight_movement;

extern unsigned int queen_movement[8];
extern unsigned int rook_movement[4];
extern unsigned int bishop_movement[4];

extern unsigned int push_movement[4];
extern unsigned int advance_movement[4];

extern bool IsHomerankSquare[board_size][player_size];
extern bool IsPromotionSquare[board_size][player_size]; 

extern unsigned int attack_movement[4];


void crawl(Piece piece, unsigned int& idx, unsigned int movement);

void slide(Offset offset, unsigned int& idx, unsigned int movement);

void march(Player player, unsigned int& idx, unsigned int movement, Flag flag);

void attack(Alliance alliance, Player opponent, unsigned int& idx, unsigned int movement);

void generate_masks();

// 2 crawl, 8 slide, 8 march, 8 castle
extern unsigned int start[board_size][26], range[board_size][26];

// all precalculated moves
extern array<Square, 100000> squares; // precalculated_squares

void initialize();

};