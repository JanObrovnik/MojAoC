/*
	t = 15:31 min
	precej lahko
	t = 19:40 min
	precej lahko
*/

#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <openssl/md5.h>
#include <cstring>
#include <sstream>
#include <algorithm>


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


std::string poisciGeslo1(const std::string& kljuc) {

	std::string resitev = "";

	std::string iskano = "......";

	int dodatek = 0;

	std::string kljuc2 = kljuc;

	while (resitev.size() < 8) {

		if (dodatek % 100000 == 0) std::cout << dodatek << ":\t" << resitev << '\n';

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc2.c_str(), kljuc2.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 3; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> iskano;

		if (iskano.substr(0, 5) == "00000")
			resitev.push_back(iskano[5]);

		kljuc2 = kljuc + std::to_string(dodatek);
		dodatek++;
	}

	return resitev;
}

std::string poisciGeslo2(const std::string& kljuc) {

	std::string resitev = "........";
	int steviloZamenjav = 0;

	std::string iskano = "........";

	int dodatek = 0;

	std::string kljuc2 = kljuc;

	while (steviloZamenjav < 8) {

		if (dodatek % 100000 == 0) std::cout << dodatek << ":\t" << resitev << '\n';

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc2.c_str(), kljuc2.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 4; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> iskano;

		if (iskano.substr(0, 5) == "00000") {
			const int mesto = iskano[5] - '0';
			const char& zamenjava = iskano[6];
			if ((mesto >= 0 && mesto <= 7) && resitev[mesto] == '.') {
				resitev[mesto] = zamenjava;
				steviloZamenjav++;
			}
		}

		kljuc2 = kljuc + std::to_string(dodatek);
		dodatek++;
	}

	return resitev;
}


int main() {

	std::string kljuc = preberiPodatke("2016/5.txt");


	std::string resitev1 = poisciGeslo1(kljuc);
	std::cout << "Potreben dodatek je " << resitev1 << ".\n";

	std::string resitev2 = poisciGeslo2(kljuc);
	std::cout << "Potreben dodatek je " << resitev2 << ".\n";


	return 0;
}
