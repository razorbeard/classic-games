#ifndef BOARD_HPP
#define BOARD_HPP

#include "utility.hpp"
#include "tetris/tetromino.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <vector>
#include <memory>
#include <set>

struct Field
{
	sf::Sprite* mSprite = nullptr;
	bool isOccupied = false;
};

class Board
{
public:
	Board(int width, int height);

	void setSpriteContainer(std::unordered_map<int, sf::Sprite>* spriteContainer);
	std::tuple<std::set<int>, int> addBlock(std::unique_ptr<Tetromino>& tetromino);
	int removeLines(std::set<int>& lineIndexes);
	bool doesPieceFit(Tetromino tetromino, Tetromino::Rotation rotation, int xShift, int yShift);

	void display(sf::RenderTarget& target);
	void setOffset(sf::Vector2i offset);
	int getSpawnLocation() const;

private:
	std::vector<std::vector<Field>> mCells;
	std::unordered_map<int, sf::Sprite>* mSpriteContainer;
	sf::Vector2i mOffset;
	int mWidth;
	int mHeight;
};

#endif // BOARD_HPP