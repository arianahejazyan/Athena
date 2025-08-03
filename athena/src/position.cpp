#include <iostream>
#include <iomanip>
#include <cstring>
#include "position.h"
#include "utility.h"

namespace athena
{

// inline const char* encodeString(SQ sq, bool debug)
// {
//     static constexpr char TABLE[SQUARE_NB][4] =
//     {
//         "a1" , "b1" , "c1" , "d1" , "e1" , "f1",  "g1",  "h1" , "i1" , "j1" , "k1" , "l1" , "m1" , "n1" , "o1" , "p1" ,
//         "a2" , "b2" , "c2" , "d2" , "e2" , "f2",  "g2",  "h2" , "i2" , "j2" , "k2" , "l2" , "m2" , "n2" , "o2" , "p2" ,
//         "a3" , "b3" , "c3" , "d3" , "e3" , "f3",  "g3",  "h3" , "i3" , "j3" , "k3" , "l3" , "m3" , "n3" , "o3" , "p3" ,
//         "a4" , "b4" , "c4" , "d4" , "e4" , "f4",  "g4",  "h4" , "i4" , "j4" , "k4" , "l4" , "m4" , "n4" , "o4" , "p4" ,
//         "a5" , "b5" , "c5" , "d5" , "e5" , "f5",  "g5",  "h5" , "i5" , "j5" , "k5" , "l5" , "m5" , "n5" , "o5" , "p5" ,
//         "a6" , "b6" , "c6" , "d6" , "e6" , "f6",  "g6",  "h6" , "i6" , "j6" , "k6" , "l6" , "m6" , "n6" , "o6" , "p6" ,
//         "a7" , "b7" , "c7" , "d7" , "e7" , "f7",  "g7",  "h7" , "i7" , "j7" , "k7" , "l7" , "m7" , "n7" , "o7" , "p7" ,
//         "a8" , "b8" , "c8" , "d8" , "e8" , "f8",  "g8",  "h8" , "i8" , "j8" , "k8" , "l8" , "m8" , "n8" , "o8" , "p8" ,
//         "a9" , "b9" , "c9" , "d9" , "e9" , "f9",  "g9",  "h9" , "i9" , "j9" , "k9" , "l9" , "m9" , "n9" , "o9" , "p9" ,
//         "a10", "b10", "c10", "d10", "e10", "f10", "g10", "h10", "i10", "j10", "k10", "l10", "m10", "n10", "o10", "p10",
//         "a11", "b11", "c11", "d11", "e11", "f11", "g11", "h11", "i11", "j11", "k11", "l11", "m11", "n11", "o11", "p11",
//         "a12", "b12", "c12", "d12", "e12", "f12", "g12", "h12", "i12", "j12", "k12", "l12", "m12", "n12", "o12", "p12",
//         "a13", "b13", "c13", "d13", "e13", "f13", "g13", "h13", "i13", "j13", "k13", "l13", "m13", "n13", "o13", "p13",
//         "a14", "b14", "c14", "d14", "e14", "f14", "g14", "h14", "i14", "j14", "k14", "l14", "m14", "n14", "o14", "p14",
//         "a15", "b15", "c15", "d15", "e15", "f15", "g15", "h15", "i15", "j15", "k15", "l15", "m15", "n15", "o15", "p15",
//         "a16", "b16", "c16", "d16", "e16", "f16", "g16", "h16", "i16", "j16", "k16", "l16", "m16", "n16", "o16", "p16",
//     };
//     return TABLE[debug ? sq : (sq - 17)];
// }

// inline char* encodeString(char* out, Piece piece, bool lowercase = false)
// {
//     switch (piece)
//     {
//     case P : *(out++) = lowercase ? 'p' : 'P'; break;
//     case N : *(out++) = lowercase ? 'n' : 'N'; break;
//     case B : *(out++) = lowercase ? 'b' : 'B'; break;
//     case R : *(out++) = lowercase ? 'r' : 'R'; break;
//     case Q : *(out++) = lowercase ? 'q' : 'Q'; break;
//     case K : *(out++) = lowercase ? 'k' : 'K'; break;
//     default: break;
//     }
//     return out;
// }

// inline char* encodeString(char* out, Color color, bool uppercase = false)
// {
//     switch (color)
//     {
//     case r : *(out++) = uppercase ? 'R' : 'r'; break;
//     case b : *(out++) = uppercase ? 'B' : 'b'; break;
//     case y : *(out++) = uppercase ? 'Y' : 'y'; break;
//     case g : *(out++) = uppercase ? 'G' : 'g'; break;
//     default: break;
//     }
//     return out;
// }

// inline char* encodeString(char* out)
// {
//     *(out++) = '0'; *(out++) = ',';
//     *(out++) = '0'; *(out++) = ',';
//     *(out++) = '0'; *(out++) = ',';
//     *(out++) = '0';
//     return out;
// }

void Position::init(char* fen, Setup setup)
{
    GS& gs = states[0];

    //
    
    //
    
    // gs.royals  = 0;
    // gs.zobrist = 0ULL;

    for (int pc = 0; pc < PIECECOLOR_NB; ++pc)
        bitboards[pc] = BLANK;

    for (int sq = 0; sq < SQUARE_NB; ++sq)
    {
        const auto s = SQ(sq);
        const auto r = s.rank();
        const auto f = s.file();
        mailbox[sq] = isInValidSquare(r, f) ? PC::STONE : PC::NOTHING;
    }

    //
    static char* tokens[32];
    const auto size = tokenize(fen, tokens, 32, "-");

    // ********************************************* //
    gs.turn = toColor(tokens[0][0]);

    // ********************************************* //
    gs.rights.encoded = 0;
    auto& ks = tokens[2];
    auto& qs = tokens[3];
    if (ks[0] == '1') gs.rights.encoded ^= Rights(Color::r, KS).encoded; if (qs[0] == '1') gs.rights.encoded ^= Rights(Color::r, QS).encoded;
    if (ks[2] == '1') gs.rights.encoded ^= Rights(Color::b, KS).encoded; if (qs[2] == '1') gs.rights.encoded ^= Rights(Color::b, QS).encoded;
    if (ks[4] == '1') gs.rights.encoded ^= Rights(Color::y, KS).encoded; if (qs[4] == '1') gs.rights.encoded ^= Rights(Color::y, QS).encoded;
    if (ks[6] == '1') gs.rights.encoded ^= Rights(Color::g, KS).encoded; if (qs[6] == '1') gs.rights.encoded ^= Rights(Color::g, QS).encoded;

    // ********************************************* //
    // gs.enpass.packed = {SQ::OFFBOARD, SQ::OFFBOARD, SQ::OFFBOARD, SQ::OFFBOARD};

    // gs.enpass[Color::r] = SQ::OFFBOARD;
    // gs.enpass[Color::b] = SQ::OFFBOARD;
    // gs.enpass[Color::y] = SQ::OFFBOARD;
    // gs.enpass[Color::g] = SQ::OFFBOARD;

    gs.enpass = 0;
    if (size == 8)
    {
        char* lbrace = std::strchr(tokens[6], '(');
        char* rbrace = std::strchr(tokens[6], ')');
        *rbrace = '\0';

        static char* epsq[(COLOR_NB - 4)] = {};
        tokenize(lbrace + 1, epsq, 4, ",");

        if (std::strlen(epsq[Color::r]) != 2) gs.enpass.packed ^= (toSQ(epsq[Color::r]) << (Color::r * 8));
        if (std::strlen(epsq[Color::b]) != 2) gs.enpass.packed ^= (toSQ(epsq[Color::b]) << (Color::b * 8));
        if (std::strlen(epsq[Color::y]) != 2) gs.enpass.packed ^= (toSQ(epsq[Color::y]) << (Color::y * 8));
        if (std::strlen(epsq[Color::g]) != 2) gs.enpass.packed ^= (toSQ(epsq[Color::g]) << (Color::g * 8));

//         char enpass[64] = {0};
//         std::strncpy(enpass, lbrace + 1, rbrace - lbrace - 1);

//         char* epsq[4] = {};
//         size_t count = tokenize(enpass, epsq, 4, ",");

//         for (int i = 0; i < 4; i++)
//         {
//             if (std::strlen(epsq[i]) != 2)
//             {
//                 int rank = (epsq[i][3] == ':' ? (epsq[i][2] - '0') : ((epsq[i][2] - '0') * 10 + (epsq[i][3] - '0')));
//                 int file = (epsq[i][1] - 'a' + 1);
//                 gs.enpass ^= encodeEnpass(encodeSQ(rank, file), static_cast<Color>(i));
//             }
//         }
    }

    // ********************************************* //
    static char* ranks[256];
    const auto num_ranks = tokenize((size == 7 ? tokens[6] : tokens[7]), ranks, 256, "/");

    // gs.royals.packed = {SQ::OFFBOARD, SQ::OFFBOARD, SQ::OFFBOARD, SQ::OFFBOARD};

    // gs.royals[Color::r] = SQ::OFFBOARD;
    // gs.royals[Color::b] = SQ::OFFBOARD;
    // gs.royals[Color::y] = SQ::OFFBOARD;
    // gs.royals[Color::g] = SQ::OFFBOARD;

    gs.royals = 0;

    int row = 14, col = 1;
    for (int rank = 0; rank < num_ranks; rank++)
    {
        static char* files[16];
        const auto num_files = tokenize(ranks[rank], files, 16, ",");

        for (int file = 0; file < num_files; file++)
        {
            const char ch = files[file][0];

            if (ch == 'x')
                col++;

            else if ('0' > ch || ch > '9')
            {
                const auto sq = SQ(row, col++);
                const auto pc = PC(toPiece(files[file][1]), toColor(files[file][0]));

                const auto piece = pc.piece();
                const auto color = pc.color(); 

                mailbox[sq.encoded] = pc;
                toggleSQ(sq, pc);
                // if (piece == Piece::K) gs.royals.packed ^= sq << (color * 8);
            }
            else col += std::stoi(files[file]);
        }
        row--, col = 1;
    }

    // ********************************************* //
    gs.capture = PC::NOTHING;
}

// inline char* encodeString(char* ptr, uint32_t enpass, Color color)
// {
//     *(ptr++)= '\'';
//     const auto epsq = decodeEnpass(enpass, color);
//     if (epsq != OFFBOARD)
//     {
//         auto offset1 = 0;
//         auto offset2 = 0;

//         switch (color)
//         {
//         case r: offset2 = +16; break;
//         case b: offset2 = +1 ; break;
//         case y: offset2 = -16; break;
//         case g: offset2 = -1 ; break;
//         default: break;
//         }

//         auto sq1 = encodeString(static_cast<SQ>(epsq + offset1), false);
//         auto sq2 = encodeString(static_cast<SQ>(epsq + offset2), false);

//         while (*sq1) *(ptr++) = *(sq1++); *(ptr++)= ':';
//         while (*sq2) *(ptr++) = *(sq2++);
//     }
//     *(ptr++)= '\'';

//     return ptr;
// }

const char* Position::fen() const
{
    static char fen[1024];
    char* ptr = fen;

//     const auto& gs = states[0];

//     ptr = encodeString(ptr, gs.turn, true); *(ptr++)= '-';
//     ptr = encodeString(ptr);                *(ptr++)= '-';
//     ptr = encodeString(ptr, gs.rights, KS); *(ptr++)= '-';
//     ptr = encodeString(ptr, gs.rights, QS); *(ptr++)= '-';
//     ptr = encodeString(ptr);                *(ptr++)= '-';

//     *(ptr++)= '0';
//     *(ptr++)= '-';

//     if (gs.enpass != 0)
//     {
//         const char* begin = "{\'enPassant\':(";
//         while (*begin) *(ptr++) = *(begin++);

//         ptr = encodeString(ptr, gs.enpass, r); *(ptr++)= ',';
//         ptr = encodeString(ptr, gs.enpass, b); *(ptr++)= ',';
//         ptr = encodeString(ptr, gs.enpass, y); *(ptr++)= ',';
//         ptr = encodeString(ptr, gs.enpass, g);

//         const char* end = ")}";
//         while (*end) *(ptr++) = *(end++); *(ptr++)= '-';
//     }

//     int count = 0;
//     for (size_t rank = 14; rank >= 1 ; rank--)
//     for (size_t file = 1 ; file <= 14; file++)
//     {
//         const auto pc = mailbox[encodeSQ(rank, file)];

//         if (pc != NOTHING)
//         {
//             if (count != 0)
//             {
//                 ptr += std::sprintf(ptr, "%d,", count);
//                 count = 0;
//             }

//             if (pc != STONE)
//             {
//                 ptr = encodeString(ptr, decodeColor(pc));
//                 ptr = encodeString(ptr, decodePiece(pc));
//                 *(ptr++) = ',';
//             }
//             else *(ptr++) = 'x', *(ptr++) = ',';;
//         }
//         else count++;

//         if (file == 14)
//         {
//             if (count != 0)
//                 ptr += std::sprintf(ptr, "%d", count);
//             else if (*(ptr - 1) == ',') ptr--;
//         }

//         if (rank != 1 && file == 14) *(ptr++)= '/';
//     }

//     *ptr = '\0';
    return fen;
}

void Position::print(bool debug) const
{
    int start = debug ? 0 : 1;
    int end   = debug ? RANK_NB : RANK_NB - 1;

    std::cout << "\n     ";
    for (int file = start; file < end; ++file)
        std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n   +" << std::string((end - start) * 3 + 1, '-') << "+\n";

    for (int rank = end - 1; rank >= start; --rank)
    {
        std::cout << (((rank - start + 1) < 10) ? " " : "") << (rank - start + 1) << " | ";
        for (int file = start; file < end; ++file)
        {
            const auto sq = SQ(rank, file);
            const auto pc = mailbox[sq.encoded];

            const auto piece = pc.piece();
            const auto color = pc.color();

            const char* sqStr;
            switch (piece.encoded)
            {
            case Piece::P: sqStr = "\u2659"; break;
            case Piece::Q: sqStr = "\u2655"; break;
            case Piece::B: sqStr = "\u2657"; break;
            case Piece::R: sqStr = "\u2656"; break;
            case Piece::N: sqStr = "\u2658"; break;
            case Piece::K: sqStr = "\u2654"; break;
            case Piece::A: sqStr = "x"; break;
            case Piece::E: sqStr = "\033[2m.\033[0m"; break;
            }

            const char* colorCode;
            switch (color.encoded)
            {
            case Color::r : colorCode = "\033[31m"; break;
            case Color::b : colorCode = "\033[34m"; break;
            case Color::y : colorCode = "\033[33m"; break;
            case Color::g : colorCode = "\033[32m"; break;
            default: colorCode = ""; break;
            }
            std::cout << colorCode << sqStr << "\033[0m  ";
            }
            std::cout << "| " << (rank - start + 1) << "\n";
    }

    std::cout << "   +" << std::string((end - start) * 3 + 1, '-') << "+\n";
    std::cout << "     ";
    for (int file = start; file < end; ++file)
        std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n";

    const auto& gs = states[0];
    constexpr size_t KEY_WIDTH = 12;
    std::cout << "\n" << std::left << std::setw(KEY_WIDTH) << "turn:";
    switch (gs.turn.encoded)
    {
    case Color::r:  std::cout << "\033[1;31mRed\033[0m"   << "\n"; break;
    case Color::b:  std::cout << "\033[1;34mBlue\033[0m"  << "\n"; break;
    case Color::y:  std::cout << "\033[1;33mYellow\033[0m"<< "\n"; break;
    case Color::g:  std::cout << "\033[1;32mGreen\033[0m" << "\n"; break;
    default: std::cout << "unknown"<< "\n"; break;
    }

    char ks[8]; ks[7] = '\0'; write(ks, gs.rights, KS);
    char qs[8]; qs[7] = '\0'; write(qs, gs.rights, QS);
    std::cout << std::left << std::setw(KEY_WIDTH) << "kingside:"  << ks << "\n"
              << std::left << std::setw(KEY_WIDTH) << "queenside:" << qs << "\n";

    const auto 
    ENPASS_R = gs.enpass[Color(Color::r)],
    ENPASS_B = gs.enpass[Color(Color::b)],
    ENPASS_Y = gs.enpass[Color(Color::y)],
    ENPASS_G = gs.enpass[Color(Color::g)];

    // char EPSQ_R[4]; (*write(EPSQ_R, gs.enpass.decode(Color::r))) = '\0';
    // char EPSQ_B[4]; (*write(EPSQ_R, gs.enpass.decode(Color::r))) = '\0';
    // char EPSQ_Y[4]; (*write(EPSQ_R, gs.enpass.decode(Color::r))) = '\0';
    // char EPSQ_G[4]; (*write(EPSQ_R, gs.enpass.decode(Color::r))) = '\0';

    std::cout << std::left << std::setw(KEY_WIDTH) 
              << "enpassant:"
              << (ENPASS_R.encoded != SQ::OFFBOARD ? toString(ENPASS_R, debug) : "-") << ","
              << (ENPASS_B.encoded != SQ::OFFBOARD ? toString(ENPASS_B, debug) : "-") << ","
              << (ENPASS_Y.encoded != SQ::OFFBOARD ? toString(ENPASS_Y, debug) : "-") << ","
              << (ENPASS_G.encoded != SQ::OFFBOARD ? toString(ENPASS_G, debug) : "-") << std::endl;

    const auto 
    ROYAL_R = gs.royals[Color::r],
    ROYAL_B = gs.royals[Color::b],
    ROYAL_Y = gs.royals[Color::y],
    ROYAL_G = gs.royals[Color::g];

    std::cout << std::left << std::setw(KEY_WIDTH) 
              << "royals:"
              << (ROYAL_R.encoded != SQ::OFFBOARD ? toString(ROYAL_R, debug) : "-") << ","
              << (ROYAL_B.encoded != SQ::OFFBOARD ? toString(ROYAL_B, debug) : "-") << ","
              << (ROYAL_Y.encoded != SQ::OFFBOARD ? toString(ROYAL_Y, debug) : "-") << ","
              << (ROYAL_G.encoded != SQ::OFFBOARD ? toString(ROYAL_G, debug) : "-") << std::endl;
}

} // namespace athena