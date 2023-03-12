#include "VertexBuffer.h"

/// <summary>
/// This method serves to set Vertex buffer to graphics pipeline
/// </summary>
/// <param name="graphics">Graphics for vertex buffer</param>
void VertexBuffer::StageToPipeline(Graphics& graphics)
{
	//IA - InputAssmbler for graphics pipeline (1. stage)
	GetDeviceContext(graphics)->IASetVertexBuffers(0u, // start slot
		1u, // number of buffers
		pVertexBuffer.GetAddressOf(), // pointer to our vertex buffer
		&stride_size_, // adrss of stride size
		&offest); // pointer to array offset
};
