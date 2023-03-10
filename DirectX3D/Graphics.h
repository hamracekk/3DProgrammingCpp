#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include "Includer.h"
#include "Exceptions.h"

#include <d3d11.h>

class Graphics
{
public:
	inline Graphics(HWND hwnd, int width, int height) { Initialize(hwnd, width, height); };
	~Graphics();
	Graphics(const Graphics&) = delete; 
	Graphics& operator = (const Graphics&) = delete;
	bool Initialize(HWND hwnd, int width, int height);
	void FlipFrame();
	void ColorBuffer(float red, float green, float blue);
private:
	ID3D11Device* pDevice = nullptr; // Pointer to graphics device
	IDXGISwapChain* pSwapChain = nullptr; // Pointer na Swap chain
	ID3D11DeviceContext* pDevContext = nullptr; // Pointer to device context
	ID3D11RenderTargetView* pRenderTargetView = nullptr; // Pointer to render of view
};

#endif //!GRAPHICS_H_
