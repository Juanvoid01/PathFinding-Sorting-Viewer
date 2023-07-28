#include "Sorting.h"


void Sorting::Swap(int list[], int i, int j, Simulation* sim) {

	if (i != j)
	{
		sim->addAction(new HighLightAction(i, j));
		sim->addAction(new SwapAction(i, j));
		sim->addAction(new HighLightAction(i, j));

		int aux = list[i];
		list[i] = list[j];
		list[j] = aux;
	}
}

void Sorting::ChangeValue(int list[], int pos, int newValue,Simulation* sim) {

	if (newValue != list[pos])
	{
		sim->addAction(new HighLightAction(pos, pos));
		sim->addAction(new ChangeValueAction(pos, newValue, list[pos]));
		sim->addAction(new HighLightAction(pos, pos));
		list[pos] = newValue;
	}
}

void Sorting::Sort(int list[], int numElems, Simulation* sim , SortType sortType) {

	sim->addAction(new NoneAction());

	switch (sortType)
	{
	case SortType::BubbleSort: BubbleSort(list,numElems,sim);
		break;
	case SortType::SelectionSort: SelectionSort(list, numElems, sim);
		break;
	case SortType::QuickSort: QuickSort(list, numElems, sim);
		break;
	case SortType::InsertionSort: InsertionSort(list, numElems, sim);
		break;
	case SortType::CocktailSort: CocktailSort(list, numElems, sim);
		break;
	case SortType::MergeSort: MergeSort(list, numElems, sim);
		break;
	default:
		break;
	}

	sim->addAction(new NoneAction());
}

void Sorting::BubbleSort(int list[], int numElems, Simulation* sim)
{
	int i = 0, j = 0;

	bool sorted = false;

	while (!sorted && i < numElems - 1)
	{
		sorted = true;
		for ( j = 0; j < numElems - 1; j++)
		{
			if (list[j] > list[j+1])
			{
				Swap(list, j, j + 1, sim);
				sorted = false;
			}
		}
	}

}

void Sorting::CocktailSort(int list[], int numElems, Simulation* sim) {
	bool notSorted = true;
	int start = 0;
	int end = numElems - 1;

	while (notSorted) {
		
		notSorted = false;

		// loop from left to right
		for (int i = start; i < end; i++) {
			if (list[i] > list[i + 1]) {
				Swap(list, i, i + 1, sim);
				notSorted = true;
			}
		}

		if (notSorted) {

			end--;

			// from right to left

			for (int i = end - 1; i >= start; i--) {
				if (list[i] > list[i + 1]) {
					Swap(list, i, i + 1, sim);
					notSorted = true;
				}
			}

			start++;
		}

		
	}
}

int Sorting::QuickSortRedistribution(int list[], int i, int j, Simulation* sim) {
	int inf = i + 1;
	int sup = j + 1;
	while (inf < sup) {
		if (list[inf] >= list[i]) {
			sup--;
			Swap(list, inf, sup, sim);
		}
		else {
			inf++;
		}
	}
	sup--;
	Swap(list,i,sup,sim);
	return sup;
}

void Sorting::QuickSortSection(int list[], int i, int j, Simulation* sim) {

	if (i < j) //Section is not empty
	{
		int p = QuickSortRedistribution(list, i, j, sim); //pivot election
		QuickSortSection(list, i, p - 1, sim);
		QuickSortSection(list, p + 1, j, sim);
	}
}

void Sorting::QuickSort(int list[], int numElems, Simulation* sim) {
	QuickSortSection(list, 0, numElems - 1, sim);
}

void Sorting::InsertionSort(int list[], int numElems, Simulation* sim) {

	int j = 0;

	for (int i = 1; i < numElems; i++)
	{
		j = i;

		while (j > 0 && list[j-1] > list[j])
		{
			Swap(list, j, j - 1, sim);
			j--;
		}

	}
}

void Sorting::SelectionSort(int list[], int numElems, Simulation* sim) {
	int min = 0;
	for (int i = 0; i < numElems - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < numElems; j++)
		{
			if (list[j] < list[min])
			{
				min = j;
			}
		}

		if (min != i)
		{
			Swap(list, i, min, sim);
		}
	}
}

void Sorting::merge(int list[], int left, int mid, int right, Simulation* sim) {


	int n1 = mid - left + 1;
	int n2 = right - mid;

	// Create temporary arrays to hold the left and right subarrays
	int* L = new int[n1];
	int* R = new int[n2];

	// Copy data to temporary arrays
	for (int i = 0; i < n1; ++i)
		L[i] = list[left + i];
	for (int j = 0; j < n2; ++j)
		R[j] = list[mid + 1 + j];

	// Merge the two temporary arrays back into the original array
	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {

		if (L[i] <= R[j]) {
			//list[k] = L[i];
			ChangeValue(list, k, L[i], sim);
			++i;
		}
		else {
			//list[k] = R[j];
			ChangeValue(list, k, R[j], sim);
			++j;
		}

		++k;
	}

	// Copy any remaining elements of L[] and R[] if there are any
	while (i < n1) {
		ChangeValue(list, k, L[i], sim);
		//list[k] = L[i];
		++i;
		++k;
	}

	while (j < n2) {
		ChangeValue(list, k, R[j], sim);
		//list[k] = R[j];
		++j;
		++k;
	}

	delete[] L;
	delete[] R;

	/*
	int i = left;
	int j = mid + 1;

	while (i <= mid && j <= right) {
		if (list[i] > list[j]) {
			// Swap the elements by moving the elements between [i, mid] one position ahead
			int temp = list[j];
			
			for (int k = j; k > i; --k) {
				//list[k] = list[k - 1];
				Swap(list, k, k - 1, sim);
			}

			sim->addAction(new HighLightAction(i, i));
			sim->addAction(new ChangeValueAction(i, temp, list[i]));
			sim->addAction(new HighLightAction(i, i));

			list[i] = temp;
			j++;
		}
		i++; 
	}
	*/
}

void Sorting::mergeSortRecursive(int list[], int left, int right, Simulation* sim) {
	if (left >= right)
		return;

	int mid = left + (right - left) / 2;

	mergeSortRecursive(list, left, mid, sim);
	mergeSortRecursive(list, mid + 1, right, sim);

	merge(list, left, mid, right, sim);
}


void Sorting::MergeSort(int list[], int numElems, Simulation* sim) {
	mergeSortRecursive(list, 0, numElems - 1, sim);
}


