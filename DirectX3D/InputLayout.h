#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include "GraphicsAbstraction.h"

/// <summary>
/// Class representing construction/creation of input layout stage
/// </summary>
class InputLayout : GraphicStageAbstraction
{
public:
	InputLayout(Graphics& graphics, vector<D3D11_INPUT_ELEMENT_DESC> arrayOfDescriptors, ID3DBlob* pVertexShaderBlob);
	void StageToPipeline(Graphics& graphics) override;
private:
	ComPtr<ID3D11InputLayout> pInputLayout; // How should be Vertex struct represented ? -> mapping of it secured by InputLayout
};

#endif //!INPUT_LAYOUT_H
