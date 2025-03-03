#include "pch.h"
#include "Philosopher.h"

constexpr DWORD MAX_COUNT_TO_MUST_EAT = 4294967295;

static inline LONGLONG setTimeMileSecond(int ms)
{
	return -1LL * (ms) * 10000;
}

Philosopher::Philosopher(PhiloInfoArgs* initData, DWORD philoIndex)
	:
	mPhiloIndex(philoIndex),
	mNumOfPhilo(initData->NumOfPhilo),
	mTimeToDie(initData->TimeToDie),
	mTimeToEat(initData->TimeToEat),
	mTimeToSleep(initData->TimeToSleep),
	mCountToMustEat((initData->CountToMustEat == 0) ? MAX_COUNT_TO_MUST_EAT : initData->CountToMustEat),
	mForkTable(initData->ForkTable),
	mState(ePhiloState::StartState)
{
	
}

Philosopher::~Philosopher()
{
	//delete wait timer
	{
		if (!CloseHandle(mHighResTimer))
		{
			_CrtDbgBreak();
		}
	}
}

bool Philosopher::Init()
{
	// initialize wait timer
	{
		// 100ns 단위
		mDueMacroTime.QuadPart = -500 * 10; // 500마이크로초 설정

		mHighResTimer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
		if (NULL == mHighResTimer)
		{
			printf("CreateWaitableTimer failed (%d)\n", GetLastError());
			return FALSE;
		}
	}

	// initialize measure time
	{
		mBaseTimePoint = QCGetCounter();
	}
	return TRUE;
}

void Philosopher::Loop()
{
	DWORD forkIndexList[2];
	float philoCurrentLife;

	if (mPhiloIndex % 2 == 0)
	{
		forkIndexList[0] = mPhiloIndex % mNumOfPhilo;
		forkIndexList[1] = (mPhiloIndex + 1) % mNumOfPhilo;
	}
	else
	{
		forkIndexList[0] = (mPhiloIndex + 1) % mNumOfPhilo;
		forkIndexList[1] = mPhiloIndex % mNumOfPhilo;
	}

	for (;;)
	{
		if (isDead())
		{
			mState = ePhiloState::Died;
		}

		switch (mState)
		{
		case ePhiloState::StartState:
			mBaseTimePoint = QCGetCounter();
			startPhiloStarvationRoutin(forkIndexList);
			mState = ePhiloState::TimeToEat;
			break;
		case ePhiloState::TimeToEat:
			philoCurrentLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

			printf("%.4fms %u is eating\n", philoCurrentLife, mPhiloIndex + 1);
			startWaitableTimer(mTimeToEat);
			mForkTable->ReleaseFork(forkIndexList[0]);
			mForkTable->ReleaseFork(forkIndexList[1]);
			mBaseTimePoint = QCGetCounter();
			mState = ePhiloState::TimeToSleep;
			break;
		case ePhiloState::TimeToSleep:
			philoCurrentLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

			printf("%.4fms %u is sleeping\n", philoCurrentLife, mPhiloIndex + 1);
			startWaitableTimer(mTimeToSleep);
			mState = ePhiloState::Thinking;
			break;
		case ePhiloState::Thinking:
			philoCurrentLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

			printf("%.4fms %u is thinking\n", philoCurrentLife, mPhiloIndex + 1);
			startPhiloStarvationRoutin(forkIndexList);
			mState = ePhiloState::TimeToEat;
			break;
		case ePhiloState::Died:
			philoCurrentLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

			printf("%.4fms %u is died\n", philoCurrentLife, mPhiloIndex + 1);
			goto lb_exit;
			break;
		default:
			_CrtDbgBreak();
		}
	}
lb_exit:
	{

	}
}

void Philosopher::startPhiloStarvationRoutin(DWORD* forkindexList)
{
	eForkState forkState;
	forkState = mForkTable->GetFork(forkindexList[0], getRemaingLife());
	if (eForkState::ForkTimeout == forkState)
	{
		return;
	}

	printf("%.4fms %u has taken a fork\n", QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint), mPhiloIndex + 1);

	forkState = mForkTable->GetFork(forkindexList[1], getRemaingLife());
	if (eForkState::ForkTimeout == forkState)
	{
		return;
	}

	printf("%.4fms %u has taken a fork\n", QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint), mPhiloIndex + 1);
}

ePhiloState Philosopher::changePhiloState(ePhiloState state)
{
	ePhiloState ret = mState;

	mState = state;

	return ret;
}

inline DWORD Philosopher::getRemaingLife()
{
	float survivedLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);
	DWORD ret = mTimeToDie - static_cast<DWORD>(survivedLife);
	return ret;
}

inline bool Philosopher::isDead()
{
	float survivedLife = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

	if ((float)mTimeToDie < survivedLife)
	{
		return true;
	}

	return false;
}

void Philosopher::setWaitableTime(DWORD ms)
{
	// 100ns 단위; 1ms == 10,000(100ns)
	// 양수는 UTC기준 절대시간, 음수는 타이머
	mDueTime.QuadPart = setTimeMileSecond(ms);

	if (!SetWaitableTimer(mHighResTimer, &mDueTime, 0, NULL, NULL, 0))
	{
		printf("SetWaitableTimer failed (%d)\n", GetLastError());
		_CrtDbgBreak();
	}
}

void Philosopher::startWaitableTimer(DWORD ms)
{
	setWaitableTime(ms);
	/*if (!SetWaitableTimer(mHighResTimer, &mDueTime, 0, NULL, NULL, 0))
	{
		printf("SetWaitableTimer failed (%d)\n", GetLastError());
		_CrtDbgBreak();
	}*/

	if (WaitForSingleObject(mHighResTimer, INFINITE) != WAIT_OBJECT_0)
	{
		printf("WaitForSingleObject failed (%d)\n", GetLastError());
	}

	float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);

	while (fElapsedTick < ms)
	{
		if (!SetWaitableTimer(mHighResTimer, &mDueMacroTime, 0, NULL, NULL, 0))
		{
			printf("SetWaitableTimer failed (%d)\n", GetLastError());
		}

		if (WaitForSingleObject(mHighResTimer, INFINITE) != WAIT_OBJECT_0)
		{
			printf("WaitForSingleObject failed (%d)\n", GetLastError());
		}
		fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), mBaseTimePoint);
	}
}
