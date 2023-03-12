#ifndef PRIMITIVE_TOPOLOGY_H
#define PRIMITIVE_TOPOLOGY_H

#include "GraphicsAbstraction.h"

/// <summary>
/// Class wrapping primitive topology
/// </summary>
class PrimitiveTopology : public GraphicStageAbstraction
{
public:
	inline PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept
		: topology_(topology) {};
	void StageToPipeline(Graphics& graphics) override;
private:
	D3D11_PRIMITIVE_TOPOLOGY topology_;
};
#endif // !PRIMITIVE_TOPOLOGY_H

