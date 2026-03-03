#include "move_parser.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include "attacks.h"
#include "offset.h"

namespace athena
{

namespace
{

bool set_error(std::string* error, const char* message)
{
    if (error) *error = message;
    return false;
}

bool parse_square_token(std::string_view input, std::size_t& idx, Square& sq)
{
    if (idx >= input.size()) return false;

    const char file_ch = static_cast<char>(std::tolower(static_cast<unsigned char>(input[idx])));
    if (file_ch < 'a' || file_ch > 'n') return false;
    idx++;

    if (idx >= input.size() || !std::isdigit(static_cast<unsigned char>(input[idx]))) return false;

    int rank = 0;
    while (idx < input.size() && std::isdigit(static_cast<unsigned char>(input[idx])))
    {
        rank = (rank * 10) + (input[idx] - '0');
        idx++;
    }

    if (rank < 1 || rank > 14) return false;

    const int file = (file_ch - 'a') + 1;
    sq = Square(rank, file);
    return !sq.stone();
}

bool is_homerank(Color color, Square sq) noexcept
{
    switch (static_cast<uint8_t>(color))
    {
        case Color::Red: return sq.rank() == HOME_RANK || sq.rank() == HOME_RANK - 1;
        case Color::Blue: return sq.file() == HOME_RANK || sq.file() == HOME_RANK - 1;
        case Color::Yellow: return sq.rank() == (RANK_NB - HOME_RANK) || sq.rank() == (RANK_NB - HOME_RANK - 1);
        case Color::Green: return sq.file() == (FILE_NB - HOME_RANK) || sq.file() == (FILE_NB - HOME_RANK - 1);
        default: return false;
    }
}

bool is_promotion_rank(Color color, Square sq) noexcept
{
    switch (static_cast<uint8_t>(color))
    {
        case Color::Red: return sq.rank() == PROMOTION_RANK;
        case Color::Blue: return sq.file() == PROMOTION_RANK;
        case Color::Yellow: return sq.rank() == (RANK_NB - PROMOTION_RANK - 1);
        case Color::Green: return sq.file() == (FILE_NB - PROMOTION_RANK - 1);
        default: return false;
    }
}

Piece parse_promo_piece(char ch)
{
    switch (static_cast<char>(std::tolower(static_cast<unsigned char>(ch))))
    {
        case 'q': return Piece::Queen;
        case 'r': return Piece::Rook;
        case 'b': return Piece::Bishop;
        case 'n': return Piece::Knight;
        default: return Piece::Empty;
    }
}

bool has_castle_right(const Position& pos, Color color, Side side)
{
    const uint8_t rights = static_cast<uint8_t>(pos.state().castle);
    return (rights & static_cast<uint8_t>(Castle(color, side))) != 0;
}

bool is_one_king_step(Square source, Square target)
{
    for (Offset off : crawl_offsets(Piece::King))
    {
        if ((source + off) == target) return true;
    }
    return false;
}

bool is_knight_step(Square source, Square target)
{
    for (Offset off : crawl_offsets(Piece::Knight))
    {
        if ((source + off) == target) return true;
    }
    return false;
}

int step_towards(int from, int to)
{
    if (to > from) return 1;
    if (to < from) return -1;
    return 0;
}

bool is_clear_path(const Position& pos, Square source, Square target, int dr, int df)
{
    int rank = source.rank() + dr;
    int file = source.file() + df;

    while (rank != target.rank() || file != target.file())
    {
        const Square sq(rank, file);
        if (sq.stone()) return false;
        if (pos.board(sq) != PieceClass::Empty()) return false;

        rank += dr;
        file += df;
    }

    return true;
}

bool is_legal_slider_move(const Position& pos, Piece piece, Square source, Square target)
{
    const int d_rank = target.rank() - source.rank();
    const int d_file = target.file() - source.file();

    const bool rook_like = (d_rank == 0) != (d_file == 0);
    const bool bishop_like = std::abs(d_rank) == std::abs(d_file) && d_rank != 0;

    if (piece == Piece::Rook)
    {
        if (!rook_like) return false;
        return is_clear_path(pos, source, target, step_towards(source.rank(), target.rank()), step_towards(source.file(), target.file()));
    }

    if (piece == Piece::Bishop)
    {
        if (!bishop_like) return false;
        return is_clear_path(pos, source, target, step_towards(source.rank(), target.rank()), step_towards(source.file(), target.file()));
    }

    if (piece == Piece::Queen)
    {
        if (!rook_like && !bishop_like) return false;
        return is_clear_path(pos, source, target, step_towards(source.rank(), target.rank()), step_towards(source.file(), target.file()));
    }

    return false;
}

} // namespace

bool parse_move_uci(const Position& pos, std::string_view uci, Move& out_move, std::string* error)
{
    std::size_t idx = 0;
    Square source;
    Square target;

    if (!parse_square_token(uci, idx, source)) return set_error(error, "invalid source square");
    if (!parse_square_token(uci, idx, target)) return set_error(error, "invalid target square");

    Piece promo_piece = Piece::Empty;
    if (idx < uci.size())
    {
        if ((idx + 1) != uci.size()) return set_error(error, "invalid move format");
        promo_piece = parse_promo_piece(uci[idx]);
        if (promo_piece == Piece::Empty) return set_error(error, "invalid promotion piece");
        idx++;
    }

    const PieceClass attacker = pos.board(source);
    const PieceClass defender = pos.board(target);

    if (attacker == PieceClass::Empty() || attacker == PieceClass::Stone()) return set_error(error, "no piece on source square");
    if (attacker.color() != pos.turn()) return set_error(error, "piece color does not match side to move");
    if (defender == PieceClass::Stone()) return set_error(error, "target is not a playable square");
    if (defender != PieceClass::Empty() && defender.color() == attacker.color()) return set_error(error, "cannot capture your own piece");

    const Color turn = attacker.color();
    const Piece piece = attacker.piece();
    Move move;

    if (piece == Piece::Pawn)
    {
        const Offset push = push_offsets(turn)[0];
        const Offset stride = push_offsets(turn)[1];
        const auto takes = take_offsets(turn);

        const bool promotion = is_promotion_rank(turn, target);
        if (promotion && promo_piece == Piece::Empty) promo_piece = Piece::Queen;
        if (!promotion && promo_piece != Piece::Empty) return set_error(error, "promotion piece only allowed on promotion rank");

        if ((source + push) == target && defender == PieceClass::Empty())
        {
            move = promotion ? Move(source, target, promo_piece) : Move(source, target, MoveType::Simple);
        }
        else if ((source + stride) == target && defender == PieceClass::Empty())
        {
            if (!is_homerank(turn, source)) return set_error(error, "pawn stride only allowed from home rank");
            if (pos.board(source + push) != PieceClass::Empty()) return set_error(error, "pawn stride path is blocked");
            if (promotion) return set_error(error, "invalid promotion stride");
            move = Move(source, target, MoveType::Stride);
        }
        else if ((source + takes[0]) == target || (source + takes[1]) == target)
        {
            if (defender != PieceClass::Empty())
            {
                move = promotion ? Move(source, target, promo_piece) : Move(source, target, MoveType::Simple);
            }
            else
            {
                bool enpass = false;
                Color victim = Color::None;
                for (Color op : opponents(turn))
                {
                    if (pos.enpass(op) == target)
                    {
                        enpass = true;
                        victim = op;
                        break;
                    }
                }

                if (!enpass) return set_error(error, "invalid pawn capture target");
                move = Move(source, target, victim);
            }
        }
        else
        {
            return set_error(error, "illegal pawn move");
        }
    }
    else if (piece == Piece::King)
    {
        bool did_castle = false;
        for (Side side : {Side::KingSide, Side::QueenSide})
        {
            const auto squares = castle_squares(turn, side);
            if (squares[0] == source && squares[1] == target)
            {
                if (!has_castle_right(pos, turn, side)) return set_error(error, "castling rights not available");

                const auto path = path_squares(turn, side);
                for (Square sq : path)
                {
                    if (pos.board(sq) != PieceClass::Empty()) return set_error(error, "castling path is blocked");
                }

                const auto safe = safe_squares(turn, side);
                if (pos.inCheck(turn, safe[0]) || pos.inCheck(turn, safe[1])) return set_error(error, "castling through check is not allowed");

                move = Move(source, target, side);
                did_castle = true;
                break;
            }
        }

        if (!did_castle)
        {
            if (!is_one_king_step(source, target)) return set_error(error, "illegal king move");
            move = Move(source, target, MoveType::Simple);
        }
    }
    else if (piece == Piece::Knight)
    {
        if (!is_knight_step(source, target)) return set_error(error, "illegal knight move");
        move = Move(source, target, MoveType::Simple);
    }
    else
    {
        if (promo_piece != Piece::Empty) return set_error(error, "promotion piece only valid for pawns");
        if (!is_legal_slider_move(pos, piece, source, target)) return set_error(error, "illegal sliding move");
        move = Move(source, target, MoveType::Simple);
    }

    Position next = pos;
    next.makemove(move);
    if (next.inCheck(turn)) return set_error(error, "move leaves king in check");

    out_move = move;
    if (error) error->clear();
    return true;
}

} // namespace athena
