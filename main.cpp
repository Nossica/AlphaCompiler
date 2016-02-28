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

#include<InputFile.hpp>
#include<OutputFile.hpp>
#include<variables.hpp>

QMap<QString, Variable*> noobVariables;
QMap<QString, QString> yarnVariables;
QMap<QString, int> numbrVariables;
QMap<QString, float> numbarVariables;
QMap<QString, bool> troofVariables;


class Parser {
protected:
    QVector<Parser*> parserMap_;
    QVector<QString> closingTokens_;
    InputFile &inputInterface_;
    OutputFile &outputInterface_;
    QString keyword_;
public:
    Parser(InputFile &inputInterface, OutputFile &outputInterface, const QString &keyword) : inputInterface_(inputInterface), outputInterface_(outputInterface), keyword_(keyword) {
        closingTokens_.clear();
    }

    virtual bool matchToken(const QString &input) {
        return input.startsWith(keyword_);
    }

    virtual bool parseToken(const QString &input) = 0;

    QString getNextStatement() {
        return inputInterface_.getNextStatement();
    }
};

typedef QMap<QString, Parser*>::iterator MapIT;

class KeywordParser : public Parser {
    KeywordParser(const QString keyword, InputFile &inputInterface, OutputFile &outputInterface)
        : Parser(inputInterface, outputInterface, keyword) {
    }


    bool parseToken(const QString &input) {
    }
};

class MultiLineCommentParser : public Parser {
public:
    MultiLineCommentParser(const QString keyword, InputFile &inputInterface, OutputFile &outputInterface)
        : Parser(inputInterface, outputInterface, keyword) {

        closingTokens_.push_back("TLDR");
    }


    virtual bool parseToken(const QString &input) {
        QString currentToken = input;
        while (currentToken.size() > 0) {
            for (int i=0; i<closingTokens_.size(); ++i) {
                if (currentToken == closingTokens_[i]) {
                    return true;
                }
            }
            currentToken = inputInterface_.getNextStatement();
        }
    }
};

class VarParser : public Parser {
public:
    VarParser(const QString keyword, InputFile &inputInterface, OutputFile &outputInterface) :
        Parser(inputInterface, outputInterface, keyword) {

    }

    virtual bool parseToken(const QString &input) {
        // the token that comes after the keyword_ is the var name
        // there might be more after that that specifies stuff
        QString varDetails = input.right(input.length() - keyword_.length());
        varDetails = varDetails.simplified();
        varDetails.replace( " ", "" );

        // if (-1 == varDetails.indexOf(" ")) {

            // TODO
            // Test what type of var it is
            // Use a smart pointer class rather than a raw pointer
            // Write the memory manager class so it can calculate the memory location
            // Pass a reference to the memory manager through to the variables so they
            // can update the value
            Variable* newVar = new Variable(outputInterface_, varDetails, 0);
            noobVariables[varDetails] = newVar;
        //}
        return false;
    }
};

class VarAssignmentParser : public Parser {
public:
    VarAssignmentParser(InputFile &inputInterface, OutputFile &outputInterface) :
        Parser(inputInterface, outputInterface, "") {
    }

    virtual bool parseToken(const QString &input) {
        QStringList list = input.split(QRegExp("\\s+"));
        // check whether var matches a declared var, if it does then set it
        QMap<QString, Variable*>::iterator it = noobVariables.find(list[0]);
        if (it != noobVariables.end()) {
            Variable* var = it.value();

            if (list[1] == "R") {
                bool ok;
                var->setValue(list[2].toInt(&ok, 16));
            }
        }
        return false;
    }
};

// this checks for the keyword and does not care what comes after it on that line
class OpenEndedKeywordParser : public Parser {
public:
    OpenEndedKeywordParser(const QString keyword, InputFile &inputInterface, OutputFile &outputInterface) :
        Parser(inputInterface, outputInterface, keyword)
    {
    }

    QString getKeyword() {
        return keyword_;
    }

    bool checkKeyword(const QString keyword) {
        return getKeyword().startsWith(keyword);
    }

    bool parseToken(const QString &input) {
        return true;
    }
};

//// this checks for the keyword and does not care what comes after it on that line
//class varInitialiserParser : public Parser {
//public:
//    varInitialiserParser(const QString keyword, QStringList &cleanFile) : Parser::Parser(cleanFile)
//    {
//    }

//    bool parseToken(const QString &input, QStringList &opCodes) {
//        NoobIT it = noobVariables.find(input.section(' ',1,1));

//        if (it == noobVariables.end())
//            return false;

//        QString val = *it;

//        if ("R" == input.section(' ',3,3)) {
//            QString baseVal = input.section(' ',4,4);
//            bool ok;

//            int intVal = baseVal.toInt(&ok, 10);
//            if (ok) {
//                numbrVariables[val] = intVal;
//                return true;
//            }

//            float floatVal = baseVal.toFloat(&ok);
//            if (ok) {
//                numbarVariables[val] = floatVal;
//                return true;
//            }

//            if (baseVal == "TRUE")
//                troofVariables[val] = true;
//            else if (baseVal == "FALSE")
//                troofVariables[val] = true;
//            else
//                yarnVariables[val] = baseVal;

//            return true;
//        }
//        return false;
//    }
//};

class FileLevelParser : public Parser {
public:
    FileLevelParser(InputFile &inputInterface, OutputFile &outputInterface) : Parser::Parser(inputInterface, outputInterface, "") {
        OpenEndedKeywordParser *haiParser = new OpenEndedKeywordParser("HAI", inputInterface, outputInterface);
        OpenEndedKeywordParser *commentParser = new OpenEndedKeywordParser("BTW", inputInterface, outputInterface);
        MultiLineCommentParser *bigCommentParser = new MultiLineCommentParser("OBTW", inputInterface, outputInterface);
        VarParser *varParser = new VarParser("I HAS A", inputInterface, outputInterface);
        VarAssignmentParser *varAssignmentParser = new VarAssignmentParser(inputInterface, outputInterface);


        parserMap_.push_back(haiParser);
        parserMap_.push_back(commentParser);
        parserMap_.push_back(bigCommentParser);
        parserMap_.push_back(varParser);
        parserMap_.push_back(varAssignmentParser);

        closingTokens_.push_back("KTHXBYE");
    }

    bool parseFile() {

    }

    virtual bool parseToken(const QString &input) {
        QString currentToken = input;

        while (currentToken.size() > 0) {
            for (int i=0; i<closingTokens_.size(); ++i) {
                if (currentToken == closingTokens_[i]) {
                    return true;
                }
            }

            for (int i=0; i<parserMap_.size(); ++i) {
                if (parserMap_[i]->matchToken(currentToken)){
                    parserMap_[i]->parseToken(currentToken);
                    break;
                }
            }

            currentToken = inputInterface_.getNextStatement();
        }
//        QStringList tokens = input.split(' ');

//        MapIT it = parserMap_.find(tokens[0]);

//        // need to split character by space and then pass the entire thing though
//        // if we find a match
//        if (it == parserMap_.end())
//            return false;
//        return (*it)->parseToken(input, opCodes);

//        for (auto it = parserMap_.begin(); it != parserMap_.end(); ++it) {
//            // does the input token start with the vector value

//            if (input.startsWith("I HAS A")) {
//                noobVariables.insert(input.section(' ',3,3));
//                return true;
//            }
//        }

    }
};

class ParsingDirector {
    FileLevelParser *fileParser_;
    InputFile& inputFile_;
    OutputFile& outputFile_;

public:
    ParsingDirector(InputFile& inputFile, OutputFile& outputFile) :  inputFile_(inputFile), outputFile_(outputFile) {
        fileParser_ = new FileLevelParser(inputFile_, outputFile_);
    }

    void parseFile() {
        fileParser_->parseToken(inputFile_.getNextStatement());
    }
};



// Load in a file and begin parsing
int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    QStringList opCodes_;

    if (args.count() != 2)
    {
        std::cerr << "argument required" << std::endl;
        return 1;
    }

    QString inputFile = static_cast<QString>(argv[1]);

    // Take the input file, remove the excessive whitespace
    // and convert it into a string list
    InputFile inputInterface(inputFile);
    OutputFile outputInterface;

    // Send list to the file parser
    ParsingDirector parser(inputInterface, outputInterface);
    parser.parseFile();

    return a.exec();
}
