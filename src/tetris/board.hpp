#ifndef BOARD_HPP
#define BOARD_HPP

#include "utility.hpp"
#include "tetris/tetromino.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <vector>
#include <memory>

struct Field
{
	sf::Sprite mSprite;
	bool isOccupied = false;
};

class Board
{
public:
	Board(int width, int height);

	void addBlock(std::unique_ptr<Tetromino>& tetromino);
	bool doesPieceFit(Tetromino tetromino, Tetromino::Rotation rotation, int xShift, int yShift);

	void removeLine();
	void clean();
	void printGrid(sf::RenderTarget& target);

private:
	std::vector<std::vector<Field>> mCells;
	std::vector<std::unique_ptr<Tetromino>> mTetrominos;
	int mWidth;
	int mHeight;
};

#endif // BOARD_HPP