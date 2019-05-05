#pragma once
#include "Movement.h"

class Chessboard;

//Base class for all types of players.
class Player
{
public:
	virtual Movement DoMovement(Chessboard& chessboard) = 0;
};
