/*
	t = 22:55 min
	precej lahko
	t = 4:07 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<std::string> preberiPodatke(const std::string& pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();

	return resitev;
}


std::pair<short, short>& operator+=(std::pair<short, short>& lhs, const std::pair<short, short>& rhs) {
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return lhs;
}

int oceniSteviloDreves(const std::vector<std::string>& bazaPodatkov, const std::pair<short, short>& korak = { 3,1 }) {

	int resitev = 0;

	std::pair<short, short> mesto = { 0,0 };

	const short visina = bazaPodatkov.size();
	const short sirina = bazaPodatkov.front().size();

	while (mesto.second < visina) {

		resitev += bazaPodatkov[mesto.second][mesto.first] == '#';

		mesto += korak;

		while (mesto.first >= sirina)
			mesto.first -= sirina;
	}

	return resitev;
}


int main(int argv, char* argc[]) {

	std::vector<std::string> bazaPodatkov(preberiPodatke("2020/3.txt"));


	int resitev1 = oceniSteviloDreves(bazaPodatkov);

	std::cout << "Zadanemo " << resitev1 << " dreves.\n";


	int resitev2 = 1;

	for (const std::pair<short, short>& korak : std::vector<std::pair<short, short>>{ {1,1},{3,1},{5,1},{7,1},{1,2} })
		resitev2 *= oceniSteviloDreves(bazaPodatkov, korak);

	std::cout << "Zadanemo " << resitev2 << " dreves.\n";


	return 0;
}
