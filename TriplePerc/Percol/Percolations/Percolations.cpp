// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "set"
//#include "stdlib"
#include "random"

class HexLattice
{
private:
	int size;
	std::set<std::pair<int, int>> GoodA;
	std::set<std::pair<int, int>> GoodB;
	std::set<std::pair<int, int>> GoodC;
	std::set<std::pair<int, int>> NotVisited;
	bool Perc = false;
	int RandState;

	void InitProbSubs()
	{
		GoodA.clear();
		GoodB.clear();
		GoodC.clear();
		NotVisited.clear();
		Perc = false;
		for (int i = 0; i <= size - 1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				RandState = distribution(gen);
				if (RandState == 0) {
					auto a = GoodA.insert((i, j));
					a = GoodB.insert((i, j));
					a = GoodC.insert((i, j));
				}
				else if (RandState == 1) {
					auto a = GoodA.insert((i, j));
				}
				else if (RandState == 2) {
					auto a = GoodB.insert((i, j));
				}
				else if (RandState == 3) {
					auto a = GoodC.insert((i, j));
				};
			}
		}
		for (int i = size; i <= 2 * size - 2; i++) {
			for (int j = i - size + 1; j <= 2 * size - 2; j++) {
				RandState = distribution(gen);
				if (RandState == 0) {
					auto a = GoodA.insert((i, j));
					a = GoodB.insert((i, j));
					a = GoodC.insert((i, j));
				}
				else if (RandState == 1) {
					auto a = GoodA.insert((i, j));
				}
				else if (RandState == 2) {
					auto a = GoodB.insert((i, j));
				}
				else if (RandState == 3) {
					auto a = GoodC.insert((i, j));
				};
			}
		}
	}
	std::set<std::pair<int, int>> GetNeighbours(int x, int y) {
		return { (x - 1, y - 1),(x - 1, y),(x, y - 1),(x + 1, y),(x, y + 1),(x + 1, y + 1) };
	}

	bool IsInBoard(int x, int y) { //проверяем лежит ли точка (x, y) на границе шестиугольника.
		return (x == 0) || (x == 2 * size - 2) || (y == 0) || (y == 2 * size) || (x - y == size) || (y - x == size);
	}

	bool CheckPercolA() {
		NotVisited = GoodA;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		return BoardVisited;
	}

	bool CheckPercolA() {
		NotVisited = GoodB;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		return BoardVisited;
	}

	bool CheckPercolA() {
		NotVisited = GoodC;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		return BoardVisited;
	}

	void dfs(int x, int y) {
		std::set<std::pair<int, int>> part;
		std::set<std::pair<int, int>> Peers = GetNeighbours(x, y);
		// описываем пересечение множества соседей и множества непосещенных вершин.
		for (auto it = Peers.begin(); it != Peers.end(), ++it) {
			if (NotVisited.find(*it) != NotVisited.end()) {
				part.insert(*it);
				NotVisited.erase(*it);
			}
		}
		for (auto it = part.begin(); it != part.end(), ++it) {
			if (IsInBoard(*it[0], *it[1])) {
				Perc = true;
			} else {
				dfs(it->first, it->second);
			};
		}
	}
public:
	explicit HexLattice(int n)
	{

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(0, 3);
		size = n;
		Perc = false;
		RandState = distribution(gen);
	}

};

int main()
{
	return 0;
}



