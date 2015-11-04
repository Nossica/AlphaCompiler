#include <QtCore/QCoreApplication>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QPointer>
#include <exception>
#include <QVector>
#include <QTextStream>



class InFile {
    QVector<QString> fileContents_;
public:
    InFile () {
        fileContents_.clear();
    }

    void add(const QString line) {
        fileContents_.push_back(line);
    }
};

class FileParser {
    QPointer<QFile> inputFile_;
    InFile parsedFile_;
public:
    FileParser(QString fileName, InFile &parsedFile) : parsedFile_(parsedFile) {
        inputFile_ = new QFile(fileName);

        if (false == inputFile_->open(QIODevice::ReadOnly|QIODevice::Text)) {
            std::exception e();
            throw "Cannot open file";
        }

        QTextStream in(inputFile_);

        while (!in.atEnd()) {
            QString f = in.readLine();

            if (f.isEmpty())
                continue;

            parsedFile_.add(f);
        }
    }
};

// Load in a file and begin parsing

int main(int &argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    InFile parsedFile;

    if (args.count() != 2)
    {
        std::cerr << "argument required" << std::endl;
        return 1;
    }

    FileParser parser(static_cast<QString>(args[1]), parsedFile);

    return a.exec();
}
