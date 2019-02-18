#pragma once
#include"CameraType.h"
namespace oocd
{
	class Camera
	{
	public:

		float FieldOfView;
		void SetFieldOfView(float InFieldOfView) { FieldOfView = InFieldOfView; }
		float OrthoWidth;
		void SetOrthoWidth(float InOrthoWidth) { OrthoWidth = InOrthoWidth; }
		float OrthoNearClipPlane;
		void SetOrthoNearClipPlane(float InOrthoNearClipPlane) { OrthoNearClipPlane = InOrthoNearClipPlane; }
		float OrthoFarClipPlane;
		void SetOrthoFarClipPlane(float InOrthoFarClipPlane) { OrthoFarClipPlane = InOrthoFarClipPlane; }
		float AspectRatio;
		void SetAspectRatio(float InAspectRatio) { AspectRatio = InAspectRatio; }
	public:
		//如果bConstrainAspectRatio为true，如果目标视图的宽高比与此摄像机请求的宽高比不同，则会添加黑条。
		uint32 bConstrainAspectRatio : 1;
		void SetConstraintAspectRatio(bool bInConstrainAspectRatio) { bConstrainAspectRatio = bInConstrainAspectRatio; }
		//如果为true，则在计算用于mesh的细节级别时考虑视角的范围。
		uint32 bUseFieldOfViewForLOD : 1;
		void SetUseFieldOfViewForLOD(bool bInUseFieldOfViewForLOD) { bUseFieldOfViewForLOD = bInUseFieldOfViewForLOD; }
		CameraProjectionMode::Type ProjectionMode;
		void SetProjectionMode(CameraProjectionMode::Type InProjectionMode) { ProjectionMode = InProjectionMode; }

		Camera();
		~Camera();

	public:
		virtual bool GetEditorPreviewInfo(float DeltaTime, CameraViewInfo& ViewOut) ;

	};


}
