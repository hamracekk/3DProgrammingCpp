#include "VertexShader.h"

/// <summary>
/// Constructor for Vertex shder
/// </summary>
/// <param name="graphics">graphics used by vertex shader</param>
VertexShader::VertexShader(Graphics& graphics)
{
	HRESULT result = D3DReadFileToBlob(L"VertexShader.cso", &pBlob); //reading runtime created binary file from hlsl transformation
	if (FAILED(result))
		throw GraphicsException(__LINE__, __FILE__, result);

	//Creating vertex shder (COM approach)
	result = GetDevice(graphics)->CreateVertexShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pVertexShader); // pointer to be filled
	if (FAILED(result))
		throw GraphicsException(__LINE__, __FILE__, result);
}

/// <summary>
/// Method used for add vertex shder to pipeline staging
/// </summary>
/// <param name="graphics">Graphics used for vertex shader staging</param>
void VertexShader::StageToPipeline(Graphics& graphics)
{
	GetDeviceContext(graphics)->VSSetShader(pVertexShader.Get(), NULL, 0); // Setting vertex shader
}
