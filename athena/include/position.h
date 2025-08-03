#ifndef POSITION_H
#define POSITION_H

#include "bitboard.h"
#include "chess.h"

#include <iostream>//delete

namespace athena
{

constexpr std::size_t PLAY_NB = 32;
    // class Board
    // {
    // public:
    //     alignas(32) BB bitboards[PIECECLASS_NB];
    //     alignas(32) PC mailbox[SQUARE_NB];
    //     alignas(32) SQ royals[(PLAYER_NB - 4)];

    //     Board() {}

    //     void reset()
    //     {
    //         for (int s = 0; s < SQUARE_NB; ++s)
    //         {
    //             auto sq = static_cast<SQ>(s);
    //             auto r = rankSQ(sq);
    //             auto f = fileSQ(sq);
    //             mailbox[sq] = isInValidSquare(r, f) ? PC(All, None) : PC(Empty, None);
    //         }

    //         for (int pc = 0; pc < PIECECLASS_NB; ++pc)
    //             bitboards[pc].zero();

    //         for (int p = 0; p < (PLAYER_NB - 4); ++p)
    //             royals[p] == OFFBOARD;

    //         bitboards[PC(Empty, None)] = BB(0x0FF00FF00FF00000, 0x7FFE7FFE7FFE7FFE, 0x7FFE7FFE7FFE7FFE, 0x00000FF00FF00FF0);
    //     }

    //     inline void setSQ(SQ sq, PC pc)
    //     {
    //         //
    //         mailbox[sq] = pc;

    //         //
    //         auto piece = pc.piece();
    //         auto player = pc.player();
    //         auto guild = toGuild(player);

    //         //
    //         bitboards[PC(Empty, None)].popSQ(sq);

    //         //
    //         bitboards[pc].setSQ(sq);

    //         //
    //         bitboards[PC(piece, guild)].setSQ(sq);
    //         bitboards[PC(piece, RBYG)].setSQ(sq);
    //         bitboards[PC(All, player)].setSQ(sq);
    //         bitboards[PC(All, guild)].setSQ(sq);
    //         bitboards[PC(All, RBYG)].setSQ(sq);
    //     }

    //     inline void popSQ(SQ sq)
    //     {
    //         auto pc = mailbox[sq];

    //         //
    //         mailbox[sq] = PC(Empty, None);

    //         //
    //         auto piece = pc.piece();
    //         auto player = pc.player();
    //         auto guild = toGuild(player);

    //         //
    //         bitboards[PC(Empty, None)].setSQ(sq);

    //         //
    //         bitboards[pc].popSQ(sq);
    //         bitboards[PC(piece, guild)].popSQ(sq);
    //         bitboards[PC(piece, RBYG)].popSQ(sq);
    //         bitboards[PC(All, player)].popSQ(sq);
    //         bitboards[PC(All, guild)].popSQ(sq);
    //         bitboards[PC(All, RBYG)].popSQ(sq);
    //     }
    // };



//     constexpr uint64_t CASTLE_ROOK_ORIGIN =
//         (1ULL << 1 ) |
//         (1ULL << 14) |
//         (1ULL << 20) |
//         (1ULL << 27) |
//         (1ULL << 36) |
//         (1ULL << 43) |
//         (1ULL << 49) |
//         (1ULL << 62);

//     inline auto encode(Color color, Side side) {
//         return static_cast<uint8_t>((1 << side) << (color << 1));
//     }

class GameState;
class Position;

using GS = GameState;
using PS = Position;

struct alignas(32) GameState
{
    uint64_t zobrist;
    Pack royals;
    Pack enpass;
    Rights rights;
    Color turn;
    PC capture;
    uint8_t padding[13];
};

class alignas(CACHELINE_SIZE) Position
{
    public:
    BB bitboards[PIECECOLOR_NB]; // make private better design!!!! remove useless BB
    PC mailbox[SQUARE_NB];
    GS states[PLAY_NB];

    inline auto at(SQ sq) const {
        return mailbox[sq];
    }

    inline void toggleSQ(SQ sq, PC pc)
    {
        const auto chunk = sq.chunk();
        const auto index = (1ULL << sq.index());
        
        // 
        const auto piece = pc.piece();
        const auto color = pc.color();
        // const auto guild = pc.guild(); 

        //
        auto& bb1 = bitboards[pc.encoded];
        auto& bb2 = bitboards[PC(piece, color.team()).encoded];

        bb1.toggleSQ(chunk, index);
        bb2.toggleSQ(chunk, index);

        //
        auto& bb3 = bitboards[PC::ORANGE + (color.encoded & 1)];
        auto& bb4 = bitboards[PC::EVERYTHING];

        bb3.toggleSQ(chunk, index);
        bb4.toggleSQ(chunk, index);
    }

    inline void toggleSQ(SQ source, SQ target, PC pc) // private
    {
        const auto chunk_source = source.chunk();
        const auto chunk_target = target.chunk();
        const auto index_source = (1ULL << source.index());
        const auto index_target = (1ULL << target.index());

        const auto piece = pc.piece();
        const auto color = pc.color();
        // const auto guild = toGuild(color);

        auto& bb1 = bitboards[pc.encoded];
        auto& bb2 = bitboards[PC(piece, color.team()).encoded];

        bb1.toggleSQ(chunk_source, index_source); bb1.toggleSQ(chunk_target, index_target);
        bb2.toggleSQ(chunk_source, index_source); bb2.toggleSQ(chunk_target, index_target);

        auto& bb3 = bitboards[PC::ORANGE + (color.encoded & 1)];
        auto& bb4 = bitboards[PC::EVERYTHING];

        bb3.toggleSQ(chunk_source, index_source); bb3.toggleSQ(chunk_target, chunk_target);
        bb4.toggleSQ(chunk_source, index_source); bb4.toggleSQ(chunk_target, chunk_target);

        // const auto pc_1 = pc;
        // const auto pc_2 = PC(piece, guild);
        // const auto pc_3 = PC(piece, RBYG );
        // const auto pc_4 = PC(All  , color);
        // const auto pc_5 = PC(All  , guild);
        // const auto pc_6 = PC(All  , RBYG );

        // bitboards[pc_1].chunks[chunk_source] ^= index_source;
        // bitboards[pc_2].chunks[chunk_source] ^= index_source;
        // bitboards[pc_3].chunks[chunk_source] ^= index_source;
        // bitboards[pc_4].chunks[chunk_source] ^= index_source;
        // bitboards[pc_5].chunks[chunk_source] ^= index_source;
        // bitboards[pc_6].chunks[chunk_source] ^= index_source;

        // bitboards[pc_1].chunks[chunk_target] ^= index_target;
        // bitboards[pc_2].chunks[chunk_target] ^= index_target;
        // bitboards[pc_3].chunks[chunk_target] ^= index_target;
        // bitboards[pc_4].chunks[chunk_target] ^= index_target;
        // bitboards[pc_5].chunks[chunk_target] ^= index_target;
        // bitboards[pc_6].chunks[chunk_target] ^= index_target;
    }

    template<std::size_t chunk, Color guild>
    bool isSquareAttacked(SQ sq) const
    {
        constexpr auto pIDX = PC(Piece::P, guild).encoded;
        constexpr auto qIDX = PC(Piece::Q, guild).encoded;
        constexpr auto bIDX = PC(Piece::B, guild).encoded;
        constexpr auto rIDX = PC(Piece::R, guild).encoded;
        constexpr auto nIDX = PC(Piece::N, guild).encoded;
        constexpr auto kIDX = PC(Piece::K, guild).encoded;

        const auto& pBB = bitboards[pIDX];
        const auto& qBB = bitboards[qIDX];
        const auto& bBB = bitboards[bIDX];
        const auto& rBB = bitboards[rIDX];
        const auto& nBB = bitboards[nIDX];
        const auto& kBB = bitboards[kIDX];

        const auto& all = bitboards[PC::EVERYTHING];

        const auto& [verti, horiz, lower, upper, padding] = STRAIGHT[compress<chunk>(sq).encoded];

        BB rook(verti);
        rook.toggleSQ<chunk>(horiz);
        if (rook.collision(rBB, qBB))
        {
            BB x;
            genSlideAttacks<chunk, Piece::R>(sq, all, x);
            if (x.collision(rBB, qBB)) return true;
        }

        const auto& [diag, anti] = DIAGONAL[compress<chunk>(sq).encoded];

        BB bishop(diag, anti);
        if (bishop.collision(bBB, qBB))
        {
            BB x;
            genSlideAttacks<chunk, Piece::B>(sq, all, x);
            if (x.collision(bBB, qBB)) return true;
        }

        const auto& [king, knight] = ADJACENT[compress<chunk>(sq).encoded];

        if (knight.collision(nBB))
            return true;

        if (king.collision(kBB))
            return true;

        if (king.collision(pBB))
        {
            constexpr auto 
            opp1 = Color(guild.encoded == Color::o ? Color::r : Color::b),
            opp2 = Color(guild.encoded == Color::o ? Color::y : Color::g);

            constexpr auto& 
            delta1 = DELTA_INFO[opp1.encoded],
            delta2 = DELTA_INFO[opp2.encoded];

            const auto 
            sq1 = SQ(sq + delta2.takeE),
            sq2 = SQ(sq + delta2.takeW),
            sq3 = SQ(sq + delta1.takeE),
            sq4 = SQ(sq + delta1.takeW);
    
            constexpr auto 
            pawn1 = PC(Piece::P, opp1),
            pawn2 = PC(Piece::P, opp2);

            return (mailbox[sq1] == pawn1) || (mailbox[sq2] == pawn1) ||
                   (mailbox[sq3] == pawn2) || (mailbox[sq4] == pawn2);
        }
            
        return false;
    }

    template<Color guild>
    bool isSquareAttacked(SQ sq) const
    {
        switch (sq.chunk())
        {
            case 0: return isSquareAttacked<0, guild>(sq);
            case 1: return isSquareAttacked<1, guild>(sq);
            case 2: return isSquareAttacked<2, guild>(sq);
            case 3: return isSquareAttacked<3, guild>(sq);
            default: return false;
        }
    }

    bool isSquareAttacked(SQ sq, Color color) const
    {
        switch (color.encoded)
        {
            case Color::r: return isSquareAttacked<Color::o>(sq);
            case Color::b: return isSquareAttacked<Color::c>(sq);
            case Color::y: return isSquareAttacked<Color::o>(sq);
            case Color::g: return isSquareAttacked<Color::c>(sq);
            default: return false;
        }
    }

    bool isRoyalSafe(Color color, u8 depth) const
    {
        switch (color.encoded)
        {
            case Color::r: return isSquareAttacked<Color::o>(states[depth].royals[Color::r]);
            case Color::b: return isSquareAttacked<Color::c>(states[depth].royals[Color::b]);
            case Color::y: return isSquareAttacked<Color::o>(states[depth].royals[Color::y]);
            case Color::g: return isSquareAttacked<Color::c>(states[depth].royals[Color::g]);
            default: return false;
        }
    }

    bool isRoyalSafe(u8 depth) const {
        return isRoyalSafe(states[depth].turn);
    }

    void init(char* fen, Setup setup = Modern);

    const char* fen() const;

    void print(bool debug = false) const;

    inline void popSQ(SQ sq, PC pc)
    {
        mailbox[sq] = PC::NOTHING;
        toggleSQ(sq, pc);
    }

    inline void setSQ(SQ sq, PC pc)
    {
        mailbox[sq] = pc;
        toggleSQ(sq, pc);
    }

    inline void updateStride(Pack& enpass, SQ sq, Color color) {
        enpass.packed ^= Pack((sq + DELTA_INFO[color].pushS), color).packed;
    }

    inline void updateEnpass(Color color, SQ sq) {
        popSQ((sq + DELTA_INFO[color].pushS), PC(Piece::P, color));
    }

    inline void updateCastle(Color color)
    {
        const auto& info = CASTLE_INFO[color];
        movePieceTo(info.rook_source, info.rook_target);
    }

    inline void updateEvolve(SQ sq, PC pawn, PC evolve)
    {
        popSQ(sq, pawn);
        setSQ(sq, evolve);
    }

    inline void revertEnpass(Color color, SQ sq) {
        setSQ((sq + DELTA_INFO[color].pushS), PC(Piece::P, color));
    }

    inline void revertCastle(Color color)
    {
        const auto& info = CASTLE_INFO[color];
        undoPieceTo(info.rook_source, info.rook_target, PC(Piece::R, color), PC(PC::NOTHING));
    }

    inline void revertEvolve(SQ sq, PC pawn, PC evolve)
    {
        popSQ(sq, evolve);
        setSQ(sq, pawn);
    }

    inline void movePieceTo(SQ source, SQ target) // remove PC
    {
        const auto attacker = mailbox[source];
        const auto defender = mailbox[target];
        
        if (defender   != PC::NOTHING) popSQ(target, defender);
        mailbox[source] = PC::NOTHING;
        mailbox[target] = attacker;
        toggleSQ(source, target, attacker);
    }

    inline void undoPieceTo(SQ source, SQ target, PC attacker, PC defender)
    {
        toggleSQ(source, target, attacker); //std::cout << "PCx: " << (int)mailbox[target] << std::endl;
        mailbox[source] = attacker;  //std::cout << "PCx: " << (int)mailbox[target] << std::endl;
        mailbox[target] = defender;  //std::cout << "PCx: " << (int)mailbox[target] << std::endl;
        if (defender.encoded   != PC::NOTHING) setSQ(target, defender), mailbox[target] = defender;
    }

    inline void makemove(Move move, u8 depth, Setup setup)
    {
        const 
        GS& curr = states[depth + 0];
        GS& next = states[depth + 1];

        const auto 
        source = move.source(),
        target = move.target();

        const auto type = move.type();
        const auto flag = move.flag();

        const auto 
        attacker = at(source),
        defender = at(target);

        // std::cout << "att " << (int)attacker << std::endl;
        // std::cout << "def " << (int)defender << std::endl;  

        next.turn = curr.turn.next(); 
        next.capture = defender;

        // std::cout << "cap " << (int)next.capture << std::endl;

        movePieceTo(source, target);
    
        // std::cout << "cap " << (int)next.capture << std::endl;

        // next.enpass.packed  = curr.enpass.packed;
        // next.enpass.packed ^= curr.enpass[curr.turn].encoded; 

        // next.rights = curr.rights;
        
        // alignas(CACHELINE_SIZE)
        // constexpr auto CASTLING_RIGHTS = []() consteval
        // {
        //     std::array<u8     , SQUARE_NB * SETUP_NB> arr {};
        //     for (int s = 0; s < SQUARE_NB * SETUP_NB; s++) arr[s] = 0xFF;

        //     arr[SQ::E2  + 0x000] = 0xEF; arr[SQ::I2  + 0x000] = 0xEE; arr[SQ::L2  + 0x000] = 0xFF;
        //     arr[SQ::B12 + 0x000] = 0xDF; arr[SQ::B8  + 0x000] = 0xDD; arr[SQ::B5  + 0x000] = 0xFD;
        //     arr[SQ::L15 + 0x000] = 0xBF; arr[SQ::H15 + 0x000] = 0xBB; arr[SQ::E15 + 0x000] = 0xFB;
        //     arr[SQ::O5  + 0x000] = 0x7F; arr[SQ::O9  + 0x000] = 0x77; arr[SQ::O12 + 0x000] = 0xF7;
        //     arr[SQ::E2  + 0x100] = 0xEF; arr[SQ::I2  + 0x100] = 0xEE; arr[SQ::L2  + 0x100] = 0xFF;
        //     arr[SQ::B5  + 0x100] = 0xDF; arr[SQ::B9  + 0x100] = 0xDD; arr[SQ::B12 + 0x100] = 0xFD;
        //     arr[SQ::L15 + 0x100] = 0xBF; arr[SQ::H15 + 0x100] = 0xBB; arr[SQ::E15 + 0x100] = 0xFB;
        //     arr[SQ::O12 + 0x100] = 0x7F; arr[SQ::O8  + 0x100] = 0x77; arr[SQ::O5  + 0x100] = 0xF7;

        //     return arr;
        // }();


        // if (attacker.piece().encoded == Piece::K)
        //     next.royals[curr.turn] = target;

        // if (next.rights.encoded)
        //     next.rights.encoded &= CASTLING_RIGHTS[source.encoded + setup * 256],
        //     next.rights.encoded &= CASTLING_RIGHTS[target.encoded + setup * 256];
        
        // if (type.special()) switch (type)
        // {
        // case Type::Stride: updateStride(next.enpass  , source, curr.turn); break;
        // case Type::Enpass: updateEnpass(move.enpass(), target);            break;
        // case Type::Castle: updateCastle(curr.turn);                        break;
        // case Type::Evolve: updateEvolve(target, attacker, move.evolve());  break;
        // default: break;
        // }
    }

    inline void undomove(Move move, u8 depth)
    {
        const GS& 
        prev = states[depth - 1],
        curr = states[depth + 0];  //std::cout << "turn " << (int)curr.turn << std::endl;

        const auto 
        source = move.source(),
        target = move.target();

        const auto type = move.type();
        const auto flag = move.flag();

        const auto 
        attacker = at(target),
        defender = curr.capture;

        // std::cout << "att " << (int)attacker << std::endl;
        // std::cout << "def " << (int)defender << std::endl;  

        // std::cout << "PC: " << (int)mailbox[target] << std::endl;

        undoPieceTo(source, target, attacker, defender);

        // std::cout << "PC: " << (int)mailbox[target] << std::endl;

        // if (type.special()) switch (type)
        // {
        // case Type::Enpass: revertEnpass(move.enpass(), target);            break;
        // case Type::Castle: revertCastle(curr.turn);                        break;
        // case Type::Evolve: revertEvolve(target, attacker, move.evolve());  break;
        // default: break;
        // }
    }
};

} // namespace athena

#endif /* POSITION_H */