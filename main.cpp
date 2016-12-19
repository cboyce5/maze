#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

//GLOBAL VARIABLES
int n;
map< int, vector< pair< int, string > > > graphDirections;
map< int, pair< int, int > > graphSearch;

//reverseDirection()
//	helper function to reverse direction when reading in file
string reverseDirection( string s ) {
	if (s == "N")
		return "S";
	else if (s == "E")
		return "W";
	else if (s == "S")
		return "N";
	else if (s == "W")
		return "E";
	else if (s == "NW")
		return "SE";
	else if (s == "NE")
		return "SW";
	else if (s == "SW")
		return "NE";
	else if (s == "SE")
		return "NW";
	else
		return "";
}

//readFile()
//	reads the input file and stores the directional data
void readFile( char* filename ) {
	ifstream input( filename );
	int a,b;
	string t;
	input >> n;
	while( input.peek() != EOF ) {	//while not at end of file
		input >> a >> b >> t;
		graphDirections[a].push_back(make_pair(b,t));	//store data
		graphDirections[b].push_back(make_pair(a,reverseDirection(t)));
	}	
}

//findReachableNodes()
//	finds nodes that are reachable using the following two rules:
//		-only move in straight line
//		-only move 3 bugs at a time
vector< int > findReachableNodes( int index ) {
	vector < int > reachable;

	//3 nested for loops to check the map at 3 levels
	for ( unsigned int i = 0; i < graphDirections[index].size(); i++ ) {
		
		//store direction and new index for better readability
		int secondIndex = graphDirections[index][i].first;
		string direction = graphDirections[index][i].second;
		
		//loop through next level of graph
		for ( unsigned int j = 0; j < graphDirections[secondIndex].size(); j++) {
			if ( direction == graphDirections[secondIndex][j].second ) {
				int thirdIndex = graphDirections[secondIndex][j].first;
				
				//loop though next level
				for ( unsigned int k = 0; k < graphDirections[thirdIndex].size(); k++ ) {
					if ( direction == graphDirections[thirdIndex][k].second ) {
						
						//store index that matches direction in return vector
						reachable.push_back( graphDirections[thirdIndex][k].first );
					}
				}
			}
		}
	}
	
	return reachable;
}

//printTraceBack()
//	prints the path of the parent pointers, only called when index 92 is found
void printTraceBack( int index ) {
	vector< int > path;
	path.push_back( index );
	
	//adds until first point
	while ( index != 0 ) {
		path.push_back( graphSearch[index].second );
		index = graphSearch[index].second;
	}
	
	//print in reverse order
	for ( unsigned int i = path.size() - 1; i >= 0; i-- ) {
		cout << path[i] << " ";
	}
	cout << endl;
}

//runBFS()
//	runs breadth first search to find a path to the lady spider
void runBFS() {
	//initialize distances and parents
	for (int i = 0; i <= n+1; i++) {
		graphSearch[i] = make_pair(-1,-1);
	}
	
	//create queue, store root index
	queue<int> q;
	q.push(0);
	
	//loop through while nodes remains
	while (!q.empty()) {
		int current = q.front();
		//cout << "Current: " << current << endl;
		q.pop();
		vector< int > reachable = findReachableNodes( current );
		for ( unsigned int i = 0; i < reachable.size(); i++ ) {
			//cout << "\tReachable: " << reachable[i] << " " << graphSearch[ reachable[i] ].first << " " << graphSearch[ reachable[i] ].second << endl;
			if ( graphSearch[ reachable[i] ].first == -1 ) {
				graphSearch[ reachable[i] ].first = graphSearch[current].first + 1;
				graphSearch[ reachable[i] ].second = current;
				q.push( reachable[i] );
			}
			if ( reachable[i] == 92 ) {
				printTraceBack(92);
				return;
			}
		}
	}
}

//main()
int main( int argc, char **argv ) {
	
	//pass in filename as argument when calling exe on CMD environment
	if ( argc != 2 ) {
		cout << "Please pass an input file as an argument." << endl;
		exit(-1);
	}
	else {
		readFile( argv[1] );
	}
	
	//running algorithm
	runBFS();
	
    exit(0);
}