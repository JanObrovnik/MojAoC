#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>


int preberiStevilo(const std::string& vrstica) {

	char char0 = '0' - 1;
	char char9 = '9' + 1;
	auto funk = [&](char c) {return c > char0 && c < char9; };

	std::string stringStevilo = "";

	auto itPrva = std::find_if(vrstica.begin(), vrstica.end(), funk);
	if (itPrva == vrstica.end()) return 0;
	stringStevilo += *itPrva;

	auto itDruga = std::find_if(vrstica.rbegin(), vrstica.rend(), funk);
	stringStevilo += *itDruga;

	return std::stoi(stringStevilo);
}


int najdiIzpisanoStevilo(std::string::const_iterator first, std::string::const_iterator last, bool obratno = false) {

	const std::array<std::string, 9> seznamIzpisanihStevil{
		"one", "two", "three", "four", "five", "six", "seven", "eight", "nine" 
	};


	std::vector<std::pair<std::string::const_iterator, int>> seznamParov;

	if (obratno) {
		auto llast = std::reverse_iterator<std::string::const_iterator>(first);
		auto ffirst = std::reverse_iterator<std::string::const_iterator>(last);

		for (int i = 0; i < seznamIzpisanihStevil.size(); i++) {
			auto it = std::search(ffirst, llast, seznamIzpisanihStevil[i].rbegin(), seznamIzpisanihStevil[i].rend());
			if (it != llast)
				seznamParov.push_back({ it.base(), i + 1 });
		}
	}
	else {

		for (int i = 0; i < seznamIzpisanihStevil.size(); i++) {
			auto it = std::search(first, last, seznamIzpisanihStevil[i].begin(), seznamIzpisanihStevil[i].end());
			if (it != last)
				seznamParov.push_back({ it, i + 1 });
		}
	}


	if (seznamParov.empty())
		return -1;
	if (seznamParov.size() == 1)
		return seznamParov.front().second;

	if (obratno) {
		auto it = std::max_element(seznamParov.begin(), seznamParov.end(), [](const auto& par1, const auto& par2) {
			return par1.first < par2.first;
			});
		return it->second;
	}
	else {
		auto it = std::min_element(seznamParov.begin(), seznamParov.end(), [](const auto& par1, const auto& par2) {
			return par1.first < par2.first;
			});
		return it->second;
	}

	return -2;
}

int preberiIzpisanoStevilo(const std::string& vrstica) {

	int resitev = 0;


	char char0 = '0' - 1;
	char char9 = '9' + 1;
	auto najdiStevko = [&](char c) {return c > char0 && c < char9; };

	std::string::const_iterator itPrva = std::find_if(vrstica.begin(), vrstica.end(), najdiStevko);
	int izpisanaPrva = najdiIzpisanoStevilo(vrstica.begin(), itPrva, false);
	if (izpisanaPrva < 0) resitev += ((*itPrva - '0') * 10);
	else resitev += izpisanaPrva * 10;

	auto itNazaj = std::find_if(vrstica.rbegin(), vrstica.rend(), najdiStevko);
	std::string::const_iterator itDruga = itNazaj.base();
	int izpisanaDruga = najdiIzpisanoStevilo(itDruga, vrstica.end(), true);
	if (izpisanaDruga < 0) resitev += ((*itNazaj - '0') * 1);
	else resitev += izpisanaDruga * 1;


	return resitev;
}


int main() {

	std::list<int> seznamStevil1;
	std::list<int> seznamStevil2;


	std::fstream podatki;
	podatki.open("Advent23/1.txt", std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "datoteke ni bilo mogoce odpreti.\n";
		return 1;
	}

	std::string vrstica;
	
	while (podatki.peek() != EOF) {
		
		std::getline(podatki, vrstica);

		seznamStevil1.push_back(preberiStevilo(vrstica));
		seznamStevil2.push_back(preberiIzpisanoStevilo(vrstica));
	}

	podatki.close();


	std::cout << "Vsota je enaka: " << std::accumulate(seznamStevil1.begin(), seznamStevil1.end(), 0) << std::endl;
	std::cout << "Vsota vseh je enaka: " << std::accumulate(seznamStevil2.begin(), seznamStevil2.end(), 0) << std::endl;



	return 0;
}