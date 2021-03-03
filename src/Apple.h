#pragma once
#include <utility>

class Apple
{
public:
	Apple();
	~Apple();
	unsigned int getX() { return m_Location.first; }
	unsigned int getY() { return m_Location.second; }
	void Create(unsigned int x, unsigned int y);
private:
	std::pair<unsigned int, unsigned int> m_Location;
};