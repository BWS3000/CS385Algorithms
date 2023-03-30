/*******************************************************************************
 * Filename: shortestpaths.cpp
 * Author  : Brian Sampson
 * Version : 1.0
 * Date    : December 4, 2022
 * Description: Finds shortest path in a graph from a text file
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const unsigned long long INF = 18446744000000000;

vector<string> stringSplit(const string &str)
{
    vector<string> result;
    istringstream iss(str);
    for (string s; iss >> s;)
    {
        result.push_back(s);
    }
    return result;
}

int testValues(vector<vector<string>> input)
{
    size_t verticies;
    int verticiesInt;

    for (size_t i = 0; i < input.at(0).at(0).size(); i++)
    {
        if (input.at(0).at(0)[i] > 57 || input.at(0).at(0)[i] < 49)
        {
            cerr << "Error: Invalid number of vertices '" << input.at(0).at(0) << "' on line 1." << endl;
            return -1;
        }
    }

    verticies = stoi(input.at(0).at(0));
    verticiesInt = stoi(input.at(0).at(0));

    size_t length = input.size();

    if (verticies == 0)
    {
        cerr << "Error: Invalid number of vertices '0' on line 1." << endl;
        return -1;
    }

    // Loop starts at the second line
    for (size_t i = 1; i < length; i++)
    {

        // Checking that every line has 3 attributes to it
        if (input.at(i).size() != 3)
        {
            string errorMessage;

            // Adding all the attributes on line i to errorMessage so its easier to print.
            for (size_t j = 0; j < input.at(i).size(); j++)
            {
                if (j < input.at(i).size() - 1)
                {
                    errorMessage += input.at(i).at(j) + " ";
                }
                else
                {
                    errorMessage += input.at(i).at(j);
                }
            }
            cerr << "Error: Invalid edge data '" << errorMessage << "' on line " << i + 1 << "." << endl;
            return -1;
        }
    }

    // Checking actual data of every line.
    for (size_t i = 1; i < length; i++)
    {
        char c;
        string currentStr;
        for (size_t j = 0; j < 2; j++)
        {
            currentStr = input.at(i).at(j);
            c = currentStr[0];

            for (size_t k = 0; k < (size_t)currentStr.size(); k++)
            {
                if ((input.at(i).at(j)[k] < 65 || input.at(i).at(j)[k] > 90) && j == 1)
                {
                    cerr << "Error: Ending vertex '" << currentStr << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                    return -1;
                }
                else if ((input.at(i).at(j)[k] < 65 || input.at(i).at(j)[k] > 90) && j == 0)
                {
                    cerr << "Error: Starting vertex '" << currentStr << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                    return -1;
                }
            }

            if (c - 65 > verticiesInt && j == 0)
            {
                cerr << "Error: Starting vertex '" << currentStr << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                return -1;
            }
            if (c - 65 > verticiesInt && j == 1)
            {
                cerr << "Error: Ending vertex '" << currentStr << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                return -1;
            }
        }
    }

    // Checking for verticies longer than 1 charecter
    for (size_t i = 1; i < length; i++)
    {
        string currentString;
        for (size_t j = 0; j < 2; j++)
        {
            currentString = input.at(i).at(j);

            for (size_t k = 0; k < (size_t)currentString.size(); k++)
            {
                if ((input.at(i).at(j)[k] < 65 || input.at(i).at(j)[k] > 90) && j == 1)
                {
                    cerr << "Error: Ending vertex '" << currentString << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                    return -1;
                }
                else if ((input.at(i).at(j)[k] < 65 || input.at(i).at(j)[k] > 90) && j == 0)
                {
                    cerr << "Error: Starting vertex '" << currentString << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                    return -1;
                }
            }

            if (currentString.size() > 1 && j == 1)
            {
                cerr << "Error: Ending vertex '" << currentString << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                return -1;
            }
            if (currentString.size() > 1 && j == 0)
            {
                cerr << "Error: Starting vertex '" << currentString << "' on line " << i + 1 << " is not among valid values A-" << char('Z' - (26 - verticies)) << "." << endl;
                return -1;
            }
        }
    }

    // Checks for valid edege weight
    for (size_t i = 1; i < length; i++)
    {
        for (size_t j = 0; i < (size_t)input.at(i).at(2)[j]; j++)
        {
            if (input.at(i).at(2)[j] > 57 || input.at(i).at(2)[j] < 48)
            {
                cerr << "Error: Invalid edge weight '" << input.at(i).at(2) << "' on line " << i + 1 << "." << endl;
                return -1;
            }
        }

        if (stoi(input.at(i).at(2)) < 1)
        {
            cerr << "Error: Invalid edge weight '" << input.at(i).at(2) << "' on line " << i + 1 << "." << endl;
            return -1;
        }
    }
    return 0;
}

vector<vector<long>> floyd(vector<vector<long>> input)
{
    int verticies = input.size();

    for (int x = 0; x < verticies; x++)
    {
        for (int y = 0; y < verticies; y++)
        {
            for (int z = 0; z < verticies; z++)
            {
                if (input.at(y).at(x) + input.at(x).at(z) < input.at(y).at(z))
                {
                    input.at(y).at(z) = input.at(y).at(x) + input.at(x).at(z);
                }
            }
        }
    }

    return input;
}

vector<vector<long>> intermediateGraph(vector<vector<long>> input)
{
    int verticies = input.size();

    vector<vector<long>> intermediateGraph;
    // Insert 0's at the diagnols

    // initialize weightGraph to all 0's
    vector<long> temp;
    for (int i = 0; i < verticies; i++)
    {
        temp.push_back(INF);
    }

    for (int i = 0; i < verticies; i++)
    {
        intermediateGraph.push_back(temp);
    }

    for (int x = 0; x < verticies; x++)
    {
        for (int y = 0; y < verticies; y++)
        {
            for (int z = 0; z < verticies; z++)
            {
                if (input.at(y).at(x) + input.at(x).at(z) < input.at(y).at(z))
                {
                    input.at(y).at(z) = input.at(y).at(x) + input.at(x).at(z);

                    intermediateGraph.at(y).at(z) = x;
                }
            }
        }
    }
    return intermediateGraph;
}

vector<vector<long>> createWeightGraph(vector<vector<string>> input)
{
    int verticies = stoi(input.at(0).at(0));
    vector<vector<long>> weightGraph;
    // Insert 0's at the diagnols

    // initialize weightGraph to all 0's
    vector<long> temp;
    for (int i = 0; i < verticies; i++)
    {
        temp.push_back(INF);
    }

    for (int i = 0; i < verticies; i++)
    {
        weightGraph.push_back(temp);
    }

    // Uses input to change weights at their proper indexes (except for diagnols)
    for (size_t i = 1; i < input.size(); i++)
    {
        int currentFrom = (input.at(i).at(0))[0] - 65;
        int currentTo = (input.at(i).at(1))[0] - 65;
        long currentWeight = stoi(input.at(i).at(2));

        weightGraph.at(currentFrom).at(currentTo) = currentWeight;
    }

    // Changes diagnols to 0
    for (int i = 0; i < verticies; i++)
    {
        weightGraph.at(i).at(i) = 0;
    }

    return weightGraph;
}

void pathsBetweenMatrixes(vector<vector<long>> IV, vector<vector<long>> PL)
{
    int verticies = PL.size();

    for (int i = 0; i < verticies; i++)
    {
        for (int j = 0; j < verticies; j++)
        {
            if(i == j) {
                cout << char('A' + i) << " -> " << char('A' + j) << ", distance: " << 0 << ", path: " << char('A' + i) << endl;
            } else {
            int verticies = IV.size();
            vector<int> currentPath;

            currentPath.push_back(i);
            currentPath.push_back(j);

            bool endOfPath = 0;
            while (!endOfPath)
            {
                for (size_t k = 0; k < currentPath.size() - 1; k++)
                {
                    long intermediate = IV.at(currentPath.at(k)).at(currentPath.at(k + 1));

                    if (intermediate < verticies)
                    {
                        currentPath.insert(currentPath.begin() + k + 1, intermediate);
                        endOfPath = false;
                        k--;
                    }
                    else if (intermediate >= verticies)
                    {
                        endOfPath = true;
                    }
                }
            }
            if((unsigned long long)PL.at(i).at(j) != INF) {
            cout << char('A' + i) << " -> " << char('A' + j) << ", distance: " << PL.at(i).at(j) << ", path: ";
            string pathString = "";
            for (size_t z = 0; z < currentPath.size(); z++)
            {
                if (z < currentPath.size() - 1)
                {
                    cout << char(currentPath.at(z) + 'A') << " -> ";
                }
                else
                {
                    cout << char(currentPath.at(z) + 'A');
                }
            }
            cout << endl;
            } else {
                cout << char('A' + i) << " -> " << char('A' + j) << ", distance: " << "infinity" << ", path: none" << endl;
            }
        }
    }
    }
}

void displayTable(vector<vector<long>> input, string label, const bool use_letters)
{
    long max_val = 0;
    int verticies = input.size();
    cout << label << endl;
    for (int i = 0; i < verticies; i++)
    {
        for (int j = 0; j < verticies; j++)
        {
            long cell = input.at(i).at(j);
            if ((unsigned long long)cell < INF && cell > max_val)
            {
                max_val = input.at(i).at(j);
            }
        }
    }

    int max_cell_width = use_letters ? to_string(max_val).size() : to_string(max(static_cast<long>(verticies), max_val)).size();

    cout << ' ';

    for (int j = 0; j < verticies; j++)
    {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }

    cout << endl;

    for (int i = 0; i < verticies; i++)
    {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < verticies; j++)
        {
            cout << " " << setw(max_cell_width);
            if ((unsigned long long)input.at(i).at(j) == INF)
            {
                cout << "-";
            }
            else if (use_letters)
            {
                cout << static_cast<char>(input.at(i).at(j) + 'A');
            }
            else
            {
                cout << input.at(i).at(j);
            }
        }
        cout << endl;
    }
    cout << endl;
}

void printDistanceMatrix(vector<vector<long>> input)
{
    string label = "Distance matrix:";
    displayTable(input, label, false);
}

void printFloyd(vector<vector<long>> floyd)
{
    string label = "Path lengths:";
    displayTable(floyd, label, false);
}

void printIntermediateVerticies(vector<vector<long>> intermediate)
{
    string label = "Intermediate vertices:";
    displayTable(intermediate, label, true);
}

int main(int argc, const char *argv[])
{
    // Make sure the right number of command line arguments exist.
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    // Create an ifstream object.
    ifstream input_file(argv[1]);
    // If it does not exist, print an error message.
    if (!input_file)
    {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    // Add read errors to the list of exceptions the ifstream will handle.
    input_file.exceptions(ifstream::badbit);
    string line;
    vector<vector<string>> lineVector;
    try
    {
        // Use getline to read in a line.
        // See http://www.cplusplus.com/reference/string/string/getline/
        while (getline(input_file, line))
        {
            vector<string> currentVector = stringSplit(line);

            lineVector.push_back(currentVector);
        }
        // Don't forget to close the file.
        input_file.close();
    }
    catch (const ifstream::failure &ff)
    {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    }
    if (testValues(lineVector) == -1)
    {
        return -1;
    }

    vector<vector<long>> weightGraph = createWeightGraph(lineVector);
    vector<vector<long>> floydGraph = floyd(weightGraph);
    vector<vector<long>> storeIntermediateGraph = intermediateGraph(weightGraph);
    printDistanceMatrix(weightGraph);
    printFloyd(floydGraph);
    printIntermediateVerticies(storeIntermediateGraph);

    pathsBetweenMatrixes(storeIntermediateGraph, floydGraph);

    return 0;
}
