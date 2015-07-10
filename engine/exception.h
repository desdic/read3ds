#ifndef __EXCEPTION_H__
#define  __EXCEPTION_H__

#include <string>

class EngineException {
public:
    std::string message;
    EngineException(const std::string m) { message = m;}
    std::string Report() { return message; }
};

#endif
