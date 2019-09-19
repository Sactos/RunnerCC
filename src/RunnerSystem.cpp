#ifndef RUNNERSYSTEM_CPP
#define RUNNERSYSTEM_CPP

#include "Configuration.cpp"
#include "Interpreters/Interpreter.cpp"
#include "Interpreters/Java.cpp"
#include "Interpreters/CPP.cpp"
#include "Interpreters/VisualCPP.cpp"
#include <string>
#include <vector>

class RunnerSystem {
public:
	RunnerSystem(Configuration& c) : _config(c) {
        auto aspirants = std::vector<Interpreter*>();
        aspirants.push_back(new Java(c));
#if _WINDOWS_   
        aspirants.push_back(new VisualCPP(c));
#endif
        aspirants.push_back(new CPP(c));
        for(auto interpreter : aspirants) {
            if(interpreter->isInterpreterAvailable()) {
                _interpreters.push_back(interpreter);
            }
        }
    }

	bool compileFile(const File& file, const string& pathIN, const string& pathOUT) {
        for(auto interpreter : _interpreters) {
            if(interpreter->isCompilable(file)) {
                return interpreter->compileFile(file, pathIN, pathOUT);
            }
        }
        return false;
	}

    bool runTest(const File& file, const string& pathIN, const string& pathOUT) {
        for(auto interpreter : _interpreters) {
            if(interpreter->isTestable(file)) {
                return interpreter->runTest(file, pathIN, pathOUT);
            }
        }
        return false;
    }

private:
	const Configuration& _config;
    std::vector<Interpreter*> _interpreters;

};

#endif