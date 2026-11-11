/*
	t = 1:56:20 ur
	relativno lahko
	t = 46:41 min
	zelo lahko (malo dolgotrajno)
*/

#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <openssl/md5.h>
#include <cstring>


std::string preberiPodatke(const std::string& pot) {

	std::string resitev;
	
	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki >> resitev;

	podatki.close();

	return resitev;
}


std::vector<char> preveriPonavljivost(const std::string& koda, const int& maxPonavljanje) {

	std::vector<char> resitev;

	int stPonavljanja = 1;
	char cPonavljanja = ' ';

	for (const char& c : koda) {

		if (c == cPonavljanja) {
			stPonavljanja++;
			if (stPonavljanja == maxPonavljanje)
				resitev.push_back(c);
		}
		else {
			stPonavljanja = 1;
			cPonavljanja = c;
		}
	}

	return resitev;
}

unsigned int isciStevilkoKljuca(const std::string& salt, int steviloKljucev, const int& ponovitevMD5 = 0) {

	std::string koda = "";

	unsigned int dodatek = 0;

	std::vector<std::pair<char, unsigned int>> seznamPonDod;

	while (true) {

		//if (dodatek % 1000 == 0) break;
		if (dodatek % 100 == 0) std::cout << "dodatek: " << dodatek << '\n';
		
		std::string kljuc = salt + std::to_string(dodatek);

		if (seznamPonDod.size() > 0 && (dodatek - seznamPonDod.front().second) > 1000)
			seznamPonDod.erase(seznamPonDod.begin());


		for (int i = 0; i < 1 + ponovitevMD5; i++) {
			unsigned char digest[MD5_DIGEST_LENGTH];
			MD5((unsigned char*)kljuc.c_str(), kljuc.length(), digest);

			std::stringstream ss;
			for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
			}
			ss >> koda;
			kljuc = koda;
		}

		std::vector<char> ponavljanje3 = preveriPonavljivost(koda, 3);
		std::vector<char> ponavljanje5 = preveriPonavljivost(koda, 5);

		for (const char & c : ponavljanje5) {

			std::vector<unsigned int> seznamMest = {};

			auto it = std::remove_if(seznamPonDod.begin(), seznamPonDod.end(), [&](const std::pair<char, unsigned int>& par) {
				if (par.first == c) {
					seznamMest.push_back(par.second);
					return true;
				}
				return false;
				});

			steviloKljucev -= (seznamPonDod.end() - it);
			
			std::cout << '-' << steviloKljucev << '\n';

			if (steviloKljucev <= 0) {
				return seznamMest[seznamMest.size() + steviloKljucev - 1];
			}

			seznamPonDod.erase(it, seznamPonDod.end());
		}
		
		if (!ponavljanje3.empty())
			seznamPonDod.push_back({ ponavljanje3.front(), dodatek});

		dodatek++;
	}

	return -1;
}


int main() {

	int steviloKljucev = 64;
	std::string salt = preberiPodatke("2016/14.txt");


	unsigned int resitev1 = isciStevilkoKljuca(salt, steviloKljucev);
	std::cout << "Kljuc " << steviloKljucev << " dosezemo v koraku " << resitev1 << ".\n";

	unsigned int resitev2 = isciStevilkoKljuca(salt, steviloKljucev, 2016);
	std::cout << "Kljuc " << steviloKljucev << " dosezemo v koraku " << resitev2 << ".\n";


	return 0;
}
