#pragma once
#include "CoreMinimal.h"
#include "FrameResource.h"

#include "RenderItem.h"

class DrawLineControl
{
public:
	DrawLineControl() {};
	vector<Vertex> Plist;
	void TransformScreenPosition(int x,int y,Matrix MVP);
	
	bool isHaveRenderItem=false;

	string LineName="LineNamea";
	RenderItem *RenderLinePlane;
};

