#include "AiPlayer.h"
#include "Ai.h"

Movement AiPlayer::DoMovement(Chessboard& chessboard)
{
	//Bigger the depth, slower the speed is. 
	//Recommended depth is in range 1-5.
	return Ai::FindBestMovement(chessboard, 3, !chessboard.is_black());
}
