#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include "Includer.h"
#include "Exceptions.h"

#include <wrl.h> // We will use ComPtr -> release object automaticly (e.g. in case of exception)
#include <d3d11.h>
#include <d3dcompiler.h> // used for reading byte files from hlsl tranformation
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Graphics
{
	friend class GraphicStageAbstraction;
public:
	inline Graphics(HWND hwnd, int width, int height) { Initialize(hwnd, width, height); };
	Graphics(const Graphics&) = delete; 
	Graphics& operator = (const Graphics&) = delete;
	~Graphics() = default; // releasing is handled by ComPtr
	bool InitDepthBuffer(HRESULT& result);
	bool InitSwapChainAndDevice(HRESULT& result, UINT width, UINT height, HWND handle);
	bool InitRenderTargetView(HRESULT& result);
	bool InitDepthStencilTexture(HRESULT& result, int width, int height);
	void InitPrimitiveTopologyAndViewport(int width, int height);
	bool InitInputLayout(HRESULT& result, ComPtr<ID3D10Blob>& pBlob);
	bool InitVertexShader(HRESULT& result, ComPtr<ID3D10Blob>& pBlob);
	bool InitPixelShader(HRESULT& result, ComPtr<ID3D10Blob>& pBlob);
	bool Initialize(HWND hwnd, int width, int height);
	void FlipFrame();
	void ColorBuffer(float red, float green, float blue);

	void DrawTriangle(float angle, float x, float z)
	{
		HRESULT hres;
		// creating vertex buffer
		struct Vertex 
		{
			float x;
			float y;
			float z;
		};

		const Vertex vertices[] =
		{
			{ -1.0f,-1.0f,-1.0f},
			{ 1.0f,-1.0f,-1.0f},
			{ -1.0f,1.0f,-1.0f},
			{ 1.0f,1.0f,-1.0f},
			{ -1.0f,-1.0f,1.0f},
			{ 1.0f,-1.0f,1.0f},
			{ -1.0f,1.0f,1.0f},
			{ 1.0f,1.0f,1.0f},
		};

		//indeces for vertexes
		const unsigned short indices[] =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4 
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
		hres = pDevice->CreateBuffer(&descriptorInd, &dataInd, &pIndexBuffer);
		pDevContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u); // setting index buffer


		ComPtr<ID3D11Buffer> pConstBuffer;
		// Define the constant data used to communicate with shaders.
		struct ConstBuffer
		{
			XMMATRIX transformMatrix; //SIMD operations
		};

		const ConstBuffer constBuffer =
		{
			XMMatrixTranspose(XMMatrixRotationZ(angle) * XMMatrixRotationX(angle) * XMMatrixTranslation(x ,0.0f, z + 4.0f)
			* XMMatrixPerspectiveLH( 1.0f, 1.0f, 0.5f, 10.0f ))
		};

		// Fill in a buffer description.
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(constBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;
		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &constBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		// Create the buffer.
		hres = pDevice->CreateBuffer(&cbDesc, &InitData,
			&pConstBuffer);
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);
		//binding of constant buffer
		pDevContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());


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
		hres = pDevice->CreateBuffer(&descriptor, &data, &pVertexBuffer);
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

		struct RGBAColorStruc
		{
			float red;
			float green;
			float blue;
			float alpha;
		};

		struct CubeFaceCollors
		{
			RGBAColorStruc cube_faces[6];
		};

		const CubeFaceCollors constBufferCube
		{
			{
			    { 1.0f, 0.0f, 0.0f},
				{ 0.0f, 1.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f},
				{ 1.0f, 1.0f, 0.0f},
				{ 0.0f, 1.0f, 1.0f},
				{ 1.0f, 0.0f, 1.0f},
			}
		};
		
		ComPtr<ID3D11Buffer> pConstBuffer2;
		D3D11_BUFFER_DESC descriptorConst2 = {}; // descriptor for buffer
		descriptorConst2.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // type of the buffer 
		descriptorConst2.Usage = D3D11_USAGE_DEFAULT; // buffer used by GPU only
		descriptorConst2.CPUAccessFlags = 0u;
		descriptorConst2.MiscFlags = 0u;
		descriptorConst2.ByteWidth = sizeof(constBufferCube); // allocating right amount of space for buffer
		descriptorConst2.StructureByteStride = 0u; // size of one vertices eleemnt
		D3D11_SUBRESOURCE_DATA data2 = {};
		data2.pSysMem = &constBufferCube; // setting pointer to our data
		data2.SysMemPitch = 0;
		data2.SysMemSlicePitch = 0;
		//filling vertex buffer (COM approach)
		hres = pDevice->CreateBuffer(&descriptorConst2, &data2, &pConstBuffer2);
		if (FAILED(hres))
			throw GraphicsException(__LINE__, __FILE__, hres);
		pDevContext->PSSetConstantBuffers(0u, 1u, pConstBuffer2.GetAddressOf());

		ComPtr<ID3DBlob> pBlob;
		//binding pixel shader
		if(!InitPixelShader(hres,pBlob))
			throw GraphicsException(__LINE__, __FILE__, hres);

		//binding vertex shader
		if (!InitVertexShader(hres, pBlob))
			throw GraphicsException(__LINE__, __FILE__, hres);	

		//binding/creating/initializing input layout
		if(!InitInputLayout(hres, pBlob))
			throw GraphicsException(__LINE__, __FILE__, hres);

		InitPrimitiveTopologyAndViewport(500, 500);

		pDevContext->DrawIndexed((UINT)std::size(indices),0u, 0u);
	}

private:
	ComPtr<ID3D11Device> pDevice; // Pointer to graphics device
	ComPtr<IDXGISwapChain> pSwapChain; // Pointer na Swap chain
	ComPtr<ID3D11DeviceContext> pDevContext; // Pointer to device context
	ComPtr<ID3D11RenderTargetView> pRenderTargetView; // Pointer to render of view
	ComPtr<ID3D11VertexShader> pVertexShader; // Pointer to our vertex shader
	ComPtr<ID3D11PixelShader> pPixelShader; // Pointer to out pixel shader
	ComPtr<ID3D11InputLayout> pInputLayout; // How should be Vertex struct represented ? -> mapping of it secured by InputLayout
	ComPtr<ID3D11DepthStencilView> pDepthStencilView; // pointer to depth view
};

#endif //!GRAPHICS_H_
