

#include "Camera.h"




void Camera::SetPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
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

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;
	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f*mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f*mFovY);

	SetPerspectiveMatrix(mFovY, mAspect, mNearZ, mFarZ);
}

void Camera::LookAt(const Vector& pos, const Vector& target, const Vector& up)
{
	mLook = (target - pos).GetSafeNormal();
	mRight = (up^mLook).GetSafeNormal();
	mUp = mLook ^ mRight;
	mPosition = pos;
	mViewDirty = true;
}

void Camera::Strafe(float d)
{
	mPosition = d * mRight + mPosition;
	mViewDirty = true;
}

void Camera::Walk(float d)
{

	mPosition = d * mLook + mPosition;

	mViewDirty = true;
}

void Camera::Pitch(float angle)
{

	if (NowPitch + angle > 89.0f)
		return;
	else if (NowPitch + angle < -89.0f)
		return;
	else	NowPitch += angle;
	Rotator tems(0, 0, angle);
	mUp = tems.RotateVector(mUp);
	mLook = tems.RotateVector(mLook);
	mViewDirty = true;
}

void Camera::RotateY(float angle)
{

	if (NowRotateY + angle > 89.0f)
		return;
	else if (NowRotateY + angle < -89.0f)
		return;
	else	NowRotateY += angle;
	Rotator Res(angle, 0, 0);

	mRight = Res.RotateVector(mRight);
	mUp = Res.RotateVector(mUp);
	mLook = Res.RotateVector(mLook);

	mViewDirty = true;
}

void Camera::UpdateViewMatrix()
{
	if (mViewDirty)
	{

		mLook.Normalize();
		mUp = (mLook ^ mRight).GetSafeNormal();
		mRight = mUp ^ mLook;
		mRight.Normalize();

		float x = -((mPosition | mRight));
		float y = -((mPosition | mUp));
		float z = -((mPosition | mLook));



		mView.M[0][0] = mRight.X;
		mView.M[1][0] = mRight.Y;
		mView.M[2][0] = mRight.Z;
		mView.M[3][0] = x;

		mView.M[0][1] = mUp.X;
		mView.M[1][1] = mUp.Y;
		mView.M[2][1] = mUp.Z;
		mView.M[3][1] = y;

		mView.M[0][2] = mLook.X;
		mView.M[1][2] = mLook.Y;
		mView.M[2][2] = mLook.Z;
		mView.M[3][2] = z;

		mView.M[0][3] = 0.0f;
		mView.M[1][3] = 0.0f;
		mView.M[2][3] = 0.0f;
		mView.M[3][3] = 1.0f;
		//mView=mView.GetTransposed();
		mViewDirty = false;
	}
}
