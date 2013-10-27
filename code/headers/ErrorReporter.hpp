/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

class ErrorReporter
{
public:
  ErrorReporter();
  void writeError();
  void writeLexicalError();
private:
  ofstream m_errorOut;
};

#endif /* ERROR_REPORTER_HPP */

