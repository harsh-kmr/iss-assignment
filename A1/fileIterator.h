#include <fstream> // Add this line to include the <fstream> header file

using namespace std;

#include <iostream>



/**
 * @brief Do not modify this code. 
 * 
 * @param path 
 * 
 */
class fileIterator
{
    ifstream file;
    string line;

    // create constructor. open the file and read the first line.
public:
    fileIterator(string path)
    {
        file.open(path);
        if (!file)
        {
            cerr << "Failed to open file." << endl;
            return;
        }
        getline(file, line);
    }

    // create destructor. close the file.
    ~fileIterator()
    {
        file.close();
    }

    // next() function to read the next line in the file. return null if end of file.
public:
    string next()
    {
        if (file.eof())
        {
            return NULL;
        }
        getline(file, line);
        return line;
    }
    // hasNext() function to check if there is a next line in the file.
public:
    bool hasNext()
    {
        return !file.eof();
    }

    // reset() function to reset the file pointer to the beginning of the file.
public:
    void reset()
    {
        file.clear();
        file.seekg(0, ios::beg);
        getline(file, line);
    }
};
