#ifndef OUTPUTFILE_HPP
#define OUTPUTFILE_HPP

#include<QStringList>
#include<QString>
#include<QFile>
#include<QTextStream>

// This is a text file which contains all of the machine code instructions

class OutputFile{
    QFile *instructions;
public:
    OutputFile() {
        QString fullPath = "/home/nossica/QtSDK/PrototypeLOLCode/outputFile.vnp";
        instructions = new QFile(fullPath);

        if (false == instructions->open(QIODevice::WriteOnly|QIODevice::Text)) {
            throw "Cannot open file";
        }
    }

    void setInt(const int value, const unsigned int location) {
        //0053_0000FF
        //0080_0000FF
        QString hexValue = QString("%1").arg(value, 6, 16, QLatin1Char( '0' ));
        QString hexLocation = QString("%1").arg(location, 6, 16, QLatin1Char( '0' ));

        QTextStream stream(instructions);

        QString loadAcc = "0053_" + hexValue;
        QString writeAccToMem = "0080_" + hexLocation;
        stream << loadAcc << endl;
        stream << writeAccToMem << endl;
    }
};



#endif // OUTPUTFILE_HPP

