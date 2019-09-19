#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include "../Configuration.cpp"
#include "../FSManager.cpp"
#include "../File.cpp"
#include <string>

class Interpreter {
public:
    virtual bool compileFile(const File& file, const std::string& pathIN, const std::string& pathOUT) = 0;

    virtual bool runTest(const File& file, const std::string& pathIN, const std::string& pathOUT) = 0;

    virtual bool isTestable(const File& file) const = 0;

    virtual bool isCompilable(const File& file) const = 0;

    virtual bool isInterpreterAvailable() const = 0;

protected:
    const string getExtraOptions(const std::string& options) const {
        if (options == "") {
            return "";
        }
        return "" + options + " ";
    }

};

#endif
