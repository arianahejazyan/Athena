#include "bitboards.h"
#include "gamestate.h"
#include "zobrist.h"

namespace ELSA
{

void Info::clone(const Info& other)
{
    hash = other.hash;
}

Info& GameState::stateInfo() const
{
    return info.node();
}

void GameState::resetGameState()
{
    info.rollback();

    Info& si = stateInfo();

    si.captured = NO_PIECE_TYPE;
    si.castle = (CastlingRights)0u;
    si.epsq = OFFBOARD;
    si.fullmove = 0;
    si.halfmove = 0;
    si.hash = 0ull;
    si.turn = WHITE;

    mailbox.fill(NO_PIECE);
    bitboard.fill(0ull);
    occupied.fill(0ull);
}

Piece GameState::at(Square sq) const {
    return mailbox[sq];
}

u64 GameState::bb(const PieceType type) const {
    return bitboard[type];
}

u64 GameState::occ(const Player player) const {
    return occupied[player];
}

void GameState::setPiece(Square sq, Piece piece)
{
    PieceType type = getPieceType(piece);
    Player player = getPlayer(piece);

    mailbox[sq] = piece;

    bitboard[type] |= BB(sq);
    occupied[player] |= BB(sq);
}

void GameState::popPiece(Square sq)
{
    PieceType type = getPieceType(mailbox[sq]);
    Player player = getPlayer(mailbox[sq]);

    mailbox[sq] = NO_PIECE;

    bitboard[type] ^= BB(sq);
    occupied[player] ^= BB(sq);
}

void GameState::makemove(const Move move)
{
    // Square source = move.source();
    // Square target = move.target();
    // Square nature = move.nature();

    // make & copy new info 
    // info.clone();

    // state info
    // Info& si = info.head();

    // PieceType type = mailbox[source];
    // PieceType take = mailbox[target];

    // si.hash ^= ZobristTurn;

    // mailbox[source] = none;
    // mailbox[target] = type;

    // occupied[type] ^= (1ull << source);
    // occupied[type] ^= (1ull << target);

    // si.hash ^= ZOBRIST[source][type];
    // si.hash ^= ZOBRIST[target][type];

    // if (take)
    // {
    //     occupied[take] ^= BB(target);
    //     si.hash ^= ZOBRIST[target][take];
    // }

    // if (nature == SIMPLE) return;

    // else if (nature == CASTLE)
    // {
    //     PieceType rook = turn == white ? WRook : BRook;

    //     Square rookSource = (Square)(source < target ? source + 3 : source - 4);
    //     Square rookTarget = (Square)(source < target ? source + 1 : source - 1);

    //     mailbox[rookSource] = none;
    //     mailbox[rookTarget] = rook;
    
    //     occupied[rook] ^= (1ull << rookSource);
    //     occupied[rook] ^= (1ull << rookTarget);

    //     si.hash ^= ZOBRIST[rookSource][type];
    //     si.hash ^= ZOBRIST[rookTarget][type];
    // }

    // else if (nature == ENPASS)
    // {
    //     Square epsq = ;
    // }

    // else if (nature == EVOLVE)
    // {

    // }
}

void GameState::undomove(const Move move)
{
    // Square source = move.source();
    // Square target = move.target();
    // Square nature = move.nature();
    
    // // retreive previous info 
    // info.restore();

    // // state info
    // Info& si = info.head();

    // PieceType type = mailbox[target];
    // PieceType take = si.captured;

    // mailbox[source] = type;
    // mailbox[target] = take;

    // occupied[type] ^= (1ull << source);
    // occupied[type] ^= (1ull << target);
    // occupied[take] ^= (1ull << target);
}

}; // namespace