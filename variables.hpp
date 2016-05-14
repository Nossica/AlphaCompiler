#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include<QString>
#include<OutputFile.hpp>

class MemoryManager {

};

class Variable
{
    QString name_;
    QString value_;

protected:
    unsigned int location_;
    OutputFile &outputFile_;

public:
    Variable(OutputFile &outputFile, const QString& name, const unsigned int location);
    virtual void setValue(const QString val) {
        value_ = val;
        outputFile_.setYarn(value_, location_);
    }
    const QString getValue() {return value_;}
};

class numbrVariable : public Variable {
    int value_;
public:
    numbrVariable(OutputFile &outputFile, const QString& name, const unsigned int location);

    virtual void setValue(const QString val) {
        bool ok;
        value_ = val.toInt(&ok, 16);
        outputFile_.setInt(value_, location_);
    }
    const int getValue() {return value_;}
};

class troofVariable : public Variable {
    int value_;
public:
    troofVariable(OutputFile &outputFile, const QString& name, const unsigned int location);

    virtual void setValue(const QString val) {
        bool ok;
        value_ = val.toInt(&ok, 2);
        outputFile_.setInt(value_, location_);
    }
    const bool getValue() {return (value_!=0);}
};

class yarnVariable : public Variable {
    QString value_;
public:
    yarnVariable(OutputFile &outputFile, const QString& name, const unsigned int location);

    virtual void setValue(const QString val) {
        value_ = val.mid(1,val.length()-2);
        outputFile_.setYarn(value_, location_);
    }
    const QString getValue() {return value_;}
};

class numbarVariable : public Variable {
    double value_;
public:
    numbarVariable(OutputFile &outputFile, const QString& name, const unsigned int location);

    virtual void setValue(const QString val) {
        bool ok;
        value_ = val.toDouble(&ok);
        outputFile_.setDouble(value_, location_);
    }
    const double getValue() {return value_;}
};
#endif // VARIABLES_HPP
