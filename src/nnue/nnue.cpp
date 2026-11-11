#include "nnue/nnue.h"
#include "nnue/network.h"

namespace athena::nnue {

// inline int32_t NNUE::evaluate(const chess::Position& pos) const noexcept {

//     const auto stm = pos.turn();
//     int32_t score = 0;

//     score += screlu_dot<A1>(stack_.self(stm).data(), network_.w2[0 * A1]);
//     score += screlu_dot<A1>(stack_.next(stm).data(), network_.w2[1 * A1]);
//     score += screlu_dot<A1>(stack_.ally(stm).data(), network_.w2[2 * A1]);
//     score += screlu_dot<A1>(stack_.prev(stm).data(), network_.w2[3 * A1]);

//     // Match the Rust quantisation path
//     // (adjust constants to the exact QA/QB/SCALE you trained with)
//     score /= QA;                          // remove one QA from the SCReLU square
//     score += network_.b2[0];              // output bias (int16 → int32)
//     score *= SCALE;                       // final eval scale (usually 400 or similar)
//     score /= (QA * QB);                   // remove remaining quantisation

//     return score;
// }

} // namespace athena
