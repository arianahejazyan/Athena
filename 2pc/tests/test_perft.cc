// #include <gtest/gtest.h>
// #include "position.h"
// #include "movegen.h"
// #include "utility.h"
// #include "perft.h"


// using namespace std;
// using namespace Aetos;

// class TestCase
// {
//     public:

//         string fen;
//         vector<u64> results;

//         TestCase(string fen, vector<u64> results): fen(fen), results(results) {}
// };

// class PerftTest : public ::testing::Test
// {
//     protected:

//         Position pos;

//     const vector<TestCase> testCases = 
//     {
//         TestCase("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", {20, 400, 8902, 197281, 4865609, 119060324}),
//         TestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", {48, 2039, 97862, 4085603, 193690690}),
//         TestCase("4k3/8/8/8/8/8/8/4K2R w K - 0 1", {15, 66, 1197, 7059, 133987, 764643}),
//         TestCase("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", {16, 71, 1287, 7626, 145232, 846648}),
//         TestCase("4k2r/8/8/8/8/8/8/4K3 w k - 0 1", {5, 75, 459, 8290, 47635, 899442}),
//         TestCase("r3k3/8/8/8/8/8/8/4K3 w q - 0 1", {5, 80, 493, 8897, 52710, 1001523}),
//         TestCase("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", {26, 112, 3189, 17945, 532933, 2788982}),
//         TestCase("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", {5, 130, 782, 22180, 118882, 3517770}),
//         TestCase("8/8/8/8/8/8/6k1/4K2R w K - 0 1", {12, 38, 564, 2219, 37735, 185867}),
//         TestCase("8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", {15, 65, 1018, 4573, 80619, 413018}),
//         TestCase("4k2r/6K1/8/8/8/8/8/8 w k - 0 1", {3, 32, 134, 2073, 10485, 179869}),
//         TestCase("r3k3/1K6/8/8/8/8/8/8 w q - 0 1", {4, 49, 243, 3991, 20780, 367724}),
//         TestCase("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", {26, 568, 13744, 314346, 7594526, 179862938}),
//         TestCase("r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1", {25, 567, 14095, 328965, 8153719, 195629489}),
//         TestCase("r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1", {25, 548, 13502, 312835, 7736373, 184411439}),
//         TestCase("r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1", {25, 547, 13579, 316214, 7878456, 189224276}),
//         TestCase("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", {26, 583, 14252, 334705, 8198901, 198328929}),
//         TestCase("2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", {25, 560, 13592, 317324, 7710115, 185959088}),
//         TestCase("r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1", {25, 560, 13607, 320792, 7848606, 190755813}),
//         TestCase("4k3/8/8/8/8/8/8/4K2R b K - 0 1", {5, 75, 459, 8290, 47635, 899442}),
//         TestCase("4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", {5, 80, 493, 8897, 52710, 1001523}),
//         TestCase("4k2r/8/8/8/8/8/8/4K3 b k - 0 1", {15, 66, 1197, 7059, 133987, 764643}),
//         TestCase("r3k3/8/8/8/8/8/8/4K3 b q - 0 1", {16, 71, 1287, 7626, 145232, 846648}),
//         TestCase("4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1", {5, 130, 782, 22180, 118882, 3517770}),
//         TestCase("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1", {26, 112, 3189, 17945, 532933, 2788982}),
//         TestCase("8/8/8/8/8/8/6k1/4K2R b K - 0 1", {3, 32, 134, 2073, 10485, 179869}),
//         TestCase("8/8/8/8/8/8/1k6/R3K3 b Q - 0 1", {4, 49, 243, 3991, 20780, 367724}),
//         TestCase("4k2r/6K1/8/8/8/8/8/8 b k - 0 1", {12, 38, 564, 2219, 37735, 185867}),
//         TestCase("r3k3/1K6/8/8/8/8/8/8 b q - 0 1", {15, 65, 1018, 4573, 80619, 413018}),
//         TestCase("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", {26, 568, 13744, 314346, 7594526, 179862938}),
//         TestCase("r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", {26, 583, 14252, 334705, 8198901, 198328929}),
//         TestCase("r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1", {25, 560, 13592, 317324, 7710115, 185959088}),
//         TestCase("r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1", {25, 560, 13607, 320792, 7848606, 190755813}),
//         TestCase("1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", {25, 567, 14095, 328965, 8153719, 195629489}),
//         TestCase("2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", {25, 548, 13502, 312835, 7736373, 184411439}),
//         TestCase("r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1", {25, 547, 13579, 316214, 7878456, 189224276}),
//         TestCase("8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1", {14, 195, 2760, 38675, 570726, 8107539}),
//         TestCase("8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1", {11, 156, 1636, 20534, 223507, 2594412}),
//         TestCase("8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1", {19, 289, 4442, 73584, 1198299, 19870403}),
//         TestCase("K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", {3, 51, 345, 5301, 38348, 588695}),
//         TestCase("k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", {17, 54, 835, 5910, 92250, 688780}),
//         TestCase("8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1", {15, 193, 2816, 40039, 582642, 8503277}),
//         TestCase("8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1", {16, 180, 2290, 24640, 288141, 3147566}),
//         TestCase("8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1", {4, 68, 1118, 16199, 281190, 4405103}),
//         TestCase("K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", {17, 54, 835, 5910, 92250, 688780}),
//         TestCase("k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", {3, 51, 345, 5301, 38348, 588695}),
//         TestCase("B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1", {17, 278, 4607, 76778, 1320507, 22823890}),
//         TestCase("8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1", {21, 316, 5744, 93338, 1713368, 28861171}),
//         TestCase("k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1", {21, 144, 3242, 32955, 787524, 7881673}),
//         TestCase("K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1", {7, 143, 1416, 31787, 310862, 7382896}),
//         TestCase("B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1", {6, 106, 1829, 31151, 530585, 9250746}),
//         TestCase("8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1", {17, 309, 5133, 93603, 1591064, 29027891}),
//         TestCase("k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1", {7, 143, 1416, 31787, 310862, 7382896}),
//         TestCase("K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1", {21, 144, 3242, 32955, 787524, 7881673}),
//         TestCase("7k/RR6/8/8/8/8/rr6/7K w - - 0 1", {19, 275, 5300, 104342, 2161211, 44956585}),
//         TestCase("R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1", {36, 1027, 29215, 771461, 20506480, 525169084}),
//         TestCase("7k/RR6/8/8/8/8/rr6/7K b - - 0 1", {19, 275, 5300, 104342, 2161211, 44956585}),
//         TestCase("R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1", {36, 1027, 29227, 771368, 20521342, 524966748}),
//         TestCase("6kq/8/8/8/8/8/8/7K w - - 0 1", {2, 36, 143, 3637, 14893, 391507}),
//         TestCase("6KQ/8/8/8/8/8/8/7k b - - 0 1", {2, 36, 143, 3637, 14893, 391507}),
//         TestCase("K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", {6, 35, 495, 8349, 166741, 3370175}),
//         TestCase("6qk/8/8/8/8/8/8/7K b - - 0 1", {22, 43, 1015, 4167, 105749, 419369}),
//         TestCase("6KQ/8/8/8/8/8/8/7k b - - 0 1", {2, 36, 143, 3637, 14893, 391507}),
//         TestCase("K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", {6, 35, 495, 8349, 166741, 3370175}),
//         TestCase("8/8/8/8/8/K7/P7/k7 w - - 0 1", {3, 7, 43, 199, 1347, 6249}),
//         TestCase("8/8/8/8/8/7K/7P/7k w - - 0 1", {3, 7, 43, 199, 1347, 6249}),
//         TestCase("K7/p7/k7/8/8/8/8/8 w - - 0 1", {1, 3, 12, 80, 342, 2343}),
//         TestCase("7K/7p/7k/8/8/8/8/8 w - - 0 1", {1, 3, 12, 80, 342, 2343}),
//         TestCase("8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", {7, 35, 210, 1091, 7028, 34834}),
//         TestCase("8/8/8/8/8/K7/P7/k7 b - - 0 1", {1, 3, 12, 80, 342, 2343}),
//         TestCase("8/8/8/8/8/7K/7P/7k b - - 0 1", {1, 3, 12, 80, 342, 2343}),
//         TestCase("K7/p7/k7/8/8/8/8/8 b - - 0 1", {3, 7, 43, 199, 1347, 6249}),
//         TestCase("7K/7p/7k/8/8/8/8/8 b - - 0 1", {3, 7, 43, 199, 1347, 6249}),
//         TestCase("8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", {5, 35, 182, 1091, 5408, 34822}),
//         TestCase("8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", {2, 8, 44, 282, 1814, 11848}),
//         TestCase("4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", {2, 8, 44, 282, 1814, 11848}),
//         TestCase("8/8/7k/7p/7P/7K/8/8 w - - 0 1", {3, 9, 57, 360, 1969, 10724}),
//         TestCase("8/8/k7/p7/P7/K7/8/8 w - - 0 1", {3, 9, 57, 360, 1969, 10724}),
//         TestCase("8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", {5, 25, 180, 1294, 8296, 53138}),
//         TestCase("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", {8, 61, 483, 3213, 23599, 157093}),
//         TestCase("8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", {8, 61, 411, 3213, 21637, 158065}),
//         TestCase("k7/8/3p4/8/3P4/8/8/7K w - - 0 1", {4, 15, 90, 534, 3450, 20960}),
//         TestCase("8/8/7k/7p/7P/7K/8/8 b - - 0 1", {3, 9, 57, 360, 1969, 10724}),
//         TestCase("8/8/k7/p7/P7/K7/8/8 b - - 0 1", {3, 9, 57, 360, 1969, 10724}),
//         TestCase("8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", {5, 25, 180, 1294, 8296, 53138}),
//         TestCase("8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", {8, 61, 411, 3213, 21637, 158065}),
//         TestCase("8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", {8, 61, 483, 3213, 23599, 157093}),
//         TestCase("k7/8/3p4/8/3P4/8/8/7K b - - 0 1", {4, 15, 89, 537, 3309, 21104}),
//         TestCase("7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", {4, 19, 117, 720, 4661, 32191}),
//         TestCase("7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", {5, 19, 116, 716, 4786, 30980}),
//         TestCase("k7/8/8/7p/6P1/8/8/K7 w - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("k7/8/7p/8/8/6P1/8/K7 w - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/8/8/6p1/7P/8/8/K7 w - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("k7/8/6p1/8/8/7P/8/K7 w - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/8/8/3p4/4p3/8/8/7K w - - 0 1", {3, 15, 84, 573, 3013, 22886}),
//         TestCase("k7/8/3p4/8/8/4P3/8/7K w - - 0 1", {4, 16, 101, 637, 4271, 28662}),
//         TestCase("7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", {5, 19, 117, 720, 5014, 32167}),
//         TestCase("7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", {4, 19, 117, 712, 4658, 30749}),
//         TestCase("k7/8/8/7p/6P1/8/8/K7 b - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("k7/8/7p/8/8/6P1/8/K7 b - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/8/8/6p1/7P/8/8/K7 b - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("k7/8/6p1/8/8/7P/8/K7 b - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/8/8/3p4/4p3/8/8/7K b - - 0 1", {5, 15, 102, 569, 4337, 22579}),
//         TestCase("k7/8/3p4/8/8/4P3/8/7K b - - 0 1", {4, 16, 101, 637, 4271, 28662}),
//         TestCase("7k/8/8/p7/1P6/8/8/7K w - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("7k/8/p7/8/8/1P6/8/7K w - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("7k/8/8/1p6/P7/8/8/7K w - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("7k/8/1p6/8/8/P7/8/7K w - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/7p/8/8/8/8/6P1/K7 w - - 0 1", {5, 25, 161, 1035, 7574, 55338}),
//         TestCase("k7/6p1/8/8/8/8/7P/K7 w - - 0 1", {5, 25, 161, 1035, 7574, 55338}),
//         TestCase("3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", {7, 49, 378, 2902, 24122, 199002}),
//         TestCase("7k/8/8/p7/1P6/8/8/7K b - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("7k/8/p7/8/8/1P6/8/7K b - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("7k/8/8/1p6/P7/8/8/7K b - - 0 1", {5, 22, 139, 877, 6112, 41874}),
//         TestCase("7k/8/1p6/8/8/P7/8/7K b - - 0 1", {4, 16, 101, 637, 4354, 29679}),
//         TestCase("k7/7p/8/8/8/8/6P1/K7 b - - 0 1", {5, 25, 161, 1035, 7574, 55338}),
//         TestCase("k7/6p1/8/8/8/8/7P/K7 b - - 0 1", {5, 25, 161, 1035, 7574, 55338}),
//         TestCase("3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", {7, 49, 378, 2902, 24122, 199002}),
//         TestCase("8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", {11, 97, 887, 8048, 90606, 1030499}),
//         TestCase("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", {24, 421, 7421, 124608, 2193768, 37665329}),
//         TestCase("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", {18, 270, 4699, 79355, 1533145, 28859283}),
//         TestCase("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", {24, 496, 9483, 182838, 3605103, 71179139}),
//         TestCase("8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", {11, 97, 887, 8048, 90606, 1030499}),
//         TestCase("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", {24, 421, 7421, 124608, 2193768, 37665329}),
//         TestCase("8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", {18, 270, 4699, 79355, 1533145, 28859283}),
//         TestCase("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", {24, 496, 9483, 182838, 3605103, 71179139}),
//         TestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", {14, 191, 2812, 43238, 674624, 11030083}),
//         TestCase("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3", {31, 570, 17546, 351806, 11139762}),
//     };
// };

// static u64 perftx(Position& pos, u64 maxDepth)
// {
//     if (maxDepth == 0) return 1;

//     Array1D<Move, MAX_MOVES> moves;

//     u64 nodes = 0;
//     u64 size = genAllLegalMoves(pos, moves.elements);
    
//     // if (maxDepth == 1) return size;

//     for (u64 i = 0; i < size; ++i)
//     {
//         pos.makemove(moves[i]);
//         nodes += perftx(pos, maxDepth - 1);
//         pos.undomove(moves[i]);
//     }

//     return nodes;
// }

// // // Define equality operator for GameState
// // bool operator==(const GameState& lhs, const GameState& rhs) {
// //     return lhs.turn == rhs.turn &&
// //            lhs.hash == rhs.hash &&
// //            lhs.epsq == rhs.epsq &&
// //            lhs.castle == rhs.castle &&
// //            lhs.captured == rhs.captured &&
// //            lhs.halfmove == rhs.halfmove &&
// //            lhs.fullmove == rhs.fullmove;
// // }

// // // Define equality operator for Position
// // bool operator==(const Position& lhs, const Position& rhs) {
// //     // Compare mailbox (piece placement)
// //     for (int sq = 0; sq < SQUARE_NB; ++sq) {
// //         if (lhs.at(static_cast<Square>(sq)) != rhs.at(static_cast<Square>(sq))) {
// //             return false;
// //         }
// //     }
    
// //     // Compare bitboards
// //     for (int color = 0; color < COLOR_NB; ++color) {
// //         if (lhs.occ(static_cast<Color>(color)) != rhs.occ(static_cast<Color>(color))) {
// //             return false;
// //         }
// //     }
    
// //     for (int piece = 0; piece < PIECE_NB; ++piece) {
// //         if (lhs.occ(static_cast<Piece>(piece)) != rhs.occ(static_cast<Piece>(piece))) {
// //             return false;
// //         }
// //     }
    
// //     // Compare game state history
// //     if (lhs.states.size() != rhs.states.size()) {
// //         return false;
// //     }
    
// //     for (size_t i = 0; i < lhs.states.size(); ++i) {
// //         if (!(lhs.states[i] == rhs.states[i])) {
// //             return false;
// //         }
// //     }
    
// //     // Compare computed state
// //     return lhs.pinned == rhs.pinned &&
// //            lhs.checks == rhs.checks &&
// //            lhs.threat == rhs.threat;
// // }

// // // Define inequality operator for Position
// // bool operator!=(const Position& lhs, const Position& rhs) {
// //     return !(lhs == rhs);
// // }

// TEST_F(PerftTest, RunAllPerftTests)
// {

//     // pos = initGameState("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

//     // EXPECT_EQ(perftx(pos, 1), -1);

//     // Position x = initGameState("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

//     // EXPECT_EQ(x, pos);

//     // pos = initGameState("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

//     // pos.computeChecks();
//     // pos.computePinned();
//     // pos.computeThreat();

//     // EXPECT_EQ(perftx(pos, 2), -1);

//     // EXPECT_EQ(pos, initGameState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

//     // EXPECT_EQ(perft(pos, 3), -1);

//     for (const auto& test : testCases)
//     {
//         for (auto depth = 1; depth <= test.results.size(); depth++) 
//         {
//             pos = initGameState(test.fen);

//             ASSERT_EQ(perft(pos, depth), test.results[depth - 1]) << " FEN: " << test.fen << " at depth " << depth;
//         }
//     }
// }


















#include <gtest/gtest.h>
#include "fen.h"
#include "move.h"
#include "position.h"
#include "perft.h"

using namespace std;
using namespace Talia;

class TestCase
{
    public:

        string fen;
        vector<int> expected;
};

class PerftTest : public ::testing::Test
{
    protected:

        Position pos;

        void checkMoves(string fen, vector<int> expected)
        {
            int depth = 1;
            for (const auto nodes: expected)
            {
                pos = fenToPosition(fen);

                int result = perft(pos, depth++);

                ASSERT_EQ(result, nodes);
            }
        }
};

TEST_F(PerftTest, RunAllTests)
{
    // https://www.chessprogramming.org/Perft_Results

    const vector<TestCase> testCases =
    {
        { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", {20, 400, 8902, 197281, 4865609, 119060324}},           // Position 1
        { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1", {48, 2039, 97862, 4085603, 193690690}},    // Position 2
        { "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", {14, 191, 2812, 43238, 674624, 11030083}},                             // Position 3
        { "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", {6, 264, 9467, 422333, 15833292}},              // Position 4
        { "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", {44, 1486, 62379, 2103487, 89941194}},                 // Position 5
        { "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", {46, 2079, 89890, 3894594, 164075551}}, // Position 6
    };

    // Run all test cases
    for (const auto& tc : testCases) checkMoves(tc.fen, tc.expected);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}