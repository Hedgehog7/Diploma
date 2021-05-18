#ifndef SCENARIO_LOADER_H
#define SCENARIO_LOADER_H

#include "scenario.h"
#include<QDir>

#include <fstream>

class ScenarioLoader
{
public:
    ScenarioLoader(const std::string& taskType);
    ~ScenarioLoader();

    const Procedure& getProcedure(const std::string& procedureName);

private:
    void loadScenario(std::ifstream& file);
    void loadProcedure(std::ifstream& file);
    bool loadCommand(std::ifstream& file, Procedure& procedure);
    void addParamsToCommand(std::ifstream& file, Command& command);
    std::string getTextParam(std::ifstream& file);

    CommandType getCommandType(const std::string& commandName);
    size_t getCommandParamsCount(CommandType type);

private:
    Scenario _scenario;
};

#endif // SCENARIO_LOADER_H
