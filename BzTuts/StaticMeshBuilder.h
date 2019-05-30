#pragma once
#include"MeshBuild.h"
class  StaticMeshBuilder : public MeshBuilder
{
public:
	StaticMeshBuilder() {};

	virtual bool Build(class StaticMeshRenderData& OutRenderData, class StaticMesh* StaticMesh, const class StaticMeshLODGroup& LODGroup) override;
	virtual ~StaticMeshBuilder() {}

private:

	void OnBuildRenderMeshStart(class UStaticMesh* StaticMesh, const bool bInvalidateLighting);
	void OnBuildRenderMeshFinish(class UStaticMesh* StaticMesh, const bool bRebuildBoundsAndCollision);

	//TSharedPtr<class FStaticMeshComponentRecreateRenderStateContext> RecreateRenderStateContext;
};
