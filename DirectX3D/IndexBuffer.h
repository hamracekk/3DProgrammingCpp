#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "GraphicsAbstraction.h"

/// <summary>
/// Class representing construction/creation of index buffer
/// </summary>
class IndexBuffer : public GraphicStageAbstraction
{
public:
	IndexBuffer(Graphics& graphics, vector<unsigned short>& indices);
	void StageToPipeline(Graphics& graphics) override;
	inline UINT GetIndicesCount() const noexcept { return indices_count_; }
private:
	UINT indices_count_; // size of the vector
	ComPtr<ID3D11Buffer> pIndexBuffer; // pointer to index buffer
};

#endif // !INDEX_BUFFER_H

