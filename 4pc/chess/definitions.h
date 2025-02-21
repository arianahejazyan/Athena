#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <array>

using namespace std;

namespace definitions
{

typedef unsigned int Square;

enum Player {Red=0, Blue=1, Yellow=2, Green=3, None=4};

enum Side {KingSide=0, QueenSide=1};

enum Piece {Empty=0, King=1, Queen=2, Rook=3, Bishop=4, Knight=5, Pawn=6};

enum Flag {Null=0, Push=1, DoublePush=2, SideWays, Advance=4, Enpassant=8, Promotion=16, Crawl=32, Slide=64, Castle=128, Capture=256, Check=512, Pass};

enum State {Alive=0, Dead=1};

enum Setup {Modern=0, Classic=1};

enum Mode {Team=0, FFA=1};

enum Alliance {RY=0, BG=1};

enum StaleMateType {Draw=0, Win=1, Lose=2};

// number of rows and cols
constexpr unsigned int dim = 14;

// number of valid squares
constexpr unsigned int board_size = 160;

// special square outside the board
constexpr Square offboard = 160;

// used to move in the board
constexpr int S = 0, U = 1, D = -1, R = 1, L = -1;

// used to convert 196 locations to 160 squares
extern const int bricks[14];

// used to extract rank and file of squares
extern const int stones[20];

// convert row and col to valid square
#define spot(row, col) (row * dim + col - bricks[row])

// extract row of valid square
#define rank(square) ((square + stones[square / 8]) / dim)

// extract column of valid square
#define file(square) ((square + stones[square / 8]) % dim)







constexpr unsigned int player_size = 4;

constexpr unsigned int side_size = 2;




bool valid_square(int row, int col);











Player& operator++(Player& player);







struct Position
{
    Player turn;
    Piece pieces[board_size];
    Player players[board_size];

    Square marked[player_size];
    Square target[player_size];
 
    Square royals[4];

    bool rights[player_size][side_size];

    void place(Square sq, Piece piece, Player player)
    {
        pieces[sq] = piece;
        players[sq] = player;
    }

    void remove(Square sq)
    {
        pieces[sq] = Empty;
        players[sq] = None;
    }

    int fifty_rule;

    State states[player_size];

    int scores[player_size];

    // unsigned short safty[4];
    //unsigned int possessions[4];
};

struct Offset
{
    int vertical;
    int horizontal;
    Player player;
    Flag flag;

    Offset(int v, int h, Flag f): vertical(v), horizontal(h), flag(f) {}

    Offset(int v, int h, Player p, Flag f): vertical(v), horizontal(h), player(p), flag(f) {}
};



extern const vector<Offset> patterns[6];


constexpr unsigned int maxrank = dim - 1;
constexpr unsigned int homerank = 1;
constexpr unsigned int promotionrank = 10;


extern Player enemy[2][2];


bool promotion_square(const Square sq, const Player player);

bool homerank_square(const Square sq, const Player player);



#define teammate(player) ((player + 2) % player_size)

#define guild(player) (player % 2)

// #define enemy(player) (guild(player) == RY ? {Blue, Green}: {Red, Yellow})

#define next(player) ((player + 1) % 4)

#define opponent(p1, p2) ((p1 + p2) % 2)

#define teamamte(p1, p2) ((p1 + p2 + 1) % 2)

};


/*



struct TimeControl
{
    enum Type {Increament, Delay};

    // Time base;
    // Time value;
    Type type;
};




*/