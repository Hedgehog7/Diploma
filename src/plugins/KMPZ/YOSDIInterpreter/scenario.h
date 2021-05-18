#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <string>
#include <map>

enum class CommandType
{
    CALL,
    FORM,
    CONNECT,
    TEXT,
    PARAM
};

struct Command
{
    CommandType type;
    std::vector<std::string> params;
};

struct Procedure
{
    std::vector<Command> commands;
};

struct Scenario
{
    std::map<std::string, Procedure> procedures;
};

#endif // SCENARIO_H
