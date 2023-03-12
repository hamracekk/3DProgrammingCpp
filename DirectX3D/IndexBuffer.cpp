#include "IndexBuffer.h"

/// <summary>
/// Constructor for index buffer
/// </summary>
/// <param name="graphics">Graphics for index buffer</param>
/// <param name="indices">vector of indices</param>
IndexBuffer::IndexBuffer(Graphics& graphics, vector<unsigned short>& indices) 
	: indeces_count_(indices.size())
{
	// creating index buffer for vertexes
	UINT byteStride = sizeof(unsigned short);
	UINT byteWidth = indeces_count_ * byteStride;

	D3D11_BUFFER_DESC descriptorInd = {}; // descriptor for buffer
	descriptorInd.BindFlags = D3D11_BIND_INDEX_BUFFER; // type of the buffer 
	descriptorInd.Usage = D3D11_USAGE_DEFAULT; // buffer used by GPU only
	descriptorInd.CPUAccessFlags = 0u;
	descriptorInd.MiscFlags = 0u;
	descriptorInd.ByteWidth = byteWidth; // allocating right amount of space for buffer
	descriptorInd.StructureByteStride = byteStride; // size of one vertices eleemnt

	D3D11_SUBRESOURCE_DATA dataInd = {};
	dataInd.pSysMem = indices.data();

	//vreating Index buffer using device (which is responsible for resources allocation)
	HRESULT hres = GetDevice(graphics)->CreateBuffer(&descriptorInd, &dataInd, &pIndexBuffer);
	if(FAILED(hres))
		throw GraphicsException(__LINE__, __FILE__, hres);
}

/// <summary>
/// This method serves to set index buffer to graphics pipeline
/// </summary>
/// <param name="graphics">Graphics for index buffer</param>
void IndexBuffer::StageToPipeline(Graphics& graphics)
{
	GetDeviceContext(graphics)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u); // setting index buffer
};
