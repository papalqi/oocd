#pragma once
#include "OCMath.h"
#include <vector>
#include<memory>
using namespace std;
struct OCMesh
{
	vector<DWORD>iList;
	vector<Vertex>vList;
	int vBufferSize;
	int iBufferSize;
	DWORD*BeginiList;
	Vertex*BeginVList;
	static OCMesh CreateTestMesh();
	DWORD *GetIndex();
	Vertex* GetVertex();
	
};




