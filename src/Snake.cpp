#include "Snake.h"

Snake::Snake(unsigned int cols, unsigned int rows)
	:m_Direction	{ left },
	 m_prevDirection{ left },
	 m_rows			{ rows },
	 m_cols			{ cols }
{
	if (m_cols > 5 && m_rows > 5)
	{
		m_Tail.push_front(std::make_pair(m_cols / 2, m_rows / 2));
		for (int i = 1; i <= 2; i++)
		{
			m_Tail.push_front(std::make_pair(m_cols / 2 + i, m_rows / 2));
		}
	}
	else
	{
		m_Tail.push_back(std::make_pair(0, 0));

	}

}

Snake::~Snake()
{
}

void Snake::Move()
{

	if (getSize() > 0)
	{
		switch (m_Direction)
		{
		case down:
			moveDown();
			m_prevDirection = down;
			break;
		case up:
			moveUp();
			m_prevDirection = up;
			break;
		case right:
			moveRight();
			m_prevDirection = right;
			break;
		case left:
			moveLeft();
			m_prevDirection = left;
			break;
		}
	}
}

void Snake::changeDirection(direction x)
{
	if(m_prevDirection == left && x != right)
		m_Direction = x;
	if (m_prevDirection == right && x != left)
		m_Direction = x;
	if (m_prevDirection == up && x != down)
		m_Direction = x;
	if (m_prevDirection == down && x != up)
		m_Direction = x;
}

void Snake::moveDown()
{
	if (m_Tail.back().second == 0)
		m_Tail.push_back(std::make_pair(m_Tail.back().first, m_cols-1));
	else
		m_Tail.push_back(std::make_pair(m_Tail.back().first, m_Tail.back().second-1));
	m_Tail.pop_front();	
}

void Snake::moveUp()
{
	if (m_Tail.back().second == m_cols-1)
		m_Tail.push_back(std::make_pair(m_Tail.back().first, 0));
	else
		m_Tail.push_back(std::make_pair(m_Tail.back().first, m_Tail.back().second + 1));
	m_Tail.pop_front();
}

void Snake::moveRight()
{
	if (m_Tail.back().first == m_rows - 1)
		m_Tail.push_back(std::make_pair(0, m_Tail.back().second));
	else
		m_Tail.push_back(std::make_pair(m_Tail.back().first + 1, m_Tail.back().second));
	m_Tail.pop_front();
}

void Snake::moveLeft()
{
	if(m_Tail.back().first == 0)
		m_Tail.push_back(std::make_pair(m_rows - 1, m_Tail.back().second));
	else
		m_Tail.push_back(std::make_pair(m_Tail.back().first-1, m_Tail.back().second));
	m_Tail.pop_front();
}

void Snake::Grow()
{
	m_Tail.push_front(std::make_pair(m_Tail.front().first, m_Tail.front().second));
}
