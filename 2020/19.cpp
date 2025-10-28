/*
	t ~ 3:33 ur
	nekoliko tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>


class Pravilo {

	short id;
	std::vector<std::vector<short>> sledenje;
	std::vector<std::string> koda;

public:
	Pravilo() : id(-1), sledenje({}), koda({}) {}
	Pravilo(short id) : id(id), sledenje({}), koda({}) {}

	void dolociSledenje(const std::vector<short>& sled) { sledenje.push_back(sled); }
	void dolociKodo(const std::string& koda) { this->koda.push_back(koda); }

	short dobiID() const { return id; }
	std::vector<std::string> dobiKodo() const { return koda; }
	std::unordered_set<std::string> dobiKodoSet() const {
		std::unordered_set<std::string> resitev;
		for (const std::string& kod : koda)
			resitev.insert(kod);
		return resitev;
	}

	int velikost() const { return koda.size(); }


	void razberiKodo(const std::vector<Pravilo>& seznamPravil) {
		
		for (const std::vector<short>& vec : sledenje) {
			
			int menjava = 1;
			for (const short& i : vec)
				menjava *= seznamPravil[i].koda.size();

			std::vector<std::string> novaKoda(menjava, "");

			for (const short& i : vec) {

				menjava /= seznamPravil[i].koda.size();

				int index = 0;
				int domenjave = 0;

				for (std::string& kod : novaKoda) {

					kod += seznamPravil[i].koda[index % seznamPravil[i].koda.size()];
					
					domenjave++;
					if (domenjave == menjava) {
						domenjave = 0;
						index++;
					}
				}
			}

			for (const std::string& kod : novaKoda)
				koda.push_back(kod);
		}
	}

	bool moznoRazbratiKodo(const std::set<short>& seznamID) const {
		
		std::vector<short> skupnoSledenje;
		for (const std::vector<short>& vec : sledenje)
			for (const short& i : vec)
				skupnoSledenje.push_back(i);
		
		return std::all_of(skupnoSledenje.begin(), skupnoSledenje.end(), [&](const short& i) {return seznamID.find(i) != seznamID.end(); });
	}


	friend void printPodatke(const std::vector<Pravilo>& seznamPravil, const std::vector<std::string>& seznamKod);
	friend std::pair<std::vector<Pravilo>, std::vector<std::string>> preberiPodatke(const std::string& pot);
	friend int main();
};


std::vector<short> preberiVrstico(const std::string::iterator& itZac, const std::string::iterator& itKon) {

	std::vector<short> resitev;

	std::string vrstica(itZac, itKon);

	while (!vrstica.empty()) {

		size_t novZac;

		resitev.push_back((short)std::stoi(vrstica, &novZac));

		vrstica = vrstica.substr(novZac);
	}

	return resitev;
}
std::pair<std::vector<Pravilo>, std::vector<std::string>> preberiPodatke(const std::string& pot) {

	std::vector<Pravilo> resitev1;
	std::vector<std::string> resitev2;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	std::string vrstica;
	bool izvirniPodatek;
	bool deljenec;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty())
			break;

		short id = (short)std::stoi(vrstica);
		resitev1.push_back(Pravilo(id));
		Pravilo& trenutnoPravilo = resitev1.back();

		auto itIzvirni = std::find(vrstica.begin(), vrstica.end(), '"');
		if (itIzvirni != vrstica.end()) {
			trenutnoPravilo.dolociKodo(std::string(itIzvirni + 1, itIzvirni + 2));
			continue;
		}

		auto itZacetek = std::find(vrstica.begin(), vrstica.end(), ':');
		auto itDeljenec = std::find(vrstica.begin(), vrstica.end(), '|');
		deljenec = (itDeljenec != vrstica.end());

		if (!deljenec) {
			trenutnoPravilo.dolociSledenje(preberiVrstico(itZacetek + 1, vrstica.end()));
		}
		else {
			trenutnoPravilo.dolociSledenje(preberiVrstico(itZacetek + 1, itDeljenec - 1));
			trenutnoPravilo.dolociSledenje(preberiVrstico(itDeljenec + 1, vrstica.end()));
		}
	}
	while (std::getline(podatki, vrstica))
		resitev2.push_back(vrstica);

	podatki.close();

	std::sort(resitev1.begin(), resitev1.end(), [](const Pravilo& pr1, const Pravilo& pr2) {return pr1.id < pr2.id; });

	return { resitev1,resitev2 };
}


std::unordered_set<std::string> razberiKodoZaID(std::vector<Pravilo> seznamPravil, const short& id) {

	std::set<short> seznamZnanihKod;

	for (const Pravilo& pravilo : seznamPravil)
		if (pravilo.velikost() > 0)
			seznamZnanihKod.insert(pravilo.dobiID());

	while (seznamPravil[id].velikost() == 0) {
		//std::cout << '\n';
		for (Pravilo& pravilo : seznamPravil) {

			if (seznamZnanihKod.find(pravilo.dobiID()) != seznamZnanihKod.end())
				continue;

			//std::cout << pravilo.dobiID() << ": " << pravilo.moznoRazbratiKodo(seznamZnanihKod) << '\n';
			if (pravilo.moznoRazbratiKodo(seznamZnanihKod)) {
				pravilo.razberiKodo(seznamPravil);
				seznamZnanihKod.insert(pravilo.dobiID());
			}
		}
	}

	return seznamPravil[id].dobiKodoSet();
}

int steviloPrekrivanaj(const std::vector<std::string>& seznamKod1, const std::unordered_set<std::string>& seznamKod2) {

	int resitev = 0;

	for (const std::string& koda1 : seznamKod1)
		if (seznamKod2.find(koda1) != seznamKod2.end())
			resitev++;

	return resitev;
}


void printPodatke(const std::vector<Pravilo>& seznamPravil, const std::vector<std::string>& seznamKod) {

	std::cout << "Seznam Pravil:\n";
	for (const Pravilo& pravilo : seznamPravil) {
		std::cout << pravilo.id << ": ";
		for (const std::vector<short>& vec : pravilo.sledenje) {
			for (const short& i : vec) {
				std::cout << i << ' ';
			}
			std::cout << "| ";
		}
		for (const std::string& koda : pravilo.koda)
			std::cout << koda << " / ";
		std::cout << '\n';
	}

	std::cout << "\nSeznam Kod:\n";
	for (const std::string& vrstica : seznamKod) {
		std::cout << vrstica << '\n';
	}
}

int main() {

	std::pair<std::vector<Pravilo>, std::vector<std::string>> podatki(preberiPodatke("2020/19.txt"));
	std::vector<Pravilo>& seznamPravil = podatki.first;
	std::vector<std::string>& seznamKod = podatki.second;
	//printPodatke(seznamPravil, seznamKod);

	std::unordered_set<std::string> seznamVeljavnihKod = razberiKodoZaID(seznamPravil, 0);

	int resitev1 = steviloPrekrivanaj(seznamKod, seznamVeljavnihKod);

	std::cout << "Stevilo vseh kod, ki se prekrivajo je " << resitev1 << ".\n";


	return 0;
}
