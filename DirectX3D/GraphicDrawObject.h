#ifndef GRAPHICS_DRAW_OBJECT_H
#define GRAPHICS_DRAW_OBJECT_H

#include "Graphics.h"
#include "IndexBuffer.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class GraphicStageAbstraction;

/// <summary>
/// Class representing object whioch can be displayed on window
/// </summary>
class GraphicsDrawObject
{
public:
	GraphicsDrawObject() = default;
	GraphicsDrawObject(const GraphicsDrawObject&) = delete;
	void Display(Graphics& graphics) const;
	virtual XMMATRIX TransformMatrix() const = 0;
	virtual void Update(float timeDiff) = 0;
	virtual ~GraphicsDrawObject() = default;
	void InsertGraphicsStageObj(unique_ptr<GraphicStageAbstraction> gsa);
	void InitindexBuffer(unique_ptr<class IndexBuffer> gsa);
private:
	vector<unique_ptr<GraphicStageAbstraction>> graphicsStagedObjs_;
	const IndexBuffer* pIndexBuffer_ = nullptr;
};

#endif // !GRAPHICS_DRAW_OBJECT_H

