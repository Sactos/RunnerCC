#ifndef RUNNEREXCEPTION_CPP
#define RUNNEREXCEPTION_CPP

#include <string>
#include <exception>

class RunnerException: public std::exception {
public:
    RunnerException(const std::string m) {
		auto temporal = new char[m.size() + 1];
		std::copy(m.cbegin(), m.cend(), temporal);
		temporal[m.size()] = '\0';
		_message = temporal;
	}

    virtual const char* what() const throw() {
		return _message;
	}

private:
    const char* _message;
};

#endif