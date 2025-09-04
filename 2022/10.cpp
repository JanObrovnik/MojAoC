/*
	Lahko
	Lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>



std::vector<int> preberiPodatke(std::string pot, std::vector<int> iskaniCikli) {

	std::vector<int> resitev1{};
	std::string resitev2;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev1;
	}

	int registerCPU(1);
	int trenutniCikelj(0);
	int cakalnaVrsta(0);

	std::string ukaz;
	int vrednost;

	while (podatki.peek() != EOF) {

		trenutniCikelj++;

		int navidezniCikelj(trenutniCikelj % 40);
		resitev2.push_back((navidezniCikelj >= registerCPU - 0 && navidezniCikelj <= registerCPU + 2) ? '#' : '.');

		if (iskaniCikli.size() > 0 && iskaniCikli.front() == trenutniCikelj) {
			resitev1.push_back(trenutniCikelj * registerCPU);
			iskaniCikli.erase(iskaniCikli.begin());
		}

		if (cakalnaVrsta == 0) {
			podatki >> ukaz;

			if (ukaz == "addx") {
				podatki >> vrednost;
				cakalnaVrsta += 2;
			}
			else {
				cakalnaVrsta += 1;
			}
		}
		else {
			registerCPU += vrednost;
		}

		cakalnaVrsta--;
	}
	std::cout << "Prikaz na zaslonu:";
	for (int i = 0; i < resitev2.size(); i++) {
		if (i % 40 == 0) std::cout << '\n';
		std::cout << resitev2[i];
	} std::cout << '\n' << std::endl;

	podatki.close();


	return resitev1;
}


int main() {

	std::vector<int> seznamResitev(preberiPodatke("Advent22/10.txt", { 20,60,100,140,180,220 }));
	int resitev1(std::accumulate(seznamResitev.begin(), seznamResitev.end(), 0));
	std::cout << "Vsota vseh moci signalov je " << resitev1 << ".\n";


	return 0;
}