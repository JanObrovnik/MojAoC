/*
	t = 28:38 min
	lahko
	t = 10:31 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>


std::vector<short> preberiPodatke(const std::string& pot) {

	std::vector<short> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	short i;

	while (podatki.peek() != EOF) {
		podatki >> i;
		podatki.ignore(1);
		resitev.push_back(i);
	}

	podatki.close();

	return resitev;
}


int najdiPovprecje(const std::vector<short>& seznampodmornic) {
	
	short velikost(seznampodmornic.size());
	int vsota(std::accumulate(seznampodmornic.begin(), seznampodmornic.end(), 0));

	return vsota / velikost;
}

int najdiOdstopek1(const std::vector<short>& seznampodmornic, const int& povprecje, const int& predhodniOdtopek) {

	int odstopek = 0;

	for (const short& i : seznampodmornic) {
		odstopek += abs(i - povprecje);
	}

	if (predhodniOdtopek < odstopek)
		return predhodniOdtopek;

	odstopek = najdiOdstopek1(seznampodmornic, povprecje - 1, odstopek);
	odstopek = najdiOdstopek1(seznampodmornic, povprecje + 1, odstopek);

	return odstopek;
}


int porabaGoriva(const int& oddaljenost) {
	const int& i = oddaljenost;
	return i * (i + 1) / 2;
}

int najdiOdstopek2(const std::vector<short>& seznampodmornic, const int& povprecje, const int& predhodniOdtopek) {

	int odstopek = 0;

	for (const short& i : seznampodmornic) {
		odstopek += porabaGoriva(abs(i - povprecje));
	}

	if (predhodniOdtopek < odstopek)
		return predhodniOdtopek;

	odstopek = najdiOdstopek2(seznampodmornic, povprecje - 1, odstopek);
	odstopek = najdiOdstopek2(seznampodmornic, povprecje + 1, odstopek);

	return odstopek;
}


int main(int argc, char* argv[]) {

	std::vector<short> seznampodmornic(preberiPodatke("2021/7.txt"));

	int prvotnaOcena(najdiPovprecje(seznampodmornic));

	int resitev1 = najdiOdstopek1(seznampodmornic, prvotnaOcena, 100000000);

	std::cout << "Skupno minimalno stevilo premikov je " << resitev1 << ".\n"; // 352331


	int resitev2 = najdiOdstopek2(seznampodmornic, prvotnaOcena, 100000000);

	std::cout << "Skupno minimalno stevilo premikov je " << resitev2 << ".\n"; // 99266250


	return 0;
}
