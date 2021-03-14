#include <iostream>
#include <vector>

#include "MergeSortBasic.h"

using namespace std;

vector<int> mergedList = vector<int>();

vector<int>* mergeSortBasic(vector<int> *numbersToSort)
{
    vector<int> toSort = *numbersToSort;

    int size = toSort.size();

    if (size == 1)
    {
        return numbersToSort;
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

        for (it = toSort.begin(); it != toSort.begin() + midpoint; it++)
        {
            lhsList.push_back(*it);
        }

        for (it; it != toSort.end(); it++)
        {
            rhsList.push_back(*it);
        }

        vector<int>* (*mergeSortLHS)(vector<int> *leftList){ &mergeSortBasic };
        vector<int>* (*mergeSortRHS)(vector<int> *rightList) { &mergeSortBasic };
        vector<int>* (*mergeLists)(vector<int> *leftList, vector<int> *rightList) { &mergeBasic };

        lhsList = *mergeSortLHS(&lhsList);
        rhsList = *mergeSortRHS(&rhsList);

        return mergeLists(&lhsList, &rhsList);
    }

    vector<int> emptyList = vector<int>();
    return &emptyList;
}

vector<int>* mergeBasic(vector<int> *lhsList, vector<int> *rhsList)
{
    mergedList = *lhsList;
    vector<int> rightHandList = *rhsList;

    unsigned int i = 0;

    for (i = 0; i < rightHandList.size(); i++)
    {
        vector<int>::iterator it;
        bool elementAdded = false;

        for (it = mergedList.begin(); it != mergedList.end(); it++)
        {
            if (rightHandList[i] <= *it)
            {
                mergedList.insert(it, rightHandList[i]);
                elementAdded = true;
                break;
            }
        }

        if (!elementAdded)
        {
            mergedList.push_back(rightHandList[i]);
        }
    }

    return &mergedList;
}