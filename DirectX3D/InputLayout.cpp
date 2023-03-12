#include "InputLayout.h"

/// <summary>
/// Constructor for input layout (creating of input layout using device)
/// </summary>
/// <param name="graphics">Graphics for input layout</param>
/// <param name="arrayOfDescriptors">Descriptors descripts how the data are maped to hlsl transformation</param>
/// <param name="pVertexShaderBlob">pointer to vertex shader bytcode</param>
InputLayout::InputLayout(Graphics& graphics, vector<D3D11_INPUT_ELEMENT_DESC> arrayOfDescriptors, ID3DBlob* pVertexShaderBlob)
{
	//Creating input layout (COM approach) using device
	HRESULT result = GetDevice(graphics)->CreateInputLayout(arrayOfDescriptors.data(), // pointer to descriptor
		(UINT)std::size(arrayOfDescriptors), // number of elements
		pVertexShaderBlob->GetBufferPointer(), // Getting content of pBlol (binary)
		pVertexShaderBlob->GetBufferSize(),
		&pInputLayout);

	if (FAILED(result))
		throw GraphicsException(__LINE__, __FILE__, result);
};

/// <summary>
/// This method serves to set input layout to graphics pipeline
/// </summary>
/// <param name="graphics">Graphics for input layout</param>
void InputLayout::StageToPipeline(Graphics& graphics)
{
	//Input assembler stage
	GetDeviceContext(graphics)->IASetInputLayout(pInputLayout.Get()); // Setting created input layout
};
