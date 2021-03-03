#include "VertexBuffer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::AddBufferSub(unsigned int size, const void* data) const
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::SetAttrib(unsigned int size) const
{
	//position
	glEnableVertexArrayAttrib(m_RendererID, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	//color
	glEnableVertexArrayAttrib(m_RendererID, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, size, (void*)12);
}