#include "Grid.h"
#include <stdlib.h>
#include <time.h>
#define GRID_SIZE 32
#include <iostream>



Grid::Grid(unsigned int ScreenWidth , unsigned int ScreenHeight)
    :m_cols          { GRID_SIZE },
     m_rows          { GRID_SIZE },
     m_MaxQuadCount  { (GRID_SIZE + GRID_SIZE) * (GRID_SIZE + GRID_SIZE) },
     m_MaxVertexCount{ (GRID_SIZE + GRID_SIZE) * (GRID_SIZE + GRID_SIZE) * 4 },
     m_MaxIndexCount { (GRID_SIZE + GRID_SIZE) * (GRID_SIZE + GRID_SIZE) * 6 },
     m_indexCount    { 0 },
     m_ScreenWidth   { ScreenWidth },
     m_ScreenHeight  { ScreenHeight },
     m_GameOver{false},
     m_AppleExists{false}
     

{
    m_Indices = new uint32_t[m_MaxIndexCount];
    m_EmptySpots = new int[GRID_SIZE * GRID_SIZE];
    m_Snake = new Snake(m_cols, m_rows);

    srand(time(0));
    m_Apple = new Apple();
    PlaceApple();

    uint32_t offset = 0;
    for (size_t i = 0; i < m_MaxIndexCount; i += 6)
    {
        m_Indices[i + 0] = 0 + offset;
        m_Indices[i + 1] = 1 + offset;
        m_Indices[i + 2] = 3 + offset;

        m_Indices[i + 3] = 1 + offset;
        m_Indices[i + 4] = 2 + offset;
        m_Indices[i + 5] = 3 + offset;
         
        offset += 4;
    }

    m_VAO = std::make_unique<VertexArray>();
    m_VAO->Bind();
    /*TODO: not sure what is the minimum amount of vertices... 
    my thought was to set it based on grid size, but had problems when snake was occupying all grid.. 
    so now it is allocating a bit of extra amount.*/
    m_VBO = std::make_unique<VertexBuffer>(sizeof(m_Vertex) * m_MaxVertexCount);
    m_EBO = std::make_unique<ElementBuffer>(m_Indices, sizeof(uint32_t) * m_MaxIndexCount);

    m_VBO->SetAttrib(sizeof(m_Vertex));
    m_VBO->Unbind();
    m_VAO->Unbind();
}

Grid::~Grid()
{
    delete[] m_Indices;
    delete[] m_EmptySpots;
    delete m_Snake;
    delete m_Apple;
}

void Grid::Update()
{
    /*TODO: not sure what is the minimum amount of vertices...
    my thought was to set it based on grid size, but had problems when snake was occupying all grid..
    so now it is allocating a bit of extra amount.*/
    std::vector<m_Vertex> vertices((GRID_SIZE + GRID_SIZE) * (GRID_SIZE + GRID_SIZE) * 4);

    m_Vertex* buffer = vertices.data();
    
    m_indexCount = 0;  

    if (m_GameOver)
    {
        m_GameOver = false;
        ResetGrid();
    }
        
    

    if (m_Snake->getHeadX() == m_Apple->getX() && m_Snake->getHeadY() == m_Apple->getY())
    {
        m_AppleExists = false;
        PlaceApple();
        m_Snake->Grow();
    }

    //snake
    for (unsigned int i = 0; i < m_Snake->getSize(); i++)
    {
        //head
        if (i == m_Snake->getSize() - 1)
            buffer = CreateQuad(buffer, m_Snake->getX(i), m_Snake->getY(i), 0.784f, 0.767f, 0.196f, 1.0f);
        //body
        else
        {
            if (m_Snake->getX(i) == m_Snake->getHeadX() && m_Snake->getY(i) == m_Snake->getHeadY())
                m_GameOver = true;
            buffer = CreateQuad(buffer, m_Snake->getX(i), m_Snake->getY(i), 0.177f, 0.299f, 0.054f, 1.0f);
        }

        m_indexCount += 6;
    }
    
    //apple
    if (m_AppleExists) 
    {
        buffer = CreateQuad(buffer, m_Apple->getX(), m_Apple->getY(), 0.848f, 0.021f, 0.159f, 1.0f);
        m_indexCount += 6;
    }
    

    m_VBO->Bind();
    m_VBO->AddBufferSub(vertices.size() * sizeof(m_Vertex), vertices.data());
}

void Grid::Draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_VAO->Bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void Grid::PlaceApple()
{
   

    SetEmptySpots();

    std::vector<int> empty_index_list;

    for (unsigned int i = 0; i < m_cols * m_rows; i++)
    {
        if (m_EmptySpots[i] == 0)
        {
            empty_index_list.push_back(i);
        }

    }

    if (empty_index_list.size() > 0)
    {
        unsigned int rand_index = empty_index_list[rand() % empty_index_list.size()];
        unsigned int col = rand_index % m_cols;
        unsigned int row = rand_index / m_cols;
        m_AppleExists = true;
        m_Apple->Create(col, row);
    }

    
   
}

void Grid::SetEmptySpots()
{
    std::fill_n(m_EmptySpots, m_cols * m_rows, 0);

    for (unsigned int i = 0; i < m_Snake->getSize(); i++)
    {
        m_EmptySpots[m_Snake->getX(i)+(m_Snake->getY(i)* m_cols)] = 1;
    }

}

void Grid::ResetGrid()
{
    delete m_Snake;
    m_Snake = new Snake(m_cols, m_rows);
    PlaceApple();
}


Grid::m_Vertex* Grid::CreateQuad(m_Vertex* target, unsigned int x, unsigned int y, float r, float g, float b, float a)
{
    target->Position = { (float)m_ScreenWidth / (float)m_cols * (float)x, (float)m_ScreenHeight / (float)m_rows * (float)y, 0.0f }; // top right
    target->Color = { r, g, b, a};
    target++;
    target->Position = { (float)m_ScreenWidth / (float)m_cols * (float)x + (float)m_ScreenWidth / (float)m_cols, (float)m_ScreenHeight / (float)m_rows * (float)y, 0.0f };  // bottom right
    target->Color = { r, g, b, a };
    target++;
    target->Position = { (float)m_ScreenWidth / (float)m_cols * (float)x + (float)m_ScreenWidth / (float)m_cols, (float)m_ScreenHeight / (float)m_rows * (float)y + (float)m_ScreenHeight / (float)m_rows, 0.0f };  // bottom left
    target->Color = { r, g, b, a };
    target++;
    target->Position = { (float)m_ScreenWidth / (float)m_cols * (float)x, (float)m_ScreenHeight / (float)m_rows * (float)y + (float)m_ScreenHeight / (float)m_rows, 0.0f };   // top left  
    target->Color = { r, g, b, a };
    target++;

    return target;
}

