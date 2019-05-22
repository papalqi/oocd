#include "DrawLineControl.h"



void DrawLineControl::TransformScreenPosition(int x, int y,Matrix MVP)
{
	Vector P(x, y, 100);
	auto invers=MVP.Inverse();
	invers=invers.GetTransposed();
	Vector InWordPositon= MVP.TransformPosition(P);
	Vertex sip;
	sip.Pos = InWordPositon;
	Plist.push_back(sip);
}

