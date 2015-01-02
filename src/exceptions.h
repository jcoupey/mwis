#ifndef Exception_H
#define Exception_H
#include <string>

class ArgsErrorException: public std::exception{
private:
  std::string _message;
public:
  ArgsErrorException(const std::string message):
    _message(message) {}

  virtual const char* what() const throw()
  {
    return _message.c_str();
  }
};

#endif
