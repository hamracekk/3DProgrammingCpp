#include "Graphics.h"

bool Graphics::Initialize(HWND handle, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC SwapDesc{}; // Descriptor structure which provides information how to construct swapChain and device

	ZeroMemory(&SwapDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); // Zero out the memory

	SwapDesc.BufferDesc.Width = width;
	SwapDesc.BufferDesc.Height = height;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Bite format for RGB collor
	SwapDesc.BufferDesc.RefreshRate.Numerator = 60; // Repetition frequency
	SwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  // No scaling require
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // No Scaline ordering
	SwapDesc.SampleDesc.Count = 1; // No aliasing
	SwapDesc.SampleDesc.Quality = 0; // No aliasing
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how it is given buffer used
	SwapDesc.BufferCount = 1; // double buffering 
	SwapDesc.OutputWindow = handle; // handle to the window
	SwapDesc.Windowed = TRUE;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Effect used for prezentation.
	SwapDesc.Flags = 0; // No flags


	//vytvorenie "zariadenia" , "bufferov" a "swap chainu"
	HRESULT res = D3D11CreateDeviceAndSwapChain(
		NULL, // use deafult adapter
		D3D_DRIVER_TYPE_HARDWARE, // use hardware device
		NULL, // handle to binary for driver
		0, // no flags
		NULL, // Featuer level array pointer
		0, 
		D3D11_SDK_VERSION, // Sdk version
		&SwapDesc, // adress for swapchain descriptor
		&pSwapChain,  // adress of swapChain to be filled (COM)
		&pDevice, // adress of Device to be filled (COM)
		NULL, // feature level support (outparamter)
		&pDevContext); //adress of device context to be filled (COM)

	if (FAILED(res)) { // kontrola
		throw GraphicsException(__LINE__, __FILE__, res);
	};

	ComPtr<ID3D11Resource> pBackBuffer; // resource for texture in swapchain
	pSwapChain->GetBuffer(0, //index of buffer (zero is backbuffer
		__uuidof(ID3D11Resource), // COM -> we are querying interface
		&pBackBuffer); // Object to be filled with interface handle

	res = pDevice->CreateRenderTargetView(pBackBuffer.Get(), // Rendering given view
										  NULL,
									      &pRenderTargetView); // View to be filled

	if (FAILED(res)) { // kontrola
		throw GraphicsException(__LINE__, __FILE__, res);
	};

	return true;
}

/// <summary>
/// Present next frame 
/// </summary>
void Graphics::FlipFrame()
{
	HRESULT res = pSwapChain->Present(1u, /* setting 60Hz repetition rate */ 0u /*No flags*/);

	if (FAILED(res))
	{
		throw GraphicsException(__LINE__, __FILE__, res);
	}
};

/// <summary>
/// This function Render view backgorund with given collors
/// </summary>
/// <param name="red"></param>
/// <param name="green"></param>
/// <param name="blue"></param>
void Graphics::ColorBuffer(float red, float green, float blue)
{
	const float colors[] = { red, green, blue, 1.0f };
	pDevContext->ClearRenderTargetView(pRenderTargetView.Get(), colors);
};

