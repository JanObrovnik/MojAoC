/*
	
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>


bool preveriMoznostIzdelave(std::string dizajn, std::list<std::string>& seznamVzorcev) {

	std::list<std::string> razdelitevDizajnov{ dizajn };
	
	while (!razdelitevDizajnov.empty()) {

		//for (std::string str : razdelitevDizajnov)
		//	std::cout << str << ' ';
		std::cout << razdelitevDizajnov.size() << '\n';

		std::list<std::string> novaRazdelitevDizajnov{};

		for (std::string& diz : razdelitevDizajnov) {

			if (diz.empty()) return true;

			for (const std::string& vzorec : seznamVzorcev) {

				if (vzorec.size() > diz.size()) continue;

				std::string delniDizajn(diz.begin(), diz.begin() + vzorec.size());

				if (delniDizajn == vzorec) {
					novaRazdelitevDizajnov.push_back(std::string(diz.begin() + vzorec.size(), diz.end()));
				}
			}
		}

		razdelitevDizajnov = novaRazdelitevDizajnov;
	}


	return false;
}


// <seznam vzorcev, seznam dizajnov>
std::pair<std::list<std::string>, std::list<std::string>> preberiPodatke(std::string pot) {

	std::list<std::string> seznamVzorcev{};
	std::list<std::string> seznamDizajnov{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);


	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return {seznamVzorcev, seznamDizajnov};
	}


	std::string vrstica;
	std::getline(podatki, vrstica);

	std::stringstream ss(vrstica);

	std::string beseda;
	while (ss >> beseda)
		seznamVzorcev.push_back(beseda);

	while (podatki >> beseda)
		seznamDizajnov.push_back(beseda);


	podatki.close();


	bool popravi = false;
	std::for_each(seznamVzorcev.rbegin(), seznamVzorcev.rend(), [&](std::string& str) {
		if (popravi) str.erase(str.begin() + str.size() - 1);
		else popravi = true;
		});


	return { seznamVzorcev, seznamDizajnov };
}


int main() {

	std::pair<std::list<std::string>, std::list<std::string>> resitev(preberiPodatke("Advent24-19.txt"));
	std::list<std::string> seznamVzorcev(resitev.first);
	std::list<std::string> seznamDizajnov(resitev.second);

	//std::for_each(seznamVzorcev.begin(), seznamVzorcev.end(), [](std::string str) {std::cout << str << ' '; });
	//std::cout << '\n' << '\n';
	//std::for_each(seznamDizajnov.begin(), seznamDizajnov.end(), [](std::string str) {std::cout << str << '\n'; });
	//std::cout << '\n';

	int steviloVeljavnih = 0;

	std::for_each(seznamDizajnov.begin(), seznamDizajnov.end(), [&](std::string& dizajn) {
		std::cout << steviloVeljavnih << '\n';
		steviloVeljavnih += preveriMoznostIzdelave(dizajn, seznamVzorcev);
		});


	std::cout << "Stevilo dizajnov, ki jih lahko naredimo iz vzorcev je " << steviloVeljavnih << ".\n";





	return 0;
}
