/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Brian Sampson
 * Date        : 10/04/2022
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;
vector<vector<int>> global;

vector<int> recurse(vector<int> vec, int num_stairs) {
    if(num_stairs <= 0) {
        global.push_back(vec);
    } if(num_stairs >= 1) {
        vec.insert(vec.begin(), 1);
        recurse(vec, num_stairs - 1);
    } if(num_stairs >= 2) {
        vec.erase(vec.begin());
        vec.insert(vec.begin(), 2);
        recurse(vec, num_stairs - 2);
    } if(num_stairs >= 3) {
        vec.erase(vec.begin());
        vec.insert(vec.begin(), 3);
        recurse(vec, num_stairs - 3);
    }
    return vec;
} 


vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
    vector<int> empty;

    recurse(empty, num_stairs);
    
    for(size_t i = 0; i < global.size(); i++) {
       reverse(global[i].begin(), global[i].end());
    }

    return global;
}



void display_ways(const vector< vector<int> > &ways, int input) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.

    //setting up for setw
    int width = global.size();
    int counter = 0;
    while(width > 0) {
        width /= 10;
        counter++;
    }


    if(input == 1) {
        cout << "1 way to climb 1 stair." << endl;
    } else {
        cout << global.size() << " ways to climb " << input << " stairs." << endl;
    }

    for(size_t i = 0; i < global.size(); i++) {
        cout << setw(counter) << i + 1 << ". ";
        for(size_t j = 0; j < global.at(i).size(); j++) {
            if(j == 0) {
                cout << "[";
            }
            
            cout << global.at(i).at(j);

            if(j != global.at(i).size() - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";
    }

}

int main(int argc, char * const argv[]) {
    istringstream iss;
    int input;

    if (argc != 2) {
        cerr << "Usage: ./stairclimber <number of stairs>" << endl;
        return 0;
    }

    iss.str(argv[1]);
    if (!(iss >> input) || input <= 0) {
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 0;
    }
    iss.clear();
    //Clears test cases


    get_ways(input);

    display_ways(global, input);

    return 0;
}
