// Philosophers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

struct Arguments
{
    int numberOfPhilo = -1;
    int timeToDie = -1;
    int timeToEat = -1;
    int timeToSleep = -1;
    int timesMustEat = -1;
};

void printArguments(Arguments& param)
{
    cout << "*******Show Input Arguments()************" << endl;
    cout << "numberOfPhilo : " << param.numberOfPhilo << endl;
    cout << "timeToDie : " << param.timeToDie << endl;
    cout << "timeToEat : " << param.timeToEat << endl;
    cout << "timeToSleep : " << param.timeToSleep << endl;
    cout << "timesMustEat : " << param.timesMustEat << endl;
    cout << "*****************************************" << endl;
}

int main(int argc, char** argv)
{
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
    {
        int* mapping[] =
        {
            &initData.numberOfPhilo,
            &initData.timeToDie,
            &initData.timeToEat,
            &initData.timeToSleep,
            &initData.timesMustEat,
        };
#pragma region Check Arguments
        {
            cout << "Check Arguments" << endl;
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
        }
#pragma endregion

        printArguments(initData);
    }

}
