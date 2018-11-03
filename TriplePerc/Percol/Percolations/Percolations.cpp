// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "set"
#include"memory"
//#include "stdlib"
#include "random"
#include "utility"
#include "functional"
#include"iostream"

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
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(0, 3);
		GoodA.clear();
		GoodB.clear();
		GoodC.clear();
		NotVisited.clear();
		Perc = false;
		for (int i = 0; i <= size - 1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				RandState = distribution(gen);
				if (RandState == 0) {
					auto a = GoodA.insert(std::make_pair(i, j));
					a = GoodB.insert(std::make_pair(i, j));
					a = GoodC.insert(std::make_pair(i, j));
				}
				else if (RandState == 1) {
					auto a = GoodA.insert(std::make_pair(i, j));
				}
				else if (RandState == 2) {
					auto a = GoodB.insert(std::make_pair(i, j));
				}
				else if (RandState == 3) {
					auto a = GoodC.insert(std::make_pair(i, j));
				};
			}
		}
		for (int i = size; i <= 2 * size - 2; i++) {
			for (int j = i - size + 1; j <= 2 * size - 2; j++) {
				RandState = distribution(gen);
				if (RandState == 0) {
					auto a = GoodA.insert(std::make_pair(i, j));
					a = GoodB.insert(std::make_pair(i, j));
					a = GoodC.insert(std::make_pair(i, j));
				}
				else if (RandState == 1) {
					auto a = GoodA.insert(std::make_pair(i, j));
				}
				else if (RandState == 2) {
					auto a = GoodB.insert(std::make_pair(i, j));
				}
				else if (RandState == 3) {
					auto a = GoodC.insert(std::make_pair(i, j));
				};
			}
		}
	}
	std::set<std::pair<int, int>> GetNeighbours(int x, int y) {
		return { std::make_pair(x - 1, y - 1),std::make_pair(x - 1, y),std::make_pair(x, y - 1),std::make_pair(x + 1, y),std::make_pair(x, y + 1),std::make_pair(x + 1, y + 1) };
	}

	bool IsInBoard(int x, int y) { //проверяем лежит ли точка (x, y) на границе шестиугольника.
		return (x == 0) || (x == 2 * size - 2) || (y == 0) || (y == 2 * size) || (x - y == size) || (y - x == size);
	}

	bool CheckPercolA() {
		NotVisited = GoodA;
		Perc = false;
		dfs(size - 1, size - 1);
		return Perc;
	}

	bool CheckPercolB() {
		NotVisited = GoodB;
		Perc = false;
		dfs(size - 1, size - 1);
		return Perc;
	}

	bool CheckPercolC() {
		NotVisited = GoodC;
		Perc = false;
		dfs(size - 1, size - 1);
		return Perc;
	}

	void dfs(int x, int y) {
		std::set<std::pair<int, int>> part;
		std::set<std::pair<int, int>> Peers = GetNeighbours(x, y);
		// описываем пересечение множества соседей и множества непосещенных вершин.
		for (auto it = Peers.begin(); it != Peers.end(); ++it) {
			if (NotVisited.find(*it) != NotVisited.end()) {
				part.insert(*it);
				NotVisited.erase(*it);
			}
		}
		for (auto it = part.begin(); it != part.end(); ++it) {
			if (IsInBoard(it->first, it->second)) {
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
	void MakeGuesses(int NumberOfGesses, int* Triple, int* Double, int* OnlyOne) {
		int Three = 0;
		int Two = 0;
		int One = 0;
		for (int i = 1; i < NumberOfGesses; ++i) {
			InitProbSubs();
			bool PercA = CheckPercolA();
			bool PercB = CheckPercolB();
			bool PercC = CheckPercolC();
			if (PercA&&PercB&&PercC) {
				Three++;
			}
			else if (((!PercA) && PercB&&PercC) || (PercA && (!PercB) && PercC) || (PercA && PercB && (!PercC))) {
				Two++;
			}
			else if (!((!PercA) && (!PercB) && (!PercC))) {
				One++;
				}
		}
		*Triple = Three;
		*Double = Two;
		*OnlyOne = One;
	}
};

void EqvlsFive(int* a) {
	*a = 5;
}

int main()
{

	int a=0;
	std::cin >> a;
	for (int i = 2; i <= a; ++i) {
		std::shared_ptr<HexLattice> qq = std::make_shared<HexLattice>(i);
		int One, Two, Three;
		qq->MakeGuesses(3000, &Three, &Two, &One);
		std::cout <<"Edge "<<i<<" -> No: "
			<< (3000-Three-Two-One)/3000.0
			<< "; One: "<< One/3000.0
			<< "; Two:  " << Two/3000.0
			<< "; Tree: " << Three/3000.0 << "\n";
		qq = 0;
	}
	EqvlsFive(&a);
	std::cout << a;
	std::cin >> a;
	return 0;
}



