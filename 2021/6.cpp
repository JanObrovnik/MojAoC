/*
	t = 19:25 min
	Lahko
	t = 34:24 min
	Relativno lahko
*/

#include <iostream>
#include <fstream>
#include <list>
#include <array>
#include <algorithm>
#include <numeric>



class SkupinaRib {
public:
	long long stevilo;
	int cas;

	SkupinaRib() : stevilo(1), cas(6) {}
	SkupinaRib(int cas) : stevilo(1), cas(cas) {}
	SkupinaRib(long long stevilo, int cas) : stevilo(stevilo), cas(cas) {}
 };


long long steviloRibPoCasu(const std::array<SkupinaRib, 9>& seznamRib, const int& cas) {

	std::array<SkupinaRib, 9> novSeznamRib = seznamRib;
	std::array<SkupinaRib, 9> starSeznamRib = seznamRib;

	for (int i = 0; i < cas; i++) {

		for (const SkupinaRib& skupina : starSeznamRib) {
		
			if (skupina.cas == 0) {
				novSeznamRib[6].stevilo += skupina.stevilo;
				novSeznamRib[8].stevilo += skupina.stevilo;
				novSeznamRib[0].stevilo -= skupina.stevilo;
			}
			else {
				novSeznamRib[skupina.cas - 1].stevilo += skupina.stevilo;
				novSeznamRib[skupina.cas].stevilo -= skupina.stevilo;
			}
		}

		starSeznamRib = novSeznamRib;
	}


	long long resitev = 0;

	for (const SkupinaRib& skupina : starSeznamRib)
		resitev += skupina.stevilo;

	return resitev;
}


long long preberiPodatke(const std::string& pot, const int& cas) {

	long long resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int vrednost;
	std::array<SkupinaRib,9> seznamRib{SkupinaRib(0,0), SkupinaRib(0,1), SkupinaRib(0,2), 
									   SkupinaRib(0,3), SkupinaRib(0,4), SkupinaRib(0,5), 
									   SkupinaRib(0,6), SkupinaRib(0,7), SkupinaRib(0,8), };

	while (podatki.peek() != EOF) {
		
		podatki >> vrednost;
		podatki.ignore(1);

		auto it = std::find_if(seznamRib.begin(), seznamRib.end(), [&](const SkupinaRib& sr) {return sr.cas == vrednost; });

		it->stevilo++;
	}

	podatki.close();

	resitev = steviloRibPoCasu(seznamRib, cas);

	return resitev;
}



int main() {

	int resitev1 = preberiPodatke("2021/6.txt", 80);

	std::cout << "Stevilo rib po 80 dneh je " << resitev1 << ".\n";


	long long resitev2 = preberiPodatke("2021/6.txt", 256);

	std::cout << "Stevilo rib po 256 dneh je " << resitev2 << ".\n";


	return 0;
}
