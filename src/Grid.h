#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include "Snake.h"
#include "Apple.h"


class Grid {
public:
	Grid(unsigned int ScreenWidth, unsigned int ScreenHeight);
	~Grid();

	void Update();
	void Draw();
	Snake* m_Snake;
	Apple* m_Apple;
	void PlaceApple();
	void SetEmptySpots();
	void ResetGrid();
	bool m_GameOver;
private:
	const unsigned int m_ScreenWidth;
	const unsigned int m_ScreenHeight;
	const unsigned int m_cols;
	const unsigned int m_rows;
	const size_t m_MaxQuadCount;
	const size_t m_MaxVertexCount;
	const size_t m_MaxIndexCount;
	int* m_EmptySpots;
	uint32_t m_indexCount;
	uint32_t *m_Indices;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<ElementBuffer> m_EBO;
	bool m_AppleExists;
	

	struct Vec3 {
		float x, y, z;
	};

	struct Vec4 {
		float x, y, z, a;
	};

	struct m_Vertex {
		Vec3 Position;
		Vec4 Color;
	};

	m_Vertex* CreateQuad(m_Vertex* target, unsigned int x, unsigned int y, float r, float g, float b, float a);
};