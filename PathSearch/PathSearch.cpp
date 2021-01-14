#pragma once
#include "PathSearch.h"


namespace ufl_cap4053
{
	namespace searches
	{
		// CLASS DEFINITION GOES HERE
		DLLEXPORT PathSearch::PathSearch()
		//:open3(isGreaterThan)
		{

		}
		DLLEXPORT PathSearch::~PathSearch()
		{
			/*if (searchGraph!=nullptr) {
				delete searchGraph;
			}
			
			delete gCosts;
			delete wholeCosts;
			delete insideOpen;
			delete parents;*/
			delete pqUCS;
			delete visited;
			delete solution;
		}

		DLLEXPORT void PathSearch::load(TileMap* _tileMap)
		{
			tMap = _tileMap;
			searchGraph = new unordered_map<Tile*, vector<Tile*>, MyHashFunction>;
			
			gCosts = new unordered_map<Tile*, double, MyHashFunction>;
			wholeCosts = new unordered_map<Tile*, double, MyHashFunction>;
			insideOpen = new unordered_map<Tile*, int, MyHashFunction>;
			//parents = new unordered_map<Tile*, pair<int, int>, MyHashFunction>;
			parents2 = new unordered_map<Tile*, Tile*, MyHashFunction>;

			for (int i = 0; i < _tileMap->getRowCount();i++) {
				for (int j = 0; j < _tileMap->getColumnCount();j++) {
					Tile* curr = _tileMap->getTile(i, j);
					if (curr != nullptr && curr->getWeight()!=0) {
						(*gCosts)[curr] = 0;
						//(*hCosts)[curr] = 0;
						(*wholeCosts)[curr] = 0;
						(*insideOpen)[curr] = 0;
						//(*parents)[curr] = make_pair(-1, -1);
						(*parents2)[curr] = nullptr;
						if (curr->getWeight() != 0) {
							vector<Tile*> neighbors;
							if ((curr->getRow()) % 2 == 0) {
								if (_tileMap->getTile(i - 1, j) != nullptr && _tileMap->getTile(i - 1, j)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i - 1, j));
								}
								if (_tileMap->getTile(i, j - 1) != nullptr && _tileMap->getTile(i, j - 1)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i, j - 1));
								}
								if (_tileMap->getTile(i + 1, j) != nullptr && _tileMap->getTile(i + 1, j)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i + 1, j));
								}
								if (_tileMap->getTile(i, j + 1) != nullptr && _tileMap->getTile(i, j + 1)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i, j + 1));
								}
								if (_tileMap->getTile(i - 1, j - 1) != nullptr && _tileMap->getTile(i - 1, j - 1)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i - 1, j - 1));
								}
								if (_tileMap->getTile(i + 1, j - 1) != nullptr && _tileMap->getTile(i + 1, j - 1)->getWeight() != 0)
									neighbors.push_back(_tileMap->getTile(i + 1, j - 1));
								//(*searchGraph)[_tileMap->getTile(i, j)] = neighbors;
								(*searchGraph).insert(make_pair(_tileMap->getTile(i, j), neighbors));
							}
							else if ((curr->getRow()) % 2 != 0) {
								if (_tileMap->getTile(i - 1, j) != nullptr && _tileMap->getTile(i - 1, j)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i - 1, j));
								}
								if (_tileMap->getTile(i, j - 1) != nullptr && _tileMap->getTile(i, j - 1)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i, j - 1));
								}
								if (_tileMap->getTile(i + 1, j) != nullptr && _tileMap->getTile(i + 1, j)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i + 1, j));
								}
								if (_tileMap->getTile(i, j + 1) != nullptr && _tileMap->getTile(i, j + 1)->getWeight() != 0) {
									neighbors.push_back(_tileMap->getTile(i, j + 1));
								}
								if (_tileMap->getTile(i + 1, j + 1) != nullptr && _tileMap->getTile(i + 1, j + 1)->getWeight() != 0)
									neighbors.push_back(_tileMap->getTile(i + 1, j + 1));
								if (_tileMap->getTile(i - 1, j + 1) != nullptr && _tileMap->getTile(i - 1, j + 1)->getWeight() != 0)
									neighbors.push_back(_tileMap->getTile(i - 1, j + 1));
								//(*searchGraph)[_tileMap->getTile(i, j)] = neighbors;
								(*searchGraph).insert(make_pair(_tileMap->getTile(i, j), neighbors));
							}
						}
					}
				}
			}
		}
		DLLEXPORT void PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol)
		{
			done = false;
			gTile = tMap->getTile(goalRow, goalCol);
			sTile = tMap->getTile(startRow, startCol);

			visited = new vector<Tile*>;
			solution = new vector<Tile const*>;
			/*CompareHeuristic comp((tMap->getTile(goalRow, goalCol))->getXCoordinate(), (tMap->getTile(goalRow, goalCol))->getYCoordinate(), scale);
			priority_queue<Tile*, vector<Tile*>, CompareHeuristic> pq2(comp);
			pq = pq2;*/
			pqUCS = new priority_queue<pair<double, Tile*>>;
			pqUCS->push(make_pair(0, tMap->getTile(startRow, startCol)));
			//insideOpen->push_back(tMap->getTile(startRow, startCol));
			visited->push_back(tMap->getTile(startRow, startCol));
			//open.push_back(tMap->getTile(startRow, startCol));
		}
		DLLEXPORT void PathSearch::update(long timeslice)
		{
			if (timeslice == 0) {
				//while loop in file
				while (pqUCS->size() != 0) {
					//while loop in file
					pair<double, Tile*> p = pqUCS->top();
					Tile* current = p.second; //acts as queue
					//p.first = p.first * -1;
					pqUCS->pop();
					(*insideOpen)[current] = 0;
					if (current == gTile) {
						done = true;
						//(*parents)[current] = make_pair(current->getRow(), current->getColumn());
						return;
					}
					for (int i = 0; i < (*searchGraph)[current].size();i++) {
						double newCosts = gCosts->at(current) + (*searchGraph)[current].at(i)->getWeight();
						if ((*parents2)[(*searchGraph)[current].at(i)] == nullptr)
							(*parents2)[(*searchGraph)[current].at(i)] = current;
						if (find(visited->begin(), visited->end(), (*searchGraph)[current].at(i)) == visited->end()) {
							double fHeuristic = distance(gTile->getXCoordinate(), gTile->getYCoordinate(), (*searchGraph)[current].at(i));
							(*gCosts)[(*searchGraph)[current].at(i)] = newCosts;
							visited->push_back((*searchGraph)[current].at(i));
							double totalCosts = fHeuristic + newCosts * tMap->getTileRadius() * 2 * 1.3;
							(*wholeCosts)[(*searchGraph)[current].at(i)] = totalCosts;
							pqUCS->push(make_pair(totalCosts * -1, (*searchGraph)[current].at(i)));
							(*insideOpen)[(*searchGraph)[current].at(i)] = 1;
						}
						else {
							if (newCosts < gCosts->at((*searchGraph)[current].at(i))) {
								(*gCosts)[(*searchGraph)[current].at(i)] = newCosts;
								double totalCosts = distance(gTile->getXCoordinate(), gTile->getYCoordinate(), (*searchGraph)[current].at(i)) + tMap->getTileRadius() * 2 * newCosts * 1.3;
								(*wholeCosts)[(*searchGraph)[current].at(i)] = totalCosts;
								//(*parents)[(*searchGraph)[current].at(i)] = make_pair(current->getRow(), current->getColumn());
								(*parents2)[(*searchGraph)[current].at(i)] = current;
								if ((*insideOpen)[(*searchGraph)[current].at(i)] == 0) {
									pqUCS->push(make_pair(totalCosts * -1, (*searchGraph)[current].at(i)));
									(*insideOpen)[(*searchGraph)[current].at(i)] = 1;
								}
							}
						}
					}
				}
			}
			else {
				//https://stackoverflow.com/questions/946167/what-is-the-best-way-to-exit-out-of-a-loop-after-an-elapsed-time-of-30ms-in-c
				clock_t endwait;
				endwait = clock() + (timeslice * 1000) * CLOCKS_PER_SEC;
				while (clock() < endwait && pqUCS->size()!=0)
				{
					//while loop in file
					pair<double, Tile*> p = pqUCS->top();
					Tile* current = p.second; //acts as queue
					//p.first = p.first * -1;
					pqUCS->pop();
					(*insideOpen)[current] = 0;
					if (current == gTile) {
						done = true;
						//(*parents)[current] = make_pair(current->getRow(), current->getColumn());
						return;
					}
					for (int i = 0; i < (*searchGraph)[current].size();i++) {
						double newCosts = gCosts->at(current) + (*searchGraph)[current].at(i)->getWeight();
						if((*parents2)[(*searchGraph)[current].at(i)] == nullptr)
							(*parents2)[(*searchGraph)[current].at(i)] = current;
						if (find(visited->begin(), visited->end(), (*searchGraph)[current].at(i)) == visited->end()) {
							double fHeuristic = distance(gTile->getXCoordinate(), gTile->getYCoordinate(), (*searchGraph)[current].at(i));
							(*gCosts)[(*searchGraph)[current].at(i)] = newCosts;
							visited->push_back((*searchGraph)[current].at(i));
							double totalCosts = fHeuristic + newCosts * tMap->getTileRadius() * 2*1.3;
							(*wholeCosts)[(*searchGraph)[current].at(i)] = totalCosts;
							pqUCS->push(make_pair(totalCosts* -1, (*searchGraph)[current].at(i)));
							(*insideOpen)[(*searchGraph)[current].at(i)] = 1;
						}
						else {
							if (newCosts < gCosts->at((*searchGraph)[current].at(i))) {
								(*gCosts)[(*searchGraph)[current].at(i)] = newCosts;
								double totalCosts = distance(gTile->getXCoordinate(), gTile->getYCoordinate(), (*searchGraph)[current].at(i)) + tMap->getTileRadius() * 2* newCosts*1.3;
								(*wholeCosts)[(*searchGraph)[current].at(i)] = totalCosts;
								//(*parents)[(*searchGraph)[current].at(i)] = make_pair(current->getRow(), current->getColumn());
								(*parents2)[(*searchGraph)[current].at(i)] = current;	
								if ((*insideOpen)[(*searchGraph)[current].at(i)] == 0 ) {
									pqUCS->push(make_pair(totalCosts * -1, (*searchGraph)[current].at(i)));
									(*insideOpen)[(*searchGraph)[current].at(i)] = 1;
								}
							}
						}
					}
				}
			}
		}
		DLLEXPORT void PathSearch::shutdown()
		{
			delete visited;
			delete solution;
			delete pqUCS;
			for (int i = 0; i < tMap->getRowCount();i++) {
				for (int j = 0; j < tMap->getColumnCount();j++) {
					Tile* curr = tMap->getTile(i, j);
					(*gCosts)[curr] = 0;
					//(*hCosts)[curr] = 0;
					(*wholeCosts)[curr] = 0;
					(*insideOpen)[curr] = 0;
					(*parents2)[curr] = nullptr;
					//(*parents)[curr] = make_pair(-1, -1);
				}
			}
			/*CompareHeuristic comp((tMap->getTile(goalRow, goalCol))->getXCoordinate(), (tMap->getTile(goalRow, goalCol))->getYCoordinate(), scale);
			priority_queue<Tile*, vector<Tile*>, CompareHeuristic> pq2(comp);
			pq = pq2;*/
		}
		DLLEXPORT void PathSearch::unload()
		{
			delete searchGraph;
			delete gCosts;
			delete wholeCosts;
			delete insideOpen;
			//delete parents;
			delete parents2;
		}
		DLLEXPORT bool PathSearch::isDone() const
		{
			return done;
		}
		DLLEXPORT std::vector<Tile const*> const PathSearch::getSolution() const
		{
			int count1 = 0;
			int count2 = 0;
			solution->push_back(gTile);
			Tile* current = gTile;

			while (current != sTile) {
				solution->push_back((*parents2)[current]);
				current = (*parents2)[current];
			}
			
			return (*solution);
		}

	}
}  // close namespace ufl_cap4053::searches
