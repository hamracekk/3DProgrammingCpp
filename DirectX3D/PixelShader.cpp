#include "PixelShader.h"

/// <summary>
/// Constructor for Pixel shder (reading bytoce and creating pixel shader
/// </summary>
/// <param name="graphics">graphics used by pixel shader</param>
PixelShader::PixelShader(Graphics& graphics)
{
	HRESULT result = D3DReadFileToBlob(L"PixelShader.cso", &pBlob); // reading dynamicly created binary file to blob
	if (FAILED(result))
		throw GraphicsException(__LINE__, __FILE__, result);

	result = GetDevice(graphics)->CreatePixelShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pPixelShader); // pointer to be filled
	if (FAILED(result))
		throw GraphicsException(__LINE__, __FILE__, result);
};

/// <summary>
/// Method used for add  pixel shader to get inside of pipeline staging
/// </summary>
/// <param name="graphics">Graphics used for pixel shader staging</param>
void PixelShader::StageToPipeline(Graphics& graphics)
{
	GetDeviceContext(graphics)->PSSetShader(pPixelShader.Get(), NULL, 0); // setting pixel shader
};
