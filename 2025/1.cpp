/*
	t = 7:17 min
	Lahko
	t = 7:19 min
	Nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::pair<short,short> preberiPodatke(const std::string& pot) {

	short resitev1 = 0;
	short resitev2 = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	short kazalec = 50;

	char smer;
	short velikost;

	while (podatki.peek() != EOF) {

		podatki >> smer >> velikost;

		if (smer == 'L') {
			for (int i = 0; i < velikost; i++) {
				kazalec--;
				if (kazalec < 0) kazalec += 100;
				if (kazalec == 0) resitev2++;
			}
		}
		else if (smer == 'R') {
			for (int i = 0; i < velikost; i++) {
				kazalec++;
				if (kazalec > 99) kazalec -= 100;
				if (kazalec == 0) resitev2++;
			}
		}

		if (kazalec == 0)
			resitev1++;
	}

	podatki.close();

	return { resitev1,resitev2 };
}

int main() {

	std::pair<short, short> resitev = preberiPodatke("2025/1.txt");

	std::cout << "Geslo po metodi 1 je " << resitev.first << ".\n";
	std::cout << "Geslo po metodi 2 je " << resitev.second << ".\n";


	return 0;
}
