#pragma once
#include "Player.h"

//Player, which uses Ai class to make a movement.
class AiPlayer : public Player
{
public:
	Movement DoMovement(Chessboard& chessboard) override;
};

