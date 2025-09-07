/*
	t = 27:37 min
	precej lahko
	t = 1:37:20 ur
	tezko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


std::vector<short> preberiPodatke(const std::string& pot) {

	std::vector<short> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	short vrednost;

	while (podatki >> vrednost)
		resitev.push_back(vrednost);

	podatki.close();

	return resitev;
}


int najdiVrednostNarascanja(std::vector<short> seznamPodatkov) {

	std::sort(seznamPodatkov.begin(), seznamPodatkov.end());
	seznamPodatkov.push_back(seznamPodatkov.back() + 3);

	std::vector<int> kolicinaRazlik{ 0,0,0 };

	int trenutnaVrednost = 0;

	for (const int& i : seznamPodatkov) {

		kolicinaRazlik[i - trenutnaVrednost - 1]++;

		trenutnaVrednost = i;
	}

	//std::cout << kolicinaRazlik[0] << ' ' << kolicinaRazlik[1] << ' ' << kolicinaRazlik[2] << '\n';
	return kolicinaRazlik[0] * kolicinaRazlik[2];
}

long long najdiSteviloRazlicnihUreditev(std::vector<short> seznamPodatkov) {

	long long resitev = 1;

	std::sort(seznamPodatkov.begin(), seznamPodatkov.end());
	seznamPodatkov.insert(seznamPodatkov.begin(), 0);
	seznamPodatkov.push_back(seznamPodatkov.back() + 3);

	std::vector<std::vector<bool>> seznamKolicineRazlik;

	for (int i = 0; i < seznamPodatkov.size(); i++) {
		seznamKolicineRazlik.push_back({ 0,0,0 });

		for (const int j : {1, 2, 3}) {

			if (i + j >= seznamPodatkov.size())
				break;

			const short& razlika = seznamPodatkov[i + j] - seznamPodatkov[i];
			if (razlika <= 3) {
				seznamKolicineRazlik.back()[razlika - 1] = true;
			}
			else break;
		}
	}

	std::map<size_t, long long> seznamRazlicnihPoti; // odcitan vzorec
	seznamRazlicnihPoti[1] = 2;
	seznamRazlicnihPoti[2] = 4;
	seznamRazlicnihPoti[3] = 7;
	seznamRazlicnihPoti[4] = 12;
	seznamRazlicnihPoti[5] = 21;

	for (int i = 0; i < seznamKolicineRazlik.size(); i++) {

		//std::cout << seznamPodatkov[i] << " :\t" << seznamKolicineRazlik[i][0] << ", "
	   	//		  << seznamKolicineRazlik[i][1] << ", " << seznamKolicineRazlik[i][2] << " :\t" 
		//		  << seznamKolicineRazlik[i][0]+seznamKolicineRazlik[i][1]+seznamKolicineRazlik[i][2] << '\n';

		int razlicnePoti = seznamKolicineRazlik[i][0] + seznamKolicineRazlik[i][1] + seznamKolicineRazlik[i][2];

		size_t velikost = 0;

		if (razlicnePoti == 2) {
	
			velikost++;

			for (int j = i - 1; j >= 0; j--) {
				
				int razlicnePoti = seznamKolicineRazlik[j][0] + seznamKolicineRazlik[j][1] + seznamKolicineRazlik[j][2];
				
				if (razlicnePoti == 3)
					velikost++;
				else if (razlicnePoti == 1)
					break;
			}

			resitev *= seznamRazlicnihPoti.at(velikost);
			velikost = 0;
		}
	}

	return resitev;
}


int main() {

	std::vector<short> bazaPodatkov(preberiPodatke("2020/10.txt"));

	int resitev1 = najdiVrednostNarascanja(bazaPodatkov);
	std::cout << "Vrednost narascanja je " << resitev1 << ".\n";

	long long resitev2 = najdiSteviloRazlicnihUreditev(bazaPodatkov);
	std::cout << "Stevilo razlicnih ureditev je " << resitev2 << ".\n";

	return 0;
}
