/*
	t ~ 2 ur
	ni mi vsec
	t = 47:51 min
	zelo lahko
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


int preglejHash5(const std::string& kljuc) {

	std::string resitev = "......";

	int dodatek = 0;

	std::string kljuc2 = kljuc;

	while (true) {

		//if (dodatek % 10000 == 0) std::cout << dodatek << '\n';

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc2.c_str(), kljuc2.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 3; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> resitev;

		if (resitev.substr(0, 5) == "00000")
			return dodatek - 1;

		kljuc2 = kljuc + std::to_string(dodatek);
		dodatek++;
	}

	return -1;
}
int preglejHash6(const std::string& kljuc) {

	std::string resitev = ".......";

	int dodatek = 0;

	std::string kljuc2 = kljuc;

	while (true) {

		//if (dodatek % 10000 == 0) std::cout << dodatek << '\n';

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc2.c_str(), kljuc2.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 3; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> resitev;

		if (resitev.substr(0, 6) == "000000")
			return dodatek - 1;

		kljuc2 = kljuc + std::to_string(dodatek);
		dodatek++;
	}

	return -1;
}


int main() {

	std::string kljuc = preberiPodatke("2015/4.txt");


	int resitev1 = preglejHash5(kljuc);
	std::cout << "Potreben dodatek je " << resitev1 << ".\n";

	int resitev2 = preglejHash6(kljuc);
	std::cout << "Potreben dodatek je " << resitev2 << ".\n";


	return 0;
}
