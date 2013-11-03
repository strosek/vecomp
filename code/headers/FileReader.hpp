/* Declaration of class FileReader */

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <fstream>
#include <vector>

#include "ErrorReporter.hpp"


class FileReader
{
public:
  FileReader* getInstance(ErrorReporter* errorReporter);

  void        readFile(const std::string& fileName);
  std::string getTextAtLine(int line);

private:
  FileReader(ErrorReporter* errorReporter);
  FileReader(const FileReader& source);
  ~FileReader();

  std::vector<std::string> m_linesText;
  ErrorReporter*           m_errorReporter;

  FileReader*              m_instance;

  FileReader&              operator=(const FileReader& rhs);
};

#endif /* FILEREADER_HPP */

