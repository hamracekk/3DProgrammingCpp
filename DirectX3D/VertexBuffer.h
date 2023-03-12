#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "GraphicsAbstraction.h"

class VertexBuffer : public GraphicStageAbstraction
{
public:
	template<typename Type>
	VertexBuffer(Graphics& graphics, vector<Type>& vertices)
		: stride_size_(sizeof(Type))
	{
		UINT byteWidth = stride_size_ * vertices.size();

		D3D11_BUFFER_DESC descriptor = {}; // descriptor for buffer
		descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER; // type of the buffer 
		descriptor.Usage = D3D11_USAGE_DEFAULT; // buffer used by GPU only
		descriptor.CPUAccessFlags = 0u;
		descriptor.MiscFlags = 0u;
		descriptor.ByteWidth = byteWidth; // allocating right amount of space for buffer
		descriptor.StructureByteStride = stride_size_; // size of one vertices eleemnt


		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = vertices; // setting pointer to our data

		//filling vertex buffer (COM approach)
		HRESULT hres = GetDevice(graphics)->CreateBuffer(&descriptor, &data, &pVertexBuffer);
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);
	};

	void StageToPipeline(Graphics& graphics) override;

protected:
	ComPtr<ID3D11Buffer> pVertexBuffer; // Buffer for verteces
	UINT stride_size_;
	const UINT offest = 0u;
};

#endif // ! VERTEX_BUFFER_H
