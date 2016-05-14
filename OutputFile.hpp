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
        //0053_0000FF  load acc
        //0080_0000FF  write acc to memory
        QString hexValue = QString("%1").arg(value, 6, 16, QLatin1Char( '0' ));
        QString hexLocation = QString("%1").arg(location, 6, 16, QLatin1Char( '0' ));

        QTextStream stream(instructions);

        QString loadAcc = "0053_" + hexValue;
        QString writeAccToMem = "0080_" + hexLocation;
        stream << loadAcc << endl;
        stream << writeAccToMem << endl;
    }


    void setYarn(const QString value, const unsigned int location) {
        // store as ASCII for now?
        setInt(value.length(), location);

        QTextStream stream(instructions);

        //0053_0000FF  load acc
        //0080_0000FF  write acc to memory
        for (int count = 0; count < value.length(); ++count)  {
            QString hexValue = QString("%1").arg(value[count].toAscii(), 6, 16, QLatin1Char( '0' ));
            QString hexLocation = QString("%1").arg(location + count + 1, 6, 16, QLatin1Char( '0' ));
            QString loadAcc = "0053_" + hexValue;
            QString writeAccToMem = "0080_" + hexLocation;
            stream << loadAcc << endl;
            stream << writeAccToMem << endl;
        }
    }

    void setDouble(const double value, const unsigned int location) {
        //0053_0000FF  load acc
        //0080_0000FF  write acc to memory
        union {
               long long i;
               double    d;
        } valueConvert;

        valueConvert.d = value;

        char buf[17];

        snprintf (buf,sizeof(buf),"%016llx",valueConvert.i);
        buf[16]=0; //make sure it is null terminated.

        QString hexValue = buf;//QString("%1").arg(value, 6, 16, QLatin1Char( '0' ));

        QString hexLocation = QString("%1").arg(location, 6, 16, QLatin1Char( '0' ));

        QTextStream stream(instructions);

        QString loadAcc = "0053_" + hexValue;
        QString writeAccToMem = "0080_" + hexLocation;
        stream << loadAcc << endl;
        stream << writeAccToMem << endl;
    }

    void setBool(const bool value, const unsigned int location) {
        setInt(value, location);
    }
};



#endif // OUTPUTFILE_HPP

