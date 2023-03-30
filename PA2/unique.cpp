/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Brian Sampson
 * Date        : 09/25/2022
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.

    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] < 'a' || s[i] > 'z') {
            return false;
        }
    }
    return true; 
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.

    // You MUST use only a single int for storage and work with bitwise
    // and bitshifting operators.  Using any other kind of solution will
    // automatically result in a grade of ZERO for the whole assignment.
    int storage = 0;

    for(size_t i = 0; i < s.size(); i++) {

        int currentIndex = s[i] - 'a';

        if ((storage & (1 << currentIndex)) > 0) {
            return false;
        } else {
            storage = storage | (1 << currentIndex);
        }

    }

    return true;

}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.

    if(argc == 1 || argc > 2) {
        cerr << "Usage: ./unique <string>";
        return 0;
    }

    if(!is_all_lowercase(argv[1])) {
        cerr << "Error: String must contain only lowercase letters.";
        return 0;
    }

    if(all_unique_letters(argv[1])) {
        cout << "All letters are unique.";
        return 0;
    } else {
        cout << "Duplicate letters found.";
        return 0;
    }
    return 0;

    
}
