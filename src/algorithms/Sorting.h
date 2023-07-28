#pragma once

#include "Simulation.h"

enum class SortType{BubbleSort,QuickSort,InsertionSort,SelectionSort, CocktailSort, MergeSort

};


class Sorting
{
private:

	static void ChangeValue(int list[], int pos, int newValue, Simulation* sim);
	static void merge(int list[], int left, int mid, int right, Simulation* sim);
	static void mergeSortRecursive(int list[], int left, int right, Simulation* sim);

	static void Swap(int list[], int i, int j, Simulation* sim);
	static int QuickSortRedistribution(int list[], int i, int j, Simulation* sim);
	static void QuickSortSection(int list[], int i, int j, Simulation* sim);
	static void BubbleSort(int list[], int numElems, Simulation* sim);
	static void QuickSort(int list[], int numElems, Simulation* sim);
	static void InsertionSort(int list[], int numElems, Simulation* sim);
	static void SelectionSort(int list[], int numElems, Simulation* sim);
	static void CocktailSort(int list[], int numElems, Simulation* sim);
	static void MergeSort(int list[], int numElems, Simulation* sim);
public:

	
	static void Sort(int* list, int numElems, Simulation* sim , SortType sortType);

};

