// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "set"
#include "stdlib"

class HexLattice
{
private:
	int size;
	std::set<std::pair<int, int>> GoodA;
	std::set<std::pair<int, int>> GoodB;
	std::set<std::pair<int, int>> GoodC;
	std::set<std::pair<int, int>> NotVisited;
	bool Perc = false;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 3);

	void InitProbSubs()
	{
		GoodA.clear();
		GoodB.clear();
		GoodC.clear();
		NotVisited.clear();
		Perc = false;
		for (int i = 0; i <= size-1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				int RandState = distribution(generator);
				if RandState == 0 {
					GoodA.insert((i, j));
					GoodB.insert((i, j));
					GoodC.insert((i, j));
				}
				else if RandState == 1 {
					GoodA.insert((i, j));
				}
				else if RandState == 2 {
					GoodB.insert((i, j));
				}
				else if RandState == 3 {
					GoodC.insert((i, j));
				};
			}
		}
		for (int i = size; i <= 2*size - 2; i++) {
			for (int j = i-size+1; j <= 2*size - 2; j++) {
				int RandState = distribution(generator);
				if RandState == 0 {
					GoodA.insert((i, j));
					GoodB.insert((i, j));
					GoodC.insert((i, j));
				}
				else if RandState == 1 {
					GoodA.insert((i, j));
				}
				else if RandState == 2 {
					GoodB.insert((i, j));
				}
				else if RandState == 3 {
					GoodC.insert((i, j));
				};
			}
		}
	}
	std::set<std::pair<int, int>> GetNeighbours(int x, y) {
		return {(x-1, y-1),(x-1, y),(x, y-1),(x+1, y),(x, y+1),(x+1, y+1)}
	}

	bool IsInBoard(int x, y) { //проверяем лежит ли точка (x, y) на границе шестиугольника.
		return (x == 0) || (x == 2 * size - 2) || (y == 0) || (y == 2 * size) || (x - y == size) || (y - x == size);
	}

	bool[3] CheckPercol() {
		bool[3] resres = [false, false, false];
		NotVisited = GoodA;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		resres[0] = BoardVisited;
		NotVisited = GoodB;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		resres[1] = BoardVisited;
		NotVisited = GoodC;
		Perc = false;
		dfs(self.size - 1, self.size - 1);
		resres[2] = self.BoardVisited);
		return resres;
	}

	void dfs(int x, y) {
		std::set<std::pair<int, int>> part;
		std::set<std::pair<int, int>> Peers = GetNeighbours(x, y);
		// описываем пересечение множества соседей и множества непосещенных вершин.
		for (auto it = Peers.begin(); it != Peers.end(), ++it) {
			if (NotVisited.find(it) != NotVisited.end()) {
				part.insert(it);
				NotVisited.erase(it);
			}
		}
		for (auto it = part.begin(); it != part.end(), ++it) {
			if IsInBoard(it[0], it[1]) {
				Perc = true;
			}
			else {
				dfs(it[0], it[1]);
			}
		}
	}
public:
	explicit HexLattice(int n)
	{
		size = n;
		Perc = false;
	}

};

int main()
{
    return 0;
}

