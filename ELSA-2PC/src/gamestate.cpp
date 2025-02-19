#include "bitboards.h"
#include "gamestate.h"
#include "zobrist.h"

namespace ELSA
{

void Info::clone(const Info& other)
{
    turn = other.turn;
    epsq = other.epsq;
    hash = other.hash;
    castle = other.castle;
    captured = other.captured;
    halfmove = other.halfmove;
    fullmove = other.fullmove;
}

Info& GameState::stateInfo() const
{
    return info.node();
}

void GameState::resetGameState()
{
    info.rollback();

    Info& si = stateInfo();

    si.captured = NO_PIECE;
    si.castle = NO_CASTLING;
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
    mailbox[sq] = piece;

    bitboard[getPieceType(piece)] |= BB(sq);
    occupied[getPlayer(piece)]    |= BB(sq);
}

void GameState::popPiece(Square sq)
{
    mailbox[sq] = NO_PIECE;

    bitboard[getPieceType(mailbox[sq])] ^= BB(sq);
    occupied[getPlayer(mailbox[sq])]    ^= BB(sq);
}

void GameState::makemove(const Move move)
{
    Square source = move.source();
    Square target = move.target();
    Square nature = move.nature();

    info.clone();

    auto& si = info.node();

    Piece type = mailbox[source];
    Piece take = mailbox[target];

    si.hash ^= ZobristTurn;

    if (take != NO_PIECE)
    {
        popPiece(target);

        si.hash ^= Zobrist[target][take];
    }

    setPiece(target, type);
    popPiece(source);

    si.hash ^= Zobrist[source][type] ^ Zobrist[target][type];


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

    si.turn = si.turn == WHITE ? BLACK : WHITE;
}

void GameState::undomove(const Move move)
{
    Square source = move.source();
    Square target = move.target();
    Square nature = move.nature();
    
    info.restore();

    auto si = info.node();

    Piece type = mailbox[target];
    Piece take = si.captured;

    setPiece(source, type);
    popPiece(target);

    if (take != NO_PIECE) setPiece(target, take);
}

}; // namespace