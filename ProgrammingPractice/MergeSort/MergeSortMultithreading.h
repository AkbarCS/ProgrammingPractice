#pragma once
#include <vector>

using namespace std;

vector<int> mergeSortMultithreading(vector<int> numbersToSort);
void mergeSort(vector<int> numbersToSort, vector<int>* sortedNumbers);
void merge(vector<int> lhsList, vector<int> rhsList, vector<int>* mergedList);