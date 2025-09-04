/*
	Zelo lahka
	Neverjetno lahka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>


bool preveriPravilnost(const std::string& oznacevalnik) {

	if (oznacevalnik[0] == '*')
		return false;

	int stVseh(oznacevalnik.size());
	int stEdinstvenih(std::set<char>(oznacevalnik.begin(), oznacevalnik.end()).size());

	return stVseh == stEdinstvenih;
}


int preberiPodatke(std::string pot, short dolzina = 4) {

	int resitev(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return 0;
	}

	char c;
	std::string oznacevalnik(dolzina, '*');

	while (podatki.peek() != EOF) {

		resitev++;

		podatki >> c;

		oznacevalnik.erase(oznacevalnik.begin());
		oznacevalnik.push_back(c);

		if (preveriPravilnost(oznacevalnik))
			break;
	}

	podatki.close();


	return resitev;
}


int main() {

	int resitev1(preberiPodatke("Advent22/6.txt"));
	std::cout << "Oznacevalnik dolzine 4 se prvic pojavi za mestom " << resitev1 << ".\n";
	int resitev2(preberiPodatke("Advent22/6.txt", 14));
	std::cout << "Oznacevalnik dolzine 14 se prvic pojavi za mestom " << resitev2 << ".\n";


	return 0;
}