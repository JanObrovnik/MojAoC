/*
	t = 15:33 min
	zelo lahko
	t = 26:14 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>


std::pair<int,int> preberiPodatke(const std::string& pot) {

	std::pair<int, int> resitev = { 0,0 };


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}


	int velikostSkupine = 0;
	std::set<char> setVrednosti;
	std::multiset<char> multisetVrednosti;
	std::string vrstica;

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);

		if (vrstica.empty()) {
			resitev.first += setVrednosti.size();
			for (const char& c : setVrednosti)
				if (multisetVrednosti.count(c) == velikostSkupine)
					resitev.second++;

			setVrednosti.clear();
			multisetVrednosti.clear();
			velikostSkupine = 0;

			continue;
		}

		for (const char& c : vrstica) {
			setVrednosti.insert(c);
			multisetVrednosti.insert(c);
		}
		velikostSkupine++;
	}
	resitev.first += setVrednosti.size();
	for (const char& c : setVrednosti)
		if (multisetVrednosti.count(c) == velikostSkupine)
			resitev.second++;

	setVrednosti.clear();
	multisetVrednosti.clear();
	velikostSkupine = 0;


	podatki.close();


	return resitev;
}


int main() {

	std::pair<int, int> resitev = preberiPodatke("2020/6.txt");

	std::cout << "Vsota vseh pozitivnih odgovorov je " << resitev.first << ".\n";
	std::cout << "Vsota vseh vedno pozitivnih odgovorov je " << resitev.second << ".\n";


	return 0;
}
