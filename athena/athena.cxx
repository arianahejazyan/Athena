// #include "engine.h"
// #include "bitboard.h"
// #include "utility.h"
// #include <immintrin.h>
#include "cli.h"
#include "perft.h"

#include "move.h"

using namespace athena;

// void print_binary_byte(uint8_t byte)
// {
//     for (int i = 7; i >= 0; i--)
//     {
//         printf("%d", (byte >> i) & 1);
//         if (i == 4) // after printing 4 bits, add a space
//             printf(" ");
//     }
// }

// #include <iostream>
// for std::hex and std::setw

int main(int argc, char *argv[])
{
    Setup setup = Modern;
    char fen[1024];
    std::strcpy(fen, "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/x,x,x,8,x,x,x/bR,bP,10,gP,gR/bN,bP,10,gP,gN/bB,bP,10,gP,gB/bQ,bP,10,gP,gK/bK,bP,10,gP,gQ/bB,bP,10,gP,gB/bN,bP,10,gP,gN/bR,bP,10,gP,gR/x,x,x,8,x,x,x/x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x");    
    PS ps;
    ps.init(fen, setup);

    static Perft perft;
    perft.run(ps, 4, false, false, false);


    // ps.print();
    
    // MoveList moves;
    // moves.genNoisyMoves(ps, setup);
    // moves.genQuietMoves(ps, setup);
    // std::cout << "size: " << moves.size << std::endl;

    // int idx = 0, depth = 0;
    // ps.makemove(moves[idx], depth, setup);
    // ps.print();
    // ps.undomove(moves[idx], depth + 1);
    // ps.print();


    // Setup setup = Modern;
    // char fen[1024];
    // std::strcpy(fen, "G-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,2,rB,5,x,x,x/x,x,x,8,x,x,x/3,gB,10/4,yB,9/1,yB,12/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
    // PS ps;
    // ps.init(fen, setup);
    // ps.print();

    // MoveList moves;
    // moves.genNoisyMoves(ps, setup);
    // moves.genQuietMoves(ps, setup);
    // moves.print(true);

    // char fen[1024];
    // std::strcpy(fen, "Y-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,1,bR,6,x,x,x/x,x,x,8,x,x,x/x,x,x,1,yR,1,rR,4,x,x,x");    
    // PS ps;
    // ps.init(fen);
    // ps.print();
    // MoveList moves;
    // moves.size = 0;
    // moves.genNoisyMoves(ps);
    // moves.genQuietMoves(ps);
    // moves.print(true);


    // Move moves[MAX_MOVES];
    // int size = 0;
    // size += gen_all_pseudo_noisy_moves(ps, moves + size, setup, 0);
    // size += gen_all_pseudo_quiet_moves(ps, moves + size, setup, 0);









    // DIAGONAL[(compress<3>(SQ::G14)).encoded].anti.print();



    // for (int i = 0; i < size; i++)
    //     std::cout << moves[i].uci(true) << std::endl;



    // (PASS[Classic][Red][KingSide]).print();
    // (PASS[Classic][Blue][KingSide]).print();
    // (PASS[Classic][Yellow][KingSide]).print();
    // (PASS[Classic][Green][KingSide]).print();

    // std::cout << "xxxxxxx" << std::endl;
    
    // (PASS[Classic][Red][QueenSide]).print();
    // (PASS[Classic][Blue][QueenSide]).print();
    // (PASS[Classic][Yellow][QueenSide]).print();
    // (PASS[Classic][Green][QueenSide]).print();

    // std::cout << "xxxxxxx" << std::endl;

    //     (PASS[Modern][Red][KingSide]).print();
    // (PASS[Modern][Blue][KingSide]).print();
    // (PASS[Modern][Yellow][KingSide]).print();
    // (PASS[Modern][Green][KingSide]).print();


    // std::cout << "xxxxxxx" << std::endl;

    //    (PASS[Modern][Red][QueenSide]).print();
    // (PASS[Modern][Blue][QueenSide]).print();
    // (PASS[Modern][Yellow][QueenSide]).print();
    // (PASS[Modern][Green][QueenSide]).print();

    // Square sq = H7;
    // BB bb{K3, D7, J14, K2};
    // BB attacks{};
    // obstruction_difference(sq, bb, attacks);
    // print(attacks);

    // Square sq = I3;
    // BB bb{LANE_MASK[I3].lower, LANE_MASK[I3].upper, LANE_MASK[I3].lineEx, 0};
    // print(bb);

    // Square sq = H5;
    // BB occ({H5, F9, K8, F3});
    // BB attacks;
    // hyperbola_quintessence(sq, occ, DIAGONAL_ATTACK[sq], attacks);
    // // print(occ);
    // print(attacks);

    // Square sq = H2;
    // BitBoard bb{};
    // print(bb);
    // minus(bb, sq);
    // print(bb);

    // Square sq = E4;
    // std::cout << (int)sq << std::endl;
    // std::cout << (int)(sq ^ 0b11110000) << std::endl;
    // std::cout << (int)(sq ^ 0b00001111) << std::endl;

    // uint64_t a = 0x100; // 256
    // uint64_t b = 0x010; // 16

    // uint64_t result = b - a;

    // std::cout << "Result (dec): " << result << "\n";
    // std::cout << "Result (hex): 0x" << std::hex << result << "\n";

    // uint64_t b = 0x1122334455667708ULL;

    // uint64_t swapped = __builtin_bswap64(b);

    // std::cout << "Original: 0x"
    //           << std::hex << std::setw(16) << std::setfill('0') << b << std::endl;

    // std::cout << "Swapped:  0x"
    //           << std::hex << std::setw(16) << std::setfill('0') << swapped << std::endl;

    //    // Load the four 64-bit chunks into a 256-bit register
    // __m256i chunks_vec = _mm256_loadu_si256((const __m256i*)chunks);

    // // Shift each 64-bit lane right by c bits
    // __m256i shifted = _mm256_srli_epi64(chunks_vec, c);

    // // Mask to select bits at positions 0, 16, 32, 48 in each lane
    // __m256i mask = _mm256_set1_epi64x(0x0001000100010001ULL);
    // __m256i bits = _mm256_and_si256(shifted, mask);

    // // Shift each 16-bit word right by 15 bits to move desired bits to MSB of bytes
    // __m256i shifted_bits = _mm256_srli_epi16(bits, 15);

    // // Extract MSB of each byte into a 32-bit mask
    // uint32_t movemask = _mm256_movemask_epi8(shifted_bits);

    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // // changes only shift to msb
    // int col = 7;

    // BitBoard bb{H3, H6, H7, H8, H9, H13, H15, B5, N6, K13};
    // print(bb);

    // __m256i chunks_vec = _mm256_load_si256((const __m256i *)bb.chunks);

    // __m256i shifted = _mm256_srli_epi64(chunks_vec, col);

    // //
    // alignas(32) uint64_t result1[4];
    // _mm256_store_si256((__m256i *)result1, shifted);
    // BitBoard bb1{result1[0], result1[1], result1[2], result1[3]};
    // print(bb1);
    // //

    // __m256i mask = _mm256_set1_epi64x(0x0001000100010001ULL);
    // __m256i bits = _mm256_and_si256(shifted, mask);

    // //
    // alignas(32) uint64_t result2[4];
    // _mm256_store_si256((__m256i *)result2, bits);
    // BitBoard bb2{result2[0], result2[1], result2[2], result2[3]};
    // print(bb2);
    // //

    // __m256i msb_ready = _mm256_slli_epi64(bits, 7);

    // //
    // alignas(32) uint64_t result3[4];
    // _mm256_store_si256((__m256i *)result3, msb_ready);
    // BitBoard bb3{result3[0], result3[1], result3[2], result3[3]};
    // print(bb3);
    // //

    // // Extract the MSB of each byte into a 32-bit mask
    // uint32_t movemask = _mm256_movemask_epi8(msb_ready);

    // printf("movemask  = 0x%08X\n", movemask);

    // uint32_t extracted = _pext_u32(movemask, 0x55555555);

    // printf("extracted = 0x%08X\n", extracted);

    // // // Compress bits 0, 2, 4, ..., 30 into low 16 bits
    // // uint32_t t = movemask & 0x55555555; // Keep only bits 0, 2, 4, ..., 30
    // // t = (t | (t >> 1)) & 0x33333333;   // Merge bits: 0->0, 2->1, 4->2, 6->3, ...
    // // t = (t | (t >> 2)) & 0x0F0F0F0F;   // Merge: 0->0,  ascended
    // // t = (t | (t >> 4)) & 0x00FF00FF;     // Merge: 0->0, 4->1, 8->2, 12->3, ...
    // // t = (t | (t >> 8)) & 0x0000FFFF;    // Merge: 0->0, 8->1, 16->2, 24->3, ...
    // // uint16_t occupancy = t;

    // for (int i = 3; i >= 0; i--)
    // { // From most significant byte to least
    //     print_binary_byte((movemask >> (i * 8)) & 0xFF);
    //     if (i > 0)
    //         printf(", ");
    // }
    // printf("\n");

    // for (int i = 3; i >= 0; i--)
    // { // From most significant byte to least
    //     print_binary_byte((extracted >> (i * 8)) & 0xFF);
    //     if (i > 0)
    //         printf(", ");
    // }
    // printf("\n");
    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    // print(PIECE_ATTACK[Bishop][G5]);

    // int sq = 120; // i8
    // int col = 8;
    // int row = 8;

    // BitBoard bb{F5, G6, K10, M12, G13, M5, C9};
    // print(bb);

    // __m256i chunks_vec = _mm256_load_si256((const __m256i *)bb.chunks);

    // static constexpr auto shift_amounts = []() consteval
    // {
    //     std::array<BitBoard, SQUARE_NB> table;

    //     for (auto sq : VALID_SQUARES)
    //     {
    //         auto rank_sq = rankSQ(sq);
    //         auto file_sq = fileSQ(sq);

    //         for (int r = 0; r < RANK_NB; ++r)
    //         {
    //             table[sq].chunks[r >> 2] |= (std::max(0, file_sq - rank_sq + (15 - r)) << (r & 0b11));
    //         }
    //     }

    //     return table;
    // }();

    // // __m256i shifts = _mm256_load_si256(reinterpret_cast<const __m256i *>(shift_amounts[sq].chunks));

    // __m256i shifts = _mm256_load_si256((const __m256i *)shift_amounts[sq].chunks);

    // // Shift each 16-bit element by its corresponding shift amount
    // __m256i shifted = _mm256_sllv_epi16(chunks_vec, shifts);

    // //
    // alignas(32) uint64_t result0[4];
    // _mm256_store_si256((__m256i *)result0, shifted);
    // BitBoard bb0{result0[0], result0[1], result0[2], result0[3]};
    // print(bb0);
    // //

    // //
    // alignas(32) uint64_t result1[4];
    // _mm256_store_si256((__m256i *)result1, shifted);
    // BitBoard bb1{result1[0], result1[1], result1[2], result1[3]};
    // print(bb1);
    //

    // alignas(32) static constexpr uint8_t shift_amounts[256 * 16] =
    //     {

    //     };

    // __m256i shifts = _mm256_load_si256((const __m256i *)shift_amounts[sq * 16]);

    // Engine engine;
    // engine.launch();

    // CommandLine commandline;
    // commandline.launch();

    return 0;
}