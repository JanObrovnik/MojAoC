/*
	t = 16:48 min
	lahko
	t = 3:03 min
	neverjetno lahko (skoraj identicna koda)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


std::pair<std::string, std::string> najdiMinMax(const std::vector<std::map<char, int>>& seznam) {

	std::string resitevMax = "";
	std::string resitevMin = "";

	for (const std::map<char, int>& skupek : seznam) {

		std::vector<std::pair<char, int>> seznamPar(skupek.begin(), skupek.end());

		auto itPar = std::minmax_element(seznamPar.begin(), seznamPar.end(), [](const std::pair<char, int>& par1, const std::pair<char, int>& par2) {
			return par1.second < par2.second;
			});

		resitevMax.push_back(itPar.second->first);
		resitevMin.push_back(itPar.first->first);
	}

	return { resitevMin,resitevMax };
}

std::pair<std::string, std::string> preberiPodatke(const std::string& pot) {

	std::pair<std::string, std::string> resitev;

	std::vector<std::map<char, int>> seznam;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	podatki >> vrstica;

	seznam.resize(vrstica.size());

	for (int i = 0; i < vrstica.size(); i++)
		seznam[i][vrstica[i]]++;

	while (podatki.peek() != EOF) {

		podatki >> vrstica;

		for (int i = 0; i < vrstica.size(); i++)
			seznam[i][vrstica[i]]++;
	}

	podatki.close();

	resitev = najdiMinMax(seznam);

	return resitev;
}


int main() {

	std::pair<std::string, std::string> resitev = preberiPodatke("2016/6.txt");
	
	std::cout << "Koda po MAX standardu je: " << resitev.second << ".\n";
	std::cout << "Koda po MIN standardu je: " << resitev.first << ".\n";


	return 0;
}
