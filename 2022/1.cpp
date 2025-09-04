/*
	Zelo lahko
	Zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>


std::pair<int, int> preberiPodatke(std::string pot) {

	int resitev1 = 0;
	std::array<int, 3> resitev2{ 0,0,0 };


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { 0,0 };
	}

	std::string vrstica;
	std::vector<int> seznamKalorij{};

	bool konec = false;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty()) {
			zadnjaIteracija:
			int vsota(std::accumulate(seznamKalorij.begin(), seznamKalorij.end(), 0));
			if (vsota > resitev1) {
				resitev1 = vsota;
			}
			//std::cout << vsota << '|' << resitev2.front() << '\n';
			if (vsota > resitev2.front()) {
				resitev2.front() = vsota;
				std::sort(resitev2.begin(), resitev2.end());
				//for (int i : resitev2) std::cout << i << ' '; std::cout << '\n';
			}
			seznamKalorij.clear();
			continue;
		}

		seznamKalorij.push_back(std::stoi(vrstica));
	}
	if (konec == false) {
		konec = true;
		goto zadnjaIteracija;
	}

	podatki.close();


	return { resitev1,std::accumulate(resitev2.begin(), resitev2.end(), 0) };
}



int main(int argc, char* argv[]) {

	std::pair<int,int> resitev = preberiPodatke("Advent22/1.txt");
	std::cout << "max kalorij, ki jih nosi skrat je " << resitev.first << ".\n";
	std::cout << "max kalorij, ki jih nosijo prvi trije skratje je " << resitev.second << ".\n";


	return 0;
}