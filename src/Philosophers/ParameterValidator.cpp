#include "pch.h"
#include "ParameterValidator.h"

using namespace std;

Arguments ParameterValidator::Validate(int argc, char** argv)
{
    Arguments ret;

    cout << "Check Arguments" << endl;

    int* mapping[] =
    {
        &ret.numberOfPhilo,
        &ret.timeToDie,
        &ret.timeToEat,
        &ret.timeToSleep,
        &ret.timesMustEat,
    };

    for (int i = 0; i < argc; i++)
    {
        //cout << i << " : " << argv[i] << endl;

        int temp = atoi(argv[i]);
        if (temp < 0)
        {
            cerr << "\033[0;31m" << "Invaild Arguments" << "\033[0m" << endl;
            cout << "The Arguments must be positive." << endl;
            exit(1);
        }

        if (i != 0)
        {
            *mapping[i - 1] = temp;
        }
    }

    return ret;
}

void ParameterValidator::PrintArguments(Arguments& param)
{
    cout << "*******Show Input Arguments()************" << endl;
    cout << "numberOfPhilo : " << param.numberOfPhilo << endl;
    cout << "timeToDie : " << param.timeToDie << endl;
    cout << "timeToEat : " << param.timeToEat << endl;
    cout << "timeToSleep : " << param.timeToSleep << endl;
    cout << "timesMustEat : " << param.timesMustEat << endl;
    cout << "*****************************************" << endl;
}
