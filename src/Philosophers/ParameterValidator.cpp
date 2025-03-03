#include "pch.h"
#include "ParameterValidator.h"

using namespace std;

bool ParameterValidator::Validate(int argc, char** argv, Arguments& out, bool verose)
{
    Arguments compare;

    INT32* retPtr;
    INT32* minRequirePtr;

    retPtr = &out.numberOfPhilo;
    minRequirePtr = &compare.numberOfPhilo;

    if (verose)
    {
        cout << "Check Arguments" << endl;
    }

    for (int i = 1; i < argc; i++)
    {
        INT32 parseNumber;
        bool result = validateNumericRangeInt32(argv[i], parseNumber);

        if (!result)
        {
            cerr << "\033[0;31m" << "Invaild Arguments" << "\033[0m" << endl;
            cout << "The Arguments must be positive." << endl;
            exit(1);
        }

        if (*(minRequirePtr + (i - 1)) <= parseNumber)
        {
            *(retPtr + (i - 1)) = parseNumber;
        }
        else
        {
            cerr << "\033[0;31m" << "Invaild Arguments" << "\033[0m" << endl;
            PrintArguments(compare, L"Minimum Required Input Value");
            exit(1);
        }
    }

    if (argc == 5)
    {
        out.timesMustEat = 0;
    }

    if (verose)
    {
        PrintArguments(out, L"Show Input Arguments");
    }

    return TRUE;
}

void ParameterValidator::PrintArguments(Arguments& param, const WCHAR* msg)
{
    wprintf_s(L"*******%s************\n", msg);
    cout << "numberOfPhilo : " << param.numberOfPhilo << endl;
    cout << "timeToDie : " << param.timeToDie << endl;
    cout << "timeToEat : " << param.timeToEat << endl;
    cout << "timeToSleep : " << param.timeToSleep << endl;
    cout << "timesMustEat : " << param.timesMustEat << endl;
    cout << "*****************************************" << endl << endl;
}

bool ParameterValidator::validateNumericRangeInt32(char* numStr, INT32& out)
{
    char* endPtr;
    INT64 inputData = strtol(numStr, &endPtr, 10);

    // 에러 체크
    if (errno == ERANGE && *endPtr != '\0') 
    {
        return FALSE;
    }

    if (INT_MAX < inputData)
    {
        return FALSE;
    }

    out = static_cast<INT32>(inputData);

    return TRUE;
}
