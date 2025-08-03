// #include <gtest/gtest.h>
// #include "perft.h"
// #include "utility.h"

// namespace athena
// {

//     constexpr auto FEN_MODERN = "modern R 0 1111 1111 -,-,-,- rr,rn,rb,rq,rk,rb,rn,rr,rp,rp,rp,rp,rp,rp,rp,rp,8,br,bp,10,gp,gr,bn,bp,10,gp,gn,bb,bp,10,gp,gb,bk,bp,10,gp,gq,bq,bp,10,gp,gk,bb,bp,10,gp,gb,bn,bp,10,gp,gn,br,bp,10,gp,gr,8,yp,yp,yp,yp,yp,yp,yp,yp,yr,yn,yb,yk,yq,yb,yn,yr";
//     constexpr auto FEN_CLASSIC = "classic R 0 1111 1111 -,-,-,- rr,rn,rb,rq,rk,rb,rn,rr,rp,rp,rp,rp,rp,rp,rp,rp,8,br,bp,10,gp,gr,bn,bp,10,gp,gn,bb,bp,10,gp,gb,bq,bp,10,gp,gk,bk,bp,10,gp,gq,bb,bp,10,gp,gb,bn,bp,10,gp,gn,br,bp,10,gp,gr,8,yp,yp,yp,yp,yp,yp,yp,yp,yr,yn,yb,yk,yq,yb,yn,yr";

//     class TestPerft : public ::testing::Test
//     {
//     protected:
//         Position pos;
//     };

//     TEST_F(TestPerft, Modern)
//     {
//         fromString(FEN_MODERN, pos);
//         runPerftTests(pos, 4, false, false, false);
//     }

// } // namespace athena


// ply=1, positions=20
// ply=2, positions=395
// ply=3, positions=7800
// ply=4, positions=152050
// ply=5, positions=3452310
// ply=6, positions=77430383
// ply=7, positions=1735784286