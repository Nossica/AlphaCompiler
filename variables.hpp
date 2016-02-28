#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include<QString>
#include<OutputFile.hpp>

class Variable
{
    // TODO
    // This is currently only supporting int values.

    QString name_;
    unsigned int location_;
    int value_;
    OutputFile &outputFile_;

public:
    Variable(OutputFile &outputFile, const QString& name, const unsigned int location);
    void setValue(const int val) {
        value_ = val;
        outputFile_.setInt(value_, location_);
    }
    const int getValue() {return value_;}

};

class YarnVariables : public Variable {
public:
    std::string value;
};

#endif // VARIABLES_HPP
