#include "Renderer.h"
#include <iostream>

class PingPong {
private:
	unsigned int m_RendererID;
	unsigned int ColorBuffers;
	
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;
public:
	PingPong();
	~PingPong();

	unsigned int GetColor();

	void Bind();
	void UnBind();
	void BindColorBuffer(unsigned int slot = 0) const;
};
