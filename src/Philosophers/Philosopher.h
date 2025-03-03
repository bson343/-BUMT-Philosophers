#pragma once
#include "MutexForkTable.h"

enum class ePhiloState
{
	StartState,
	TimeToEat,
	TimeToSleep,
	Thinking,
	Died,
	Count
};

struct PhiloInfoArgs
{
	const INT32 NumOfPhilo;
	const INT32 TimeToDie;
	const INT32 TimeToEat;
	const INT32 TimeToSleep;
	const INT32 CountToMustEat;
	MutexForkTable* ForkTable;
};

class Philosopher
{
public:
	Philosopher(PhiloInfoArgs* initData, DWORD philoIndex);
	~Philosopher();

	bool Init();

	void Loop();

private:
	void startPhiloStarvationRoutin(DWORD* forkindexList);
	ePhiloState changePhiloState(ePhiloState state);
	inline DWORD getRemaingLife();
	inline bool isDead();

	void setWaitableTime(DWORD ms);
	void startWaitableTimer(DWORD ms);

	const DWORD mPhiloIndex;
	const DWORD mNumOfPhilo;
	const DWORD mTimeToDie;
	const DWORD mTimeToEat;
	const DWORD mTimeToSleep;
	const DWORD mCountToMustEat;

	MutexForkTable* mForkTable;

	LARGE_INTEGER mBaseTimePoint;
	ePhiloState mState;

	HANDLE mHighResTimer;
	LARGE_INTEGER mDueTime, mDueMacroTime;

};

