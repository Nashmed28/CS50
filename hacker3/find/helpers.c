/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Nabib Ahmed
 * nahmed@college.harvard.edu
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

#define LIMIT 65536

/**
 * Returns true if value is in array of n values, else false.
 * Uses a recursive binary search algorithm.
 */
bool search(int value, int values[], int n)
{
    // if values has size 0, can't search
    if (n == 0)
    {
        return false;
    }


    // if the "middle" element in values equals value, return true
    int middle = n / 2;
    if (values[middle] == value)
    {
        return true;
    }

    // if there are still more values in the array, then if the middle element
    // was lower/higher than what we are looking for, we recursively call search
    // on the upper/lower half (excluding the middle element) of the array with
    // the size of array cut from middle over in the first case and set equal to
    // middle in the latter
    else if (values[middle] < value)
    {
        return search(value, values + middle + 1, n - middle - 1);
    }
    else
    {
        return search(value, values, middle);
    }
}

/**
 * Sorts array of n values.  Returns true if successful, else false.
 * Implemented using counting sort.
 */
void sort(int values[], int n)
{
    // counting_array is of size LIMIT, equal to the highest random number
    // generate can output
    int counting_array[LIMIT] = { 0 };

    // iterate through values, incrementing counting array at the index that
    // is equal to values[i]
    for (int i = 0; i < n; i++)
    {
        counting_array[values[i]]++;
    }

    // the int stored in counting_array[idx] is the number of idx's in
    // "values"; if counting_array[0] == 6, then the first 6 elements of values
    // are set to 0, and so on
    for (int i = 0, idx = 0; i < n; idx++)
    {
        for (int j = 0; j < counting_array[idx]; j++)
        {
            values[i++] = idx;
        }
    }
}
