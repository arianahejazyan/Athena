// #include <gtest/gtest.h>
// #include "nnue/features/nnue_feature_transformer.h"
// #include "position.h"

// namespace athena
// {

//     class TestFeatureTransformer : public ::testing::Test
//     {
//     protected:
//         FeatureTransformer transformer;
//     };

//     TEST_F(TestFeatureTransformer, TransformEmptyPosition)
//     {
//         Position pos;
//         pos.board.clear();
//         pos.board.setSQ(C5, PieceClass(Queen, Blue));

//         int features[Lx0] = {};

//         transformer.transform(pos, features);

//         features[feature_index(C5, PieceClass(Queen, Blue))] = 1;
//     }

// } // namespace athena
