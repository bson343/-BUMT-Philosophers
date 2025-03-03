// Philosophers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "ParameterValidator.h"
#include "Philosopher.h"
#include "MutexForkTable.h"

using namespace std;

UINT __stdcall PhiloThreadFunc(void* pArg);

enum class eThreadEvent
{
    Process,
    Destroy,
    Count
};

struct LoggingThreadArgs
{
    HANDLE hThread;
    HANDLE EventList[static_cast<DWORD>(eThreadEvent::Count)];
    DWORD ThreadIndex;
};

struct PhiloThreadArgs
{
    HANDLE hThread;
    HANDLE EventList[static_cast<DWORD>(eThreadEvent::Count)];
    DWORD ThreadIndex;
    PhiloInfoArgs* InitData;
};


int main(int argc, char** argv)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    QCInit();

    cout << "Project Philosophers\n";
    cout << "Console.Ver\n";

    cout << "argc : " << argc << endl << endl;

    if (!(argc == 5 || argc == 6))
    {
        cerr << "\033[0;31m" << "Invaild Arguments" << "\033[0m" << endl;

        cout << "Usage: <number of philosophers> <time to die> <time to eat> <time to sleep> [number of times each philosopher must eat]" << endl;

        exit(1);
    }

    Arguments initData;

    ParameterValidator::Validate(argc, argv, initData, true);

    PhiloThreadArgs* philoThreadArgs = new PhiloThreadArgs[initData.numberOfPhilo];
    memset(philoThreadArgs, 0, sizeof(PhiloThreadArgs) * initData.numberOfPhilo);

    MutexForkTable forkTable(initData.numberOfPhilo);
    forkTable.Init();

    PhiloInfoArgs philoInfoData{ 
        initData.numberOfPhilo,
        initData.timeToDie,
        initData.timeToEat,
        initData.timeToSleep,
		initData.timesMustEat,
        &forkTable 
    };

	for (DWORD i = 0; i < philoInfoData.NumOfPhilo; i++)
	{
		for (DWORD j = 0; j < static_cast<DWORD>(eThreadEvent::Count); j++)
		{
			philoThreadArgs[i].EventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		philoThreadArgs[i].ThreadIndex = i;
		philoThreadArgs[i].InitData = &philoInfoData;

		UINT threadID = 0;
		philoThreadArgs[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, PhiloThreadFunc, philoThreadArgs + i, 0, &threadID);

		WCHAR wchThreadName[64] = {};
		swprintf_s(wchThreadName, L"PhiloThread[%u]", i);
		SetThreadDescription(philoThreadArgs[i].hThread, wchThreadName);
	}

	for (DWORD i = 0; i < philoInfoData.NumOfPhilo; i += 2)
	{
		SetEvent(philoThreadArgs[i].EventList[static_cast<DWORD>(eThreadEvent::Process)]);
	}
	for (DWORD i = 1; i < philoInfoData.NumOfPhilo; i += 2)
	{
		SetEvent(philoThreadArgs[i].EventList[static_cast<DWORD>(eThreadEvent::Process)]);
	}

	for (DWORD i = 0; i < philoInfoData.NumOfPhilo; i++)
	{
		SetEvent(philoThreadArgs[i].EventList[static_cast<DWORD>(eThreadEvent::Destroy)]);
	}

	if (philoThreadArgs)
	{
		for (DWORD i = 0; i < philoInfoData.NumOfPhilo; i++)
		{
			WaitForSingleObject(philoThreadArgs[i].hThread, INFINITE);
			CloseHandle(philoThreadArgs[i].hThread);
			for (DWORD j = 0; j < static_cast<DWORD>(eThreadEvent::Count); j++)
			{
				CloseHandle(philoThreadArgs[i].EventList[j]);
				philoThreadArgs[i].EventList[j] = nullptr;
			}

		}
		delete[] philoThreadArgs;
		philoThreadArgs = nullptr;
	}


#ifdef _DEBUG
    _ASSERT(_CrtCheckMemory());
#endif
}

UINT __stdcall PhiloThreadFunc(void* pArg)
{
	PhiloThreadArgs* threadArg = static_cast<PhiloThreadArgs*>(pArg);

	DWORD threadID = GetCurrentThreadId();
	DWORD threadIndex = threadArg->ThreadIndex;
	const DWORD threadEventCount = static_cast<DWORD>(eThreadEvent::Count);

	DWORD philoIndex = threadIndex;
	std::unique_ptr<Philosopher> philo;

	wprintf_s(L"Start Worker Thread [Index: %u]\n", threadIndex);

	philo = std::make_unique<Philosopher>(threadArg->InitData, philoIndex);
	philo->Init();

	while (TRUE)
	{
		eThreadEvent event =
			static_cast<eThreadEvent>(WaitForMultipleObjects(threadEventCount, threadArg->EventList, FALSE, INFINITE));
		switch (event)
		{
		case eThreadEvent::Process:
			philo->Loop();
			break;

		case eThreadEvent::Destroy:
			philo = nullptr;
			goto lb_exit;
			break;
		default:
			_CrtDbgBreak();
		}
	}

lb_exit:
	wprintf_s(L"End Worker Thread [Index: %u]\n", threadIndex);
	_endthreadex(0);
	return 0;
}
