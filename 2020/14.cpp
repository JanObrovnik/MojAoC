/*
	t ~ 1 ura
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <map>


std::bitset<36> maskiranje(const int& vrednost, const std::string& maska) {

	std::bitset<36> resitev(vrednost);

	int index = 0;

	for (auto it = maska.rbegin(); it != maska.rend(); it++) {
		if (*it != 'X')
			resitev.set(index, *it - '0');
		index++;
	}

	return resitev;
}

std::map<int, std::bitset<36>> preberiPodatke(const std::string& pot) {

	std::map<int, std::bitset<36>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string prepoznava;
	std::string maska;
	int mesto;
	int vrednost;

	while (podatki.peek() != EOF) {

		podatki >> prepoznava;

		if (prepoznava == "mask") {
			podatki.ignore(2);
			podatki >> maska;
		}
		else {
			mesto = std::stoi(prepoznava.substr(4, prepoznava.size() - 5));
			podatki.ignore(2);
			podatki >> vrednost;

			resitev[mesto] = maskiranje(vrednost, maska);
		}
	}

	return resitev;
}


unsigned long long vsota(const std::map<int, std::bitset<36>>& podatki) {

	unsigned long long resitev = 0;

	for (const auto& i : podatki)
		resitev += i.second.to_ullong();

	return resitev;
}

int main() {

	std::map<int, std::bitset<36>> podatki(preberiPodatke("2020/14.txt"));

	unsigned long long resitev1 = vsota(podatki);
	std::cout << "Vsota vseh vrednosti je " << resitev1 << ".\n";


	return 0;
}