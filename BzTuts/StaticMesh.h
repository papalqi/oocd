#pragma once
#include<memory>
#include"Vector3.h"
#include "StaticMeshResources.h"
#include"Type.h"
#include<array>
#include<string>
using namespace std;
namespace oocd
{
	class StaticMesh
	{
	public:
		//指向用于渲染的指针
		unique_ptr< StaticMeshRenderData> RenderData;
		//遮挡相关
		unique_ptr< StaticMeshOccluderData> OccluderData;

		int32 MinLOD;
		//todo:增加材质系统

		/** A fence which is used to keep track of the rendering thread releasing the static mesh resources. */
		FRenderCommandFence ReleaseResourcesFence;

		/**
		 * For simplified meshes, this is the fully qualified path and name of the static mesh object we were
		 * originally duplicated from.  This is serialized to disk, but is discarded when cooking for consoles.
		 */
		string HighResSourceMeshName;



		/** For simplified meshes, this is the CRC of the high res mesh we were originally duplicated from. */
		uint32 HighResSourceMeshCRC;

		/** Unique ID for tracking/caching this mesh during distributed lighting */
		FGuid LightingGuid;

		/**
		 *	Array of named socket locations, set up in editor and used as a shortcut instead of specifying
		 *	everything explicitly to AttachComponent in the StaticMeshComponent.
		 */

		array<class StaticMeshSocket*> Sockets;

		/** Data that is only available if this static mesh is an imported SpeedTree */
		TSharedPtr<class FSpeedTreeWind> SpeedTreeWind;

		/** Bound extension values in the positive direction of XYZ, positive value increases bound size */

		Vector PositiveBoundsExtension;
		/** Bound extension values in the negative direction of XYZ, positive value increases bound size */

		Vector NegativeBoundsExtension;
		/** Original mesh bounds extended with Positive/NegativeBoundsExtension */

		FBoxSphereBounds ExtendedBounds;



	protected:
		/**
		 * Index of an element to ignore while gathering streaming texture factors.
		 * This is useful to disregard automatically generated vertex data which breaks texture factor heuristics.
		 */

		int32 ElementToIgnoreForTexFactor;

		/** Array of user data stored with the asset */

		array<UAssetUserData*> AssetUserData;

		/** Tracks whether InitResources has been called, and rendering resources are initialized. */
		bool bRenderingResourcesInitialized;

	public:
		/** The editable mesh representation of this static mesh */
		// @todo: Maybe we don't want this visible in the details panel in the end; for now, this might aid debugging.

		class UObject* EditableMesh;

		/**
		 * Registers the mesh attributes required by the mesh description for a static mesh.
		 */
		static void RegisterMeshAttributes(FMeshDescription& MeshDescription);


		/** Pre-build navigation collision */

		UNavCollisionBase* NavCollision;
	public:
		/**
		 * Default constructor
		 */
		StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



		virtual void PostInitProperties();
		virtual void PostLoad();
		virtual bool IsPostLoadThreadSafe() const;
		virtual void BeginDestroy();
		virtual bool IsReadyForFinishDestroy();
		virtual void GetAssetRegistryTags(array<FAssetRegistryTag>& OutTags) const;
		virtual string GetDesc();
		virtual void GetResourceSizeEx(FResourceSizeEx& CumulativeResourceSize);
		virtual bool CanBeClusterRoot() const;
		//~ End UObject Interface.

		/**
		 * Rebuilds renderable data for this static mesh.
		 * @param bSilent - If true will not popup a progress dialog.
		 */
		void Build(bool bSilent = false, array<FText>* OutErrors = nullptr);

		/**
		 * Initialize the static mesh's render resources.
		 */
		virtual void InitResources();

		/**
		 * Releases the static mesh's render resources.
		 */
		virtual void ReleaseResources();

		/**
		 * Update missing material UV channel data used for texture streaming.
		 *
		 * @param bRebuildAll		If true, rebuild everything and not only missing data.
		 */
		void UpdateUVChannelData(bool bRebuildAll);

		/**
		 * Returns the material bounding box. Computed from all lod-section using the material index.
		 *
		 * @param MaterialIndex			Material Index to look at
		 * @param TransformMatrix		Matrix to be applied to the position before computing the bounds
		 *
		 * @return false if some parameters are invalid
		 */
		FBox GetMaterialBox(int32 MaterialIndex, const FTransform& Transform) const;

		/**
		 * Returns the UV channel data for a given material index. Used by the texture streamer.
		 * This data applies to all lod-section using the same material.
		 *
		 * @param MaterialIndex		the material index for which to get the data for.
		 * @return the data, or null if none exists.
		 */
		const FMeshUVChannelInfo* GetUVChannelData(int32 MaterialIndex) const;

		/**
		 * Returns the number of vertices for the specified LOD.
		 */
		int32 GetNumVertices(int32 LODIndex) const;

		/**
		 * Returns the number of LODs used by the mesh.
		 */
		int32 GetNumLODs() const;

		/**
		 * Returns true if the mesh has data that can be rendered.
		 */
		bool HasValidRenderData(bool bCheckLODForVerts = true, int32 LODIndex = INDEX_NONE) const;

		/**
		 * Returns the number of bounds of the mesh.
		 *
		 * @return	The bounding box represented as box origin with extents and also a sphere that encapsulates that box
		 */
		FBoxSphereBounds GetBounds() const;

		/** Returns the bounding box, in local space including bounds extension(s), of the StaticMesh asset */
		FBox GetBoundingBox() const;

		/** Returns number of Sections that this StaticMesh has, in the supplied LOD (LOD 0 is the highest) */
		int32 GetNumSections(int32 InLOD) const;

		/**
		 * Gets a Material given a Material Index and an LOD number
		 *
		 * @return Requested material
		 */
		UMaterialInterface* GetMaterial(int32 MaterialIndex) const;

		/**
		* Gets a Material index given a slot name
		*
		* @return Requested material
		*/

		int32 GetMaterialIndex(FName MaterialSlotName) const;

		int32 GetMaterialIndexFromImportedMaterialSlotName(FName ImportedMaterialSlotName) const;

		/**
		 * Returns the render data to use for exporting the specified LOD. This method should always
		 * be called when exporting a static mesh.
		 */
		const FStaticMeshLODResources& GetLODForExport(int32 LODIndex) const;

		/**
		 * Static: Processes the specified static mesh for light map UV problems
		 *
		 * @param	InStaticMesh					Static mesh to process
		 * @param	InOutAssetsWithMissingUVSets	Array of assets that we found with missing UV sets
		 * @param	InOutAssetsWithBadUVSets		Array of assets that we found with bad UV sets
		 * @param	InOutAssetsWithValidUVSets		Array of assets that we found with valid UV sets
		 * @param	bInVerbose						If true, log the items as they are found
		 */
		static void CheckLightMapUVs(StaticMesh* InStaticMesh, array< string >& InOutAssetsWithMissingUVSets, array< string >& InOutAssetsWithBadUVSets, array< string >& InOutAssetsWithValidUVSets, bool bInVerbose = true);

		//~ Begin Interface_CollisionDataProvider Interface
		virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData);
		virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const;
		virtual bool WantsNegXTriMesh()
		{
			return true;
		}
		virtual void GetMeshId(string& OutMeshId);
		//~ End Interface_CollisionDataProvider Interface

		/** Return the number of sections of the StaticMesh with collision enabled */
		int32 GetNumSectionsWithCollision() const;



		/**
		 * Create BodySetup for this staticmesh if it doesn't have one
		 */
		void CreateBodySetup();

		/**
		 * Calculates navigation collision for caching
		 */
		void CreateNavCollision(const bool bIsUpdate = false);



		/** Configures this SM as bHasNavigationData = false and clears stored NavCollision */
		void MarkAsNotHavingNavigationData();


		/**
		 *	Find a socket object in this StaticMesh by name.
		 *	Entering NAME_None will return NULL. If there are multiple sockets with the same name, will return the first one.
		 */
		class StaticMeshSocket* FindSocket(FName InSocketName);
		/**
		 * Returns vertex color data by position.
		 * For matching to reimported meshes that may have changed or copying vertex paint data from mesh to mesh.
		 *
		 *	@param	VertexColorData		(out)A map of vertex position data and its color. The method fills this map.
		 */
		void GetVertexColorData(TMap<Vector, FColor>& VertexColorData);

		/**
		 * Sets vertex color data by position.
		 * Map of vertex color data by position is matched to the vertex position in the mesh
		 * and nearest matching vertex color is used.
		 *
		 *	@param	VertexColorData		A map of vertex position data and color.
		 */
		void SetVertexColorData(const TMap<Vector, FColor>& VertexColorData);

		/** Removes all vertex colors from this mesh and rebuilds it (Editor only */
		void RemoveVertexColors();

		/** Make sure the Lightmap UV point on a valid UVChannel */
		void EnforceLightmapRestrictions();

		/** Calculates the extended bounds */
		void CalculateExtendedBounds();

		inline bool AreRenderingResourcesInitialized() const { return bRenderingResourcesInitialized; }



		/**
		 * Sets a Material given a Material Index
		 * @note Only available in editor.
		 */

		void SetMaterial(int32 MaterialIndex, UMaterialInterface* NewMaterial);

		/**
		 * Returns true if LODs of this static mesh may share texture lightmaps.
		 */
		bool CanLODsShareStaticLighting() const;

		/**
		 * Retrieves the names of all LOD groups.
		 */
		static void GetLODGroups(array<FName>& OutLODGroups);

		/**
		 * Retrieves the localized display names of all LOD groups.
		 */
		static void GetLODGroupsDisplayNames(array<FText>& OutLODGroupsDisplayNames);

		void GenerateLodsInPackage();

		virtual void PostDuplicate(bool bDuplicateForPIE);

		/** Get multicast delegate broadcast prior to mesh building */
		FOnPreMeshBuild& OnPreMeshBuild() { return PreMeshBuild; }

		/** Get multicast delegate broadcast after mesh building */
		FOnPostMeshBuild& OnPostMeshBuild() { return PostMeshBuild; }

	private:
		/**
		 * Converts legacy LODDistance in the source models to Display Factor
		 */
		void ConvertLegacyLODDistance();

		/**
		 * Converts legacy LOD screen area in the source models to resolution-independent screen size
		 */
		void ConvertLegacyLODScreenArea();

		/**
		 * Fixes up static meshes that were imported with sections that had zero triangles.
		 */
		void FixupZeroTriangleSections();

		/**
		* Return mesh data key. The key is the ddc filename for the mesh data
		*/
		bool GetMeshDataKey(string& OutKey);

		/**
		* Caches mesh data.
		*/
		void CacheMeshData();

	public:
		/**
		 * Caches derived renderable data.
		 */
		void CacheDerivedData();

	private:

		FOnPreMeshBuild PreMeshBuild;
		FOnPostMeshBuild PostMeshBuild;

		/**
		 * Fixes up the material when it was converted to the new staticmesh build process
		 */
		bool CleanUpRedondantMaterialPostLoad;

	};
}


