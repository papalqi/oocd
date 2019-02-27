#include "d3dx12.h"
#include <DirectXMath.h>
class Camera
{
public:

	Camera();
	~Camera();

	// Get/Set world camera position.
	DirectX::XMVECTOR GetPosition()const;
	DirectX::XMFLOAT3 GetPosition3f()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRight()const;
	DirectX::XMFLOAT3 GetRight3f()const;
	DirectX::XMVECTOR GetUp()const;
	DirectX::XMFLOAT3 GetUp3f()const;
	DirectX::XMVECTOR GetLook()const;
	DirectX::XMFLOAT3 GetLook3f()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// Get View/Proj matrices.
	DirectX::XMMATRIX GetView()const;
	DirectX::XMMATRIX GetProj()const;

	DirectX::XMFLOAT4X4 GetView4x4f()const;
	DirectX::XMFLOAT4X4 GetProj4x4f()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

private:

	// Camera coordinate system with coordinates relative to world space.
	DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 mView = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4X4 mProj = DirectX::XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
};




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
		float FarZ)
	{

		float    SinFov;
		float    CosFov;
		XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

		float Height = CosFov / SinFov;
		float Width = Height / AspectRatio;
		float fRange = FarZ / (FarZ - NearZ);

		mProj.M[0][0] = Width;
		mProj.M[0][1] = 0.0f;
		mProj.M[0][2] = 0.0f;
		mProj.M[0][3] = 0.0f;

		mProj.M[1][0] = 0.0f;
		mProj.M[1][1] = Height;
		mProj.M[1][2] = 0.0f;
		mProj.M[1][3] = 0.0f;

		mProj.M[2][0] = 0.0f;
		mProj.M[2][1] = 0.0f;
		mProj.M[2][2] = fRange;
		mProj.M[2][3] = 1.0f;

		mProj.M[3][0] = 0.0f;
		mProj.M[3][1] = 0.0f;
		mProj.M[3][2] = -fRange * NearZ;
		mProj.M[3][3] = 0.0f;

	}
	void SetLens(float fovY, float aspect, float zn, float zf)

	{
		mFovY = fovY;
		mAspect = aspect;
		mNearZ = zn;
		mFarZ = zf;
		mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f*mFovY);
		mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f*mFovY);

		SetPerspectiveMatrix(mFovY, mAspect, mNearZ, mFarZ);
	
	};

	void LookAt(const Vector& pos, const Vector& target, const Vector& up)
	{
		mLook  = (target-pos).GetSafeNormal();
		mRight = (up^mLook).GetSafeNormal();
		mUp = mLook ^ mRight;
		mPosition=pos;
		mViewDirty = true;
	}

	// Get View/Proj matrices.
	Matrix GetView()const { return mView; };
	Matrix GetProj()const { return mProj; }
	Matrix GetView4x4f()const { return mView; };

	// Strafe/Walk the camera a distance d.
	void Strafe(float d)
	{

		mPosition = d * mRight + mPosition;
		mViewDirty = true;
	}
	void Walk(float d)
	{
	
		mPosition = d * mLook + mPosition;

		mViewDirty = true;
	}

	// Rotate the camera.
	void Pitch(float angle)
	{
		RotationMatrix R(Rotator(mRight.X+angle, mRight.Y + angle, mRight.Z + angle));
		mUp= R.TransformPosition(mUp);
		mLook= R.TransformPosition(mLook);

		mViewDirty = true;
	}
	void RotateY(float angle)
	{
		RotationMatrix Res{ Rotator(angle) };

		mRight= Res.TransformPosition(mRight);
		mUp= Res.TransformPosition(mUp);
		mLook= Res.TransformPosition(mLook);

		mViewDirty = true;
	}

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix()
	{
		if (mViewDirty)
		{
		
			mLook.Normalize();
			mUp = (mLook ^ mRight).GetSafeNormal();
			mRight = mUp ^ mLook;


			float x = -((mPosition | mRight));
			float y = -((mPosition | mUp));
			float z = -((mPosition | mLook));

		

			mView.M[0][ 0] = mRight.X;
			mView.M[1][ 0] = mRight.Y;
			mView.M[2][ 0] = mRight.Z;
			mView.M[3][ 0] = x;

			mView.M[0][ 1] = mUp.X;
			mView.M[1][ 1] = mUp.Y;
			mView.M[2][ 1] = mUp.Z;
			mView.M[3][ 1] = y;

			mView.M[0][ 2] = mLook.X;
			mView.M[1][ 2] = mLook.Y;
			mView.M[2][ 2] = mLook.Z;
			mView.M[3][ 2] = z;

			mView.M[0][ 3] = 0.0f;
			mView.M[1][ 3] = 0.0f;
			mView.M[2][ 3] = 0.0f;
			mView.M[3][ 3] = 1.0f;

			mViewDirty = false;
		}
	}
private:

	// Camera coordinate system with coordinates relative to world space.
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

