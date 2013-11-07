/* Definition of member functions of class FileReader */

#include "../headers/FileReader.hpp"

#include <string>
#include <fstream>
#include <vector>

using namespace std;


FileReader::FileReader(ErrorReporter* errorReporter) :
  m_nLines(0),
  m_linesText(),
  m_errorReporter(errorReporter)
{
}

FileReader::FileReader(const FileReader& source) :
  m_nLines(0),
  m_linesText(),
  m_errorReporter()
{
  m_instance = source.m_instance;
}

FileReader* FileReader::m_instance = nullptr;

FileReader::~FileReader()
{
  delete m_instance;
}

FileReader* FileReader::getInstance(ErrorReporter* errorReporter)
{
  if (m_instance != nullptr)
  {
    return new FileReader(errorReporter);
  }
  
  return m_instance;
}

void FileReader::readFile(const std::string& fileName)
{
  ifstream file;
  file.open(fileName);

  if (file.is_open())
  {
    string line;

    while (file.good())
    {
      getline(file, line);
      line += "\n";

      m_linesText.push_back(line);
      ++m_nLines;
    }
    file.close();
  }
  else
  {
    m_errorReporter->writeError("error al abrir archivo");
  }
}

size_t FileReader::getTotalLines() const
{
  return m_nLines;
}

string FileReader::getTextAtLine(int line)
{
  return m_linesText.at(line);
}

ErrorReporter& ErrorReporter::operator=(const ErrorReporter& rhs)
{
  m_instance = rhs.m_instance;

  return *this;
}

