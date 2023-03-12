#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include "GraphicsAbstraction.h"

/// <summary>
/// Class wrapping creation and setting Constant buffers
/// </summary>
template<typename Type>
class ConstantBuffer : public GraphicStageAbstraction
{
public:
	ConstantBuffer(Graphics& graphics, Type& buffer_entity);
	ConstantBuffer(Graphics& graphics);
	void UpdateBuffer(Graphics& graphics, Type& bufferEntity);
protected:
	ComPtr<ID3D11Buffer> pConstBuffer;
};

template<typename Type>
inline ConstantBuffer<Type>::ConstantBuffer(Graphics& graphics, Type& buffer_entity)
{
	D3D11_BUFFER_DESC descriptor = {}; // descriptor for buffer
	descriptor.Usage = D3D11_USAGE_DYNAMIC;
	descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descriptor.MiscFlags = 0u;
	descriptor.ByteWidth = sizeof(buffer_entity); // allocating right amount of space for buffer
	descriptor.StructureByteStride = 0u; // size of one vertices element

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &buffer_entity;

	//Creating buffer (COM approach)
	HRESULT hres = GetDevice(graphics)->CreateBuffer(&descriptor, &data, &pConstBuffer);
	if (FAILED(hres))
		throw GraphicsException(__LINE__, __FILE__, hres);
}

template<typename Type>
inline ConstantBuffer<Type>::ConstantBuffer(Graphics& graphics)
{
	D3D11_BUFFER_DESC descriptor = {}; // descriptor for buffer
	descriptor.Usage = D3D11_USAGE_DYNAMIC;
	descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descriptor.MiscFlags = 0u;
	descriptor.ByteWidth = sizeof(Type); // allocating right amount of space for buffer
	descriptor.StructureByteStride = 0u; // size of one vertices element

	//Creating buffer (COM approach)
	HRESULT hres = GetDevice(graphics)->CreateBuffer(&descriptor, NULL, &pConstBuffer);
	if (FAILED(hres))
		throw GraphicsException(__LINE__, __FILE__, hres);
};

/// <summary>
/// Constant buffer can be updated every frame
/// </summary>
/// <typeparam name="Type">Type of entity in constant buffer</typeparam>
/// <param name="graphics">Graphics for constant buffer</param>
/// <param name="bufferEntity">Entity in constant buffer</param>
template<typename Type>
inline void ConstantBuffer<Type>::UpdateBuffer(Graphics& graphics, Type& bufferEntity)
{
	// data are copied to subresource
	D3D11_MAPPED_SUBRESOURCE mapedSubresource = {};
	//Map function locks memory for given resource
	HRESULT result = GetDeviceContext(graphics)
		->Map(pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapedSubresource);
	// Writing to the locked memory
	memcpy(mapedSubresource.pData, &bufferEntity, sizeof(bufferEntity));
	// Unlocking the locked memory
	GetDeviceContext(graphics)->Unmap(pConstBuffer.Get(), 0u);
};

// Two possible types of constant buffer
template<typename Type>
class VertexConstantBuffer : public ConstantBuffer<Type>
{
public:
	using ConstantBuffer<Type>::ConstantBuffer; // using constructor of parent class
	void StageToPipeline(Graphics& graphics) override;
};

/// <summary>
/// This method serves to set constant buffer to graphics pipeline for vertex shader
/// </summary>
/// <typeparam name="Type">Type of entity in constant buffer for vertex shader</typeparam>
/// <param name="graphics">Graphics for index buffer</param>
template<typename Type>
inline void VertexConstantBuffer<Type>::StageToPipeline(Graphics& graphics)
{
	//binding of constant buffer for vertex shader
	this->GetDeviceContext(graphics)->VSSetConstantBuffers(0u, 1u, this->pConstBuffer.GetAddressOf());
};

template<typename Type>
class PixelConstantBuffer : public ConstantBuffer<Type>
{
	// We are telling that we want to use parent class memeber (teplated) and parent class function
public:
	using ConstantBuffer<Type>::ConstantBuffer; // using constructor of parent class
	void StageToPipeline(Graphics& graphics) override;
};

/// <summary>
/// This method serves to set constant buffer to graphics pipeline for pixel shader
/// </summary>
/// <typeparam name="Type">Type of entity in constant buffer for pixel shader</typeparam>
/// <param name="graphics">Graphics for index buffer</param>
template<typename Type>
inline void PixelConstantBuffer<Type>::StageToPipeline(Graphics& graphics)
{
	//binding of constant buffer for pixel shader
	this->GetDeviceContext(graphics)->PSSetConstantBuffers(0u, 1u, this->pConstBuffer.GetAddressOf());
}
#endif // !CONSTANT_BUFFER_H

