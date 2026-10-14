/*
	t = 40:58 min
	relativno lahko
	t = 32:14 min
	relativno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bit>


std::vector<short> preberiPodatke(const std::string& pot) {

	std::vector<short> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	short velikost;

	while (podatki >> velikost)
		resitev.push_back(velikost);

	podatki.close();

	return resitev;
}


bool zadovljuje(const std::vector<short>& zalogovniki, const short& kolicina, const int& kombinacija) {

	short skupno = 0;

	for (int i = 0; i < zalogovniki.size(); i++)
		if (((1 << i) & kombinacija) == ((1 << i)))
			skupno += zalogovniki[i];

	return skupno == kolicina;
}

int steviloRazlicnihKombinacij(const std::vector<short>& zalogovniki, const short& kolicina) {

	int resitev = 0;

	int max = pow(2, zalogovniki.size());

	for (int i = 1; i < max; i++)
		resitev += zadovljuje(zalogovniki, kolicina, i);

	return resitev;
}

std::pair<int,int> zadovoljujeMin(const std::vector<short>& zalogovniki, const short& kolicina) {

	int minSteviloZalogovnikov = zalogovniki.size();
	int steviloZalogovnikov = 1;

	int max = pow(2, zalogovniki.size());

	for (unsigned int i = 1; i < max; i++) {
		
		if (std::popcount(i) <= minSteviloZalogovnikov && zadovljuje(zalogovniki, kolicina, i)) {

			if (std::popcount(i) < minSteviloZalogovnikov) {
				minSteviloZalogovnikov = std::popcount(i);
				steviloZalogovnikov = 1;
			}
			else {
				steviloZalogovnikov++;
			}
		}
	}

	return { minSteviloZalogovnikov, steviloZalogovnikov };
}




int main() {

	std::vector<short> zalogovniki = preberiPodatke("2015/17.txt");

	int kolicina = 150;


	int resitev1 = steviloRazlicnihKombinacij(zalogovniki, kolicina);
	std::cout << "Razdelimo lahko na stevilo kombinacij " << resitev1 << ".\n";

	std::pair<int, int> resitev2 = zadovoljujeMin(zalogovniki, kolicina);
	std::cout << "Najmanjse stevilo zalogovnikov je " << resitev2.first << " s " << resitev2.second << "-imi razlicnimi kombinacijami.\n";


	return 0;
}
