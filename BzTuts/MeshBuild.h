#pragma once
class  MeshBuilder
{
public:
	MeshBuilder() {};

	/**
	 */
	virtual bool Build(class StaticMeshRenderData& OutRenderData, class StaticMesh* StaticMesh, const class StaticMeshLODGroup& LODGroup) = 0;

private:

};
