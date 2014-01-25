/* Definition of member functions of class FileReader */

#include "../headers/FileReader.hpp"

#include <string>
#include <fstream>
#include <vector>

using namespace std;


FileReader::FileReader() :
  m_nLines(0),
  m_linesText(),
  m_errorReporter(nullptr)
{
}

FileReader* FileReader::m_instance = nullptr;

FileReader::~FileReader()
{
  delete m_instance;
}

FileReader* FileReader::getInstance()
{
  if (m_instance == nullptr)
  {
    m_instance = new FileReader();
    return m_instance;
  }
  
  return m_instance;
}

void FileReader::readFile(const std::string& fileName)
{
  ifstream file;
  file.open(fileName);

  m_currentFileName = fileName;

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

void FileReader::setErrorReporter(ErrorReporter* errorReporter)
{
  m_errorReporter = errorReporter;
}

string FileReader::getCurrentFileName() const
{
  return m_currentFileName;
}

size_t FileReader::getTotalLines() const
{
  return m_nLines;
}

string FileReader::getTextAtLine(int line)
{
  return m_linesText.at(line);
}

