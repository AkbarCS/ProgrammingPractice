#include <iostream>
#include <vector>

#include "MergeSortBasic.h"
#include "MergeSortMultithreading.h"

using namespace std;

vector<int>* generateRandomNumbers(int numberOfRandomNumbersToGenerate);
void performMergeSortThroughMultithreading();

vector<int> randomNumbers;

vector<int>* generateRandomNumbers(int numberOfRandomNumbersToGenerate)
{
    randomNumbers = vector<int>();

    int i = 0;
    for (i = 0; i < numberOfRandomNumbersToGenerate; i++)
    {
        randomNumbers.push_back(rand());
    }

    return &randomNumbers;
}

void performMergeSortThroughMultithreading()
{

}

int main()
{
    //vector<int>* sortedNumbers = mergeSortBasic(generateRandomNumbers(10000));
    //vector<int> sortedNumbersList = *sortedNumbers;
    performMergeSortThroughMultithreading();
}

