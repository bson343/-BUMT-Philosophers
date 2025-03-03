#pragma once
#include <memory>

enum class eForkState
{
	ForkGet,
	ForkRelease,
	ForkTimeout,
	ABORT
};

class MutexForkTable
{
public:
	MutexForkTable(DWORD numOfPilo);
	~MutexForkTable();

	void Init();
	eForkState GetFork(const DWORD forkIndex, const DWORD waitTime);
	eForkState ReleaseFork(const DWORD forkIndex);

private:
	const DWORD mNumOfPhilo;
	
	bool isStop = false;
	HANDLE* mForkList = nullptr;


};

