#include <QtCore/QCoreApplication>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QPointer>
#include <exception>
#include <QVector>
#include <QTextStream>
#include <QMap>
#include <QSet>

QSet<QString> noobVariables;
QMap<QString, QString> yarnVariables;
QMap<QString, int> numbrVariables;
QMap<QString, float> numbarVariables;
QMap<QString, bool> troofVariables;

class Parser {
protected:
    QMap<QString, Parser*> parserMap_;
    QStringList inputFile_;
    QString currentToken_;
    int listPosition;
    QVector<QString> closingTokens_;

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
typedef QSet<QString>::iterator NoobIT;

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

class MultiLineCommentParser : public Parser {
public:
    MultiLineCommentParser(QStringList &cleanFile) : Parser::Parser(cleanFile) {
    }

    virtual bool parseToken(const QString &input, QStringList &opCodes) {
        // check for obtw
        // advance the token until reaching TLDR
        // return true once we find it
        do {
            advanceToken();
        } while (!currentToken_.endsWith("TLDR"));
    }
};

class VarParser : public Parser {
public:
    VarParser(QStringList &cleanFile) : Parser::Parser(cleanFile) {
    }

    virtual bool parseToken(const QString &input, QStringList &opCodes) {
        if (input.startsWith("I HAS A")) {
            noobVariables.insert(input.section(' ',3,3));
            return true;
        }

        return false;
    }
};


// this checks for the keyword and does not care what comes after it on that line
class OpenEndedKeywordParser : public Parser {
    const QString keyword_;
public:
    OpenEndedKeywordParser(const QString keyword, QStringList &cleanFile) : Parser::Parser(cleanFile), keyword_(keyword)
    {
    }

    bool parseToken(const QString &input, QStringList &opCodes) {
        // Not sure whether this test is required.
        // The word would have been checked to get here.
        return input.startsWith(keyword_);
    }
};

// this checks for the keyword and does not care what comes after it on that line
class varInitialiserParser : public Parser {
public:
    varInitialiserParser(const QString keyword, QStringList &cleanFile) : Parser::Parser(cleanFile)
    {
    }

    bool parseToken(const QString &input, QStringList &opCodes) {
        NoobIT it = noobVariables.find(input.section(' ',1,1));

        if (it == noobVariables.end())
            return false;

        QString val = *it;

        if ("R" == input.section(' ',3,3)) {
            QString baseVal = input.section(' ',4,4);
            bool ok;

            int intVal = baseVal.toInt(&ok, 10);
            if (ok) {
                numbrVariables[val] = intVal;
                return true;
            }

            float floatVal = baseVal.toFloat(&ok);
            if (ok) {
                numbarVariables[val] = floatVal;
                return true;
            }

            if (baseVal == "TRUE")
                troofVariables[val] = true;
            else if (baseVal == "FALSE")
                troofVariables[val] = true;
            else
                yarnVariables[val] = baseVal;

            return true;
        }
        return false;
    }
};

class FileLevelParser : public Parser {
public:
    FileLevelParser(QStringList &cleanFile) : Parser::Parser(cleanFile) {
        OpenEndedKeywordParser *haiParser = new OpenEndedKeywordParser("HAI", cleanFile);
        parserMap_["HAI"] = haiParser;

        OpenEndedKeywordParser *commentParser = new OpenEndedKeywordParser("BTW", cleanFile);
        parserMap_["BTW"] = commentParser;

        MultiLineCommentParser *bigCommentParser = new MultiLineCommentParser(cleanFile);
        parserMap_["OBTW"] = bigCommentParser;

        // what is the best way to end the program?
        VarParser *varParser = new VarParser(cleanFile);
        parserMap_["I"] = varParser;
    }

    // need to split the lines by spaces too!!!
    virtual bool parseToken(const QString &input, QStringList &opCodes) {
        QStringList tokens = input.split(' ');

        MapIT it = parserMap_.find(tokens[0]);

        // need to split character by space and then pass the entire thing though
        // if we find a match
        if (it == parserMap_.end())
            return false;
        return (*it)->parseToken(input, opCodes);
    }
};


// This class takes in the file to be compiled and a reference to the input file.
// The input file will be filled with a clean version of the source file.
// It will have no comments, no excessive white space
class ParsingDirector {
    QStringList cleanFile_;
    FileLevelParser *fileParser_;
    QString separators_;

public:
    ParsingDirector(const QStringList &cleanFile) :  cleanFile_(cleanFile) {
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
    ParsingDirector parser(cleanFile_);
    parser.parseFile(opCodes_);

    return a.exec();
}
