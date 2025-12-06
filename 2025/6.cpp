/*
	t = 37:43 min
	zelo lahko
	t ~ 2:09 ur
	srednja tezavnost
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class BazaPodatkov1 {
public:
	std::vector<std::vector<short>> vrednosti;
	std::vector<char> operatorji;

	long long resitev() {

		long long resitev = 0;

		for (int i = 0; i < operatorji.size(); i++) {

			long long resitevVmesna = vrednosti[0][i];

			for (int st = 1; st < vrednosti.size(); st++) {
				if (operatorji[i] == '+')
					resitevVmesna += vrednosti[st][i];
				else if (operatorji[i] == '*')
					resitevVmesna *= vrednosti[st][i];
			}

			resitev += resitevVmesna;
		}

		return resitev;
	}
};

class BazaPodatkov2 {
public:
	std::vector<std::string> vrednosti;

	long long resitev() {

		long long resitev = 0;

		std::vector<int> seznamMest;
		for (int i = 0; i < vrednosti.back().size(); i++)
			if (vrednosti.back()[i] == '*' || vrednosti.back()[i] == '+')
				seznamMest.push_back(i);

		for (int it = 0; it < seznamMest.size(); it++) {

			int yMin = 0;
			int yMax = vrednosti.size() - 1;
			int xMin = seznamMest[it];
			int xMax;
			if (it != seznamMest.size() - 1) xMax = seznamMest[it + 1] - 1;
			else xMax = vrednosti.front().size();

			std::vector<long long> seznamVrednosti;
			for (int x = xMin; x < xMax; x++) {
				std::string test = "";
				for (int y = yMin; y < yMax; y++) {
					test.push_back(vrednosti[y][x]);
				}
				seznamVrednosti.push_back(std::stoll(test));
			}

			long long resitevVmesna = seznamVrednosti.front();
			for (int i = 1; i < seznamVrednosti.size(); i++) {
				if		(vrednosti.back()[seznamMest[it]] == '+') resitevVmesna += seznamVrednosti[i];
				else if (vrednosti.back()[seznamMest[it]] == '*') resitevVmesna *= seznamVrednosti[i];
				else std::cout << "Nedefinirana operacija.\n";
			}

			resitev += resitevVmesna;
		}

		return resitev;
	}
};


BazaPodatkov1 preberiPodatke1(const std::string& pot) {

	BazaPodatkov1 resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	short vrednost;
	char opertor;

	while (std::getline(podatki, vrstica)) {

		bool stevilcneVrednosti = true;

		for (const char& c : vrstica) {
			if (c >= '0' && c <= '9') {
				stevilcneVrednosti = true;
				break;
			}
			else if (c == '*' || c == '+') {
				stevilcneVrednosti = false;
				break;
			}
		}

		std::stringstream ss(vrstica);

		if (stevilcneVrednosti) {
			resitev.vrednosti.push_back({});
			while (ss >> vrednost)
				resitev.vrednosti.back().push_back(vrednost);
		}
		else {
			while (ss >> opertor)
				resitev.operatorji.push_back(opertor);
		}
	}

	podatki.close();

	return resitev;
}

BazaPodatkov2 preberiPodatke2(const std::string& pot) {

	BazaPodatkov2 resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (std::getline(podatki, vrstica))
		resitev.vrednosti.push_back(vrstica);

	podatki.close();

	return BazaPodatkov2(resitev);
}


int main() {

	BazaPodatkov1 bazaPodatkov1 = preberiPodatke1("2025/6.txt");
	std::cout << "Vsota vseh izracunov po pravilniku 1 je " << bazaPodatkov1.resitev() << ".\n";

	BazaPodatkov2 bazaPodatkov2 = preberiPodatke2("2025/6.txt");
	std::cout << "Vsota vseh izracunov po pravilniku 2 je " << bazaPodatkov2.resitev() << ".\n";


	return 0;
}
