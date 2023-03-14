#ifndef CUBE_GRAPHICS_DRAW_OBJECT_H
#define CUBE_GRAPHICS_DRAW_OBJECT_H

#include "GraphicDrawObject.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PrimitiveTopology.h"

#include <random>

/// <summary>
/// Cube is one of graphics object which can be displayed
/// </summary>
class CubeGraphicsObject : public GraphicsDrawObject
{
public:
	CubeGraphicsObject(Graphics& graphics, mt19937 range,
		uniform_real_distribution<float>& radiusUiformDistrib,
		uniform_real_distribution<float>& deltaUniformDistrib,
		uniform_real_distribution<float>& originAngleuniformDistrib,
		uniform_real_distribution<float>& originAngleDeltaUniformDistrib);

	void Update(float timeDiff) override;
	XMMATRIX TransformMatrix() const override;
private:
	float radius; // distance from origin of space
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float zAngle; 
	float yAngle;
	float xAngle;
	// How should positional arguments change in time
	float deltaRoll;
	float deltaPitch;
	float deltaYaw;
	float deltaXAngle;
	float deltaYAngle;
	float deltaZAngle;
};
#endif // !CUBE_GRAPHICS_DRAW_OBJECT_H

