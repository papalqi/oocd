#pragma once
#include"../GCMath/Math.h"
#include"EngineDefine.h"
namespace CameraProjectionMode
{
	enum Type
	{
		Perspective,
		Orthographic
	};
}

namespace CameraAnimPlaySpace
{
	enum Type
	{
		CameraLocal,
		World,
		UserDefined,
	};
}

struct CameraViewInfo
{
	Vector Location;
	Rotator Rotation;
	//只在透视投影中起作用
	float FOV;
	//原始的FOV
	float DesiredFOV;
	//正交投影使用
	float OrthoWidth;
	//正交投影的近面
	float OrthoNearClipPlane;
	//正交投影的远面
	float OrthoFarClipPlane;
	float AspectRatio;
	CameraProjectionMode::Type ProjectionMode;
	CameraViewInfo()
		: Location(0, 0, 0)
		, Rotation(0, 0, 0)
		, FOV(90.0f)
		, DesiredFOV(90.0f)
		, OrthoWidth(512.0f)
		, OrthoNearClipPlane(0.0f)
		, OrthoFarClipPlane(WORLD_MAX)
		, AspectRatio(1.33333333f)
		, ProjectionMode(CameraProjectionMode::Perspective)

	{
	}
};
