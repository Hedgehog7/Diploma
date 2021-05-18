#include "scenario_loader.h"

#include <assert.h>
#include <iostream>

std::string get_path()
{
    QDir dir;
    QString path = dir.currentPath();
    std::cout << path.toStdString();
    path.truncate(path.lastIndexOf('/'));
    path.truncate(path.lastIndexOf('/'));
    path.truncate(path.lastIndexOf('/'));
    return path.toStdString();
}

ScenarioLoader::ScenarioLoader(const std::string& taskType)
{
    std::ifstream file(get_path() + "/src/plugins/KMPZ/YOSDIInterpreter/scenarios/dio.scn");//УКАЗАТЬ ВЕРНЫЙ ПУТЬ!!!
    loadScenario(file);
}

ScenarioLoader::~ScenarioLoader()
{}

const Procedure& ScenarioLoader::getProcedure(const std::string& procedureName)
{
    auto procedure =_scenario.procedures.find(procedureName);
    if( procedure == _scenario.procedures.end() )
    {
        static Procedure empty;
        return empty;
    }

    return procedure->second;
}

void ScenarioLoader::loadScenario(std::ifstream& file)
{
    while(!file.eof())
    {
        std::string word;
        file >> word;

        if( word.empty() )
            continue;
        if( word == "procedure" )
        {
            loadProcedure(file);
            continue;
        }

        //assert(!"correct scn format");
    }
}

void ScenarioLoader::loadProcedure(std::ifstream& file)
{
    std::string procedureName;
    file >> procedureName;

    while(loadCommand(file, _scenario.procedures[procedureName])){}
}

bool ScenarioLoader::loadCommand(std::ifstream& file, Procedure& procedure)
{
    std::string commandName;
    file >> commandName;

    if( commandName[0] == '#')
    {
        char buff[256];
        file.getline(buff, 250);
        return true;
    }
    if( commandName == "end" )
        return false;

    CommandType commandType = getCommandType(commandName);
    Command command;
    command.type = commandType;

    addParamsToCommand(file, command);

    procedure.commands.push_back(command);

    return true;
}

void ScenarioLoader::addParamsToCommand(std::ifstream& file, Command& command)
{
    if( command.type == CommandType::TEXT )
    {
        std::string param;
        file >> param;
        command.params.push_back(param);

        const std::string text = getTextParam(file);
        command.params.push_back(text);
    }
    else
    {
        size_t qParams = getCommandParamsCount(command.type);
        while(qParams--)
        {
            std::string param;
            file >> param;

            command.params.push_back(param);
        }
    }
}

std::string ScenarioLoader::getTextParam(std::ifstream& file)
{
    char c = '0';
    while(c != '"')
        file >> c;
    c = file.get();

    std::string res;
    while(c != '"')
    {
        if(c == '\\')
        {
            c = file.get();
            res += c;
        }

        res += c;

        c = file.get();
    }

    return res;
}

CommandType ScenarioLoader::getCommandType(const std::string& commandName)
{
    if( commandName == "call" )
        return CommandType::CALL;
    if( commandName == "form" )
        return CommandType::FORM;
    if( commandName == "connect" )
        return CommandType::CONNECT;
    if( commandName == "text" )
        return CommandType::TEXT;
    if( commandName == "param" )
        return CommandType::PARAM;

    assert(!"known param");
    return CommandType::CALL;
}

size_t ScenarioLoader::getCommandParamsCount(CommandType type)
{
    switch (type) {
    case CommandType::CALL      : return 1;
    case CommandType::FORM      : return 1;
    case CommandType::CONNECT   : return 2;
    case CommandType::TEXT      : return 2;
    case CommandType::PARAM     : return 2;
    }

    assert(!"known param");
    return 0;
}
