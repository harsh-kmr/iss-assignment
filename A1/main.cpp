#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <chrono>
#include <sstream>
#include "user_code.h"
using namespace std;
using namespace std::chrono;

/**
 * @brief Do not modify this code.
 *
 */
void question1(string hashtagsPath, string purchasesPath, string pricesPath, int k, string outputPath)
{

    fileIterator hashtags(hashtagsPath);
    fileIterator purchases(purchasesPath);
    fileIterator prices(pricesPath);

    auto start = high_resolution_clock::now();

    groupCustomersByHashtags(hashtags, purchases, prices, k, outputPath);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto computeDuration = duration.count() - fileIo;
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "Total time taken: " << computeDuration << " microseconds" << endl;

    hashtags.reset();
    purchases.reset();
    prices.reset();
}

/**
 * @brief Do not modify this code.
 *
 */
void question2(string purchasesPath, string pricesPath, string groupsPath, string outputPath2)
{

    fileIterator purchases(purchasesPath);
    fileIterator prices(pricesPath);
    fileIterator groups(groupsPath);
   

    float avgExpense;
    ofstream outputFile(outputPath2);

    while (groups.hasNext())
    {

        string group = groups.next();
        stringstream ss(group);
        vector<int> customerList;

        // parse the string to get the list of customers
        int i;
        while (ss >> i)
        {
            customerList.push_back(i);
            if (ss.peek() == ',')
                ss.ignore();
        }

        // call function for each list of customers
        avgExpense = calculateGroupAverageExpense(customerList, purchases, prices);

        // write output to file
        outputFile << avgExpense << '\n';

        // reset the file pointers
        purchases.reset();
        prices.reset();

    }
    outputFile.close();
}

/**
 * @brief Process a batch of hashtags and write the output to a file.
 *
 * @param hashtags
 * @param purchases
 * @param prices
 * @param productToHashtagsList
 * @param fileSuffix
 * @param k
 */

void processBatch(
    fileIterator &hashtags,
    fileIterator &purchases,
    fileIterator &prices,
    vector<string> &productToHashtagsList,
    int &fileSuffix,
    int k,
    string outputFolderPath)
{
    string outputFilePath = outputFolderPath + "/output." + to_string(fileSuffix++) + ".csv";
    groupCustomersByHashtagsForDynamicInserts(hashtags, purchases, prices, productToHashtagsList, k, outputFilePath);
    productToHashtagsList.clear();
}

/**
 * @brief Do not modify this code.
 *
 */

void question3(
    string hashtagsPath, string purchasesPath, string pricesPath, string newHashtagPath, int k, string outputPath3)
{

    fileIterator hashtags(hashtagsPath);
    fileIterator purchases(purchasesPath);
    fileIterator prices(pricesPath);
    fileIterator newHashTags(newHashtagPath);
    auto start = high_resolution_clock::now();
    vector<string> productToHashtagsList;
    int fileSuffix = 0;
    int i = 0;
    int m = 5;

    // Initial call
    groupCustomersByHashtags(hashtags, purchases, prices, k, outputPath3 + "/output." + to_string(fileSuffix++) + ".csv");

    //reset the file pointers
    hashtags.reset();
    purchases.reset();
    prices.reset();


    // Process new hashtags in batches
    while (newHashTags.hasNext())
    {
        productToHashtagsList.push_back(newHashTags.next());
        if (++i == m)
        {
            i = 0;
            processBatch(hashtags, purchases, prices, productToHashtagsList, fileSuffix, k, outputPath3);
            hashtags.reset();
            purchases.reset();
            prices.reset();
        }
    }

    // Process any remaining hashtags if the batch size was not evenly divided
    if (!productToHashtagsList.empty())
    {
        processBatch(hashtags, purchases, prices, productToHashtagsList, fileSuffix, k, outputPath3);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto computeDuration = duration.count() - fileIo;
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "Total time taken: " << computeDuration << " microseconds" << endl;
}

//////////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THIS SECTION
//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Do not modify this code. FIXME add comments.
 *
 * @param argc
 * @param argv
 * @return int
 */

int main(int argc, char **argv)
{

    string hashtagsPath = argv[1];
    string purchasesPath = argv[2];
    string pricesPath = argv[3];
    string groupsPath = argv[4];
    string outputPath = argv[5];  // File path for output file
    string outputPath2 = argv[6]; // File path for output file
    string outputPath3 = argv[7]; //  Folder path for output files
    string newHashtagPath = argv[8];

    int k = 2; // Q1
    question1(hashtagsPath, purchasesPath, pricesPath, k, outputPath);

    question2(purchasesPath, pricesPath, groupsPath, outputPath2);

    question3(hashtagsPath, purchasesPath, pricesPath, newHashtagPath, k, outputPath3);
    return 0;
}
