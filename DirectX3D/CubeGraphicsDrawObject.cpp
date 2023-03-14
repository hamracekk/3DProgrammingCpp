#include "CubeGraphicsDrawObject.h"

/// <summary>
/// Initialization of positional arguments of cube
/// </summary>
/// <param name="graphics"></param>
/// <param name="range">distribution range</param>
/// <param name="radiusUiformDistrib">random uniform distribution</param>
/// <param name="deltaUniformDistrib">random uniform distribution</param>
/// <param name="originAngleuniformDistrib">random uniform distribution</param>
/// <param name="originAngleDeltaUniformDistrib">random uniform distribution</param>
CubeGraphicsObject::CubeGraphicsObject(Graphics& graphics, mt19937 range,
	uniform_real_distribution<float>& radiusUiformDistrib,
	uniform_real_distribution<float>& deltaUniformDistrib,
	uniform_real_distribution<float>& originAngleuniformDistrib,
	uniform_real_distribution<float>& originAngleDeltaUniformDistrib) :
	radius(radiusUiformDistrib(range)), deltaPitch(deltaUniformDistrib(range)),
	deltaRoll(deltaUniformDistrib(range)), deltaYaw(deltaUniformDistrib(range)),
	deltaXAngle(originAngleDeltaUniformDistrib(range)), deltaYAngle(originAngleDeltaUniformDistrib(range)),
	deltaZAngle(originAngleDeltaUniformDistrib(range)), xAngle(originAngleuniformDistrib(range)),
	yAngle(originAngleuniformDistrib(range)), zAngle(originAngleuniformDistrib(range))
{
	struct Vertex
	{
		float x;
		float y;
		float z;
	};

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

	const vector<Vertex> vertices =
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

	//indeces for vertexes clockvise orientation of triangles
	const vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
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

	// descriptor for inpiut layout
	const vector<D3D11_INPUT_ELEMENT_DESC> arrayOfDescriptors =
	{
		{"POSITION", // semantic name (in hlsl file)
		0, // index of semantic (not using)
		DXGI_FORMAT_R32G32B32_FLOAT, // format of the data -> float2 Vertex(x,y)
		0, // binding to slot 0 
		0, // offset
		D3D11_INPUT_PER_VERTEX_DATA, // default
		0 },
	};

	InsertGraphicsStageObj(make_unique<VertexBuffer>(graphics, vertices)); // adding VertexBuffer to graphics staging process

	unique_ptr<VertexShader> pVertexShder = make_unique<VertexShader>(graphics);
	auto vertexBytcode = pVertexShder->GetBytCode();

	InsertGraphicsStageObj(move(pVertexShder)); // inserting Vertex shader to pipeline staging phase

	InsertGraphicsStageObj(make_unique<PixelShader>(graphics)); // inserting Pixel shader to pipeline staging phase

	InsertGraphicsStageObj(make_unique<PixelConstantBuffer<CubeFaceCollors>>(graphics, constBufferCube)); // inserting Pixel buffer with face cube collors to pipeline staging phase

	InsertGraphicsStageObj(make_unique<InputLayout>(graphics, arrayOfDescriptors, vertexBytcode));

	InsertGraphicsStageObj(make_unique<PrimitiveTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)); // represent vertex as list of vertices for triangles (clockvise)

	// transform Cbuff
};

/// <summary>
/// updating coordinates of cube in space
/// </summary>
/// <param name="timeDiff">time difference between updates</param>
void CubeGraphicsObject::Update(float timeDiff)
{
	roll += deltaRoll * timeDiff;
	pitch += deltaPitch * timeDiff;
	yaw += deltaYaw * timeDiff;
	xAngle += deltaXAngle * timeDiff;
	yAngle += deltaYAngle * timeDiff;
	zAngle += deltaZAngle * timeDiff;
};

/// <summary>
/// Getting transformation matrix for cube
/// </summary>
/// <returns>matrix of transformation</returns>
XMMATRIX CubeGraphicsObject::TransformMatrix() const
{
	XMMATRIX matrix = XMMatrixRotationRollPitchYaw(roll, pitch, yaw)
		* XMMatrixTranslation(radius, 0.0f, 0.0f)
		* XMMatrixRotationRollPitchYaw(xAngle, yAngle, zAngle)
		* XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	return matrix;
}
