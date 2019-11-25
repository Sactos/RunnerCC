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
	RunnerSystem(const Configuration& c) : _config(c) {
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

    virtual ~RunnerSystem() {
        while(!_interpreters.empty()) {
            auto interpreter = _interpreters.back();
            _interpreters.pop_back();
            delete interpreter;
        }
    }

    Interpreter* interpreter(const std::string& extension) const {
        for (auto interpreter : _interpreters) {
            if (interpreter->isCompilable(extension)) {
                return interpreter;
            }
        }
        return nullptr;
    }

private:
	const Configuration& _config;
    std::vector<Interpreter*> _interpreters;

};

#endif