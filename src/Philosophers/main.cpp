// Philosophers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "ParameterValidator.h"

using namespace std;

int main(int argc, char** argv)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    cout << "Project Philosophers\n";
    cout << "Console.Ver\n";

    cout << "argc : " << argc << endl << endl;

    if (argc < 2)
    {
        cerr << "\033[0;31m" << "Invaild Arguments" << "\033[0m" << endl;

        cout << "Usage: <number of philosophers> <time to die> <time to eat> <time to sleep> [number of times each philosopher must eat]" << endl;

        exit(1);
    }

    Arguments initData;

    initData = ParameterValidator::Validate(argc, argv);

    ParameterValidator::PrintArguments(initData);

#ifdef _DEBUG
    _ASSERT(_CrtCheckMemory());
#endif
}
