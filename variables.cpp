#include "variables.hpp"

Variable::Variable(OutputFile &outputFile, const QString& name, const unsigned int location) :
    outputFile_(outputFile), name_(name), location_(location)
{
}

numbrVariable::numbrVariable(OutputFile &outputFile, const QString& name, const unsigned int location) :
    Variable(outputFile, name, location)
{
}

troofVariable::troofVariable(OutputFile &outputFile, const QString& name, const unsigned int location) :
    Variable(outputFile, name, location)
{
}


yarnVariable::yarnVariable(OutputFile &outputFile, const QString& name, const unsigned int location) :
    Variable(outputFile, name, location)
{
}


numbarVariable::numbarVariable(OutputFile &outputFile, const QString& name, const unsigned int location) :
    Variable(outputFile, name, location)
{
}
