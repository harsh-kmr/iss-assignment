#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include <chrono>
#include <algorithm>
using namespace std::chrono;
using namespace std;

/**
 * @brief Do not modify this code. 
 * 
 */
#pragma GCC diagnostic ignored "-Wreturn-type"
extern auto fileIo = 0;


/**
 * @brief Do not modify this code. 
 * 
 */
void writeOutputToFile(const vector<int> group,string outputPath)
{
    auto start = high_resolution_clock::now();
    ofstream outputFile(outputPath, ios::app);
    if (!outputFile)
    {
        cerr << "Failed to open output file." << endl;
        return;
    }

    for (const auto &value : group)
    {
        // ignore the last comma
        if (value == group.back())
        {
            outputFile << value;
            break;
        }
        outputFile << value << ",";
    }
    outputFile << '\n';

    outputFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    fileIo += duration.count();
    cout<<"Time taken by file write: "<<duration.count()<<" microseconds"<<endl;
    cout<<"Total time taken by file write: "<<fileIo<<" microseconds"<<endl;

}


