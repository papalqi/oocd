#pragma once
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.



#include "CoreMinimal.h"








struct FRawMesh
{
	//���ʵ�����
	vector<int32> FaceMaterialIndices;
	//ƽ��mask
	vector<uint32> FaceSmoothingMasks;

	//�ֲ�����������
	vector<Vector> VertexPositions;

	/** ���� */
	vector<uint32> WedgeIndices;
	/** uv x */
	vector<Vector>	WedgeTangentX;
	/** uv y */
	vector<Vector>	WedgeTangentY;
	//����
	vector<Vector>	WedgeTangentZ;
	//��������
	vector<Vector2D> WedgeTexCoords[8];
	//��ɫ
	vector<Vector> WedgeColors;

	 void Empty();

	 bool IsValid() const;
	//�������������ǿ����޸�����Ч��Ϣ����΢��Ч����Ϣ���򷵻�true
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

	/// **ʹ�ù�ϣ��ΪGUID�������ڷ�ֹ�ڼ��������ʲ�ʱ�����µ�GUID��*/
	 string GetIdString() const;

	 void UseHashAsGuid(class UObject* Owner);

	

	 /**���������û�п��õ��������ݣ��򷵻�true��*/
	 void Empty();

};
