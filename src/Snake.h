#pragma once
#include <deque>
#include <utility>


enum direction { left, right, up, down };

class Snake {

public:
	Snake(unsigned int cols, unsigned int rows);
	~Snake();
	unsigned int getSize(){ return m_Tail.size(); }
	unsigned int getX(unsigned int i) { return m_Tail[i].first; }
	unsigned int getY(unsigned int i) { return m_Tail[i].second; }
	unsigned int getHeadX() { return m_Tail.back().first; }
	unsigned int getHeadY() { return m_Tail.back().second; }
	void Grow();
	void Move();
	void changeDirection(direction x);

private:
	std::deque<std::pair<unsigned int, unsigned int>> m_Tail;
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	
	direction m_Direction;
	direction m_prevDirection;
	unsigned int m_rows;
	unsigned int m_cols;
}; 