/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Brian Sampson
 * Version     : 1.0
 * Date        : 10/29/2022
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time.
 */

long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!
    vector<int> copy(length);

    return mergesort(array, &copy[0], 0, length - 1);
}

long count_inversions_slow(int array[], int length) {
    // TODO
    

    long inversions = 0;
    for(long i = 0; i < length; i++) {
        for(long j = i; j < length; j++) {
            if(array[j] < array[i]) {
                inversions++;
            }
        }
    }
    return inversions;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */


//Returns the inversion count of that recursive level
long merge(int array[], int scratch[], int low, int mid, int high) {
    long i1 = low;
    long i2 = mid + 1;
    long i = low;
    long inversions = 0;
    while(i1 <= mid && i2 <= high) {
        if(array[i1] <= array[i2]) {
            scratch[i++] = array[i1++];
        } else {
            scratch[i++] = array[i2++];
            inversions += mid - i1 + 1;
        }
    }

    while(i1 <= mid) {
        scratch[i++] = array[i1++];
    }

    while(i2 <= high) {
        scratch[i++] = array[i2++];
    }

    for(long z = low; z <= high; z++) {
        array[z] = scratch[z];
    }

    return inversions;
}


static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
   long mid;
   long inversionCount = 0;


   if(low < high) {
    mid = low + ((high - low) / 2);
    inversionCount += mergesort(array, scratch, low, mid) + mergesort(array, scratch, mid + 1, high) + merge(array, scratch, low, mid, high);
   }
   return inversionCount;
}


int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
        if(argc > 2 || argc < 1) {
       cerr << "Usage: ./inversioncounter [slow]";
       return -1;
    }

    string input;
    istringstream iss;

    if(argc == 2){
    iss.clear();
    iss.str(argv[1]);
    iss >> input;
    } else {
        input = "";
    }

    if(input != "slow" && input != "") {
        cerr << "Error: Unrecognized option '" << argv[1] << "'.";
        return -1;
    }


    iss.clear();
    cout << "Enter sequence of integers, each followed by a space: " << flush;

    
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }
    
    // TODO: produce output


    if(values.size() == 0) {
        cerr << "Error: Sequence of integers not received.";
        return -1;
    }

    if(input == "slow") {
        cout << "Number of inversions: " << count_inversions_slow(&values[0], values.size());
    } else {
        cout << "Number of inversions: " << count_inversions_fast(&values[0], values.size());
    }

    return 0;
}
