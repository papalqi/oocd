#pragma once
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.



#include "CoreMinimal.h"








struct FRawMesh
{
	//材质的索引
	vector<int32> FaceMaterialIndices;
	//平滑mask
	vector<uint32> FaceSmoothingMasks;

	//局部顶带你索引
	vector<Vector> VertexPositions;

	/** 索引 */
	vector<uint32> WedgeIndices;
	/** uv x */
	vector<Vector>	WedgeTangentX;
	/** uv y */
	vector<Vector>	WedgeTangentY;
	//法线
	vector<Vector>	WedgeTangentZ;
	//纹理坐标
	vector<Vector2D> WedgeTexCoords[8];
	//颜色
	vector<Vector> WedgeColors;

	 void Empty();

	 bool IsValid() const;
	//如果网格包含我们可以修复的有效信息或稍微无效的信息，则返回true
	 bool IsValidOrFixable() const;

	 Vector GetWedgePosition(int32 WedgeIndex) const
	{
		return VertexPositions[WedgeIndices[WedgeIndex]];
	}


	 void CompactMaterialIndices();

};


class FRawMeshBulkData
{
	Guid MGuid;
	bool bGuidIsHash;

public:
	 FRawMeshBulkData();
	
	 void SaveRawMesh(struct FRawMesh& InMesh);

	
	 void LoadRawMesh(struct FRawMesh& OutMesh);

	/// **使用哈希作为GUID，有助于防止在加载遗留资产时创建新的GUID。*/
	 string GetIdString() const;

	 void UseHashAsGuid(class UObject* Owner);

	

	 /**如果此网格没有可用的批量数据，则返回true。*/
	 void Empty();

};
