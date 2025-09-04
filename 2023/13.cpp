/*
	Relativno lahko
	Relativno lahko (nerazumljivo besedilo)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs) {
	for (const std::string& vrstica : vs)
		os << vrstica << '\n';
	return os;
}


int najdiZrcaloVertikalno(const std::vector<std::string>& vzorec) {

	std::vector<int> reflekcija1{};
	std::vector<int> reflekcija2{};
	std::vector<int> resitev{};

	for (int i = 0; i < vzorec[0].size() - 1; i++) {

		bool enakost = true;

		for (int j = 0; j < vzorec.size(); j++) {

			if (vzorec[j][i] != vzorec[j][i + 1]) {
				enakost = false;
				break;
			}
		}

		if (enakost) {
			reflekcija1.push_back(i);
			reflekcija2.push_back(i + 1);
			resitev.push_back(i + 1);
		}
	}

	if (!resitev.empty()) {

		for (int i = 0; i < resitev.size(); i++) {

			bool pravilen = true;
			
			while (reflekcija1[i] >= 0 && reflekcija2[i] < vzorec.front().size()) {

				for (int j = 0; j < vzorec.size(); j++) {
					if (vzorec[j][reflekcija1[i]] != vzorec[j][reflekcija2[i]]) {
						pravilen = false;
						break;
					}
				}
				if (!pravilen)
					break;

				reflekcija1[i]--;
				reflekcija2[i]++;
			}

			if (pravilen)
				return resitev[i];
		}
	}

	return 0;
}
int najdiZrcaloHorizontalno(const std::vector<std::string>& vzorec) {

	std::vector<int> reflekcija1{};
	std::vector<int> reflekcija2{};
	std::vector<int> resitev{};

	for (int i = 0; i < vzorec.size() - 1; i++) {

		if (vzorec[i] == vzorec[i + 1]) {
			reflekcija1.push_back(i);
			reflekcija2.push_back(i + 1);
			resitev.push_back(i + 1);
		}
	}

	if (!resitev.empty()) {

		for (int i = 0; i < resitev.size(); i++) {
			
			bool pravilen = true;
			
			while (reflekcija1[i] >= 0 && reflekcija2[i] < vzorec.size()) {

				if (vzorec[reflekcija1[i]] != vzorec[reflekcija2[i]]) {
					pravilen = false;
					break;
				}

				reflekcija1[i]--;
				reflekcija2[i]++;
			}

			if (pravilen)
				return resitev[i];
		}
	}

	return 0;
}

int najdiZrcalo(const std::vector<std::string>& vzorec) {

	//std::cout << najdiZrcaloHorizontalno(vzorec) << '0';
	int resitevH = najdiZrcaloHorizontalno(vzorec) * 100;
	if (resitevH != 0)
		return resitevH;

	//std::cout << najdiZrcaloVertikalno(vzorec) << '\n';
	int resitevV = najdiZrcaloVertikalno(vzorec);
	if (resitevV != 0)
		return resitevV;
	

	return 0;
	/////////////////////////// mogoc ce je horizontalna ne more biti verikalna???
}


int razlicniElementi(const std::string& str1, const std::string& str2) {

	int resitev = 0;

	const size_t velikost = str1.size();
	for (int i = 0; i < velikost; i++)
		if (str1[i] != str2[i])
			resitev++;

	return resitev;
}

int najdiZrcaloVsestransko(const std::vector<std::string>& vzorec) {

	std::vector<int> reflekcija1{};
	std::vector<int> reflekcija2{};
	std::vector<int> resitev{};

	for (int i = 0; i < vzorec.size() - 1; i++) {

		if (razlicniElementi(vzorec[i], vzorec[i+1]) < 2) {
			reflekcija1.push_back(i);
			reflekcija2.push_back(i + 1);
			resitev.push_back(i + 1);
		}
	}

	for (int i = 0; i < resitev.size(); i++) {
	
		int steviloNapak = 0;

		while (reflekcija1[i] >= 0 && reflekcija2[i] < vzorec.size()) {

			steviloNapak += razlicniElementi(vzorec[reflekcija1[i]], vzorec[reflekcija2[i]]);

			reflekcija1[i]--;
			reflekcija2[i]++;
		}
		//std::cout << "nap: " << steviloNapak << ' ';
		if (steviloNapak == 1)
			return resitev[i];
	}

	return 0;
}

std::vector<std::string> transform(const std::vector<std::string>& vzorec) {

	std::vector<std::string> resitev(vzorec.front().size(), "");

	for (int s = 0; s < vzorec.front().size(); s++)
		for (int v = 0; v < vzorec.size(); v++)
			resitev[s].push_back(vzorec[v][s]);

	return resitev;
}

int najdiZrcalo2(std::vector<std::string> vzorec) {

	int resitev1 = najdiZrcaloVsestransko(vzorec) * 100;
	//std::cout << '\n' << resitev1 << '\t';
	if (resitev1 != 0)
		return resitev1;

	vzorec = transform(vzorec);
	int resitev2 = najdiZrcaloVsestransko(vzorec);
	//std::cout << resitev2;
	if (resitev2 != 0)
		return resitev2;


	return 0;
}


std::pair<int,int> preberiPodatke(const std::string& pot) {

	int resitev1 = 0;
	int resitev2 = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	std::vector<std::string> vzorec;
	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty()) {
			resitev1 += najdiZrcalo(vzorec);
			resitev2 += najdiZrcalo2(vzorec);
			vzorec.clear();
			continue;
		}

		vzorec.push_back(vrstica);
	}
	resitev1 += najdiZrcalo(vzorec);
	resitev2 += najdiZrcalo2(vzorec);

	podatki.close();


	return { resitev1,resitev2 };
}



int main() {

	std::pair<int, int> resitevi(preberiPodatke("Advent23/13.txt"));

	std::cout << "Sumerizacija je " << resitevi.first << ".\n";
	std::cout << "Sumerizacija z odpravljeno napako je " << resitevi.second << ".\n";


	


	return 0;
}
