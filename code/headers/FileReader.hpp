/* Declaration of class FileReader */

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <fstream>
#include <vector>

// Forward declaration for avoind dependency problems.
class ErrorReporter;
#include "ErrorReporter.hpp"


class FileReader
{
public:
  static FileReader* getInstance();

  FileReader&              operator=(const FileReader& rhs);

  void        readFile(const std::string& fileName);
  size_t      getTotalLines() const;
  std::string getTextAtLine(int line);
  void        setErrorReporter(ErrorReporter* errorRepoter);

private:
  FileReader();
  FileReader(const FileReader& source);
  ~FileReader();

  size_t                   m_nLines;
  std::vector<std::string> m_linesText;
  ErrorReporter*           m_errorReporter;

  static FileReader* m_instance;
};

#endif /* FILEREADER_HPP */

