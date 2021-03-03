#pragma once

class ElementBuffer {
public:
	ElementBuffer(const unsigned int* data, unsigned int size);
	~ElementBuffer();

	void Bind();

private:
	unsigned int m_RendererID;
};