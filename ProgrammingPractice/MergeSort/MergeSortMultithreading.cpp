#include <iostream>
#include <vector>
#include <thread>

#include "MergeSortMultithreading.h"

#define NUMBER_OF_THREADS 7

using namespace std;

//todo
//ongoing - odd numbers of elements and odd number of threads scenario
//insert in a better place
//tidy up mergeSortMultithreading method

vector<int> mergeSortMultithreading(vector<int> numbersToSort)
{
    int numberOfElements = numbersToSort.size();
    int subArraySize = numberOfElements / NUMBER_OF_THREADS;

    int numberOfRemainingElements = numberOfElements % (subArraySize * NUMBER_OF_THREADS);

    vector<int> subArrays[NUMBER_OF_THREADS+1];
    
    if (numberOfRemainingElements > 0)
    {
        vector<int>::iterator startOfSubArray = numbersToSort.end() - numberOfRemainingElements;
        vector<int>::iterator endOfSubArray = numbersToSort.end();

        subArrays[NUMBER_OF_THREADS] = vector<int>(startOfSubArray, endOfSubArray);
    }

    vector<thread> threads = vector<thread>();
    int i = 0;

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        vector<int>::iterator startOfSubArray = numbersToSort.begin() + (i * subArraySize);
        vector<int>::iterator endOfSubArray = startOfSubArray + subArraySize;

        subArrays[i] = vector<int>(startOfSubArray, endOfSubArray);

        threads.push_back(thread(&mergeSort, subArrays[i], &subArrays[i]));
    }

    i = 0;
    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        threads[i].join();
    }

    int numberOfMergedLists = NUMBER_OF_THREADS;

    if (numberOfMergedLists % 2 == 1)
    {
        //ongoing - odd numbers of elements and odd number of threads scenario
        //insert in a better place
        if (NUMBER_OF_THREADS % 2 == 1)
        {
            vector<int> tempVector = subArrays[NUMBER_OF_THREADS];
            tempVector.insert(tempVector.end(), subArrays[NUMBER_OF_THREADS - 1].begin(), subArrays[NUMBER_OF_THREADS - 1].end());
            subArrays[NUMBER_OF_THREADS] = tempVector;
        }

        numberOfMergedLists--;
    }

    vector<vector<int>> mergedLists = vector<vector<int>>();
    
    i = 0;
    for (i = 0; i < numberOfMergedLists; i++)
    {
        mergedLists.push_back(subArrays[i]);
    }
    
    numberOfMergedLists = numberOfMergedLists / 2;
       
    while (numberOfMergedLists > 0)
    {
        threads.clear();

        vector<vector<int>> mergedListsTmp = vector<vector<int>>();
        mergedListsTmp.resize(numberOfMergedLists);

        i = 0;
        for (i = 0; i < numberOfMergedLists; i++)
        {
            vector<int> mergedList = vector<int>();

            threads.push_back(thread(&merge, mergedLists[i * 2], mergedLists[(i * 2) + 1], &mergedListsTmp[i]));
            
            threads[i].join();            
        }

        numberOfMergedLists = numberOfMergedLists / 2; 

        mergedLists = mergedListsTmp;
    }

    if (numberOfRemainingElements > 0)
    {
        vector<int> mergedList = vector<int>();
        merge(mergedLists[0], subArrays[NUMBER_OF_THREADS], &mergedList);
        return mergedList;
    }

    return mergedLists[0];
}

void mergeSort(vector<int> numbersToSort, vector<int>* sortedNumbers)
{
    int size = numbersToSort.size();

    if (size == 1)
    {
        *sortedNumbers = numbersToSort;
    }
    else
    {
        vector<int>::iterator it;
        int midpoint = 0;

        if (size % 2 == 1)
        {
            midpoint = size / 2 + 1;
        }
        else
        {
            midpoint = size / 2;
        }

        vector<int> lhsList = vector<int>();
        vector<int> rhsList = vector<int>();

        for (it = numbersToSort.begin(); it != numbersToSort.begin() + midpoint; it++)
        {
            lhsList.push_back(*it);
        }

        for (it; it != numbersToSort.end(); it++)
        {
            rhsList.push_back(*it);
        }

        vector<int> mergedList = vector<int>();
                
        mergeSort(lhsList, &lhsList);
        mergeSort(rhsList, &rhsList);

        merge(lhsList, rhsList, &mergedList);

        *sortedNumbers = mergedList;
    }
}

void merge(vector<int> lhsList, vector<int> rhsList, vector<int>* mergedListToReturn)
{
    vector<int> mergedList = lhsList;

    unsigned int i = 0;

    for (i = 0; i < rhsList.size(); i++)
    {
        vector<int>::iterator it;
        bool elementAdded = false;

        for (it = mergedList.begin(); it != mergedList.end(); it++)
        {
            if (rhsList[i] <= *it)
            {
                mergedList.insert(it, rhsList[i]);
                elementAdded = true;
                break;
            }
        }

        if (!elementAdded)
        {
            mergedList.push_back(rhsList[i]);
        }
    }

    *mergedListToReturn = mergedList;
}