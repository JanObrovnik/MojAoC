/*
	t = 1:28:45 ur
	nekoliko lahko
	t = 3:01 min
	neverjetno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Disk {
public:
	int steviloPozicij;
	int zacetnaPozicija;
	int casovniZamik;
	int casovnaNicla;

	Disk() : steviloPozicij(-1), zacetnaPozicija(-1), casovniZamik(-1) { dolociNiclo(); }
	Disk(int stPozicij, int zacPozicija, int casZamik) : steviloPozicij(stPozicij), zacetnaPozicija(zacPozicija), casovniZamik(casZamik) { dolociNiclo(); }

	void dolociNiclo() {
		casovnaNicla = steviloPozicij - zacetnaPozicija - casovniZamik;
		while (casovnaNicla < 0) casovnaNicla += steviloPozicij;
	}

	int pozicijaObCasu(const int& cas) const {
		return (cas - casovnaNicla) % steviloPozicij;
	}
};


std::vector<Disk> preberiPodatke(const std::string& pot) {

	std::vector<Disk> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int steviloPozicij, zacetnaPozicija;
	int zamik = 1;

	while (podatki.peek() != EOF) {
		
		podatki.ignore(12);
		podatki >> steviloPozicij;
		podatki.ignore(41);
		podatki >> zacetnaPozicija;
		podatki.ignore(1);

		resitev.push_back(Disk(steviloPozicij, zacetnaPozicija, zamik++));
	}

	podatki.close();

	return resitev;
}


int isciPoravnanje(const std::vector<Disk>& seznamDiskov) {

	// y = inkrement x + zacetek
	int inkrement = 0;
	int zacetek = 0;

	for (const Disk& disk : seznamDiskov) {
		if (inkrement < disk.steviloPozicij) {
			inkrement = disk.steviloPozicij;
			zacetek = disk.casovnaNicla;
		}
	}

	int cas = zacetek - inkrement;
	bool poravnano = false;

	while (!poravnano) {

		if (cas % (10 * inkrement) == 0)
			std::cout << cas << '\n';

		cas += inkrement;
		poravnano = true;

		for (int i = 0; i < seznamDiskov.size(); i++) {
			const Disk& disk = seznamDiskov[i];
			//std::cout << disk.steviloPozicij << " | " << disk.zacetnaPozicija << " | " << disk.casovnaNicla << " | " << disk.casovniZamik << '\n';

			const int pozicija = disk.pozicijaObCasu(cas);

			if (pozicija != 0) {
				poravnano = false;
				break;
			}
		}
	}

	return cas;
}


int main() {

	std::vector<Disk> seznamDiskov = preberiPodatke("2016/15.txt");


	int resitev1 = isciPoravnanje(seznamDiskov);
	std::cout << "Poravnanje dobimo ob casu " << resitev1 << ".\n";

	seznamDiskov.push_back(Disk(11, 0, seznamDiskov.size() + 1));

	int resitev2 = isciPoravnanje(seznamDiskov);
	std::cout << "Z dodatnim diskom dobimo poravnanje ob casu " << resitev2 << ".\n";


	return 0;
}
