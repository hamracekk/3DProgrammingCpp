#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include "Includer.h"
#include "Exceptions.h"

#include <wrl.h> // We will use ComPtr -> release object automaticly (e.g. in case of exception)
#include <d3d11.h>
#include <d3dcompiler.h> // used for reading byte files from hlsl tranformation

using namespace Microsoft::WRL;

class Graphics
{
public:
	inline Graphics(HWND hwnd, int width, int height) { Initialize(hwnd, width, height); };
	Graphics(const Graphics&) = delete; 
	Graphics& operator = (const Graphics&) = delete;
	~Graphics() = default; // releasing is handled by ComPtr
	bool Initialize(HWND hwnd, int width, int height);
	void FlipFrame();
	void ColorBuffer(float red, float green, float blue);

	void DrawTriangle()
	{
		// creating vertex buffer
		struct Vertex 
		{
			float x;
			float y;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char alpha;
		};

		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 255, 0, 0, 0 },
			{ 0.5f, -0.5f, 0, 255, 0, 0},
			{ -0.5f, -0.5f, 0, 0, 255, 0},
			{ -0.3f, 0.3f, 255, 0, 0, 0 },
			{ 0.3f, 0.3f, 0, 255, 0, 0},
			{ 0.0f, -0.8f, 0, 0, 255, 0},
		};

		//indeces for vertexes
		const unsigned short indices[] =
		{
			0,1,2,
			0,2,3,
			0,4,1,
			2,1,5,
		};

		// creating index buffer for vertexes
		ComPtr<ID3D11Buffer> pIndexBuffer;
		D3D11_BUFFER_DESC descriptorInd = {}; // descriptor for buffer
		descriptorInd.BindFlags = D3D11_BIND_INDEX_BUFFER; // type of the buffer 
		descriptorInd.Usage = D3D11_USAGE_DEFAULT; // buffer used by GPU only
		descriptorInd.CPUAccessFlags = 0u;
		descriptorInd.MiscFlags = 0u;
		descriptorInd.ByteWidth = sizeof(indices); // allocating right amount of space for buffer
		descriptorInd.StructureByteStride = sizeof(unsigned short); // size of one vertices eleemnt
		D3D11_SUBRESOURCE_DATA dataInd;
		dataInd.pSysMem = indices;
		pDevice->CreateBuffer(&descriptorInd, &dataInd, &pIndexBuffer);
		pDevContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u); // setting index buffer

		ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC descriptor = {}; // descriptor for buffer
		descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER; // type of the buffer 
		descriptor.Usage = D3D11_USAGE_DEFAULT; // buffer used by GPU only
		descriptor.CPUAccessFlags = 0u;
		descriptor.MiscFlags = 0u;
		descriptor.ByteWidth = sizeof(vertices); // allocating right amount of space for buffer
		descriptor.StructureByteStride = sizeof(Vertex); // size of one vertices eleemnt


		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = vertices; // setting pointer to our data

		//filling vertex buffer (COM approach)
		HRESULT hres = pDevice->CreateBuffer(&descriptor, &data, &pVertexBuffer);
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);

		// bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offest = 0u;
		//IA - InputAssmbler for graphics pipeline (1. stage)
		pDevContext->IASetVertexBuffers(0u, // start slot
			1u, // number of buffers
			pVertexBuffer.GetAddressOf(), // pointer to our vertex buffer
			&stride, // adrss of stride size
			&offest); // pointer to array offset

		//binding pixel shader
		ComPtr<ID3D11PixelShader> pPixelShader;
		ComPtr<ID3DBlob> pBlob;
		hres = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), // pointer to buffer
			pBlob->GetBufferSize(), // buffer size
			NULL,
			&pPixelShader); // pointer to be filled
		pDevContext->PSSetShader(pPixelShader.Get(), NULL, 0); // setting pixel shader

		//binding vertex shader
		ComPtr<ID3D11VertexShader> pVertexShader;
		hres = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), // pointer to buffer
									pBlob->GetBufferSize(), // buffer size
									NULL,
									&pVertexShader); // pointer to be filled
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);
		pDevContext->VSSetShader(pVertexShader.Get(), NULL, 0);

		// How should be Vertex struct represented ? -> mapping of it secured by InputLayout
		ComPtr<ID3D11InputLayout> pInputLayout;

		const D3D11_INPUT_ELEMENT_DESC arrayOfDescriptors[] =
		{
			{"SV_POSITION", // semantic name (in hlsl file)
			0, // index of semantic (not using)
			DXGI_FORMAT_R32G32_FLOAT, // format of the data -> float2 Vertex(x,y)
			0, // binding to slot 0 
			0, // offset
			D3D11_INPUT_PER_VERTEX_DATA, // default
			0 },

			{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};
		//Creating input layout (COM approach)
		hres = pDevice->CreateInputLayout(arrayOfDescriptors, // pointer to descriptor
			(UINT)std::size(arrayOfDescriptors), // number of elements
			pBlob->GetBufferPointer(), //
			pBlob->GetBufferSize(),
			&pInputLayout);
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);
		pDevContext->IASetInputLayout(pInputLayout.Get());


		//binding of render target 
		pDevContext->OMSetRenderTargets(1u, // we rae binding 1 view
			pRenderTargetView.GetAddressOf(), // without releasing buffer 
			nullptr);

		// Setting primitive topology (how are pointes interpreted ?) - Input assembler
		pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//viewport mapping
		D3D11_VIEWPORT viewport;
		viewport.Width = 500;
		viewport.Height = 500;
		viewport.MinDepth = 0; // Depth of the view
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0; // Coordinate of viewport in render target
		viewport.TopLeftY = 0;
		pDevContext->RSSetViewports(1u, //number of viewports
									&viewport);

		pDevContext->DrawIndexed((UINT)std::size(indices),0u, 0u);
	}

private:
	ComPtr<ID3D11Device> pDevice; // Pointer to graphics device
	ComPtr<IDXGISwapChain> pSwapChain; // Pointer na Swap chain
	ComPtr<ID3D11DeviceContext> pDevContext; // Pointer to device context
	ComPtr<ID3D11RenderTargetView> pRenderTargetView; // Pointer to render of view
};

#endif //!GRAPHICS_H_
