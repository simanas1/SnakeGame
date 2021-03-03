#pragma once

class VertexBuffer {
public:
	VertexBuffer(unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void AddBufferSub(unsigned int size, const void* data) const;
	void SetAttrib(unsigned int size) const;
private:
	unsigned int m_RendererID;
};