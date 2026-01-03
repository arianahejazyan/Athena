#include "movegen.h"
#include "chess.h"
#include "position.h"

namespace athena
{

template<Piece piece>
Move* genCrawlMoves(const Position& pos, Move* moves, Square source)
{
    for (auto offset : crawl_offsets(piece))
    {
        Square target = source + offset;
        PieceClass pc = pos.board(target);

        // *moves = Move(source, target, MoveType::Simple);

        // moves += (pc != PieceClass::Stone()) && ((pc == PieceClass::Empty()) || (isOpponent(pc.color(), pos.turn())));

        if (pc == PieceClass::Stone())
            continue;

        if (pc == PieceClass::Empty())
        {
            *moves = Move(source, target, MoveType::Simple);
            moves++;
            continue;
        }

        if (isOpponent(pc.color(), pos.turn()))
        {
            *moves = Move(source, target, MoveType::Simple);
            moves++;
        }
    }
    
    return moves;
}

template<Piece piece>
Move* genSlideMoves(const Position& pos, Move* moves, Square source)
{
    for (auto offset : slide_offsets(piece))
    {
        Square target = source;

        bool blocked = false;
        int step = 0;
        while (step < max_step(piece) && !blocked)
        {
            target += offset;
            PieceClass pc = pos.board(target);

            *moves = Move(source, target, MoveType::Simple);

            moves += (pc != PieceClass::Stone()) && ((pc == PieceClass::Empty()) || (isOpponent(pc.color(), pos.turn())));

            blocked = (pc == PieceClass::Stone()) || (pc != PieceClass::Empty());
            step++;
        }
    }

    return moves;
}

template<>
Move* genSlideMoves<Piece::Queen>(const Position& pos, Move* moves, Square source)
{
    moves = genSlideMoves<Piece::Rook  >(pos, moves, source);
    moves = genSlideMoves<Piece::Bishop>(pos, moves, source);
    return moves;
}

template<Color turn>
Move* genEvolveMoves(Move* moves, Square source, Square target)
{
    if (isPromotion<turn>(target))
    {
        *(moves++) = Move(source, target, Piece::Queen );
        *(moves++) = Move(source, target, Piece::Rook  );
        *(moves++) = Move(source, target, Piece::Bishop);
        *(moves++) = Move(source, target, Piece::Knight);
    }

    else *(moves++) = Move(source, target, MoveType::Simple); 

    return moves;
}

template<Color turn>
Move* genPushMoves(const Position& pos, Move* moves, Square source)
{
    Square target = source + push_offsets(turn)[0];

    if (pos.board(target) == PieceClass::Empty())
    {
        moves = genEvolveMoves<turn>(moves, source, target);
        target = target + push_offsets(turn)[0];

        if (pos.board(target) == PieceClass::Empty() && isHomerank<turn>(source))
        {
            *(moves++) = Move(source, target, MoveType::Stride);
        }   
    }

    return moves;
}

template<Color turn>
Move* genTakeMoves(const Position& pos, Move* moves, Square source)
{
    for (auto offset : take_offsets(turn))
    {
        Square target = source + offset;
        PieceClass pc = pos.board(target);

        if (pc == PieceClass::Empty() || pc == PieceClass::Stone() || isTeammate(pc.color(), turn))
            continue;

        moves = genEvolveMoves<turn>(moves, source, target);
    }

    return moves;
}

template<Color turn>
Move* genMarchMoves(const Position& pos, Move* moves, Square source)
{
    moves = genTakeMoves<turn>(pos, moves, source);
    moves = genPushMoves<turn>(pos, moves, source);
    return moves;
}

template<Color turn>
Move* genEnpassMoves(const Position& pos, Move* moves)
{
    for (Color opponent: opponents(turn))
    {
        Square target = pos.enpass(opponent);

        if (target == Square::Offboard)
            continue;

        constexpr std::array<Offset, TAKE_NB> takeR = take_offsets(Color::Red   );
        constexpr std::array<Offset, TAKE_NB> takeB = take_offsets(Color::Blue  );
        constexpr std::array<Offset, TAKE_NB> takeY = take_offsets(Color::Yellow);
        constexpr std::array<Offset, TAKE_NB> takeG = take_offsets(Color::Green );

        for (auto offset : (turn == Color::Red ? takeY : turn == Color::Blue ? takeG : turn == Color::Yellow ? takeR : takeB))
        {
            Square source = target + offset;

            if (pos.board(source) == PieceClass(turn, Piece::Pawn))
            {
                moves = genEvolveMoves<turn>(moves, source, target);
            }
        }
    }

    return moves;
}

template<Color color, Side side>
Move* genSingleCastleMove(const Position& pos, Move* moves)
{
    const auto& castle = pos.state().castle;

    if (!hasCastle<color, side>(castle))
        return moves;

    constexpr Square path0 = path_squares(color, side)[0];
    constexpr Square path1 = path_squares(color, side)[1];
    constexpr Square path2 = path_squares(color, side)[2];

    if (
    pos.board(path0) != PieceClass::Empty() || 
    pos.board(path1) != PieceClass::Empty() || 
    pos.board(path2) != PieceClass::Empty()
    )
        return moves;

    constexpr Square safe0 = safe_squares(color, side)[0];
    constexpr Square safe1 = safe_squares(color, side)[1];

    if (
    pos.inCheck(color, safe0) || 
    pos.inCheck(color, safe1)
    )
        return moves;

    constexpr Square king_source = castle_squares(color, side)[0];
    constexpr Square king_target = castle_squares(color, side)[1];
    constexpr Square rook_source = castle_squares(color, side)[2];
    constexpr Square rook_target = castle_squares(color, side)[3];

    *(moves++) = Move(king_source, king_target, side);

    return moves;
}

template<Color color>
Move* genCastleMoves(const Position& pos, Move* moves)
{
    moves = genSingleCastleMove<color, Side::KingSide >(pos, moves);
    moves = genSingleCastleMove<color, Side::QueenSide>(pos, moves); 
    return moves;
}

template<Color turn>
Move* genMoves(const Position& pos, Move* moves)
{
    // checkmask
    // pinned

    for (Square sq = Square::E2; sq <= Square::L15; sq++)
    {
        PieceClass pc = pos.board(sq);

        if (pc.color() == turn)
        {
            switch (pc.piece())
            {
                case Piece::Pawn  : moves = genMarchMoves<turn         >(pos, moves, sq); break;
                case Piece::Knight: moves = genCrawlMoves<Piece::Knight>(pos, moves, sq); break;
                case Piece::Bishop: moves = genSlideMoves<Piece::Bishop>(pos, moves, sq); break;
                case Piece::Rook  : moves = genSlideMoves<Piece::Rook  >(pos, moves, sq); break;
                case Piece::Queen : moves = genSlideMoves<Piece::Queen >(pos, moves, sq); break;
                case Piece::King  : moves = genCrawlMoves<Piece::King  >(pos, moves, sq); break;

                default: break;
            }               
        }  
    }

    moves = genEnpassMoves<turn>(pos, moves);
    moves = genCastleMoves<turn>(pos, moves);

    return moves;
}

std::size_t genAllPseudoMoves(const Position& pos, Move* moves)
{
    return 
    pos.turn() == Color::Red    ? genMoves<Color::Red   >(pos, moves) - moves :
    pos.turn() == Color::Blue   ? genMoves<Color::Blue  >(pos, moves) - moves :
    pos.turn() == Color::Yellow ? genMoves<Color::Yellow>(pos, moves) - moves :
    pos.turn() == Color::Green  ? genMoves<Color::Green >(pos, moves) - moves : 0;
}

} // namespace athena