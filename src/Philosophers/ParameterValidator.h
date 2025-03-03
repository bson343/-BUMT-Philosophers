#pragma once

struct Arguments
{
    INT32 numberOfPhilo = 2;
    INT32 timeToDie = 0;
    INT32 timeToEat = 0;
    INT32 timeToSleep = 0;
    INT32 timesMustEat = 0;
};

class ParameterValidator
{
public:
    static bool Validate(int argc, char** argv, Arguments& out, bool verose);
    static void PrintArguments(Arguments& param, const WCHAR* msg);

private:
    static bool validateNumericRangeInt32(char* numStr, int& out);
};

