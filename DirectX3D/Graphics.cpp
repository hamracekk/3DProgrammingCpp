#include "Graphics.h"

/// <summary>
/// Method responsible for Creating/Setting depth stencil state
/// </summary>
/// <param name="result">HRESULT of whole method</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitDepthBuffer(HRESULT& result)
{
	D3D11_DEPTH_STENCIL_DESC descriptor = {}; // drscriptor for depth buffer
	descriptor.DepthEnable = true; // enable depth 
	descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descriptor.DepthFunc = D3D11_COMPARISON_LESS; // how should be object compared if they are drawn on top of each other
    
	ComPtr<ID3D11DepthStencilState> pDepthStencilState; // pointer to depth buffer
	result = pDevice->CreateDepthStencilState(&descriptor, &pDepthStencilState);
	if(FAILED(result))
		return false;

	// binding depth stencil state
	pDevContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u); // second value is random :)
	return true;
}

/// <summary>
/// This method is reponsible for initilization of the swapchain a nd device
/// </summary>
/// <param name="result">input HRESULT for controlling if everything succeed and if not passing to the exception</param>
/// <param name="width">width of the screen</param>
/// <param name="height">height of the screen</param>
/// <param name="handle">handle to the window</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitSwapChainAndDevice(HRESULT& result, UINT width, UINT height, HWND handle)
{
	DXGI_SWAP_CHAIN_DESC SwapDesc = {}; // Descriptor structure which provides information how to construct swapChain and device
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
	result = D3D11CreateDeviceAndSwapChain(
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

	return FAILED(result) ? false : true;
};

/// <summary>
/// Method responsible for creating Render taget view
/// </summary>
/// <param name="result">input HRESULT for controlling if everything succeed and if not passing to the exception</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitRenderTargetView(HRESULT& result)
{
	ComPtr<ID3D11Resource> pBackBuffer; // resource for texture in swapchain
	pSwapChain->GetBuffer(0, //index of buffer (zero is backbuffer
		__uuidof(ID3D11Resource), // COM -> we are querying interface
		&pBackBuffer); // Object to be filled with interface handle

	result = pDevice->CreateRenderTargetView(pBackBuffer.Get(), // Rendering given view
		NULL,
		&pRenderTargetView); // View to be filled

	return FAILED(result) ? false : true;
};

/// <summary>
/// Method responsible for preparing and creating depth texture and depth stencil view
/// </summary>
/// <param name="result">input HRESULT for controlling if everything succeed and if not passing to the exception</param>
/// <param name="width">width of the screen</param>
/// <param name="height">height of the screen</param>
/// <returns>True if method succeeded</returns>
bool Graphics::InitDepthStencilTexture(HRESULT& result, int width, int height)
{
	ComPtr<ID3D11Texture2D> pDepthTexture2D;
	D3D11_TEXTURE2D_DESC descriptor = {};
	descriptor.Width = width;
	descriptor.Height = height;
	descriptor.MipLevels = 1u;
	descriptor.ArraySize = 1u; // onyl single texture
	descriptor.Format = DXGI_FORMAT_D32_FLOAT; // 32bit float for depth values
	descriptor.SampleDesc.Count = 1u; // used for anti-aliasing - (we are not using it -> 1u 0u)
	descriptor.SampleDesc.Quality = 0u;
	descriptor.Usage = D3D11_USAGE_DEFAULT; 
	descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL; // what er are actualy binding

	// creating 2D depth stencil texture
	result = pDevice->CreateTexture2D(&descriptor, NULL, &pDepthTexture2D);
	if (FAILED(result))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC descriptorView = {};
	descriptorView.Format = DXGI_FORMAT_D32_FLOAT; // 32bit float for depth values
	descriptorView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descriptorView.Texture2D.MipSlice = 0u;

	result = pDevice->CreateDepthStencilView(pDepthTexture2D.Get(), // pointer to depth 2D texture
		&descriptorView,
		&pDepthStencilView); // pointer to be filled with view

	if (FAILED(result))
		return false;

	//binding of depth stencil view to Output manager
	pDevContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

	return true;
}

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
	if (FAILED(result)) 
		return false;
	
	//Creating vertex shder (COM approach)
	result = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pVertexShader); // pointer to be filled
	if (FAILED(result)) 
		return false;

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
	if (FAILED(result)) 
		return false;

	result = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), // pointer to buffer
		pBlob->GetBufferSize(), // buffer size
		NULL,
		&pPixelShader); // pointer to be filled
	if (FAILED(result)) 
		return false;

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
	};

	//Creating input layout (COM approach) using device
	result = pDevice->CreateInputLayout(arrayOfDescriptors, // pointer to descriptor
		(UINT)std::size(arrayOfDescriptors), // number of elements
		pBlob->GetBufferPointer(), // Getting content of pBlol (binary)
		pBlob->GetBufferSize(),
		&pInputLayout);
	if (FAILED(result)) 
		return false;

	pDevContext->IASetInputLayout(pInputLayout.Get()); // Setting created input layout
	return true;
};

/// <summary>
/// This method wrap initilization of all graphics pipeline components
/// </summary>
/// <param name="width">width of the screen</param>
/// <param name="height">height of the screen</param>
/// <param name="handle">handle to the window</param>
/// <returns>True if method succeeded</returns>
bool Graphics::Initialize(HWND handle, int width, int height)
{
	HRESULT result;

	if (!InitSwapChainAndDevice(result, width, height, handle))
		throw GraphicsException(__LINE__, __FILE__, result);

	if (!InitRenderTargetView(result))
		throw GraphicsException(__LINE__, __FILE__, result);

	if (!InitDepthBuffer(result))
		throw GraphicsException(__LINE__, __FILE__, result);

	if (!InitDepthStencilTexture(result, width, height))
		throw GraphicsException(__LINE__, __FILE__, result);

	return true;
};

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
	// Clearing buffers before next frame
	pDevContext->ClearRenderTargetView(pRenderTargetView.Get(), colors);
	pDevContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

/// <summary>
/// Simple call to Device context to draw using index buffer
/// </summary>
/// <param name="indexCount">Number of indices to draw</param>
void Graphics::DisplayUsingIndexBuffer(unsigned int indexCount)
{
	pDevContext->DrawIndexed(indexCount, 0u, 0u);
};

