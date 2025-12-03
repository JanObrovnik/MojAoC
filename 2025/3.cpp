/*
	t = 24:38 min
	neverjetno lahko
	t = 55:23 min
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


std::vector<std::vector<short>> preberiPodatke(const std::string& pot) {

	std::vector<std::vector<short>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica) {

		resitev.push_back({});
		
		for (const char& c : vrstica) {
			resitev.back().push_back(c - '0');
		}
	}

	podatki.close();

	return resitev;
}


int dolociMaksSteviloV0(const std::vector<short>& seznam) {

	std::vector<short>::const_iterator maxIt = seznam.begin();
	short maxVrednost1 = *seznam.begin();

	for (auto it = seznam.begin() + 1; it != seznam.end() - 1; it++) {

		if (*it > maxVrednost1) {
			maxVrednost1 = *it;
			maxIt = it;
		}
	}

	short maxVrednost2 = *(maxIt + 1);

	for (auto it = maxIt + 1; it != seznam.end(); it++) {

		if (*it > maxVrednost2)
			maxVrednost2 = *it;
	}

	return maxVrednost1 * 10 + maxVrednost2;
}
long long dolociMaksSteviloV1(const std::vector<short>& seznam, const short& steviloStevk) {

	if (steviloStevk > seznam.size())
		return -1;

	std::string resitev = "";

	std::vector<short>::const_iterator zacetek = seznam.cbegin();
	std::vector<short>::const_iterator konec = seznam.cend() - steviloStevk + 1;

	for (int i = 0; i < steviloStevk; i++) {

		auto it = std::max_element(zacetek, konec);

		resitev.push_back(*it + '0');

		zacetek = it + 1;
		if (konec != seznam.cend()) konec++;
	}

	return std::stoll(resitev);
}


long long vsotaVsehSeznamov(const std::vector<std::vector<short>>& bazaPodatkov, const short& steviloStevk) {

	long long resitev = 0;

	for (const std::vector<short>& podatek : bazaPodatkov)
		resitev += dolociMaksSteviloV1(podatek, steviloStevk);

	return resitev;
}


int main() {

	const std::vector<std::vector<short>> bazaPodatkov = preberiPodatke("2025/3.txt");

	long long resitev1 = vsotaVsehSeznamov(bazaPodatkov, 2);
	std::cout << "Vsota najvecjih vrednosti dvomestnih stevil je " << resitev1 << ".\n";

	long long resitev2 = vsotaVsehSeznamov(bazaPodatkov, 12);
	std::cout << "Vsota najvecjih vrednosti dvanajstmestnih stevil je " << resitev2 << ".\n";


	return 0;
}
