#include "definitions.hpp"
#include "pseudo.hpp"

using namespace pseudo;
using namespace definitions;

namespace generator
{

template<Piece Attacker>
bool AttackedByCrawl(const Position& pos, Square sq, Player player)
{
    int head = heads[CrawlShift(sq, Attacker)];
    int tail = tails[CrawlShift(sq, Attacker)];

    for (int shift = head; shift != tail; ++shift)
    {
        Square target = moves[shift];

        if (opponent(pos.players[target], player) && pos.pieces[target] == Attacker)
        {
            return true;
        }
    }

    return false;
}

template<int Ray>
bool AttackedBySlide(const Position& pos, Square sq, Player player)
{

}

template<Player Attacker>
bool AttackedByAdvance(const Position& pos, Square sq, Player player)
{

}

bool IsSquareSafe(const Position& pos, Square sq, Player player);

//bool AttackedByCrawl(const Position& pos, Square sq, Player player, Piece attacker);

// bool isSquareSafe(const Position& pos, Square sq);

// void generate_crawl_moves(const Position& parent, Square from, vector<Position>& moves);

//void generate_all_moves(const Position& pos, vector<Position>& moves);

}; // namespace


/*
namespace Generator
{
    void generate_all_positions(const Position& pos, std::vector<Position>& list);
};



struct list;

namespace Engine
{
    void generate_all_positions(Position pos, list moves);

    void generate_positions(Position pos, Square sq, list moves);

    void generate_leap();

    void generate_slide();

    void generate_push();

    void generate_advance();

    void generate_castle();
};

*/