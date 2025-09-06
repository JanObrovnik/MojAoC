/*
	t = 29:32 min
	lahko
	t ~ 20 min
	lahko (zelo vsec)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>


std::vector<long long> preberiPodatke(const std::string& pot) {

	std::vector<long long> resitev;

	std::ifstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	long long vrednost;

	while (podatki >> vrednost)
		resitev.push_back(vrednost);

	podatki.close();

	return resitev;
}


bool mozenSestevek(const std::vector<long long>& seznamStevil, const long long& iskanoStevilo) {

	for (int i = 0; i < seznamStevil.size() - 1; i++)
		for (int j = i + 1; j < seznamStevil.size(); j++)
			if (seznamStevil[i] + seznamStevil[j] == iskanoStevilo)
				return true;

	return false;
}

long long najdiPrvoNapako(const std::vector<long long>& bazaPodatkov, const int& velikostUvoda = 25) {

	std::vector<long long> tekociPas;
	tekociPas.reserve(velikostUvoda);

	for (const long long& vrednost : bazaPodatkov) {

		if (tekociPas.size() == velikostUvoda) {

			if (!mozenSestevek(tekociPas, vrednost))
				return vrednost;

			tekociPas.erase(tekociPas.begin());
		}

		tekociPas.push_back(vrednost);
	}

	return -1;
}

long long najdiSibkost(const std::vector<long long>& bazaPodatkov, const long long& iskanoStevilo) {

	std::vector<long long> tekociPas;
	int index = 0;

	while (index < bazaPodatkov.size()) {

		const long long vsotaPasu = std::accumulate(tekociPas.begin(), tekociPas.end(), 0ll);

		if (vsotaPasu < iskanoStevilo) {
			tekociPas.push_back(bazaPodatkov[index++]);
			continue;
		}
		if (vsotaPasu > iskanoStevilo) {
			tekociPas.erase(tekociPas.begin());
			continue;
		}

		const auto par = std::minmax_element(tekociPas.begin(), tekociPas.end());
		return *par.first + *par.second;
	}

	return -1;
}


int main() {

	std::vector<long long> bazaPodatkov(preberiPodatke("2020/9.txt"));


	long long resitev1 = najdiPrvoNapako(bazaPodatkov, 25);

	std::cout << "Prvo napacno stevilo je " << resitev1 << ".\n";


	long long resitev2 = najdiSibkost(bazaPodatkov, resitev1);

	std::cout << "Sibkost enkripcije je " << resitev2 << ".\n";


	return 0;
}
