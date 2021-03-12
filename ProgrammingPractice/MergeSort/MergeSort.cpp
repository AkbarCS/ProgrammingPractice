#include <iostream>
#include <vector>

using namespace std;

vector<int> mergeSort(vector<int> numbersToSort);
vector<int> merge(vector<int> lhsList, vector<int> rhsList);
vector<int> generateRandomNumbers(int numberOfRandomNumbersToGenerate);

vector<int> mergeSort(vector<int> numbersToSort)
{
    int size = numbersToSort.size();

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
        
        for (it = numbersToSort.begin(); it != numbersToSort.begin() + midpoint; it++)
        {
            lhsList.push_back(*it);
        }
        
        for (it; it != numbersToSort.end(); it++)
        {
            rhsList.push_back(*it);
        }

        return merge(mergeSort(lhsList), mergeSort(rhsList));
    }
    
    return vector<int>();
}

vector<int> merge(vector<int> lhsList, vector<int> rhsList)
{
    vector<int> mergedList = lhsList;
    
    int i = 0;

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

    return mergedList;
}

vector<int> generateRandomNumbers(int numberOfRandomNumbersToGenerate)
{
    vector<int> randomNumbers = vector<int>();

    int i = 0;
    for (i = 0; i < numberOfRandomNumbersToGenerate; i++)
    {
        randomNumbers.push_back(rand());
    }

    return randomNumbers;
}

int main()
{
    vector<int> sortedNumbers = mergeSort(generateRandomNumbers(3000));
}

