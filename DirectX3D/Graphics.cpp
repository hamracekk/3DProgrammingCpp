#include "Graphics.h"

#define RETURN_FALSE_IF_FAILED(result) if(FAILED(result)) return false

/// <summary>
/// This function initialize PrimitiveTopology (How should be points in Vertex buffer respresented).
/// This function also initialize viewport (How should be view mapped to window screen)
/// </summary>
/// <param name="width">width of viewport</param>
/// <param name="height">height of viewport</param>
void Graphics::InitPrimitiveTopologyAndViewport(int width, int height)
{
	// Setting primitive topology (how are pointes interpreted ?) - Input assembler
	pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//viewport mapping
	D3D11_VIEWPORT viewport;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0; // Depth of the view
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0; // Coordinate of viewport in render target
	viewport.TopLeftY = 0;
	pDevContext->RSSetViewports(1u, /*number of viewports */&viewport);
};

/// <summary>
/// This method is responsible for Creating/Setting vertex shader
/// </summary>
/// <param name="result">HRESULT of whole method</param>
/// <param name="pBlob">Pointer to buffer for binary input</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitVertexShader(HRESULT& result, ComPtr<ID3D10Blob>& pBlob)
{
	result = D3DReadFileToBlob(L"VertexShader.cso", &pBlob); //reading runtime created binary file from hlsl transformation
	RETURN_FALSE_IF_FAILED(result);
	
	//Creating vertex shder (COM approach)
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pVertexShader); // pointer to be filled
	RETURN_FALSE_IF_FAILED(result);

	pDevContext->VSSetShader(pVertexShader.Get(), NULL, 0); // Setting vertex shader

	return true;
};

/// <summary>
/// Method responsible for Creating/ Setting pixel shader
/// </summary>
/// <param name="result">HRESULT of whole method</param>
/// <param name="pBlob">Pointer to buffer for binary input</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitPixelShader(HRESULT& result, ComPtr<ID3D10Blob>& pBlob)
{

	result = D3DReadFileToBlob(L"PixelShader.cso", &pBlob); // reading dynamicly created binary file to blob
	RETURN_FALSE_IF_FAILED(result);

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pPixelShader); // pointer to be filled
	RETURN_FALSE_IF_FAILED(result);

	pDevContext->PSSetShader(pPixelShader.Get(), NULL, 0); // setting pixel shader

	return true;
};

/// <summary>
/// This method is responsible for initializing/creating/setting Input Layout
/// </summary>
/// <returns>True if initialization succeed</returns>
bool Graphics::InitInputLayout(HRESULT& result, ComPtr<ID3D10Blob>& pBlob)
{
	// descriptor for inpiut layout
	const D3D11_INPUT_ELEMENT_DESC arrayOfDescriptors[] =
	{
		{"POSITION", // semantic name (in hlsl file)
		0, // index of semantic (not using)
		DXGI_FORMAT_R32G32B32_FLOAT, // format of the data -> float2 Vertex(x,y)
		0, // binding to slot 0 
		0, // offset
		D3D11_INPUT_PER_VERTEX_DATA, // default
		0 },

		{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Creating input layout (COM approach) using device
	result = pDevice->CreateInputLayout(arrayOfDescriptors, // pointer to descriptor
		(UINT)std::size(arrayOfDescriptors), // number of elements
		pBlob->GetBufferPointer(), // Getting content of pBlol (binary)
		pBlob->GetBufferSize(),
		&pInputLayout);
	RETURN_FALSE_IF_FAILED(result);

	pDevContext->IASetInputLayout(pInputLayout.Get()); // Setting created input layout
	return true;
};

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

