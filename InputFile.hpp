#ifndef INPUTFILE_HPP
#define INPUTFILE_HPP

#include<QStringList>
#include<QString>
#include<QFile>
#include<QTextStream>

class InputFile{
    QStringList fileContents_;
    QStringList::Iterator currentStatement_;
    int tokenPos_;

    QStringList loadFile(QString &inputFileName) {
        QString fullPath = "/home/nossica/QtSDK/PrototypeLOLCode/" + inputFileName;
        QFile *inputInterface = new QFile(fullPath);
        QStringList inputFile;

        if (false == inputInterface->open(QIODevice::ReadOnly|QIODevice::Text)) {
            throw "Cannot open file";
        }

        QTextStream in(inputInterface);

        while (!in.atEnd()) {
            QString f = in.readLine();
            inputFile.append(f);
        }
        return inputFile;
    }

public:
    InputFile(QString& inputFile) : tokenPos_(0), currentStatement_(0) {
        fileContents_ = loadFile(inputFile);
        currentStatement_ = fileContents_.begin();
    }

    QString getNextStatement() {
        static bool intialised = false;

        if (fileContents_.empty())
            return "";

        if (intialised == false) {
            intialised = true;
            currentStatement_ = fileContents_.begin();
        }
        else {            
            ++currentStatement_;
        }

        while ((*currentStatement_).isEmpty())
            ++currentStatement_;

        return *currentStatement_;
    }
};


#endif // INPUTFILE_HPP
