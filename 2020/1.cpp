/*
	t = 9:51 min
	zelo lahko
	t = 7:56 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> preberiPodatke(std::string pot) {

	std::vector<int> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce najti.\n";
		return resitev;
	}

	int vrednsot;

	while (podatki >> vrednsot)
		resitev.push_back(vrednsot);

	return resitev;
}


int najdiZmozekVsote2(std::vector<int> seznamStevil, const int& vsota = 2020) {

	for (int i = 0; i < seznamStevil.size() - 1; i++)
		for (int j = i + 1; j < seznamStevil.size(); j++)
			if (seznamStevil[i] + seznamStevil[j] == vsota)
				return seznamStevil[i] * seznamStevil[j];

	return -1;
}

int najdiZmozekVsote3(std::vector<int> seznamStevil, const int& vsota = 2020) {

	for (int i = 0; i < seznamStevil.size() - 1; i++)
		for (int j = i + 1; j < seznamStevil.size(); j++)
			for (int k = j + 1; k < seznamStevil.size(); k++)
				if (seznamStevil[i] + seznamStevil[j] + seznamStevil[k] == vsota)
					return seznamStevil[i] * seznamStevil[j] * seznamStevil[k];

	return -1;
}


int main() {

	std::vector<int> seznamPlacil(preberiPodatke("2020/1.txt"));
	

	int resitev1 = najdiZmozekVsote2(seznamPlacil);

	std::cout << "zmozek vsote dveh je " << resitev1 << ".\n";


	int resitev2 = najdiZmozekVsote3(seznamPlacil);

	std::cout << "zmozek vsote treh je " << resitev2 << ".\n";


	return 0;
}
