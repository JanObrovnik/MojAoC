/*
	t = 56:45 min
	zelo lahko

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<bool> preberiPodatke(const std::string& pot) {

	std::vector<bool> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	char vrednost;

	while (podatki.peek() != EOF) {
		podatki >> vrednost;
		resitev.push_back(vrednost == '1');
	}

	podatki.close();

	return resitev;
}


void razsiriVectorBool(std::vector<bool>& seznam, const int& iskanaDolzina) {

	while (seznam.size() < iskanaDolzina) {
		std::cout << seznam.size() << '\n';

		seznam.push_back(false);

		for (int i = seznam.size() - 2; i >= 0; i--) {
			seznam.push_back(!seznam[i]);
		}
	}

	seznam.erase(seznam.begin() + iskanaDolzina, seznam.end());
}

void zdruziVektorBool(std::vector<bool>& seznam) {

	std::vector<bool> novSeznam;
	novSeznam.reserve(seznam.size() / 2);

	while (seznam.size() % 2 == 0) {
		std::cout << seznam.size() << '\n';

		for (int i = 0; i < seznam.size(); i += 2) {
			novSeznam.push_back(seznam[i] == seznam[i + 1]);
		}

		seznam = novSeznam;
		novSeznam.clear();
	}
}


int main() {

	int iskanaDolzina1 = 272;
	std::vector<bool> seznam1 = preberiPodatke("2016/16.txt");
	seznam1.reserve(iskanaDolzina1 * 2);

	//for (const bool& b : seznam) std::cout << b; std::cout << '\n';
	razsiriVectorBool(seznam1, iskanaDolzina1);
	//for (const bool& b : seznam) std::cout << b; std::cout << '\n';
	zdruziVektorBool(seznam1);
	
	std::cout << "Resitev je "; for (const bool& b : seznam1) std::cout << b; std::cout << ".\n";


	int iskanaDolzina2 = 35651584;
	std::vector<bool> seznam2 = preberiPodatke("2016/16.txt");
	seznam2.reserve(iskanaDolzina2 * 2);

	//for (const bool& b : seznam) std::cout << b; std::cout << '\n';
	razsiriVectorBool(seznam2, iskanaDolzina2);
	//for (const bool& b : seznam) std::cout << b; std::cout << '\n';
	zdruziVektorBool(seznam2);

	std::cout << "Resitev je "; for (const bool& b : seznam2) std::cout << b; std::cout << ".\n";


	return 0;
}
