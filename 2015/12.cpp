/*
	t = 12:00 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>


int preberiPodatke(const std::string& pot) {

	int resitev = 0;
	
	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	char c;
	std::string stevilka;

	while (podatki.peek() != EOF) {

		podatki >> c;

		if ((c >= '0' && c <= '9') || c == '-') {
			stevilka.push_back(c);
		}
		else if (!stevilka.empty()) {
			resitev += std::stoi(stevilka);
			stevilka.clear();
		}
	}

	podatki.close();

	return resitev;
}


int main() {

	int resitev1 = preberiPodatke("2015/12.txt");

	std::cout << "Vsota vseh stevilk je " << resitev1 << ".\n";


	return 0;
}
