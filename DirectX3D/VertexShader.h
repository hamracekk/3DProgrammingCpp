#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "GraphicsAbstraction.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h> // used for reading byte files from hlsl tranformation

/// <summary>
/// Class representing vertex shader (creating/setting to staging pipeline phase)
/// </summary>
class VertexShader : public GraphicStageAbstraction
{
public:
	VertexShader(Graphics& graphics);
	void StageToPipeline(Graphics& graphics) override;
	inline ID3D10Blob* GetBytCode() { return pBlob.Get(); };
private:
	ComPtr<ID3D10Blob> pBlob; //pointer to bytoce file
	ComPtr<ID3D11VertexShader> pVertexShader; // Pointer to our vertex shader
};

#endif // !VERTEX_SHADER_H

