#include "search.h"
#include "movegen.h"

namespace Talia
{

//     constexpr int INFINITY = std::numeric_limits<int>::max();


// Move think(const Position& pos)
// {
//     Position root = pos;
//     Move bestMove;
//     int bestScore = -INFINITY;
//     int alpha = -INFINITY;
//     int beta = INFINITY;

//     // Generate all possible moves
//     StaticArray<Move, MAX_MOVES> moves;
//     int size = genAllLegalMoves(root, moves.begin());

//     for (const Move& move : moves)
//     {
//         root.makemove(move);
//         int score = -negamax(root, -beta, -alpha);
//         root.undomove(move);

//         // Update the best move if a better score is found
//         if (score > bestScore)
//         {
//             bestScore = score;
//             bestMove = move;
//         }

//         // Alpha-beta pruning
//         alpha = std::max(alpha, score);

//         if (alpha >= beta) {
//             break; // Cut-off
//         }
        
//     }

//     return bestMove; // Return the best move found
// }

// int negamax(Position& pos, int alpha, int beta, int depth)
// {
//     if (depth == 0) {
//         return evaluate(pos);
//     }

//     int bestScore = -INFINITY;
    
//     // Generate all possible moves
//     StaticArray<Move, MAX_MOVES> moves;
//     int size = genAllLegalMoves(pos, moves.begin());

//     for (int idx = 0; idx < size; ++idx)
//     {
//         const Move& move = moves[idx];

//         pos.makemove(move);
//         int score = -negamax(pos, -beta, -alpha, depth - 1);
//         pos.undomove(move);

//         // Update the best score
//         bestScore = std::max(bestScore, score);

//         // Alpha-beta pruning
//         alpha = std::max(alpha, score);
        
//         if (alpha >= beta) {
//             break; // Cut-off
//         }
//     }

//     return bestScore;
// }

} // namespace