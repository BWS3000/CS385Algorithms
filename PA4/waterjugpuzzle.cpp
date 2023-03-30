/*******************************************************************************
 * Filename: waterjugpuzzle.cpp
 * Author  : Dimitrios Haralampopoulos & Brian Sampson
 * Version : 1.0
 * Date    : October 16, 2022
 * Description: Computes the water jug problem.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
using namespace std;

//Have variables for the capacity and goal water jugs
int A, B, C, goalA, goalB, goalC;

//This will break the BFS if the goal has been reached. See testGoalReached method is below.
int goalReached = 0;

//Class to set up every state of the BFS process
//We want to keep track of the current contents of a, b, c and the parent pointer so once we find a reached goal, we are able to ride the
//pointers all the way back up to the original function call amount.
//The directions string just keeps track of what jug gets poured into what. Ex. "Pour 5 gallons from C to B."

class State {
    private:
        int a;
        int b;
        int c;
        string directions;
    	State *parent;
    public:
		//Default Constructor
		State() {
			a = -1;
			b = -1;
			c = -1;
			directions = "";
			parent = nullptr;

		}
        //Constructor
        State(int _a, int _b, int _c, string _directions) : a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} {}

        //Get variables

		void setstate(State x){
			a = x.get_a();
			b = x.get_b();
			c = x.get_c();
			directions = x.getdir();
			parent = x.getParent();
		}

        int get_a() {
            return a;
        }

		void set_a(int x){
			a = x;
		}

        int get_b() {
            return b;
        }

		void set_b(int x){
			b = x;
		}

        int get_c() {
            return c;
        }

		void set_c(int x){
			c = x;
		}

		string getdir(){
			return directions;
		}

		void setdir(string s){
			directions = s;
		}

		void setParent(State* pointer) {
			parent = pointer;
		}

		State* getParent(){
			return parent;
		}

        // String representation of state in tuple form.
    	string to_string() {
        ostringstream oss;
        oss << directions << " (" << a << ", " << b << ", " << c << ")";
        return oss.str();
    	}



};

bool testGoalReached(int a, int b) {
	if(a == goalA && b == goalB) {
		return true;
	}
	return false;
}

//Things to note:
//We dont want to visit a new state with the same jug amounts as a state that has been already visited. (this causes infinite cycle)
//Once a state has the desired amounts, we want to ride pointers back up and put all the proper strings in a vector and reverse it so the test script likes the result
vector<State> BFS(int currentA, int currentB, int currentC) {
	//This will be the final path of states to get to the solution. To get this, follow the path back up the tree using pointers in each state until the pointer is null.
	vector<State> finalPath;

	//Make a queue
	queue<State> inQueue;

	//Creates a list of lists
	State** visited = new State*[A + 1];

	//Fill visited with 0's to start all elements as not visited
	for (int i = 0; i < A + 1; ++i) {
		visited[i] = new State[B + 1];
		// Fill the array with zeros.
		//fill(visited[i], visited[i] + B + 1, nullptr);
	}

	//Create the start state
	State cur(0, 0, currentC, "Initial state.");

	inQueue.push(cur);

	//BFS Algorithm goes in here. Working down the possibilites.
	while(!inQueue.empty()) {
		currentA = inQueue.front().get_a();
		currentB = inQueue.front().get_b();
		currentC = inQueue.front().get_c();
		string dir = inQueue.front().getdir();
		State* par = inQueue.front().getParent();

		cur.setstate(inQueue.front());
		inQueue.pop();

		if(testGoalReached(currentA, currentB)) {
			//The goal has been reached, set goalReached to true and break out of while loop.
			goalReached = true;
			break;
		}

		//Get front of queue and pop it (moving on to next in queue)
		

		//Check if it has been visited already (do this for every path)
		if(visited[currentA][currentB].get_a()!=-1) {
			continue;
		}

		visited[currentA][currentB].set_a(currentA);
		visited[currentA][currentB].set_b(currentB);
		visited[currentA][currentB].set_c(currentC);
		visited[currentA][currentB].setdir(dir);
		visited[currentA][currentB].setParent(par);

		//Pouring C into A
		//"A" means the capacity of A
		//General template for the other 5 cases...
		if (currentA < A && currentC != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newA = currentA + currentC;
			if (newA > A)
				newA = A;
			int newC = currentC - (newA - currentA);
			stringstream ss;

			if(newA - currentA > 1) {
				ss << newA - currentA;
				State newState(newA, currentB, newC, "Pour " + ss.str() + " gallons from C to A.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newA - currentA;
				State newState(newA, currentB, newC, "Pour " + ss.str() + " gallon from C to A.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}
		}

		//Pours from jug B to jug A.
		if (currentA < A && currentB != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newA = currentA + currentB;
			if (newA > A)
				newA = A;
			int newB = currentB - (newA - currentA);
			stringstream ss;

			if(newA - currentA > 1) {
				ss << newA - currentA;
				State newState(newA, newB, currentC, "Pour " + ss.str() + " gallons from B to A.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newA - currentA;
				State newState(newA, newB, currentC, "Pour " + ss.str() + " gallon from B to A.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}
		}

		//Pours from jug C to jug B.
		if (currentB < B && currentC != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newB = currentB + currentC;
			if (newB > B)
				newB = B;
			int newC = currentC - (newB - currentB);
			stringstream ss;

			if(newB - currentB > 1) {
				ss << newB - currentB;
				State newState(currentA, newB, newC, "Pour " + ss.str() + " gallons from C to B.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newB - currentB;
				State newState(currentA, newB, newC, "Pour " + ss.str() + " gallon from C to B.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}
		}

		//Pours from jug A to jug B.
		if (currentB < B && currentA != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newB = currentB + currentA;
			if (newB > B)
				newB = B;
			int newA = currentA - (newB - currentB);
			stringstream ss;

			if(newB - currentB > 1) {
				ss << newB - currentB;
				State newState(newA, newB, currentC, "Pour " + ss.str() + " gallons from A to B.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newB - currentB;
				State newState(newA, newB, currentC, "Pour " + ss.str() + " gallon from A to B.");
				//Set parent of new state
			    newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}

		}

		//Pours from jug B to jug C.
		if (currentC < C && currentB != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newC = currentB + currentC;
			if (newC > C)
				newC = C;
			int newB = currentB - (newC - currentC);
			stringstream ss;

			if(newC - currentC > 1) {
				ss << newC - currentC;
				State newState(currentA, newB, newC, "Pour " + ss.str() + " gallons from B to C.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newC - currentC;
				State newState(currentA, newB, newC, "Pour " + ss.str() + " gallon from B to C.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}
		}

		//Pours from jug A to jug C.
		if (currentA < C && currentA != 0) {
			//Make new state, set the pointer to current parent. push it to the queue.
			int newC = currentA + currentC;
			if (newC > C)
				newC = C;
			int newA = currentA - (newC - currentC);
			stringstream ss;

			if(newC - currentC > 1) {
				ss << newC - currentC;
				State newState(newA, currentB, newC, "Pour " + ss.str() + " gallons from A to C.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			} else {
				ss << newC - currentC;
				State newState(newA, currentB, newC, "Pour " + ss.str() + " gallon from A to C.");
				//Set parent of new state
				newState.setParent(&visited[currentA][currentB]);
				//push regardless
				inQueue.push(newState);
			}
		}
	}

	if(!goalReached){
		//No Solution.

		for (int i = 0; i <= A; ++i) {
		delete[] visited[i];
		}
		delete[] visited;

		return vector<State>();
	}

	//Need to actually add the solution to this vector
	//finalPath = finder(&top, finalPath);
	while (cur.getParent()!=nullptr){
		finalPath.insert(finalPath.begin(), cur);
		cur.setstate(*cur.getParent());
	}
	if (cur.getParent() == nullptr){
		finalPath.insert(finalPath.begin(), cur);
	}

	//Deleting everything in the heap thats no longer used.
	for (int i = 0; i <= A; ++i) {
		delete[] visited[i];
	}

	delete[] visited;

	return finalPath;
}


int main(int argc, char* argv[]) {
    istringstream iss;
    int input = 0;

    if(argc != 7) {
        cerr << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
        return 0;
    }

    for(int i = 1; i < argc - 3; i++) {
        iss.str(argv[i]);

        if(!(iss >> input) || input <= 0) {
            cerr << "Error: Invalid capacity '" << argv[i] << "' for jug " <<  char('A' + i - 1) << ".";
            return 0;
        }
        iss.clear();
    }


    for(int i = 3; i < argc; i++) {
        iss.str(argv[i]);
        if(!(iss >> input) || input < 0) {
            cerr << "Error: Invalid goal '" << argv[i] << "' for jug " <<  char('A' + i - 4) << "."; 
            return 0;
        }
        iss.clear();
    }

    

    //Making capacity variables
    iss.str(argv[1]);
    iss >> A;
    iss.clear();
    iss.str(argv[2]);
    iss >> B;
    iss.clear();
    iss.str(argv[3]);
    iss >> C;
    iss.clear();

    //Making goal variables
    iss.str(argv[4]);
    iss >> goalA;
    iss.clear();
    iss.str(argv[5]);
    iss >> goalB;
    iss.clear();
    iss.str(argv[6]);
    iss >> goalC;
    iss.clear();


    //Checks if goal > capacity of any jugs
    if(goalA > A) {
        cerr << "Error: Goal cannot exceed capacity of jug A.";
        return 0;
    } else if(goalB > B) {
        cerr << "Error: Goal cannot exceed capacity of jug B.";
        return 0;
    } else if(goalC > C) {
        cerr << "Error: Goal cannot exceed capacity of jug C.";
        return 0;
    }

    //Makes sure all goal states add up to C, otherwise end with error.
    if(goalA + goalB + goalC != C) {
        cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
        return 0;
    }


	vector<State> sol = BFS(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
	if (sol.empty()){
		cout << "No solution." << endl;
	} else {
		for (State& i: sol){
			cout << i.to_string() << endl;
		}
	}
}