#include "pch.h"
#include "MutexForkTable.h"


MutexForkTable::MutexForkTable(DWORD numOfPilo)
	:
	mNumOfPhilo(numOfPilo)
{
}

MutexForkTable::~MutexForkTable()
{
	// delete mForkList
	{
		UINT waitResult =
			WaitForMultipleObjects(
				mNumOfPhilo,
				mForkList,
				TRUE,
				0
			);

		_ASSERT(WAIT_OBJECT_0 == waitResult);

		if (mForkList)
		{
			for (DWORD i = 0; i < mNumOfPhilo; i++)
			{
				if (!CloseHandle(mForkList[i]))
				{
					_CrtDbgBreak();
				}
			}
		}

		delete[] mForkList;
		mForkList = nullptr;
	}

}

void MutexForkTable::Init()
{
	//Initialize mForkList
	{
		mForkList = new HANDLE[mNumOfPhilo];

		for (DWORD i = 0; i < mNumOfPhilo; i++)
		{
			mForkList[i] = CreateMutex(
				NULL,
				FALSE,
				NULL
			);
		}
	}

}

eForkState MutexForkTable::GetFork(const DWORD forkIndex, const DWORD waitTime)
{
	DWORD waitResult;
	eForkState ret = eForkState::ABORT;

	waitResult = WaitForSingleObject(
		mForkList[forkIndex],
		waitTime
	);

	switch (waitResult)
	{
	case WAIT_OBJECT_0:
		ret = eForkState::ForkGet;
		break;
	case WAIT_TIMEOUT:
		ret = eForkState::ForkTimeout;
		break;


	case WAIT_ABANDONED:
		_CrtDbgBreak();
		break;
	case WAIT_FAILED:
		_CrtDbgBreak();
		break;
	default:
		_CrtDbgBreak();
	}

	return ret;
}

eForkState MutexForkTable::ReleaseFork(const DWORD forkIndex)
{
	bool mutexResult;

	mutexResult = ReleaseMutex(
		mForkList[forkIndex]
	);

	_ASSERT(mutexResult == TRUE);

	return eForkState::ForkRelease;
}


