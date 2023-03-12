#include "PrimitiveTopology.h"

/// <summary>
/// This method serves to set primitive topology to graphics pipeline
/// </summary>
/// <param name="graphics">Graphics for topology</param>
void PrimitiveTopology::StageToPipeline(Graphics& graphics)
{
	// Setting primitive topology (how are pointes interpreted ?) - Input assembler
	GetDeviceContext(graphics)->IASetPrimitiveTopology(topology_);
};
