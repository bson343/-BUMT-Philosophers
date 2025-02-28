#pragma once

struct Arguments
{
    int numberOfPhilo = -1;
    int timeToDie = -1;
    int timeToEat = -1;
    int timeToSleep = -1;
    int timesMustEat = -1;
};

class ParameterValidator
{
public:
    static Arguments Validate(int argc, char** argv);
    static void PrintArguments(Arguments& param);

private:

};

