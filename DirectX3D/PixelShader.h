#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include "GraphicsAbstraction.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h> // used for reading byte files from hlsl tranformation

/// <summary>
/// Class representing pixel shader (creating/setting to staging pipeline phase)
/// </summary>
class PixelShader : public GraphicStageAbstraction
{
public:
	PixelShader(Graphics& graphics);
	void StageToPipeline(Graphics& graphics) override;
private:
	ComPtr<ID3D10Blob> pBlob; //pointer to bytoce file
	ComPtr<ID3D11PixelShader> pPixelShader; // Pointer to our vertex shader
};

#endif // !PIXEL_SHADER_H
