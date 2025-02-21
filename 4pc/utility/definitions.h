#pragma once
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

namespace definitions
{

bool valid_square(int row, int col);


// number of rows and cols
constexpr unsigned int dim = 14;

typedef unsigned int Square;

constexpr Square offboard = 160;

constexpr unsigned int board_size = 160;

constexpr unsigned int player_size = 4;

constexpr unsigned int side_size = 2;

enum Player {Red=0, Blue=1, Yellow=2, Green=3, None=4};

enum Side {KingSide=0, QueenSide=1};

enum Piece {Brick=-1, Empty=0, King=1, Queen=2, Rook=3, Bishop=4, Knight=5, Pawn=6};

Player& operator++(Player& player);

// used to convert 196 locations to 160 squares
extern const int bricks[14];

// used to extract rank and file of squares
extern const int stones[20];

#define Spot(row, col) (row * dim + col - bricks[row])
#define Rank(square) ((square + stones[square / 8]) / dim)
#define File(square) ((square + stones[square / 8]) % dim)

struct Position
{
    Player turn;
    Piece pieces[board_size];
    Player players[board_size];

    Square marked[player_size];
    Square target[player_size];
 

    bool rights[player_size][side_size];

    void place(Square sq, Piece piece, Player player)
    {
        pieces[sq] = piece;
        players[sq] = player;
    }
};

};


/*
#define opponent(p1, p2) ((p1 + p2) % 2)
#define teamamte(p1, p2) ((p1 + p2 + 1) % 2)

enum Square {A1,  B1,  C1,  D1,  E1,  F1,  G1,  H1,  I1,  J1,  K1,  L1,  M1,  N1,
             A2,  B2,  C2,  D2,  E2,  F2,  G2,  H2,  I2,  J2,  K2,  L2,  M2,  N2,
             A3,  B3,  C3,  D3,  E3,  F3,  G3,  H3,  I3,  J3,  K3,  L3,  M3,  N3,
             A4,  B4,  C4,  D4,  E4,  F4,  G4,  H4,  I4,  J4,  K4,  L4,  M4,  N4,
             A5,  B5,  C5,  D5,  E5,  F5,  G5,  H5,  I5,  J5,  K5,  L5,  M5,  N5,
             A6,  B6,  C6,  D6,  E6,  F6,  G6,  H6,  I6,  J6,  K6,  L6,  M6,  N6,
             A7,  B7,  C7,  D7,  E7,  F7,  G7,  H7,  I7,  J7,  K7,  L7,  M7,  N7,
             A8,  B8,  C8,  D8,  E8,  F8,  G8,  H8,  I8,  J8,  K8,  L8,  M8,  N8,
             A9,  B9,  C9,  D9,  E9,  F9,  G9,  H9,  I9,  J9,  K9,  L9,  M9,  N9,
             A10, B10, C10, D10, E10, F10, G10, H10, I10, J10, K10, L10, M10, N10,
             A11, B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11,
             A12, B12, C12, D12, E12, F12, G12, H12, I12, J12, K12, L12, M12, N12,
             A13, B13, C13, D13, E13, F13, G13, H13, I13, J13, K13, L13, M13, N13,
             A14, B14, C14, D14, E14, F14, G14, H14, I14, J14, K14, L14, M14, N14, X};

enum Piece {Brick=-1, Empty=0, King=1, Queen=2, Rook=3, Bishop=4, Knight=5, Pawn=6};

enum Offset3 {S=0, U=1, R=1, D=-1, L=-1};

enum Flag {Null=0, Push=1, DoublePush=2, SideWays, Advance=4, Enpassant=8, Promotion=16, Leap=32, Slide=64, Castle=128, Capture=256, Check=512, Pass};



enum GameState {Ongoing, Over};

enum PlayerState {Alive, DeadS};

enum Player {Red=0, Blue=1, Yellow=2, Green=3, None=4, Dead=5};

enum Mode {RB, RY, RG, WB, FFA};


enum Setup {Custom, Modern, Classic, Traditional};

enum StaleMateType {Draw, Win, Lose};


struct TimeControl
{
    enum Type {Increament, Delay};

    // Time base;
    // Time value;
    Type type;
};
*/
/*
Offset operator*(int scalar, Offset offset);
Offset operator*(Offset offset, int scalar);*/
//Offset operator+(Offset x, Offset y);
//Offset operator+(Square x, Offset y);
//Offset operator+(Offset x, Square y);
//Offset operator+(int x, Offset y);
//Offset operator+(Offset x, int y);
/*
Flag operator|(Flag f1, Flag f2);

typedef int Ray;


struct Tile
{
    unsigned int row;
    unsigned int col;
};

struct Move
{
    Square from;
    Square to;
    Piece piece;
    Player player;
    Piece promotion;
    Square marked;
    Square target;
    Side side;
    Flag flag;

    Move();
    Move(Square from, Square to, Piece piece, Flag flag);
    Move(Square from, Square to, Piece piece, Player player, Piece promotion, Square marked, Flag flag);
    ~Move();
};





struct Position
{

    Piece* pieces;
    Player* players;  
    Square* royals;

    bool* alive;
    unsigned short* safty;
    bool** rights;
    unsigned int* possessions;

    PlayerState state;
    int time;
    int points;

    Position();
    Position(const Position& pos);
    ~Position();
};

struct Direction
{
    Offset horizontal;
    Offset vertical;
    Player player;
    Flag flag;

    Direction(Offset rank, Offset file, Player player, Flag flag);
};


struct Dir
{
    int horizontal;
    int vertical;

    Dir(int x, int y): horizontal(x), vertical(y) {}
}


#define next(player) ((player + 1) % 4)

namespace Definitions
{

    extern std::vector<Piece, std::vector<Direction>> PIECE_DEFINITIONS;


    extern bool* PROTECTED_PIECE;

    void init();
};

namespace engine
{
    std::vector<Square> valid_squares;
};*/