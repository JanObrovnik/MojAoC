/*
	t = 27:15 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::pair<int, std::vector<int>> preberiPodatke(const std::string& pot) {

	int resitev1;
	std::vector<int> resitev2;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1, resitev2 };
	}

	std::string vmesnik;

	podatki >> resitev1;
	while (std::getline(podatki, vmesnik, ','))
		if (vmesnik != "x")
			resitev2.push_back(std::stoi(vmesnik));

	podatki.close();

	return { resitev1,resitev2 };
}


int najdiNajhitrejsiPrevoz(const int& cas, const std::vector<int>& vozniRed) {

	int id = 0;
	int min = 2 * cas;

	for (const int& bus : vozniRed) {

		if (cas % bus == 0)
			return 0;

		int stObhodov = cas / bus;
		int naslednjiObhod = bus * (stObhodov + 1);

		if (naslednjiObhod < min) {
			min = naslednjiObhod;
			id = bus;
		}
		std::cout << min << ' ' << id << '\n';
	}

	return (min - cas) * id;
}


int main() {

	std::pair<int, std::vector<int>> podatki(preberiPodatke("2020/13.txt"));
	int& cas = podatki.first;
	std::vector<int>& vozniRed = podatki.second;

	int resitev1 = najdiNajhitrejsiPrevoz(cas, vozniRed);
	std::cout << "Ocena idealnega prevoza je " << resitev1 << ".\n";


	return 0;
}
