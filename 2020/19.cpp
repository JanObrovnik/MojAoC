/*
	t ~ 3:33 ur
	nekoliko tezko
	t ~ 4:35 ur
	tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
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
				int doMenjave = 0;

				for (std::string& kod : novaKoda) {

					kod += seznamPravil[i].koda[index % seznamPravil[i].koda.size()];
					
					doMenjave++;
					if (doMenjave == menjava) {
						doMenjave = 0;
						index++;
					}
				}
			}

			for (const std::string& kod : novaKoda)
				koda.push_back(kod);
		}
	}

	bool moznoRazbratiKodo(std::set<short> seznamID) const {
		
		seznamID.insert(id);

		std::vector<short> skupnoSledenje;
		for (const std::vector<short>& vec : sledenje)
			for (const short& i : vec)
				skupnoSledenje.push_back(i);
		
		return std::all_of(skupnoSledenje.begin(), skupnoSledenje.end(), [&](const short& i) {return seznamID.find(i) != seznamID.end(); });
	}

	void razberiKodoSPonavljanjem(const std::vector<Pravilo>& seznamPravil, const short maxDolzina) {
		
		const std::vector<short>& vec = sledenje.front();

		int menjava = 1;
		for (const short& i : vec)
			menjava *= seznamPravil[i].koda.size();

		std::vector<std::string> novaKoda(menjava, "");

		for (const short& i : vec) {

			menjava /= seznamPravil[i].koda.size();

			int index = 0;
			int doMenjave = 0;

			for (std::string& kod : novaKoda) {

				kod += seznamPravil[i].koda[index % seznamPravil[i].koda.size()];

				doMenjave++;
				if (doMenjave == menjava) {
					doMenjave = 0;
					index++;
				}
			}
		}

		for (const std::string& kod : novaKoda)
			koda.push_back(kod);
	}

	bool zajemaSamSebe() {

		for (const std::vector<short>& vec : sledenje)
			for (const short& i : vec)
				if (i == id)
					return true;
		return false;
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


std::unordered_set<std::string> razberiKodoZaID(std::vector<Pravilo> seznamPravil, const short& id, const short& maxDolzina) {

	std::set<short> seznamZnanihKod;

	for (const Pravilo& pravilo : seznamPravil)
		if (pravilo.velikost() > 0)
			seznamZnanihKod.insert(pravilo.dobiID());

	while (seznamPravil[id].velikost() == 0) {

		for (Pravilo& pravilo : seznamPravil) {

			if (seznamZnanihKod.find(pravilo.dobiID()) != seznamZnanihKod.end())
				continue;

			if (pravilo.moznoRazbratiKodo(seznamZnanihKod)) {

				if (pravilo.zajemaSamSebe()) {
					pravilo.razberiKodoSPonavljanjem(seznamPravil, maxDolzina);
					seznamZnanihKod.insert(pravilo.dobiID());
				}
				else {
					pravilo.razberiKodo(seznamPravil);
					seznamZnanihKod.insert(pravilo.dobiID());
				}
			}
		}
	}

	return seznamPravil[id].dobiKodoSet();
}

int steviloPrekrivanja(const std::vector<std::string>& seznamKod1, const std::unordered_set<std::string>& seznamKod2) {

	int resitev = 0;

	for (const std::string& koda1 : seznamKod1)
		if (seznamKod2.find(koda1) != seznamKod2.end())
			resitev++;

	return resitev;
}

bool moznoSestavitiNivo0(const std::string& subKoda, const short& velikostKode, const std::unordered_set<std::string>& setKod) {

	std::vector<std::string> seznamDelovKode;

	for (short meja = 0; meja < subKoda.size(); meja += velikostKode)
		seznamDelovKode.push_back(subKoda.substr(meja, velikostKode));

	return std::all_of(seznamDelovKode.begin(), seznamDelovKode.end(), [&](const std::string& i) {return setKod.find(i) != setKod.end(); });
}
bool moznoSestavitiNivo1(const std::string& subKoda, const std::vector<std::unordered_set<std::string>>& seznamVeljavnihKod, const short& maxDolzina) {

	const short& velikostKode0 = seznamVeljavnihKod[0].begin()->size();
	const short& velikostKode1 = seznamVeljavnihKod[1].begin()->size();

	const short meja = subKoda.size() / 2;

	const std::string& kodaDel0 = subKoda.substr(0, meja);
	const std::string& kodaDel1 = subKoda.substr(meja);

	if (moznoSestavitiNivo0(kodaDel0, velikostKode0, seznamVeljavnihKod[0]) && moznoSestavitiNivo0(kodaDel1, velikostKode1, seznamVeljavnihKod[1]))
		return true;

	return false;
}

int steviloPrekrivanja(const std::vector<std::string>& vektorKod, const std::vector<std::unordered_set<std::string>>& seznamVeljavnihKod, const short& maxDolzina) {

	int resitev = 0;

	for (const std::string& gledanaKoda : vektorKod) {

		const short& velikostKode0 = seznamVeljavnihKod[0].begin()->size();
		const short& velikostKode1 = seznamVeljavnihKod[1].begin()->size();

		for (short meja = velikostKode0; meja < maxDolzina - velikostKode1; meja += velikostKode0) {
		
			const std::string& kodaDel0 = gledanaKoda.substr(0, meja);
			const std::string& kodaDel1 = gledanaKoda.substr(meja);

			if (kodaDel1.empty())
				break;
		
			if (kodaDel1.size() % velikostKode1 != 0)
				continue;
			

			bool obstaja0 = moznoSestavitiNivo0(kodaDel0, velikostKode0, seznamVeljavnihKod[0]);
			if (!obstaja0) break;
		
			const std::vector<std::unordered_set<std::string>> seznamSubVeljavnihKod(seznamVeljavnihKod.begin() + 2, seznamVeljavnihKod.end());
			bool obstaja1 = moznoSestavitiNivo1(kodaDel1, seznamSubVeljavnihKod, maxDolzina);
			if (!obstaja1) continue;


			resitev++;
			break;
		}
	}

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

	short maxDolzina = 0;
	for (const std::string koda : seznamKod)
		maxDolzina = (koda.size() > maxDolzina) ? koda.size() : maxDolzina;


	std::unordered_set<std::string> veljavneKode = razberiKodoZaID(seznamPravil, 0, maxDolzina);
	
	int resitev1 = steviloPrekrivanja(seznamKod, veljavneKode);
	std::cout << "Stevilo vseh kod, ki se prekrivajo je " << resitev1 << ".\n";


	seznamPravil[8].dolociSledenje({ 42,8 });
	seznamPravil[11].dolociSledenje({ 42,11,31 });
	
	std::vector<std::unordered_set<std::string>> seznamVeljavnihKod;
	for (short i : {8,11,42,31})
		seznamVeljavnihKod.push_back(razberiKodoZaID(seznamPravil, i, maxDolzina));

	int resitev2 = steviloPrekrivanja(seznamKod, seznamVeljavnihKod, maxDolzina);
	std::cout << "Stevilo vseh kod, ki se prekrivajo z ponavljanjem je " << resitev2 << ".\n";


	return 0;
}
