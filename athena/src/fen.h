#pragma once

#include <string>
#include <string_view>

namespace athena
{

using FEN = std::string_view;
static constexpr FEN STARTPOS =
#ifdef GAME_SETUP_MODERN
"R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
"x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
"x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
"x,x,x,8,x,x,x/"
"bR,bP,10,gP,gR/"
"bN,bP,10,gP,gN/"
"bB,bP,10,gP,gB/"
"bQ,bP,10,gP,gK/"
"bK,bP,10,gP,gQ/"
"bB,bP,10,gP,gB/"
"bN,bP,10,gP,gN/"
"bR,bP,10,gP,gR/"
"x,x,x,8,x,x,x/"
"x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
"x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";    
#else
"R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
"x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
"x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
"x,x,x,8,x,x,x/"
"bR,bP,10,gP,gR/"
"bN,bP,10,gP,gN/"
"bB,bP,10,gP,gB/"
"bK,bP,10,gP,gQ/"
"bQ,bP,10,gP,gK/"
"bB,bP,10,gP,gB/"
"bN,bP,10,gP,gN/"
"bR,bP,10,gP,gR/"
"x,x,x,8,x,x,x/"
"x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
"x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";
#endif

} // namespace athena