/*******************************************************************************
 * Filename: sqrt.cpp
 * Author  : Brian Sampson
 * Version : 1.0
 * Date    : September 12, 2022
 * Description: Computes the square root using newtons algorithm.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <typeinfo>
#include <iomanip>
using namespace std;

double newtonSqrt(double lastGuess, double epsilon) {
    double nextGuess;
    double n = lastGuess;

    while(true) {
        nextGuess = (lastGuess + n / lastGuess) / 2;

        if(abs(nextGuess - lastGuess) < epsilon){
            break;
        } else {
            lastGuess = nextGuess;
        }
    }
    return nextGuess;
}

int main(int argc, char* argv[]) {
    istringstream iss; //Input string stream iss
    double num;
    double epsilon;

    if (argc > 3 || argc < 2) {
        cerr << "Usage: ./sqrt <value> [epsilon]" << endl;
        return -1;
    }

    iss.str(argv[1]);
    if(!(iss >> num) || num < 0) {
        if(num < 0) {
            cerr << "nan" << endl;
            return -1;
        }

        cerr << "Error: Value argument must be a double." << endl;
        return -1;
    }

    if(argc == 2) {
        epsilon = 0.0000001;

    } else if(argc == 3) {
        iss.clear();
        iss.str(argv[2]);
        if(!(iss >> epsilon) || epsilon <= 0) {
            cerr << "Error: Epsilon argument must be a positive double." << endl;

            return -1;
        }
    }
    cout << fixed;
    cout << setprecision(8);

    if (num == 0) {
        cout << 0.00000000 << endl;
        return -1;
    }

    cout << newtonSqrt(num, epsilon) << endl;
    return -1;
}



