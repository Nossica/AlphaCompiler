#include <QtCore/QCoreApplication>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QPointer>
#include <exception>
#include <QVector>
#include <QTextStream>
#include <QMap>

class Parser {
protected:
    QMap<QString, Parser*> parserMap_;
    QStringList inputFile_;
    QString currentToken_;
    int listPosition;

public:
    Parser(QStringList &cleanFile) : inputFile_(cleanFile), listPosition(0) {
        currentToken_ = inputFile_[listPosition];
    }

    virtual bool parseToken(const QString &input, QStringList &opCodes) = 0;

    bool advanceToken() {
        if (inputFile_.size() == ++listPosition)
            return false;

        currentToken_ = inputFile_[listPosition];
        return true;
    }
};

typedef QMap<QString, Parser*>::iterator MapIT;


class MultiLineCommentParser : public KeywordParser {
    public:
    CommentParser(QStringList &cleanFile) : Parser::Parser(cleanFile)
    {
    }

    virtual bool parseToken(const QString &input, QStringList &opCodes) {
        return true;
    }
};

class KeywordParser : public Parser {
    const QString keyword_;
public:
    KeywordParser(const QString keyword, QStringList &cleanFile) : Parser::Parser(cleanFile), keyword_(keyword)
    {
    }

    bool parseToken(const QString &input, QStringList &opCodes) {
        return keyword_ == input;
    }
};


class OpenEndedKeywordParser : public Parser {
    const QString keyword_;
public:
    KeywordParser(const QString keyword, QStringList &cleanFile) : Parser::Parser(cleanFile), keyword_(keyword)
    {
    }

    bool parseToken(const QString &input, QStringList &opCodes) {

        return keyword_ == input;
    }
};


class FileLevelParser : public Parser {
public:
    FileLevelParser(QStringList &cleanFile) : Parser::Parser(cleanFile) {
        KeywordParser *haiParser = new OpenEndedKeywordParser("HAI", cleanFile);
        parserMap_["HAI"] = haiParser;

        OpenEndedKeywordParser *commentParser = new OpenEndedKeywordParser("BTW", cleanFile);
        parserMap_["BTW"] = commentParser;

        MultiLineCommentParser *bigCommentParser = new OpenEndedKeywordParser("OBTW", cleanFile);
        parserMap_["OBTW"] = bigCommentParser;

    }

    // need to split the lines by spaces too!!!
    virtual bool parseToken(const QString &input, QStringList &opCodes) {
        MapIT it = parserMap_.find(input);
        if (it == parserMap_.end())
            return false;
        return (*it)->parseToken(input, opCodes);
    }
};


//class OutFile {
//    QVector<QString> fileContents_;
//    Parser* currentParser_;

//public:
//    OutFile() {
//        fileContents_.clear();
//        currentParser_ = new FileLevelParser();
//    }

//    void parse(const QString line) {
//        QString inputLine = line.simplified();
//        QStringList inputLines = inputLine.split(',');

//        // go through each item on the inputLines and read them in by space
//        // Send them through the current parser and see if it parses.
//        // If not then read another token in until.
//        // Do we set them up as a stack?
//        // how about each parser has all possible parsers as members?
//        QString outputLine;
//        foreach(QString item, inputLines) {
//            if (currentParser_->parseToken(item, outputLine)) {
//                fileContents_.push_back(line);
//            }
//        }
//    }

//    void add(const QString line) {
//        fileContents_.push_back(line);
//    }
//};

// This class takes in the file to be compiled and a reference to the input file.
// The input file will be filled with a clean version of the source file.
// It will have no comments, no excessive white space
class FileParser {
    QStringList cleanFile_;
    FileLevelParser *fileParser_;
    QString separators_;
public:
    FileParser(const QStringList &cleanFile) :  cleanFile_(cleanFile) {
        fileParser_ = new FileLevelParser(cleanFile_);
    }

    // This will take the cleaned file (in string list form) and parse
    // it will return QStringList of opcodes
    void parseFile(QStringList &opCodes) {
        for (int i = 0; i < cleanFile_.size(); ++i)
            fileParser_->parseToken(cleanFile_[i], opCodes);
    }
};


QStringList loadFile(QString &inputFileName) {
    QFile *inputFile = new QFile("/home/nossica/QtSDK/PrototypeLOLCode/test.lol"/*fileName*/);
    QStringList cleanFile;

    if (false == inputFile->open(QIODevice::ReadOnly|QIODevice::Text)) {
        throw "Cannot open file";
    }

    QTextStream in(inputFile);

    while (!in.atEnd()) {
        QString f = in.readLine();
        if (f.isEmpty())
            continue;

        cleanFile.append(f);
    }
    return cleanFile;
}

// Load in a file and begin parsing
int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    QStringList cleanFile_;
    QStringList opCodes_;
    //OutFile parsedFile;

    if (args.count() != 2)
    {
        std::cerr << "argument required" << std::endl;
        return 1;
    }

    QString inputFile = static_cast<QString>(argv[1]);

    // Take the input file, remove the excessive whitespace
    // and convert it into a string list
    cleanFile_ = loadFile(inputFile);

    // Send list to the file parser
    FileParser parser(cleanFile_);
    parser.parseFile(opCodes_);

    return a.exec();
}
