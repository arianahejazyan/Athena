// #include "search.h"
// #include "movegen.h"
// #include "thread.h"
// #include "eval.h"

// namespace athena
// {

//     int SCORE_INFINITY = 100000;
//     int SCORE_DRAW = 0;
//     int SCORE_CHECKMATE = 10000;
//     int MAX_PLAY = 128;

//     Move MOVE_DRAW_FIFTY_MOVE;
//     Move MOVE_CHECKMATE;
//     Move MOVE_STALEMATE;

//     int negamax(Position &pos, Thread &thread, int alpha, int beta, int depth, int play)
//     {
//         if ((play == depth) || (play >= MAX_PLAY))
//         {
//             return evaluate(pos);
//         }

//         const GameState &gs = pos.states.back();

//         // Handle fifty move rule
//         if (gs.clock > 50)
//         {
//             if (play == 0)
//             {
//                 thread.score = SCORE_DRAW;
//                 thread.move = MOVE_DRAW_FIFTY_MOVE;
//             }
//             return SCORE_DRAW;
//         }

//         int bestScore = -SCORE_INFINITY;

//         Move moves[MAX_MOVES];
//         int size = 0;
//         size += genAllNoisyMoves(pos, moves + size);
//         size += genAllQuietMoves(pos, moves + size);

//         bool anyLegalMove = false;
//         for (int idx = 0; idx < size; ++idx)
//         {
//             auto move = moves[idx];

//             pos.makemove(move);

//             if (!isRoyalSafe(pos, gs.turn))
//             {
//                 pos.undomove(move);
//                 continue;
//             }

//             else
//                 anyLegalMove = true;

//             int score = -negamax(pos, thread, -beta, -alpha, depth - 1);

//             pos.undomove(move);

//             if (score > bestScore)
//             {
//                 bestScore = score;
//                 thread.score = bestScore;
//                 thread.move = move;
//             }

//             if (score >= beta)
//             {
//                 break; // Cut-off
//             }

//             if (score > alpha)
//             {
//                 alpha = score;
//             }
//         }

//         // [negamax]: handling stalemate and checkmate
//         if (!anyLegalMove)
//         {
//             if (isRoyalSafe(pos, gs.turn))
//             {
//                 if (play == 0)
//                 {
//                     thread.score = SCORE_DRAW;
//                     thread.move = MOVE_STALEMATE;
//                 }
//                 return SCORE_DRAW;
//             }

//             else
//             {
//                 if (play == 0)
//                 {
//                     thread.score = SCORE_CHECKMATE;
//                     thread.move = MOVE_CHECKMATE;
//                 }
//                 return SCORE_CHECKMATE;
//             }
//         }

//         return bestScore;
//     }

// } // namespace athena