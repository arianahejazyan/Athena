#include "definitions.h"

namespace definitions
{

// used to convert 196 locations to 160 squares
constexpr int bricks[14] = {3,9,15,18,18,18,18,18,18,18,18,21,27,33};

// used to extract rank and file of squares
constexpr int stones[20] = {3,9,15,18,18,18,18,18,18,18,18,18,18,18,18,18,18,21,27,33};

bool valid_square(int row, int col)
{
    if (row < 0)  return false;
    if (col < 0)  return false;
    if (row > 13) return false;
    if (col > 13) return false;
    if (row < 3  && col < 3)  return false;
    if (row < 3  && col > 10) return false;
    if (row > 10 && col < 3)  return false;
    if (row > 10 && col > 10) return false;
    
    return true;
}

Player& operator++(Player& player)
{
    player = static_cast<Player>((player + 1) % 5);

    return player;
}

} // namespace

/*

namespace Definitions
{


    const Direction KING_DEFINITION[8] =
    {
        Direction(Offset::U, Offset::S, Player::None, Flag::Leap),
        Direction(Offset::D, Offset::S, Player::None, Flag::Leap),
        Direction(Offset::S, Offset::R, Player::None, Flag::Leap),
        Direction(Offset::S, Offset::L, Player::None, Flag::Leap),
        Direction(Offset::U, Offset::R, Player::None, Flag::Leap),
        Direction(Offset::U, Offset::L, Player::None, Flag::Leap),
        Direction(Offset::D, Offset::R, Player::None, Flag::Leap),
        Direction(Offset::D, Offset::L, Player::None, Flag::Leap),
    };

    const Direction QUEEN_DEFINITION[8] = 
    {
        Direction(Offset::U, Offset::S, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::S, Player::None, Flag::Slide),
        Direction(Offset::S, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::S, Offset::L, Player::None, Flag::Slide),
        Direction(Offset::U, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::U, Offset::L, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::L, Player::None, Flag::Slide),
    };

    const Direction ROOK_DEFINITION[4] = 
    {
        Direction(Offset::U, Offset::S, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::S, Player::None, Flag::Slide),
        Direction(Offset::S, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::S, Offset::L, Player::None, Flag::Slide),
    };

    const Direction BISHOP_DEFINITION[4] = 
    {
        Direction(Offset::U, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::U, Offset::L, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::R, Player::None, Flag::Slide),
        Direction(Offset::D, Offset::L, Player::None, Flag::Slide),
    };

    const Direction KNIGHT_DEFINITION[8] = 
    {
        Direction(2 * Offset::U, Offset::R, Player::None, Flag::Leap),
        Direction(2 * Offset::U, Offset::L, Player::None, Flag::Leap),
        Direction(Offset::U, 2 * Offset::R, Player::None, Flag::Leap),
        Direction(Offset::U, 2 * Offset::L, Player::None, Flag::Leap),
        Direction(2 * Offset::D, Offset::R, Player::None, Flag::Leap),
        Direction(2 * Offset::D, Offset::R, Player::None, Flag::Leap),
        Direction(Offset::D, 2 * Offset::R, Player::None, Flag::Leap),
        Direction(Offset::D, 2 * Offset::L, Player::None, Flag::Leap),
    };

    const Direction PAWN_DEFINITION[20] = 
    {
        Direction(Offset::U, Offset::S, Player::Red, Flag::Push),
        Direction(Offset::S, Offset::R, Player::Red, Flag::SideWays),
        Direction(Offset::S, Offset::L, Player::Red, Flag::SideWays),
        Direction(Offset::U, Offset::R, Player::Red, Flag::Advance),
        Direction(Offset::U, Offset::L, Player::Red, Flag::Advance),

        Direction(Offset::S, Offset::R, Player::Blue, Flag::Push),
        Direction(Offset::U, Offset::S, Player::Blue, Flag::SideWays),
        Direction(Offset::D, Offset::S, Player::Blue, Flag::SideWays),
        Direction(Offset::U, Offset::R, Player::Blue, Flag::Advance),
        Direction(Offset::D, Offset::R, Player::Blue, Flag::Advance),

        Direction(Offset::D, Offset::S, Player::Yellow, Flag::Push),
        Direction(Offset::S, Offset::R, Player::Yellow, Flag::SideWays),
        Direction(Offset::S, Offset::L, Player::Yellow, Flag::SideWays),
        Direction(Offset::D, Offset::R, Player::Yellow, Flag::Advance),
        Direction(Offset::D, Offset::L, Player::Yellow, Flag::Advance),

        Direction(Offset::S, Offset::L, Player::Green, Flag::Push),
        Direction(Offset::U, Offset::S, Player::Green, Flag::SideWays),
        Direction(Offset::D, Offset::S, Player::Green, Flag::SideWays),
        Direction(Offset::U, Offset::L, Player::Green, Flag::Advance),
        Direction(Offset::D, Offset::L, Player::Green, Flag::Advance),
    };

    std::vector<Piece, std::vector<Direction>> PIECE_DEFINITIONS;
};

Offset operator*(int scalar, Offset offset) 
{
    return static_cast<Offset>(scalar * offset);
}

Offset operator*(Offset offset, int scalar) 
{
    return static_cast<Offset>(scalar * offset);
}

Offset operator+(Offset x, Offset y) 
{
    return static_cast<Offset>(x + y);
}


Move::Move():

    from(DEFS::OFFBOARD), 
    to(DEFS::OFFBOARD), 
    piece(Piece::Empty), 
    player(Player::None), 
    promotion(Piece::Empty), 
    marked(DEFS::OFFBOARD), 
    side(Side::Neither), 
    flag(Flag::Null) {}

Move::Move(Square from, Square to, Piece piece, Flag flag): 

    from(from), 
    to(to), 
    piece(piece), 
    player(Player::None), 
    promotion(Piece::Empty), 
    marked(DEFS::OFFBOARD), 
    side(Side::Neither), 
    flag(Flag::Null) {}

Move::Move(Square from, Square to, Piece piece, Player player, Piece promotion, Square marked, Flag flag): 

    from(from), 
    to(to), 
    piece(piece), 
    player(player), 
    promotion(promotion), 
    marked(marked), 
    side(Side::Neither), 
    flag(flag) {}

Move::~Move() {}

Position::Position() {};

Position::Position(const Position& pos)
{
    turn = pos.turn;


    for (Square sq: CONFIG::VALID_SQUARES)
    {
        pieces[sq] = pos.pieces[sq];
        players[sq] = pos.players[sq];
    }

    for (Player player: CONFIG::PLAYERS)
    {
        alive[player] = pos.alive[player];


        if (pos.alive[player])
        {
            royals[player] = pos.royals[player];
            marked[player] = pos.marked[player];
            target[player] = pos.target[player];
            safty[player] = pos.safty[player];
            rights[player][KingSide] = pos.rights[player][KingSide];
            rights[player][QueenSide] = pos.rights[player][QueenSide];
        }
    }
}

Position::~Position() {};


//Player::Player(Player Player): Player(Player) {}

//Player::~Player() {}


//Direction::Direction(Offset rank, Offset file, Player player, Flag flag): rank(rank), file(file), player(player), flag(flag) {}


void PLAYER::foo()
{
    for (const Player& p1: Player_LIST)
    {
        for (const Player& p2: Player_LIST)
        {
            switch (Config::setup)
            {
                case (FFA):
                    TEAMMATE[p1][p2] = (p1 == p2);
                    break;

                case (RB):
                    TEAMMATE[p1][p2] = (p1 == Red || p1 == Blue) ? (p2 == Red || p2 == Blue): (p2 == Yellow || p2 == Green);
                    break;

                case (RY):
                    TEAMMATE[p1][p2] = (p1 == Red || p1 == Yellow) ? (p2 == Red || p2 == Yellow): (p2 == Blue || p2 == Green);
                    break;

                case (RG):
                    TEAMMATE[p1][p2] = (p1 == Red || p1 == Green) ? (p2 == Red || p2 == Green): (p2 == Blue || p2 == Yellow);
                    break;
            
                default: break;
            }

            OPPONENT[p1][p2] = !TEAMMATE[p1][p2];
        }
    }    
}

*/
