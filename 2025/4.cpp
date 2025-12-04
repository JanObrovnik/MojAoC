/*
	t = 21:42 min
	presenetljivo lahko
	t ~ 18 min
	neverjetno lahko
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
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();

	return resitev;
}


int steviloSosed(const std::vector<std::string>& bazaPodatkov, const int& x, const int& y) {

	int resitev = 0;

	for (int yOff = -1; yOff <= 1; yOff++) {
		int yy = y + yOff;

		if (yy < 0 || yy >= bazaPodatkov.size())
			continue;
		
		for (int xOff = -1; xOff <= 1; xOff++) {
			int xx = x + xOff;

			if (xx < 0 || xx >= bazaPodatkov[yy].size())
				continue;
			if (yOff == 0 && xOff == 0)
				continue;

			if (bazaPodatkov[yy][xx] == '@')
				resitev++;
		}
	}

	return resitev;
}

int poisciSteviloOsamelihTock(std::vector<std::string> bazaPodatkov, std::vector<std::string>* kazalecPodatkov = nullptr, const int& prevelikoSteviloSosed = 4) {

	int resitev = 0;

	std::vector<std::string> kopija = bazaPodatkov;

	for (int y = 0; y < bazaPodatkov.size(); y++) {
		for (int x = 0; x < bazaPodatkov.size(); x++) {

			if (steviloSosed(bazaPodatkov, x, y) < prevelikoSteviloSosed && bazaPodatkov[y][x] == '@') {
				resitev++;
				kopija[y][x] = '.';
			}
		}
	}

	if (kazalecPodatkov != nullptr)
		*kazalecPodatkov = kopija;

	return resitev;
}

int veckratonoOdstranjevanje(std::vector<std::string> bazaPodatkov, const int& prevelikoSteviloSosed) {

	int resitev = 0;
	int steviloOdstanjenihKosov = 0;

	do {
		steviloOdstanjenihKosov = poisciSteviloOsamelihTock(bazaPodatkov, &bazaPodatkov, prevelikoSteviloSosed);
		resitev += steviloOdstanjenihKosov;

	} while (steviloOdstanjenihKosov > 0);

	return resitev;
}


int main() {

	std::vector<std::string> bazaPodatkov = preberiPodatke("2025/4.txt");

	int resitev1 = poisciSteviloOsamelihTock(bazaPodatkov, nullptr, 4);
	std::cout << "Poberemo lahko " << resitev1 << " vreck papirja.\n";

	int resitev2 = veckratonoOdstranjevanje(bazaPodatkov, 4);
	std::cout << "Skupno lahko poberemo " << resitev2 << " vreck papirja.\n";


	return 0;
}
