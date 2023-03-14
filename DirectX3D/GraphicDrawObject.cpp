#include "GraphicDrawObject.h"

/// <summary>
/// Function responsible for staging all resources to graphics pipeline and displaying objects
/// </summary>
/// <param name="graphics">Graphics object for given window</param>
void GraphicsDrawObject::Display(Graphics& graphics) const
{
	for (auto it = graphicsStagedObjs_.begin(); it != graphicsStagedObjs_.end(); ++it)
	{
		(*it)->StageToPipeline(graphics); // abstraction staging
	}
	graphics.DisplayUsingIndexBuffer(pIndexBuffer_->GetIndicesCount()); // drawing objects
}

/// <summary>
/// Adding staging object to the vector
/// </summary>
/// <param name="gsa">GraphicStageAbstraction object</param>
void GraphicsDrawObject::InsertGraphicsStageObj(unique_ptr<GraphicStageAbstraction> gsa)
{
	if (typeid(*gsa) == typeid(IndexBuffer)) // we are not adding Index buffer here
		throw GraphicsDrawException(__LINE__, __FILE__);
	graphicsStagedObjs_.push_back(move(gsa));
};

void GraphicsDrawObject::InitindexBuffer(unique_ptr<class IndexBuffer> indexBuffer)
{
	if(pIndexBuffer_ != nullptr) // we are not rewriting index buffer if it is already initialized
		throw GraphicsDrawException(__LINE__, __FILE__);

	pIndexBuffer_ = indexBuffer.get(); // getting pointer for index buffer
	graphicsStagedObjs_.push_back(move(indexBuffer)); // adding index buffer to graphics staging pipeline
};
