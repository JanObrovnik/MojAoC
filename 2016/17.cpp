/*
	t = 59:26 min
	nekoliko lahko
	t = 5:03 min
	zelo lahko
*/

#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <iomanip>
#include <openssl/md5.h>
#include <cstring>
#include <sstream>



enum class Smer {
	GOR,
	DOL,
	LEVO,
	DESNO,
};

class Tocka {
public:
	int x, y;
	int velikost;

	Tocka() : x(-1), y(-1), velikost(-1) {}
	Tocka(int x, int y, int velikost) : x(x), y(y), velikost(velikost) {}

	bool mozenPomik(const Smer& smer) const {
		int novaKoor;
		if		(smer == Smer::GOR)	  novaKoor = y - 1;
		else if (smer == Smer::DOL)	  novaKoor = y + 1;
		else if (smer == Smer::LEVO)  novaKoor = x - 1;
		else						  novaKoor = x + 1;
		return novaKoor >= 0 && novaKoor < velikost;
	}

	void pomik(const std::string& pot) {
		for (const char& c : pot) {
			if		(c == 'U') y--;
			else if (c == 'D') y++;
			else if (c == 'L') x--;
			else if (c == 'R') x++;
		}
	}

	bool konec() const {
		return x == velikost - 1 && y == velikost - 1;
	}
};


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


std::string najkrajsaPot(std::string kljuc) {

	std::map<int, char> usmeritev;
	usmeritev[0] = 'U';
	usmeritev[1] = 'D';
	usmeritev[2] = 'L';
	usmeritev[3] = 'R';

	std::string sifra = "";

	std::queue<std::string> zaporedniSeznam;
	zaporedniSeznam.push(kljuc);

	int dolzinaKljuca = kljuc.size();

	while (!zaporedniSeznam.empty()) {

		kljuc = zaporedniSeznam.front();
		zaporedniSeznam.pop();

		Tocka tocka(0, 0, 4);
		tocka.pomik(kljuc);

		if (tocka.konec())
			return std::string(kljuc.begin() + dolzinaKljuca, kljuc.end());

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc.c_str(), kljuc.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 2; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> sifra;


		for (int i = 0; i < 4; i++)
			if (sifra[i] >= 'b' && sifra[i] <= 'f')
				if (tocka.mozenPomik(static_cast<Smer>(i))) {
					std::string novKljuc = kljuc;
					novKljuc.push_back(usmeritev[i]);
					zaporedniSeznam.push(novKljuc);
				}
	}

	return "NAPAKA";
}

std::string najdalsaPot(std::string kljuc) {

	std::string resitev = "";

	std::map<int, char> usmeritev;
	usmeritev[0] = 'U';
	usmeritev[1] = 'D';
	usmeritev[2] = 'L';
	usmeritev[3] = 'R';

	std::string sifra = "";

	std::queue<std::string> zaporedniSeznam;
	zaporedniSeznam.push(kljuc);

	int dolzinaKljuca = kljuc.size();

	while (!zaporedniSeznam.empty()) {

		kljuc = zaporedniSeznam.front();
		zaporedniSeznam.pop();

		Tocka tocka(0, 0, 4);
		tocka.pomik(kljuc);

		if (tocka.konec()) {
			resitev = std::string(kljuc.begin() + dolzinaKljuca, kljuc.end());
			continue;
		}

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)kljuc.c_str(), kljuc.length(), digest);

		std::stringstream ss;
		for (int i = 0; i < 2; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ss >> sifra;


		for (int i = 0; i < 4; i++)
			if (sifra[i] >= 'b' && sifra[i] <= 'f')
				if (tocka.mozenPomik(static_cast<Smer>(i))) {
					std::string novKljuc = kljuc;
					novKljuc.push_back(usmeritev[i]);
					zaporedniSeznam.push(novKljuc);
				}
	}

	return resitev;
}


int main() {

	std::string kljuc = preberiPodatke("2016/17.txt");

	std::string resitev1 = najkrajsaPot(kljuc);
	std::cout << "Najkrajsa pot je " << resitev1 << ".\n";

	int resitev2 = najdalsaPot(kljuc).size();
	std::cout << "Najdaljsa pot je " << resitev2 << " dolga.\n";


	return 0;
}
