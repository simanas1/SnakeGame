#include "Apple.h"

Apple::Apple()
{
}

Apple::~Apple()
{
}

void Apple::Create(unsigned int x, unsigned int y)
{
	m_Location = std::make_pair(x, y);
}
