#ifndef USER_CODE_H
#define USER_CODE_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <sstream>
#include <set>
#include <chrono>
#include "fileIterator.h"
#include "fileWriter.h"
#include <unordered_set>


using namespace std;

class WeightedOrder {
private:
    map<string, int> weightedOrder;
    mutable bool isSorted = true;
    mutable vector<pair<string, int>> sortedOrder;

public:
    void add(const string& item) {
        weightedOrder[item]++;
        isSorted = false;
    }

    vector<pair<string, int>> sortMap() const {
        if (!isSorted) {
            sortedOrder = vector<pair<string, int>>(weightedOrder.begin(), weightedOrder.end());
            sort(sortedOrder.begin(), sortedOrder.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
                if (a.second == b.second) {
                    return a.first < b.first;
                }
                return a.second > b.second;
            });
            isSorted = true;
        }
        return sortedOrder;
    }

    void print() const {
        for (const auto& it : sortMap()) {
            cout << it.first << " ";
        }
        cout << endl;
    }

    string getTopK(int k) const {
        const auto& sorted = sortMap();
        set<string> topKSet;
        for (int i = 0; i < min(k, static_cast<int>(sorted.size())); ++i) {
            topKSet.insert(sorted[i].first);
        }
        stringstream ss;
        for (const auto& item : topKSet) {
            ss << item << " ";
        }
        return ss.str();
    }
};



//////////////////////////////////////////////////////////////////////////////////
// MODIFY THIS SECTION
//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Modify this code to solve the assigment. Include relevant document. Mention the prompts used prefixed by #PROMPT#.
 *        
 * 
 * @param hashtags 
 * @param purchases 
 * @param prices 
 */
void groupCustomersByHashtags(fileIterator& hashtags, fileIterator& purchases,fileIterator& prices, int k, string outputFilePath)
{
    //Use this to log compute time    
    auto start = high_resolution_clock::now();
    //  Write your code here
    map<int, vector<string>> productHashtags;
    while(hashtags.hasNext()) {
        string line = hashtags.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while(getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        int productId;
        try {
            productId = stoi(tokens[0]);
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid product ID in line: " << line << endl;
            continue;
        }
        
        for (int i = 1; i < tokens.size(); i++) {
            productHashtags[productId].push_back(tokens[i]);
        }
    }

    map<int, WeightedOrder> consumerOrders;
    while (purchases.hasNext()) {
        string line = purchases.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<int> tokens;
        while(getline(ss, token, ',')) {
            tokens.push_back(stoi(token));
        }
        
        int consumerId = tokens[0];
        int productId = tokens[1];
        
        if (productHashtags.find(productId) != productHashtags.end()) {
            for (const auto& hashtag : productHashtags[productId]) {
                consumerOrders[consumerId].add(hashtag);
            }
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by compute part of the function: "<< duration.count() << " microseconds" << endl;

    // Use the below utility function to write the output to a file
    // Call this function for every group as a vector of integers
    /*vector<int> group;
    writeOutputToFile(group, outputFilePath);*/
    map<string, vector<int>> groups;
    for (const auto& [consumerId, weightedOrder] : consumerOrders) {
        string topK = weightedOrder.getTopK(k);
        groups[topK].push_back(consumerId);
    }
    for (const auto& [group, consumers] : groups) {
        writeOutputToFile(consumers, outputFilePath);
    }
    return;
}

//////////////////////////////////////////////////////////////////////////////////
// MODIFY THIS SECTION
//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Modify this code to solve the assigment. Include relevant document. Mention the prompts used prefixed by #PROMPT#.
 *        
 * 
 * @param customerList 
 * @param purchases 
 * @param prices
 */
float calculateGroupAverageExpense(vector<int> customerList, fileIterator& purchases,fileIterator& prices){
    //Use this to log compute time    
    auto start = high_resolution_clock::now();
    //  Write your code here
    // Remove empty customer IDs
    customerList.erase(remove_if(customerList.begin(), customerList.end(), [](int id) { return id == 0; }), customerList.end());
    
    // Create a set of customer IDs for faster lookup
    unordered_set<int> customerSet(customerList.begin(), customerList.end());
    
    //print customerSet
    cout << "Customer Set: ";
    for (auto i = customerSet.begin(); i != customerSet.end(); i++) {
        cout << *i << " ";
    }
    cout << endl;


    // Load prices into a map for faster lookup
    unordered_map<int, float> priceMap;
    while (prices.hasNext()) {
        string line = prices.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string productIdStr, priceStr;
        getline(ss, productIdStr, ',');
        getline(ss, priceStr, ',');
        
        int productId = stoi(productIdStr);
        float price = stof(priceStr);
        priceMap[productId] = price;
    }

    // Process purchases and calculate total expense
    float totalExpense = 0.0;
    while (purchases.hasNext()) {
        string line = purchases.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string consumerIdStr, productIdStr;
        getline(ss, consumerIdStr, ',');
        getline(ss, productIdStr, ',');
        
        int consumerId = stoi(consumerIdStr);
        int productId = stoi(productIdStr);
        
        if (customerSet.find(consumerId) != customerSet.end()) {
            totalExpense += priceMap[productId];
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by this function: "<< duration.count() << " microseconds" << endl;

    return float(totalExpense / customerList.size());
}



//////////////////////////////////////////////////////////////////////////////////
// MODIFY THIS SECTION
//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Modify this code to solve the assigment. Include relevant document. Mention the prompts used prefixed by #PROMPT#.
 *        
 * 
 * @param hashtags 
 * @param purchases 
 * @param prices
 * @param newHashtags
 * @param k
 * @param outputFilePath
 */
void groupCustomersByHashtagsForDynamicInserts(fileIterator& hashtags, fileIterator& purchases, fileIterator& prices, vector<string> newHashtags, int k, string outputFilePath) {
    // Use this to log compute time    
    auto start = high_resolution_clock::now();

    // Write your code here
    map<int, vector<string>> productHashtags;
    map<int, WeightedOrder> consumerOrders;

    // Process existing hashtags
    while (hashtags.hasNext()) {
        string line = hashtags.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        int productId = stoi(tokens[0]);
        for (size_t i = 1; i < tokens.size(); i++) {
            productHashtags[productId].push_back(tokens[i]);
        }
    }

    // Process new hashtags
    for (const string& newHashtag : newHashtags) {
        stringstream ss(newHashtag);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        int productId = stoi(tokens[0]);
        for (size_t i = 1; i < tokens.size(); i++) {
            productHashtags[productId].push_back(tokens[i]);
        }
    }

    // Process purchases
    while (purchases.hasNext()) {
        string line = purchases.next();
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<int> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(stoi(token));
        }
        
        int consumerId = tokens[0];
        int productId = tokens[1];
        
        if (productHashtags.find(productId) != productHashtags.end()) {
            for (const auto& hashtag : productHashtags[productId]) {
                consumerOrders[consumerId].add(hashtag);
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by compute part of the function: " << duration.count() << " microseconds" << endl;

    // Group customers by their top k hashtags
    map<string, vector<int>> groups;
    for (const auto& [consumerId, weightedOrder] : consumerOrders) {
        string topK = weightedOrder.getTopK(k);
        groups[topK].push_back(consumerId);
    }

    // Write output to file
    for (const auto& [group, consumers] : groups) {
        writeOutputToFile(consumers, outputFilePath);
    }
}


#endif // USER_CODE_H
