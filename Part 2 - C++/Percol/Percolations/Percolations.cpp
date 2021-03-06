// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "set"
#include"memory"
//#include "stdlib"
#include "random"
#include "utility"
#include "functional"
#include "iostream"
#include "fstream"
#include "assert.h"
#include <string>
#include <vector>



std::set<std::pair<int, int>> SetUnion(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec);
std::set<std::pair<int, int>> SetIntersect(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec);
std::set<std::pair<int, int>> SetDiffer(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec);
std::set<std::pair<int, int>> make_circle(int size, int len_from_center, int hole_index);
int BiteSum(int number);



class HexLattice
{
private:
	int size;
	int liquids;
	std::vector<std::vector<int>> Lattice, CopyCat;
	bool Perc = false;
	int RandState;

	void InitClear() {
		Lattice.clear();
		CopyCat.clear();
		Perc = false;
		for (int k = 0; k < size * 2 - 1; k++) {
			for (int i = 0; i <= size - 1; i++) {
				std::vector<int> l;
				l.clear();
				Lattice.push_back(l);
				CopyCat.push_back(l);
				for (int j = 0; j <= size + i - 1; j++) {
					int l = 0;
					Lattice[i].push_back(l);
					CopyCat[i].push_back(l);
				}
			}
			for (int i = size; i <= 2 * size - 2; i++) {
				std::vector<int> l;
				l.clear();
				Lattice.push_back(l);
				CopyCat.push_back(l);
				for (int j = 0; j <= 2 * size - 2; j++) {
					int l = 0;
					Lattice[i].push_back(l);
					CopyCat[i].push_back(l);
				}
			}
		}
	}

	void FillRandomly() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(0, 1);
		for (int k = 1; k < liquids; k++) {
			for (int i = 0; i <= size - 1; i++) {
				for (int j = 0; j <= size + i - 1; j++) {
					RandState = distribution(gen);
					Lattice[i][j] = (Lattice[i][j] + RandState) * 2;
				}
			}
			for (int i = size; i <= 2 * size - 2; i++) {
				for (int j = i - size + 1; j <= 2 * size - 2; j++) {
					RandState = distribution(gen);
					Lattice[i][j] = (Lattice[i][j] + RandState) * 2;
				}
			}
		}
	}

	void EstablishZeroLiquid() {
		for (int i = 0; i <= size - 1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				RandState = BiteSum(Lattice[i][j]) ^ 1;
				Lattice[i][j] = Lattice[i][j] + RandState;
			}
		}
		for (int i = size; i <= 2 * size - 2; i++) {
			for (int j = i - size + 1; j <= 2 * size - 2; j++) {
				RandState = BiteSum(Lattice[i][j]) ^ 1;
				Lattice[i][j] = Lattice[i][j] + RandState;
			}
		}
	}

	void InitProbSubs() {
		InitClear();
		FillRandomly();
		EstablishZeroLiquid();
	}

	std::set<std::pair<int, int>> GetNeighbours(int x, int y) {
		return { std::make_pair(x - 1, y - 1),std::make_pair(x - 1, y),std::make_pair(x, y - 1),std::make_pair(x + 1, y),std::make_pair(x, y + 1),std::make_pair(x + 1, y + 1) };
	}

	bool IsInBoard(int x, int y) { //проверяем лежит ли точка (x, y) на границе шестиугольника.
		return (x == 0) || (x == 2 * size - 2) || (y == 0) || (y == 2 * size - 2) || (x - y == size - 1) || (y - x == size - 1);
	}

	bool IsInside(int x, int y) { //проверяем лежит ли точка (x, y) в допустимом диапазоне шестиугольника.
		return (x >= 0) && (x <= 2 * size - 2) && (y >= 0) && (y <= 2 * size - 2) && (x - y <= size - 1) && (y - x <= size - 1);
	}

	bool CheckPercol(int LiqNum) {
		for (int i = 0; i <= size - 1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				CopyCat[i][j] = (Lattice[i][j] >> LiqNum) & 1;
			}
		}
		for (int i = size; i <= 2 * size - 2; i++) {
			for (int j = i - size + 1; j <= 2 * size - 2; j++) {
				CopyCat[i][j] = (Lattice[i][j] >> LiqNum) & 1;
			}
		}
		dfs(size - 1, size - 1);
		return Perc;

	}

	void dfs(int x, int y) {
		Perc = false;
		int NowX = x;
		int NowY = y;
		std::set<std::pair<int, int>> part;
		std::vector<std::pair<int, int>> dfsStack;
		dfsStack.clear();
		dfsStack.push_back(std::make_pair(NowX, NowY));
		while (!Perc && (dfsStack.size() > 0)) {
			// отмечаем пересечение множества соседей и множества непосещенных вершин.
			int en = dfsStack.size() - 1;
			assert(en >= 0);
			NowX = dfsStack[en].first;
			NowY = dfsStack[en].second;
			dfsStack.pop_back();
			std::set<std::pair<int, int>> Peers = GetNeighbours(NowX, NowY);
			for (auto it = Peers.begin(); it != Peers.end(); ++it) {
				if (IsInside(it->first, it->second)) {
					if (CopyCat[it->first][it->second] == 1) {
						CopyCat[it->first][it->second] = 0;
						dfsStack.push_back(*it);
						if (IsInBoard(it->first, it->second)) {
							Perc = true;
						}
					}
				}
			}
		}
	}
public:
	explicit HexLattice(int n, int c)
	{

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(0, 1);
		size = n;
		liquids = c;
		InitClear();
	}

	std::vector<int> MakeGuesses(int NumberOfGesses) {
		int NumberOfPerc = 0;
		std::vector<int> ResultVector;
		for (int i = 0; i <= liquids; ++i) {
			ResultVector.push_back(0);
		}
		for (int j = 1; j <= NumberOfGesses; ++j) {
			InitProbSubs();
			for (int i = 0; i < liquids; ++i) {
				NumberOfPerc += CheckPercol(i);
			}
			ResultVector[NumberOfPerc]++;
			NumberOfPerc = 0;
		}
		return ResultVector;
	}

	int SetConfigAndCheck(std::set<std::pair<int, int>> TypeA, std::set<std::pair<int, int>> TypeB) {
		if (liquids != 3) {
			return -1;
		}
		InitClear();
		for (int i = 0; i <= size - 1; i++) {
			for (int j = 0; j <= size + i - 1; j++) {
				if (TypeA.find(std::make_pair(i, j)) != TypeA.end()) {
					Lattice[i][j]++;
				}
				Lattice[i][j] *= 2;
				if (TypeB.find(std::make_pair(i, j)) != TypeB.end()) {
					Lattice[i][j]++;
				}
				Lattice[i][j] *= 2;
			}
		}
		for (int i = size; i <= 2 * size - 2; i++) {
			for (int j = i - size + 1; j <= 2 * size - 2; j++) {
				if (TypeA.find(std::make_pair(i, j)) != TypeA.end()) {
					Lattice[i][j]++;
				}
				Lattice[i][j] *= 2;
				if (TypeB.find(std::make_pair(i, j)) != TypeB.end()) {
					Lattice[i][j]++;
				}
				Lattice[i][j] *= 2;
			}
		}
		EstablishZeroLiquid();
		bool ForA = false;
		bool ForB = false;
		bool ForC = false;
		int res = 0;
		ForA = CheckPercol(2);
		ForB = CheckPercol(1);
		ForC = CheckPercol(0);
		if (ForA) {
			res += 4;
		}
		if (ForB) {
			res += 2;
		}
		if (ForC) {
			res += 1;
		}
		return res;
	}
};


///////////////

int numberOfTests = 0;
int numberOfSuccessfullTests = 0;
int numberOfFailedTests = 0;

void reportOk(std::string testName) 
{
    std::cout << "OK: " << testName << "\n";
    numberOfSuccessfullTests++;
    numberOfTests++;
}

void reportFail(std::string testName) 
{
    std::cout << "FAILED: " << testName << "\n";
    numberOfFailedTests++;
    numberOfTests++;
}

void test_configuration(std::string testName, int size, bool expected_result)
{
    std::shared_ptr<HexLattice> lattice = 
        std::make_shared<HexLattice>(size, 3);
	std::set<std::pair<int, int>> One;
	One.clear();
	std::set<std::pair<int, int>> Two;
	Two.clear();
    int ind = lattice->SetConfigAndCheck(One, Two);

    if (expected_result == (ind == 1 || (size == 1 && ind == 0))) {
        reportOk(testName);
    }
    else {
        reportFail(testName);
    }
}

void test_MKS(bool expected_result)
{
	std::shared_ptr<HexLattice> lattice =
		std::make_shared<HexLattice>(2, 3);
	std::vector<int> res;
	res = lattice->MakeGuesses(200);
	int qq = 0;
	int bb = res.size();
	for (int i = 0; i < bb; ++i) {
		qq += res[i];
	}
	if (expected_result == (qq == 200)) {
		reportOk("MBSk_test");
	}
	else {
		reportFail("MBSk_test");
	}
}

void test_k_circle(std::string testName, int size, int len_from_center, bool expected_result) {
	std::shared_ptr<HexLattice> lattice =
		std::make_shared<HexLattice>(size, 3);
	std::set<std::pair<int, int>> One;
	One.clear();
	One = make_circle(size, len_from_center, -1);

	std::set<std::pair<int, int>> Two;
	Two.clear();
	int ind = lattice->SetConfigAndCheck(One, Two);

	if (expected_result == (ind == 0 || (size == 1 && ind == 4))) {
		reportOk(testName);
	}
	else {
		reportFail(testName);
	}
}

void test_one_circle_with_hole(std::string testName, int size, bool expected_result) {
	std::shared_ptr<HexLattice> lattice =
		std::make_shared<HexLattice>(size, 3);
	std::mt19937 gen(57); //Standard mersenne_twister_engine seeded with 57
	std::uniform_int_distribution<> dis(1, 5040);
	std::set<std::pair<int, int>> One;
	for (int i = 0; i < 200; ++i) {
		One.clear();
		int distan = dis(gen) % (size - 1) + 1;
		int ejected = dis(gen) % (6 * distan);
		One = make_circle(size, distan, ejected);
		std::set<std::pair<int, int>> Two;
		Two.clear();
		int ind = lattice->SetConfigAndCheck(One, Two);
		if (expected_result != (ind == 1 || (size == 2 && ind == 5))) {
			reportFail(testName);
			return;
		}
	}
	reportOk(testName);
}

void test_circles_with_hole(std::string testName, int size, bool expected_result) {
	std::shared_ptr<HexLattice> lattice =
		std::make_shared<HexLattice>(size, 3);
	std::mt19937 gen(57); //Standard mersenne_twister_engine seeded with 57
	std::uniform_int_distribution<> dis(1, 5040);
	std::set<std::pair<int, int>> Two;
	for (int i = 0; i < 100; ++i) {
		Two.clear();
		for (int j = 1; j < size; j += 2) {
			int distan = dis(gen) % 2;
			if (distan == 0) {
				int ejected = dis(gen) % (6 * j);
				Two = SetUnion(Two, make_circle(size, j, ejected));
			}
		}
		std::set<std::pair<int, int>> One;
		One.clear();
		int ind = lattice->SetConfigAndCheck(One, Two);
		if (expected_result != (ind == 1 || (size == 2 && ind == 3))) {
			reportFail(testName);
			return;
		}
	}
	reportOk(testName);
}

int run_all_tests() 
{
    test_configuration("test size=1", 1, true);
	test_configuration("test size=2", 2, true);
	test_configuration("test size=3", 3, true);
	test_configuration("test size=100", 100, true);
	test_k_circle("test circle size 100", 100, 5, true);
	test_k_circle("test circle size 1", 1, 1, true);
	test_k_circle("test circle size 2", 2, 1, false);
	test_k_circle("test circle size 3", 3, 1, true);
	test_k_circle("test circle size 4", 4, 3, true);
	test_one_circle_with_hole("test holed circle size 2", 2, true);
	test_one_circle_with_hole("test holed circle size 4", 4, true);
	test_one_circle_with_hole("test holed circle size 20", 20, true);
	test_circles_with_hole("test many holed circles size 2", 2, true);
	test_circles_with_hole("test many holed circles size 7", 7, true);
	test_circles_with_hole("test many holed circles size 20", 20, true);
	test_MKS(true);
    const bool allOk = numberOfFailedTests == 0;
    if (allOk) {
        std::cout << "All tests OK: " << numberOfSuccessfullTests << " OK out of " << numberOfTests << "\n";
        return 0;
    }
    else {
        std::cout << "Tests FAILED!!!: " << numberOfSuccessfullTests << " OK, " << numberOfFailedTests << " FAILED " << "out of " << numberOfTests << "\n";
        return 1;
    }
}

///////////////

int main(int argc, char* argv[])
{
    if (argc == 2) {
        if (std::string(argv[1]) == "--test") {
            const int testResult = run_all_tests();
            return testResult;
        }
    }
	
	int guesses = 0;
	int size = 0;
	std::cout << "Size: from 5 to 500 with step 5";
	//std::cin >> size;
	std::cout << "\nNumber of guesses: ";
	std::cin >> guesses;
	std::ofstream result_file;
	std::shared_ptr<HexLattice> qq = std::make_shared<HexLattice>(100, 2);
	std::vector<int> res;
	res = qq->MakeGuesses(2000);
	float prot = float(res[1]) / 2 + float(res[2]);
	float m = 0;
	float d = 0;
	m = (prot) / 2000;
	d = (prot*(1 - m)*(1 - m) + (2000 - prot)*m*m) / 2000;
	std::cout << m << "\n" << d;
	//for (int p = 10; p < 501; p += 5) {
	//	result_file.open("D:\\Misha\\Универ\\2018-19\\к курсово\\ress.txt", std::ios::app);
	//	//result_file << "\n\n" << "-----------" << p << "--------------" << "\n\n";
	//	//std::cout << "\n\n" << "-----------" << p << "--------------" << "\n\n";
	//	std::vector<int> res;
	//	/*for (int i = 2; i <= 30; ++i) {
	//		std::shared_ptr<HexLattice> qq = std::make_shared<HexLattice>(p, i);
	//		res = qq->MakeGuesses(guesses);
	//		std::cout << "\n" << i << "\t";
	//		result_file << "\n" << i << "\t";
	//		for (int j = 0; j <= i; ++j) {
	//			std::cout << (double)res[j] / guesses << "\t";
	//			result_file << (double)res[j] / guesses << "\t";
	//		}
	//	}*/
	//	const int i = 3;
	//	std::shared_ptr<HexLattice> qq = std::make_shared<HexLattice>(p, i);
	//	res = qq->MakeGuesses(guesses);
	//	std::cout << "\n" << i << "\t" << p << "\t";
	//	result_file << "\n" << i << "\t" << p << "\t";
	//	std::cout << (double)res[i] / guesses << "\t";
	//	result_file << (double)res[i] / guesses << "\t";
	//	result_file.close();

	//}
	std::cout << size;
	result_file.close();
	return 0;
}



std::set<std::pair<int, int>> SetUnion(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec) {
	std::set<std::pair<int, int>> res = Sec;
	for (auto it = First.begin(); it != First.end(); ++it) {
		if (Sec.find(*it) == Sec.end()) {
			res.insert(*it);
		}
	}
	return res;
}

std::set<std::pair<int, int>> SetIntersect(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec) {
	std::set<std::pair<int, int>> res;
	res.clear();
	for (auto it = First.begin(); it != First.end(); ++it) {
		if (Sec.find(*it) != Sec.end()) {
			res.insert(*it);
		}
	}
	return res;
}

std::set<std::pair<int, int>> SetDiffer(std::set<std::pair<int, int>> First, std::set<std::pair<int, int>> Sec) {
	std::set<std::pair<int, int>> res;
	res.clear();
	for (auto it = First.begin(); it != First.end(); ++it) {
		if (Sec.find(*it) == Sec.end()) {
			res.insert(*it);
		}
	}
	return res;
}

std::set<std::pair<int, int>> make_circle(int size, int len_from_center, int hole_index){
	std::set<std::pair<int, int>> One;
	One.clear();
	int count = 0;
	for (int i = - 1 - len_from_center; i <= -1; ++i) {
		if (count != hole_index) {
			One.insert(std::make_pair(size + i, size - 1 - len_from_center));
		}
		count++;
		if (count != hole_index) {
			One.insert(std::make_pair(size + i, size + i + len_from_center));
		}
		count++;
	}
	for (int i = - len_from_center; i < -1; ++i) {
		if (count != hole_index) {
			One.insert(std::make_pair(size - 1 - len_from_center, size + i));
		}
		count++;
		if (count != hole_index) {
			One.insert(std::make_pair(size + i + len_from_center, size + i));
		}
		count++;
	}
	for (int i = size; i <= size - 1 + len_from_center; ++i) {
		if (count != hole_index) {
			One.insert(std::make_pair(i, size - 1 + len_from_center));
		}
		count++;
	}
	for (int i = size - 1; i <= size + len_from_center - 2; ++i) {
		if (count != hole_index) {
			One.insert(std::make_pair(size - 1 + len_from_center, i));
		}
		count++;
	}
	return One;
}

int BiteSum(int number) {
	int result = 0;
	while (number != 0) {
		result += number & 1;
		number = number >> 1;
	}
	result = result & 1;
	return result;
}
