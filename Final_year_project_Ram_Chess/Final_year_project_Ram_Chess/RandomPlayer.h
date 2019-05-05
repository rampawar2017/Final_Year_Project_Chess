#pragma once
#include "Player.h"

//Player, which chooses movements randomly.
class RandomPlayer : public Player
{
public:
	Movement DoMovement(Chessboard& chessboard) override;
};

