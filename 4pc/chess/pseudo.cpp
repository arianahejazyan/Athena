#include "pseudo.h"

/*

namespace pseudo
{

// [0]: RoyalFrom, [1]: RoyalKingSide, [2]: RoyalQueenSide, [3]: RookFromKingSide, [4]: RookToKingSide, [5]: RookFromQueenSide, [6]: RookToQueenSide, 
constexpr array<Square, 7> castling_squares[4][2] =
{
    // Red
    {
        {4, 6, 2, 7, 5, 0, 3}, // Modern
        {4, 6, 2, 7, 5, 0, 3}, // Classic
    },
    // Blue
    {
        {66, 38, 94, 24, 52, 122, 80},  // Modern        
        {80, 108, 52, 122, 94, 24, 66}, // Classic
    },
    // Yellow
    {
        {155, 153, 157, 152, 154, 159, 156}, // Modern
        {155, 153, 157, 152, 154, 159, 156}, // Classic
    },
    // Green
    {
        {93, 121, 65, 135, 107, 37, 79}, // Modern
        {79, 51, 107, 37, 65, 135, 93},  // Classic
    }
};

// [0,1]: KingSide, [2,3,4]: QueenSide
constexpr array<Square, 5> passing_squares[4][2] =
{
    // Red
    {
        {5, 6, 1, 2, 3}, // Modern
        {5, 6, 1, 2, 3}, // Classic
    },
    // Blue
    {
        {38, 52, 80, 94, 108}, // Modern        
        {94, 108, 38, 52, 66}, // Classic
    },
    // Yellow
    {
        {153, 154, 156, 157, 158}, // Modern
        {153, 154, 156, 157, 158}, // Classic
    },
    // Green
    {
        {107, 121, 51, 65, 79}, // Modern        
        {51, 65, 93, 107, 121}, // Classic
    }
};

// [0,1,2]: KingSide, [2,3,4]: QueenSide 
constexpr array<Square, 5> secure_squares[4][2] =
{
    // Red
    {
        {5, 6, 4, 3, 2}, // Modern
        {5, 6, 4, 3, 2}, // Classic
    },
    // Blue
    {
        {38, 52, 66, 80, 94},  // Modern        
        {108, 94, 80, 66, 52}, // Classic
    },
    // Yellow
    {
        {153, 154, 155, 156, 157}, // Modern
        {153, 154, 155, 156, 157}, // Classic
    },
    // Green
    {
        {121, 107, 93, 79, 65}, // Modern        
        {51, 65, 79, 93, 107},  // Classic
    }
};

}; // namespace



*/

namespace pseudo
{

unsigned int king_movement   = 0;
unsigned int knight_movement = 1;

unsigned int queen_movement[8]  = {2, 3, 4, 5, 6, 7, 8, 9};
unsigned int rook_movement[4]   = {3, 5, 6, 8};
unsigned int bishop_movement[4] = {2, 4, 7, 9};

unsigned int push_movement[4]    = {10, 11, 12, 13};
unsigned int advance_movement[4] = {14, 15, 16, 17};

unsigned int attack_movement[4] = {18, 19, 20, 21};

bool IsHomerankSquare[board_size][player_size];
bool IsPromotionSquare[board_size][player_size]; 

unsigned int start[board_size][26], range[board_size][26];

array<Square, 100000> squares;

void initialize()
{
    unsigned int idx = 0, movement = 0;

    for (Piece piece: {King, Knight})
    {
        crawl(piece, idx, movement++);
    }

    for (Offset offset: patterns[Queen - 1])
    {
        slide(offset, idx, movement++);
    }

    for (Flag flag: {Push, Advance})
    {
        for (Player player: {Red, Blue, Yellow, Green})
        {
            march(player, idx, movement++, flag); 
        }        
    }

    for (Alliance alliance: {RY, BG})
    {
        for (Player opponent: enemy[alliance])
        {
            attack(alliance, opponent, idx, movement++);
        }
    }

    generate_masks();
}

void crawl(Piece piece, unsigned int& idx, unsigned int movement)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        for (Offset offset: patterns[piece - 1])
        {
            int r = rank(sq) + offset.vertical;
            int f = file(sq) + offset.horizontal;

            if (valid_square(r, f))
            {
                squares[idx + count++] = spot(r, f);
            }
        }

        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}

void slide(Offset offset, unsigned int& idx, unsigned int movement)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        int r = rank(sq) + offset.vertical;
        int f = file(sq) + offset.horizontal;

        while (valid_square(r, f))
        {
            squares[idx + count++] = spot(r, f);
            
            r += offset.vertical;
            f += offset.horizontal;
        }

        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}

void march(Player player, unsigned int& idx, unsigned int movement, Flag flag)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        for (Offset offset: patterns[Pawn - 1])
        {
            if (offset.player == player && offset.flag == flag)
            {
                int r = rank(sq) + offset.vertical;
                int f = file(sq) + offset.horizontal;        

                if (valid_square(r, f))
                {
                    squares[idx + count++] = spot(r, f);
                } 
            }
        }
        
        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}

void attack(Alliance alliance, Player opponent, unsigned int& idx, unsigned int movement)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        for (Offset offset: patterns[Pawn - 1])
        {
            if (offset.player == teammate(opponent) && offset.flag == Advance)
            {
                int r = rank(sq) + offset.vertical;
                int f = file(sq) + offset.horizontal;        

                if (valid_square(r, f))
                {
                    squares[idx + count++] = spot(r, f);
                } 
            }
        }

        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}

void generate_masks()
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        for (Player player: {Red, Blue, Yellow, Green})
        {
            IsPromotionSquare[sq][player] = promotion_square(sq, player);
            IsHomerankSquare[sq][player] = homerank_square(sq, player);
        }
    }
}

/*
void push(Player player, unsigned int& idx, unsigned int movement, Offset offset)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        int r1 = Rank(sq) + offset.vertical;
        int f1 = File(sq) + offset.horizontal;

        if (valid_square(r1, f1))
        {
            squares[idx + count++] = Spot(r, f);
        }
        
        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}

void advance(Player player, unsigned int& idx, unsigned int movement, Offset offset)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        unsigned int count = 0;

        int r1 = Rank(sq) + offset.vertical;
        int f1 = File(sq) + offset.horizontal;

        if (valid_square(r1, f1))
        {
            squares[idx + count++] = Spot(r, f);
        }
                
        start[sq][movement] = idx;
        range[sq][movement] = count;

        idx += count;
    }
}
*/
}; // namespace


/*

void promote()
{
    if (promotion_square(r, f))
    {
        for (Piece piece: promotion_list)
        {

        }
    }

    else squares[idx + count++] = Spot(r, f);
}



namespace pseudo
{

void calculate_push_squares()
{
    iterate_march
    {
        for (Offset offset: patterns[Pawn])
        {
            switch (offset.flag)
            {
                case Push:
                {
                    if (valid)
                    {

                    }

                    if (homerank || torpedo && valid1 && valid2)
                    {

                    }

                    break;
                }

                case SideWays:
                {
                    if (rules.sideways && valid)
                    {

                    }

                    break;
                }

                case Advance:
                {
                    if (valid)
                    {

                    }

                    break;
                }
            
                default: break;  
            }
        }
    }
}

void calculate_advance_squares()
{
    iterate_march
    {
        for (Offset offset: patterns[Pawn])
        {
            
        }
    }
}

void calculate_promotion_squares()
{

}


}; // namespace
*/

/*


void pseudo::calculate_march_moves(const Square origin, const Piece piece, const Offset offset)
{
    switch (offset.flag)
    {
        case Push:    calculate_push_moves();    break;
        case Advance: calculate_advance_moves(); break;
        
    
        default: break;
    }




    Spot destination(origin);



    switch (offset.flag)
    {
        case Push:
        {

        }

        break;
    
    default:
        break;
    }


    // push
    // double push
    // 




    int r = rank(sq) + dir.horizontal;
    int f = file(sq) + dir.vertical;

    if (is_valid_square(r, f))
    {
        if (dir.flag == Push)
        {
            calculate_promote_moves();
        }

        else if (dir.flag == DoublePush)
        {

        }

        else if (dir.flag == Advance)
        {

        }
    }


    int r1 = rank(sq) + dir.horizontal, f1 = file(sq) + dir.vertical;
    int r2 = rank(sq) + 2 * dir.horizontal, f2 = file(sq) + 2 * dir.vertical;

    if (isValidSquare(r1, f1))
    {
        if (dir.flag == SideWays)
        {
            if (Config::SIDE_WAYS) promote(sq, square(r1, f1), Config::OFFBOARD, piece, dir.player, Flag::Push);
        }

        else if (dir.flag == Push)
        {

            promote(sq, square(r1, f1), Config::OFFBOARD, piece, dir.player, Flag::Push);


            if (isValidSquare(r2, f2) && (Config::TORPERDO || isHomeRankSquare(sq, dir.player))) promote(sq, square(r2, f2), square(r1, f1), piece, dir.player, Flag::DoublePush);
        }
    }


    int r = rank(sq) + dir.horizontal, f = file(sq) + dir.vertical;

    if (isValidSquare(r, f))
    {

        promote(sq, square(r, f), Config::OFFBOARD, piece, dir.player, Flag::Advance);


        if (!Config::NO_ENPASSANT)
        {
            promote(sq, loc, Config::OFFBOARD, piece, dir.player, Flag::Enpassant);
        }
        
    }
}

void pseudo::calculate_promotion_moves()
{



    if (tile.promotion(player))
    {
        for (const Piece promotion: config::promotion_list)
        {
            calculated_march_moves[][].emplace_promotion(from, to, );
        }
    }

    else
    {
        
    }




        if (isPromotionSquare(loc, player))
    {
        for (const Piece promote: Config::PROMOTION_LIST)
        {
            LookupTable::PSEUDO_LEGAL[hashSquarePiecePlayer(sq, piece, player)].emplace_back(sq, loc, piece, player, promote, marked, flag | Promotion);
        }
    }

    else LookupTable::PSEUDO_LEGAL[hashSquarePiecePlayer(sq, piece, player)].emplace_back(sq, loc, piece, player, Piece::Empty, marked, flag);
}

static bool isPromotionSquare(const Square sq, const Player player)
{
    switch (player)
    {
        case (Red):    return rank(sq) == Config::PROMOTION_RANK;
        case (Blue):   return file(sq) == Config::PROMOTION_RANK;
        case (Yellow): return rank(sq) == Config::MAX_RANK - Config::PROMOTION_RANK;
        case (Green):  return file(sq) == Config::MAX_FILE - Config::PROMOTION_RANK;

        default: return false;
    }
}

static bool isHomeRankSquare(const Square sq, const Player player)
{
    switch (player)
    {
        case (Red):    return rank(sq) == Config::HOME_RANK || rank(sq) == (Config::HOME_RANK - 1); 
        case (Blue):   return file(sq) == Config::HOME_RANK || file(sq) == (Config::HOME_RANK - 1);
        case (Yellow): return rank(sq) == Config::MAX_RANK - Config::HOME_RANK || rank(sq) == (Config::MAX_RANK - Config::HOME_RANK + 1);
        case (Green):  return file(sq) == Config::MAX_FILE - Config::HOME_RANK || file(sq) == (Config::MAX_RANK - Config::HOME_RANK + 1);

        default: return false;
    }
}

static bool isValidSquare(const int r, const int f)
{
    if (r < 0) return false;
    if (f < 0) return false;
    if (r > Config::MAX_RANK) return false;
    if (f > Config::MAX_FILE) return false;
    
    return Config::BRICKS[square(r, f)] == false;
}
--------------------------------------------------------------------------



inline static void push(const Square sq, const Piece piece, const Direction& dir)
{
    int r1 = rank(sq) + dir.horizontal, f1 = file(sq) + dir.vertical;
    int r2 = rank(sq) + 2 * dir.horizontal, f2 = file(sq) + 2 * dir.vertical;

    if (isValidSquare(r1, f1))
    {
        if (dir.flag == SideWays)
        {
            if (Config::SIDE_WAYS) promote(sq, square(r1, f1), Config::OFFBOARD, piece, dir.player, Flag::Push);
        }

        else if (dir.flag == Push)
        {
       
            promote(sq, square(r1, f1), Config::OFFBOARD, piece, dir.player, Flag::Push);

         
            if (isValidSquare(r2, f2) && (Config::TORPERDO || isHomeRankSquare(sq, dir.player))) promote(sq, square(r2, f2), square(r1, f1), piece, dir.player, Flag::DoublePush);
        }
    }
}

inline static void advance(const Square sq, const Piece piece, const Direction& dir)
{
    int r = rank(sq) + dir.horizontal, f = file(sq) + dir.vertical;

    if (isValidSquare(r, f))
    {
        
        promote(sq, square(r, f), Config::OFFBOARD, piece, dir.player, Flag::Advance);

     
        if (!Config::NO_ENPASSANT)
        {
            promote(sq, loc, Config::OFFBOARD, piece, dir.player, Flag::Enpassant);
        }
        
    }
}

inline static void promote(const Square sq, const Square loc, const Square marked, const Piece piece, const Player player, const Flag flag)
{
    if (isPromotionSquare(loc, player))
    {
        for (const Piece promote: Config::PROMOTION_LIST)
        {
            LookupTable::PSEUDO_LEGAL[hashSquarePiecePlayer(sq, piece, player)].emplace_back(sq, loc, piece, player, promote, marked, flag | Promotion);
        }
    }

    else LookupTable::PSEUDO_LEGAL[hashSquarePiecePlayer(sq, piece, player)].emplace_back(sq, loc, piece, player, Piece::Empty, marked, flag);
}

*/