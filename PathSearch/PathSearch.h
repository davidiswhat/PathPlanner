#pragma once
#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/Tile.h"
#include "../Framework/TileSystem/TileMap.h"
#include <map>
#include <vector>
#include <utility>
#include <unordered_map>
#include <ctime>
#include <algorithm>
#include <list>
#include <queue>
#include <iostream>
//#include "../PriorityQueue.h"

using namespace std;
namespace ufl_cap4053
{
	namespace searches
	{
		class PathSearch
		{
		// CLASS DECLARATION GOES HERE
			private:
				bool done;
				//double scale;
				Tile* sTile;
				Tile* gTile;
				TileMap* tMap;
				//https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-user-defined-class-in-cpp/
				//https://stackoverflow.com/questions/2634690/good-hash-function-for-a-2d-index
				class MyHashFunction {
					public:
						size_t operator()(const Tile* t) const
						{
							return 53* t->getXCoordinate() + 53*t->getYCoordinate();
						}
				};
				//https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
				//https://stackoverflow.com/questions/15325730/priority-queue-of-custom-class-using-comparator-with-argument-on-constructor-in
				double distance(double x, double y, Tile* t) {
					double x1 = t->getXCoordinate();
					double y1 = t->getYCoordinate();
					double distance = sqrt(pow(x1 - x, 2) + pow(y1 - y, 2));
					return distance;
				}
				unordered_map<Tile*, vector<Tile*>, MyHashFunction>* searchGraph;
				unordered_map<Tile*, double, MyHashFunction>* gCosts;
				//unordered_map<Tile*, double, MyHashFunction>* hCosts;
				unordered_map<Tile*, double, MyHashFunction>* wholeCosts;
				//unordered_map<Tile*, pair<int,int>, MyHashFunction>* parents;
				unordered_map<Tile*,Tile*, MyHashFunction>* parents2;
				unordered_map<Tile*, int, MyHashFunction>* insideOpen;
				//unordered_map<Tile*, int, MyHashFunction>* cost;
				//priority_queue<Tile*, vector<Tile*>, CompareHeuristic> pq;
				priority_queue<pair<double, Tile*>>* pqUCS;
				//priority_queue<Tile*, vector<Tile*>, CompareHeuristic> copyForSolution;
				//deque<Tile*>* copyForSolution;
				vector<Tile const*>* solution;
				vector <Tile*>* visited;
				//vector <Tile*>* insideOpen;
				//list <Tile*> open;
				//vector <Tile*>* open2;
			public:
				DLLEXPORT PathSearch(); // EX: DLLEXPORT required for public methods - see platform.h
				DLLEXPORT ~PathSearch(); //The destructor should perform any final cleanup required before deletion of the object.
				DLLEXPORT void load(TileMap* _tileMap); //Called after the tile map is loaded.This is usually where the search graph is generated.
				DLLEXPORT void initialize(int startRow, int startCol, int goalRow, int goalCol); //Called before any update of the path planner; should prepare for search to be performed between the tiles at the coordinates indicated.
				DLLEXPORT void update(long timeslice); // Called to allow the path planner to execute for the specified timeslice(in milliseconds).Within this method the search should be performed until the time expires or the solution is found.If timeslice is zero(0), this method should only do a single iteration of the algorithm.Otherwise the update should only iterate for the indicated number of milliseconds.
				DLLEXPORT void shutdown(); // Called when the current search data is no longer needed.It should clean up any memory allocated for this search.Note that this is not the same as the destructor, as the search object may be reset to perform another search on the same map.
				DLLEXPORT void unload();// Called when the tile map is unloaded.It should clean up any memory allocated for this tile map.Note that this is not the same as the destructor, as the search object may be reinitialized with a new map.
				DLLEXPORT bool isDone() const; //const Returns true if the update function has finished because it found a solution, and false otherwise.
				DLLEXPORT std::vector<Tile const*> const getSolution() const;
		};
	}
}  // close namespace ufl_cap4053::searches


