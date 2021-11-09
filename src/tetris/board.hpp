#ifndef BOARD_HPP
#define BOARD_HPP

#include "utility.hpp"
#include "tetris/tetromino.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <vector>
#include <memory>

struct Field
{
	sf::Sprite* mSprite = nullptr;
	bool isOccupied = false;
};

class Board
{
public:
	Board(int width, int height, sf::Vector2i offset);

	void setSpriteContainer(std::unordered_map<int, sf::Sprite>* spriteContainer);
	void addBlock(std::unique_ptr<Tetromino>& tetromino);
	bool doesPieceFit(Tetromino tetromino, Tetromino::Rotation rotation, int xShift, int yShift);

	void removeLine();
	void clean();
	void printGrid(sf::RenderTarget& target);

private:
	std::vector<std::vector<Field>> mCells;
	std::unordered_map<int, sf::Sprite>* mSpriteContainer;
	int mWidth;
	int mHeight;
};

#endif // BOARD_HPP