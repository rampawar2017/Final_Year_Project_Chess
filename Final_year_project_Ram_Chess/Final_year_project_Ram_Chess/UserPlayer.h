#pragma once
#include "Player.h"
#include <xstring>

//Player, which takes user's input to make a movement.
class UserPlayer : public Player
{
	static bool IsXCorrect(char srcXSymbol);
	static bool IsYCorrect(char srcXSymbol);
public:
	Movement DoMovement(Chessboard& chessboard) override;
};

