#ifndef GRAPHICS_ABSTRACTION_H
#define GRAPHICS_ABSTRACTION_H

#include "Graphics.h"

#include <vector>

using namespace std;

/// <summary>
/// Abstraction class for every resource that can be bound to Graphics pipeline
/// </summary>
class GraphicStageAbstraction
{
public:
	virtual void StageToPipeline(Graphics& graphics) = 0;
	virtual ~GraphicStageAbstraction() = default;
protected:
	//This abstract class is friend of Graphics and we can thus get private members of Graphics
	inline static ID3D11Device* GetDevice(Graphics& graphics) { return graphics.pDevice.Get(); };
	inline static ID3D11DeviceContext* GetDeviceContext(Graphics& graphics) { return graphics.pDevContext.Get(); };
};

#endif // !GRAPHICS_ABSTRACTION_H

