#pragma once
#include "Type.h"
class GameTimer
{
public:
	GameTimer();
	float TotalTime() const; 
	float DeltaTime()const; 
	void Reset(); 
	void Start(); 
	void Stop();  
	void Tick(); 

private:
	double mSecondsPerCount;
	double mDeltaTime;
	int64 mBaseTime;
	int64 mPausedTime;
	int64 mStopTime;
	int64 mPrevTime;
	int64 mCurrTime;
	bool mStopped;
};

