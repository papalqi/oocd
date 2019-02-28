#include "d3dx12.h"
#include"CameraType.h"
#include"OCMath.h"
using namespace oocd;
class NCamera
{
public:
	CameraProjectionMode::Type ProjectMode;

	NCamera() 
	{
	
		SetLens(0.25f*PI, 1.0f, 1.0f, 1000.0f);
	};
	~NCamera() {};
	
	Vector GetPosition()const { return mPosition; }
	void SetPosition(float x, float y, float z) { mPosition = { x,y,z }; }
	void SetPosition(const Vector& v) { mPosition = v; }

	Vector GetRight()const { return mRight; }
	Vector GetUp()const { return mUp; }
	Vector GetLook()const { return mLook; }

	float GetNearZ()const { return mNearZ; };
	float GetFarZ()const { return mFarZ; };
	float GetAspect()const { return mAspect; };
	float GetFovY()const { return mFovY; };
	float GetFovX()const {return mFovY;};

	
	float GetNearWindowWidth()const { return mAspect * mNearWindowHeight; }
	float GetNearWindowHeight()const { return mNearWindowHeight; };
	float GetFarWindowWidth()const { return mAspect * mFarWindowHeight; }
	float GetFarWindowHeight()const { return mFarWindowHeight; }
	void SetPerspectiveMatrix(float FovAngleY,
		float AspectRatio,
		float NearZ,
		float FarZ);
	void SetLens(float fovY, float aspect, float zn, float zf);;

	void LookAt(const Vector& pos, const Vector& target, const Vector& up);

	Matrix GetView()const { return mView; };
	Matrix GetProj()const { return mProj; }
	Matrix GetView4x4f()const { return mView; };

	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

	void UpdateViewMatrix();
private:

	Vector mPosition = { 0.0f, 0.0f, 0.0f };
	Vector mRight = { 1.0f, 0.0f, 0.0f };
	Vector mUp = { 0.0f, 1.0f, 0.0f };
	Vector mLook = { 0.0f, 0.0f, 1.0f };

	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	Matrix mView = Matrix::Identity;
	Matrix mProj = Matrix::Identity;
};

