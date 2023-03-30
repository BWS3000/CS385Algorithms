/*******************************************************************************
 * Name        : fastmult.cpp
 * Author      : Brian Sampson
 * Date        : 11/25/2022
 * Description : Does karatsuba multiplication by doing string addition and subtraction.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
using namespace std;

//Goal:
//Get 2 very long base-10 decimmal strings in the command line in the form of strings.
//Input strings range from size 1 to 50,000 digits. Holy shit?
//Must use the karatsuba fast mult method O(n^~1.59)

//Pads both string with an equal amount of 0's infront.
//Returns max length
//Ex. in1 = 999, in2 = 1
//Output: 3
int equalPadding(string &a, string &b) {
    int lengthA = a.size();
    int lengthB = b.size();

    if(lengthA < lengthB) {
        for(int i = lengthA; i < lengthB; i++) {
            a = "0" + a;
        }
        return a.size();
    } else {
        for(int i = lengthB; i < lengthA; i++) {
            b = "0" + b;
        }
        return b.size();
    }
    return -1;
}

//Removes the padded zeros that lead other digits in a string
void removePaddedZeros(string &a) {
    int nearestIndex = min(a.find_first_not_of('0'), a.size()-1);
    a.erase(0, nearestIndex);
}

string add(string leftSide, string rightSide) {
    int length = equalPadding(leftSide, rightSide) - 1;
    int currentCarry = 0;
    int currentSum;

    string ret;
    
    for (int i = length; i >= 0; i--) {
        int left = leftSide[i] - '0';
        int right = rightSide[i] - '0';

        currentSum = left + right + currentCarry;
        currentCarry = currentSum / 10;

        //In case of a remainder
        ret.insert(0, to_string(currentSum % 10));
    }

    //When there is a carry, insert it.
    if(currentCarry) {
        ret.insert(0, to_string(currentCarry));
    }

    removePaddedZeros(ret);
    return ret;
}

string subtract(string leftSide, string rightSide) {
    int length = equalPadding(leftSide, rightSide) - 1;
    int difference;
    string ret;

    for(int i = length; i >= 0; i--) {
        difference = (leftSide[i] - '0') - (rightSide[i] - '0');
        if(difference >= 0) {
            ret.insert(0, to_string(difference));
        } else {

            int moveOver = i - 1;

            while(moveOver >= 0) {
                char moveOverVal = leftSide[moveOver];
                //Substituting value. (Getting bigger nuber so subtration is possible)
                int substituteVal = ((moveOverVal - '0') - 1) % 10;
                leftSide[moveOver] =  substituteVal + '0';

                if(leftSide[moveOver] != '9') {
                    break;
                }

                moveOver--;
            }
            ret.insert(0, to_string(difference + 10));
        }
    }
    removePaddedZeros(ret);
    return ret;
}

string multiply(string leftSide, string rightSide) {
    int length = equalPadding(leftSide, rightSide);

    //Basecase
    if (length == 1) {
        return to_string((leftSide[0]-'0')*(rightSide[0]-'0'));
    }

    string leftSide1 = leftSide.substr(0,length/2);
    string leftSide2 = leftSide.substr(length/2,length-length/2);
    string rightSide2 = rightSide.substr(length/2,length-length/2);
    string rightSide1 = rightSide.substr(0,length/2);
    
    string mult1 = multiply(leftSide1,rightSide1);
    string mult2 = multiply(leftSide2,rightSide2);
    string mult3_1 = add(leftSide1,leftSide2);
    string mult3_2 = add(rightSide1,rightSide2);
    string mult3 = multiply(mult3_1 , mult3_2);
    string mult4 = subtract(mult3,add(mult1,mult2));    

    int diffLength = length - length / 2;
    for (int i = 0; i < diffLength; i++)
        mult4.append("0");
    for (int i = 0; i < diffLength * 2; i++)
        mult1.append("0");
        
    string ret = add(mult1,mult2);
    ret = add(ret, mult4);

    removePaddedZeros(ret);
    return ret;
}


int main(int argc, char *argv[]) {
    string a = argv[1];
    string b = argv[2];

    cout << multiply(a, b) << endl;

    return -1;
}