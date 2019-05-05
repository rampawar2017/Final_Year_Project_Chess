#include "Chessboard.h"
#include "Pawn.h"
#include <windows.h>
#include <iostream>
#include <algorithm>

//Returns color of a cell.
bool Chessboard::IsBlack(Cell cell)
{
	return cell >= BlackPawn;
}

//Getters for chessboard states.
//>>>
bool Chessboard::is_black() const
{
	return isBlack;
}

bool Chessboard::is_check() const
{
	return isCheck;
}

bool Chessboard::is_mate() const
{
	return isMate;
}

bool Chessboard::is_stalemate() const
{
	return isStalemate;
}
//<<<

//Verifies if there is a checkmate.
bool Chessboard::IsCheck(bool isBlack)
{
	//Take all enemies' movements.
	auto movements = GetAllMovements(!isBlack);
	const Cell king = isBlack ? BlackKing : WhiteKing;

	bool res = false;

	//Check if own king is under attack.
	for (auto movement : movements)
	{
		if (cells[movement.destCoord.x][movement.destCoord.y] == king)
		{
			res = true;
			break;
		}
	}

	return res;
}

//Initialize chessboard.
void Chessboard::Init()
{
	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			cells[x][y] = Empty;
		}
	}

	Cell whiteRow[] = { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook };
	Cell blackRow[] = { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook };
	for (int x = 0; x < size; ++x)
	{
		cells[x][0] = whiteRow[x];
		cells[x][1] = WhitePawn;
		cells[x][6] = BlackPawn;
		cells[x][7] = blackRow[x];
	}

	//Any other figure can be added for testing.
	//cells[3][4] = WhitePawn;
	//cells[5][6] = WhiteBishop;
	//cells[2][5] = BlackKnight;
	//cells[3][5] = BlackKing;
}

//Returns display symbol of a cell.
char Chessboard::CellToSymbol(Cell cell) const
{
	switch (cell)
	{
	case Empty:
		return ' ';
	case WhitePawn:
	case BlackPawn:
		return 'P';
	case WhiteKnight:
	case BlackKnight:
		return 'N';
	case WhiteBishop:
	case BlackBishop:
		return 'B';
	case WhiteRook:
	case BlackRook:
		return 'R';
	case WhiteQueen:
	case BlackQueen:
		return 'Q';
	case WhiteKing:
	case BlackKing:
		return 'K';
	}

	return '?';
}

//Visualizes chessboard.
void Chessboard::Show() const
{
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	const int defaultColors = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	for (int y = size - 1; y >= 0; --y)
	{
		SetConsoleTextAttribute(console, defaultColors);
		cout << y + 1 << " ";

		for (int x = 0; x < size; ++x)
		{
			const int background = (x + y) % 2
				? (BACKGROUND_BLUE | BACKGROUND_GREEN)
				: BACKGROUND_INTENSITY;

			const Cell cell = cells[x][y];

			int foreground = 0;
			if (cell != Empty)
			{
				foreground = IsBlack(cell)
					? 0
					: (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}

			SetConsoleTextAttribute(console, foreground | background);
			cout << CellToSymbol(cell) << " ";
		}
		cout << endl;
	}

	SetConsoleTextAttribute(console, defaultColors);
	cout << "  ";
	for (int x = 0; x < size; ++x)
	{
		cout << char('a' + x) << " ";
	}
	cout << endl << endl;
}

//Checks if the movement is valid.
bool Chessboard::IsValid(const Movement& movement) const
{
	auto moves = GetMoves(movement.srcCoord);
	return find(moves.begin(), moves.end(), movement.destCoord) != moves.end();
}

//Checks if the coordinate is inside chessboard.
bool Chessboard::IsInside(const Coord& coord)
{
	return coord.x >= 0 && coord.x < size && coord.y >= 0 && coord.y < size;
}

//Detects current state of the chessboard.
//if full is true checks for check, stalemate and checkmate, otherwise for check only (it is used for performance reason).
void Chessboard::UpdateState(bool full)
{
	isCheck = IsCheck(isBlack);

	if (full) {
		const auto correctMovements = GetCorrectMovements();
		isMate = isCheck && correctMovements.empty();
		isStalemate = !isCheck && correctMovements.empty();
	}
}

//Returns all possible movement from specified coordinate.
vector<Coord> Chessboard::GetMoves(const Coord& srcCoord) const
{
	const Cell cell = cells[srcCoord.x][srcCoord.y];

	vector<Coord> res;

	const bool isBlack = IsBlack(cell);

	struct Direction
	{
		int x, y;
	};

	//Different branches are used for different piece types.
	if (cell == Empty)
	{
		//There are no valid moves.
	}
	else if (cell == WhitePawn || cell == BlackPawn) //Pawn
	{
		const static vector<Direction> whiteMoveDirections = {
			Direction{0,1}, Direction{0,2}
		};
		const static vector<Direction> whiteHitDirections = {
			Direction{-1,1}, Direction{1,1}
		};
		const static vector<Direction> blackMoveDirections = {
			Direction{0,-1}, Direction{0,-2}
		};
		const static vector<Direction> blackHitDirections = {
			Direction{-1,-1}, Direction{1,-1}
		};

		//Direction for moves and hits processed separately.
		vector<Direction> moveDirections = isBlack ? blackMoveDirections : whiteMoveDirections;
		vector<Direction> hitDirections = isBlack ? blackHitDirections : whiteHitDirections;

		for (auto direction : moveDirections)
		{
			Coord destCoord{ srcCoord.x + direction.x, srcCoord.y + direction.y };
			if (IsInside(destCoord))
			{
				const Cell destCell = cells[destCoord.x][destCoord.y];

				if (destCell == Empty)
					res.push_back(destCoord);
				//Another figure detected. Stop looking.
				else break;

				//Pawn already moved. Only one cell movement is available.
				if (!isBlack && srcCoord.y != 1 || isBlack && srcCoord.y != 6) break;
			}
		}

		for (auto direction : hitDirections)
		{
			Coord destCoord{ srcCoord.x + direction.x, srcCoord.y + direction.y };
			if (IsInside(destCoord))
			{
				const Cell destCell = cells[destCoord.x][destCoord.y];
				const bool destIsBlack = IsBlack(destCell);

				if (destCell != Empty && isBlack != destIsBlack)
					res.push_back(destCoord);
			}
		}
	}
	else if (cell == WhiteKnight || cell == BlackKnight) //Knight
	{
		const static vector<Direction> directions = {
			Direction{-2,-1}, Direction{-2,1}, Direction{2,-1}, Direction{2,1},
			Direction{-1,-2}, Direction{1,-2}, Direction{-1,2}, Direction{1,2}
		};

		for (auto direction : directions)
		{
			Coord destCoord{ srcCoord.x + direction.x, srcCoord.y + direction.y };
			if (IsInside(destCoord))
			{
				const Cell destCell = cells[destCoord.x][destCoord.y];
				const bool destIsBlack = IsBlack(destCell);

				if (destCell == Empty || isBlack != destIsBlack)
					res.push_back(destCoord);
			}
		}
	}
	else //All other figures.
	{
		//Different directions for different piece types.
		const static vector<Direction> bishopDirections = { Direction{-1,-1}, Direction{-1,1}, Direction{1,-1}, Direction{1,1} };
		const static vector<Direction> rookDirections = { Direction{-1,0}, Direction{1,0}, Direction{0,-1}, Direction{0,1} };
		const static vector<Direction> queenDirections = {
			Direction{-1,-1}, Direction{-1,1}, Direction{1,-1}, Direction{1,1},
			Direction{-1,0}, Direction{1,0}, Direction{0,-1}, Direction{0,1}
		};

		//By default a figure can move up to 7 cells.
		int maxDistance = 7;
		const vector<Direction>* directions;
		switch (cell) {
		case WhiteBishop:
		case BlackBishop:
			directions = &bishopDirections;
			break;
		case WhiteRook:
		case BlackRook:
			directions = &rookDirections;
			break;
		case WhiteKing:
		case BlackKing:
			//King can move only to nearest cells.
			maxDistance = 1;
		case WhiteQueen:
		case BlackQueen:
			directions = &queenDirections;
			break;
		default:
			throw exception("Wrong cell");
		}

		for (auto direction : *directions)
		{
			for (int distance = 1; distance <= maxDistance; ++distance)
			{
				Coord destCoord{ srcCoord.x + direction.x*distance, srcCoord.y + direction.y*distance };
				if (IsInside(destCoord))
				{
					const Cell destCell = cells[destCoord.x][destCoord.y];
					const bool destIsBlack = IsBlack(destCell);

					if (destCell == Empty || isBlack != destIsBlack)
						res.push_back(destCoord);

					if (destCell != Empty)
						//Another figure is detected Stop looking to this direction.
						break;
				}
				//out of chessboard.
				else break;
			}
		}
	}

	return res;
}

//Returns all potential movement for the specified color.
vector<Movement> Chessboard::GetAllMovements(bool isBlack) const
{
	vector<Movement> res;

	//Try each cell.
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < size; ++y)
		{
			const Cell cell = cells[x][y];
			//If color is the same, get all movements for the figure.
			if (cell != Empty && IsBlack(cell) == isBlack)
			{
				auto srcCoord = Coord(x, y);
				const auto moves = GetMoves(srcCoord);
				for (auto move : moves)
				{
					res.emplace_back(srcCoord, move);
				}
			}
		}
	}

	return res;
}

//Returns only those movements, which don't end with check.
vector<Movement> Chessboard::GetCorrectMovements()
{
	vector<Movement> res;
	bool currentIsBlack = isBlack;

	auto movements = GetAllMovements(currentIsBlack);
	for (auto movement : movements)
	{
		Move(movement);
		const bool isCheck = IsCheck(currentIsBlack);
		Rollback();

		if (!isCheck)
		{
			res.push_back(movement);
		}
	}

	return res;
}

//Make a movement.
void Chessboard::Move(const Movement& movement)
{
	Cell cell = cells[movement.srcCoord.x][movement.srcCoord.y];

	//Commented out for performance. Movement should be valid.
	//if (IsValid(movement))
	{
		//Store current movement in the stack of movements.
		const State state{
			cell,
			cells[movement.destCoord.x][movement.destCoord.y],
			movement
		};
		states.push(state);

		//Change the chessboard.
		cells[movement.srcCoord.x][movement.srcCoord.y] = Empty;

		//Pawn promotion to queen.
		if (cell == WhitePawn && movement.destCoord.y == size - 1)
			cell = WhiteQueen;
		else if (cell == BlackPawn && movement.destCoord.y == 0)
			cell = BlackQueen;

		cells[movement.destCoord.x][movement.destCoord.y] = cell;
		isBlack = !isBlack;
	}

	//throw exception("Invalid movement");
}

//Restore chessboard to the state before last movement.
void Chessboard::Rollback()
{
	if (states.empty())
	{
		throw exception("States is empty");
	}
	else
	{
		const State state = states.top();
		states.pop();

		cells[state.movement.destCoord.x][state.movement.destCoord.y] = state.prevDestCell;
		cells[state.movement.srcCoord.x][state.movement.srcCoord.y] = state.prevSrcCell;
		isBlack = !isBlack;
	}
}

//Initializes chessboard.
Chessboard::Chessboard()
{
	Init();
}