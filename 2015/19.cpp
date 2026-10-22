/*
	t = 47:51 min
	precej lahko
	t ~ 22:35:00 ur
	zelo tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>


enum class Omejenost {
	LEVO,
	DESNO,
	UMESNO,
};


struct Ukaz {
	std::string zamenjava;
	short zacetek;
	short konec;
	Ukaz() : zamenjava(""), zacetek(-1), konec(-1) {}
	Ukaz(std::string zamenjava, short zacetek, short konec) : zamenjava(zamenjava), zacetek(zacetek), konec(konec) {}
};

bool operator==(const Ukaz& uk1, const Ukaz& uk2) {
	return uk1.zamenjava == uk2.zamenjava && uk1.zacetek == uk2.zacetek && uk1.konec == uk2.konec;
}
std::ostream& operator<<(std::ostream& os, const Ukaz& uk) {
	return os << uk.zamenjava << ": " << uk.zacetek << " | " << uk.konec;
}


void preberiPodatke(const std::string& pot, std::map<std::string, std::vector<std::string>>& zamenjave, std::string& molekula) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string vrstica;

	std::string vhod, izhod;

	std::getline(podatki, vrstica);

	while (vrstica != "") {

		std::stringstream ss(vrstica);

		ss >> vhod;
		ss.ignore(4);
		ss >> izhod;

		zamenjave[vhod].push_back(izhod);

		std::getline(podatki, vrstica);
	}

	podatki >> molekula;

	podatki.close();

	return;
}


int steviloMoznihZamenjav(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula) {

	std::set<std::string> vseMolekule;

	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++) {
		for (const std::string zamenjava : it->second) {
			
			std::string iskanje;
			for (const char& c : it->first)
				iskanje.push_back('x');

			for (int i = 0; i < molekula.size(); i++) {
				
				iskanje.erase(iskanje.begin());
				iskanje.push_back(molekula[i]);

				if (iskanje == it->first) {

					int mesto = i - (it->first.size() - 1);

					std::string novaMolekula = molekula;

					novaMolekula.erase(novaMolekula.begin() + mesto, novaMolekula.begin() + mesto + it->first.size());
					novaMolekula.insert(novaMolekula.begin() + mesto, zamenjava.begin(), zamenjava.end());

					vseMolekule.insert(novaMolekula);
				}
			}
		}
	}

	return vseMolekule.size();
}


std::set<std::string> narediSeznamAtomov(const std::map<std::string, std::vector<std::string>>& zamenjave) {

	std::set<std::string> resitev;

	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++)
		resitev.insert(it->first);

	return resitev;
}

std::vector<std::string> razdeliMolekula(const std::string& molekula) {

	std::vector<std::string> resitev;

	std::string atom;
	
	for (int i = 0; i < molekula.size(); i++) {

		if (atom.empty()) {
			atom.push_back(molekula[i]);
			continue;
		}

		if (molekula[i] >= 'A' && molekula[i] <= 'Z') {
			resitev.push_back(atom);
			atom.clear();
			atom.push_back(molekula[i]);
		}
		else if (molekula[i] >= 'a' && molekula[i] <= 'z') {
			atom.push_back(molekula[i]);
		}
	}

	resitev.push_back(atom);


	return resitev;
}
std::string sestaviMolekulo(const std::vector<std::string>& molekulaVec) {

	std::string resitev = "";

	for (const std::string& atom : molekulaVec) {
		resitev += atom;
	}

	return resitev;
}
std::string sestaviMolekulo(std::vector<std::string>::iterator itZacetek, const std::vector<std::string>::iterator itKonec) {

	std::string resitev = "";

	while (itZacetek != itKonec) {
		resitev += *itZacetek;
		itZacetek++;
	}

	return resitev;
}

std::vector<std::vector<std::string>> narediSeznamIzhodov(const std::map<std::string, std::vector<std::string>>& zamenjave) {

	std::vector<std::vector<std::string>> resitev;

	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++) {
		
		for (const std::string molekula : it->second) {

			resitev.push_back(razdeliMolekula(molekula));

		}
	}

	return resitev;
}

std::vector<std::string> najdiManjkajoce(const std::set<std::string>& seznamVhodov, const std::vector<std::string>& molekulaVec) {

	std::vector<std::pair<std::string, int>> manjkajoce;

	for (const std::string& atom : molekulaVec) {

		if (seznamVhodov.find(atom) == seznamVhodov.end()) {

			auto it = std::find_if(manjkajoce.begin(), manjkajoce.end(), [&](std::pair<std::string, int> par) {return par.first == atom; });
			
			if (it == manjkajoce.end())
				manjkajoce.push_back({ atom,1 });
			else
				it->second++;
		}
	}

	std::sort(manjkajoce.begin(), manjkajoce.end(), [](std::pair<std::string, int> levi, std::pair<std::string, int> desni) {
		return levi.second > desni.second;
		});

	std::vector<std::string> resitev;

	for (auto par : manjkajoce) {
		//std::cout << par.first << ": " << par.second << '\n';
		resitev.push_back(par.first);
	}

	return resitev;
}

std::map<std::string, std::string> obrniZamenjave(const std::map<std::string, std::vector<std::string>>& zamenjave) {

	std::map<std::string, std::string> resitev;

	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++)
		for (const std::string& str : it->second)
			resitev[str] = it->first;

	return resitev;
}


inline void zamenjaj(std::string& molekula, const std::string& zamenjava, const short& zacetek, const short& konec) {

	molekula.erase(molekula.begin() + zacetek, molekula.begin() + konec);
	molekula.insert(molekula.begin() + zacetek, zamenjava.begin(), zamenjava.end());
}
inline void zamenjaj(std::string& molekula, const Ukaz& ukaz) {

	zamenjaj(molekula, ukaz.zamenjava, ukaz.zacetek, ukaz.konec);
}


int simuliraj(const std::map<std::string, std::string>& zamenjave2, const std::string& molekula) {

	int najdaljsaIzhodnaMolekula = 0;

	for (auto it = zamenjave2.begin(); it != zamenjave2.end(); it++) {

		if (it->first.size() > najdaljsaIzhodnaMolekula) {
			najdaljsaIzhodnaMolekula = it->first.size();
		}
	}

	std::vector<Ukaz> seznamUkazov;
	Ukaz zadnjiUkaz;

	std::string novaMolekula = molekula;
	int trenutnaNajdaljsa = najdaljsaIzhodnaMolekula;

	bool zamenjavaOpravljena = true;
	int st = 0;

	while (zamenjavaOpravljena) {
		
		//if (st++ > 50) break;
		//zamenjavaOpravljena = false;

		trenutnaNajdaljsa = najdaljsaIzhodnaMolekula;

		if (!seznamUkazov.empty()) {
			zadnjiUkaz = seznamUkazov.back();
			seznamUkazov.pop_back();
			trenutnaNajdaljsa = zadnjiUkaz.zamenjava.size();
		}

		novaMolekula = molekula;
		
		for (const Ukaz& ukaz : seznamUkazov) {
			//std::cout << "dela\n";
			zamenjaj(novaMolekula, ukaz);
		}
		
		while (trenutnaNajdaljsa > 0) {

			std::string iskana(trenutnaNajdaljsa, 'x');

			int mesto = 0;

			if (trenutnaNajdaljsa == zadnjiUkaz.zamenjava.size()) {
				mesto = zadnjiUkaz.zacetek + 1;
			}

			while (mesto < novaMolekula.size()) {

				iskana.erase(iskana.begin());
				iskana.push_back(novaMolekula[mesto]);

				auto it = zamenjave2.find(iskana);

				if (it != zamenjave2.end()) {

					const int zacetek = mesto - iskana.size() + 1;
					const int konec = mesto + 1;

					Ukaz novUkaz(it->second, zacetek, konec);

					if (novUkaz == zadnjiUkaz) {
						mesto++;
						continue;
					}
					
					zamenjaj(novaMolekula, novUkaz);

					zamenjavaOpravljena = true;

					seznamUkazov.push_back(novUkaz);

					trenutnaNajdaljsa = najdaljsaIzhodnaMolekula + 1;
					break;
				}

				mesto++;
			}

			trenutnaNajdaljsa--;
		}
		std::cout << novaMolekula << '\n';
		
		//std::string test;
		//std::getline(std::cin, test);
	}

	return seznamUkazov.size();
}


std::map<std::string, int> ustvariUtezi(const std::map<std::string, std::string>& zamenjave2, const std::vector<std::string>& seznamManjkajocih) {

	std::map<std::string, int> resitev;

	for (const std::string& manjkajoca : seznamManjkajocih) {

		int dolzinaIskane = manjkajoca.size();
		
		for (auto it = zamenjave2.begin(); it != zamenjave2.end(); it++) {

			std::vector<std::string> molekulaVec = razdeliMolekula(it->first);

			for (int i = 0; i < molekulaVec.size(); i++) {

				if (molekulaVec[i] == manjkajoca) {

					int st = 0;

					for (const std::string& manjkajoca : seznamManjkajocih) {

						int zamik = it->first.find(manjkajoca, 0);
						
						while (true) {
						
							if (zamik == std::string::npos)
								break;
						
							st++;
						
							zamik = it->first.find(manjkajoca, zamik + 1);
						}
					}
					
					if (resitev.find(it->first) == resitev.end()) {
						resitev[it->first] = st;
					}
					else if (st > resitev[it->first]) {
						resitev[it->first] = st;
					}
				}
			}
		}
	}

	return resitev;
}


int simuliraj2(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula) {

	const std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	
	std::string novaMolekula = molekula;


	std::set<std::string> seznamVhodov = narediSeznamAtomov(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(novaMolekula);

	std::vector<std::string> seznamManjkajocih = najdiManjkajoce(seznamVhodov, molekulaVec);



	std::map<std::string, int> uteziMap = ustvariUtezi(zamenjave2, seznamManjkajocih);
	std::vector<std::pair<std::string, int>> utezi(uteziMap.begin(), uteziMap.end());

	std::sort(utezi.begin(), utezi.end(), [](std::pair<std::string, int> levi, std::pair<std::string, int> desni) {
		return levi.second > desni.second;
		});
	//for (std::pair<std::string, int> utez : utezi) std::cout << utez.first << ": " << utez.second << '\n';


	bool obstaja = true;

	while (obstaja) {

		obstaja = false;

		for (int i = 0; i < utezi.size(); i++) {

			size_t mesto = novaMolekula.find(utezi[i].first);

			if (mesto != std::string::npos) {
				//std::cout << utezi[i].first << " (" << mesto << ")\n";

				zamenjaj(novaMolekula, zamenjave2.at(utezi[i].first), mesto, mesto + utezi[i].first.size());

				obstaja = true;
				break;
			}
		}

		//std::cout << novaMolekula << '\n' << '\n';

	}


	molekulaVec = razdeliMolekula(novaMolekula);

	seznamManjkajocih = najdiManjkajoce(seznamVhodov, molekulaVec);



	return -1;
}


int najdiUjemanje(const std::map<std::string, std::string>& zamenjave2, const std::vector<std::string>& tekocaMolVec) {

	int resitev = 0;

	std::string tekocaMol = sestaviMolekulo(tekocaMolVec);

	for (auto it = zamenjave2.begin(); it != zamenjave2.end(); it++) {
		
		resitev += (it->first.find(tekocaMol) != std::string::npos);
	}

	return resitev;
}
bool najdiCelotno(const std::map<std::string, std::string>& zamenjave2, const std::vector<std::string>& tekocaMolVec) {

	std::string tekocaMol = sestaviMolekulo(tekocaMolVec);

	return zamenjave2.find(tekocaMol) != zamenjave2.end();
}

void zamenjaj(const std::map<std::string, std::string>& zamenjave2, std::vector<std::string>& molekulaVec, const std::vector<std::string>& tekocaMolVec, const int& mestoNaslednje) {

	std::string tekocaMol = sestaviMolekulo(tekocaMolVec);
	std::string zamenjava = zamenjave2.at(tekocaMol);

	int konec = mestoNaslednje;
	int zacetek = mestoNaslednje - tekocaMolVec.size();

	molekulaVec.erase(molekulaVec.begin() + zacetek, molekulaVec.begin() + konec);
	molekulaVec.insert(molekulaVec.begin() + zacetek, zamenjava);
}

int levoPraviloOG(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula) {

	int resitev = 0;

	std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);

	int mestoNaslednje = 1;
	std::vector<std::string> tekocaMolVec({ molekulaVec.front() });


	while (molekulaVec != std::vector<std::string>{"e"}) {

		//std::cout << molekulaVec.size() << ": ";
		//for (int i = 0; i < (16 < molekulaVec.size() ? molekulaVec.size() : molekulaVec.size()); i++) {
		//	std::cout << molekulaVec[i] << ' ';
		//} std::cout << "\t|\t";
		//for (int i = 0; i < tekocaMolVec.size(); i++) {
		//	std::cout << tekocaMolVec[i] << ' ';
		//} std::cout << '\n';


		//if (mestoNaslednje >= molekulaVec.size()) {
		//
		//	std::cout << "NAPAKA:\tmestoNaslednje izven obmocja seznama\n";
		//	break;
		//}


		int dolzinaTekoceMol = tekocaMolVec.size();

		if (dolzinaTekoceMol == 1) {

			tekocaMolVec.push_back(molekulaVec[mestoNaslednje]);

			mestoNaslednje++;
			continue;
		}


		int steviloNajdenihUjemanj = najdiUjemanje(zamenjave2, tekocaMolVec);

		if (steviloNajdenihUjemanj == 0) {

			tekocaMolVec.erase(tekocaMolVec.begin());

			continue;
		}


		bool celotna = najdiCelotno(zamenjave2, tekocaMolVec);

		if (celotna) {

			if (steviloNajdenihUjemanj == 1) {

				resitev++;

				zamenjaj(zamenjave2, molekulaVec, tekocaMolVec, mestoNaslednje);

				tekocaMolVec = { molekulaVec.front() };
				mestoNaslednje = 1;
				continue;
			}
			else {
				std::cout << "MANJKA:\tif (celotna) && else\n";
				break;
			}
		}
		else {

			tekocaMolVec.push_back(molekulaVec[mestoNaslednje]);

			mestoNaslednje++;
			continue;
		}


		std::cout << "NAPAKA:\tkonec\n";
		break;
	}

	return resitev;
}
int desnoPraviloOG(const std::map<std::string, std::vector<std::string>>& zamenjave, std::string& molekula) {

	int resitev = 0;

	std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);

	int mestoNaslednje = molekulaVec.size() - 2;
	std::vector<std::string> tekocaMolVec({ molekulaVec.back() });


	while (molekulaVec != std::vector<std::string>{"e"}) {

		//if (molekulaVec.size() < 6) {
		//	std::cout << molekulaVec.size() << ": ";
		//	for (int i = (molekulaVec.size() - 16 < 0 ? 0 : molekulaVec.size() - 16); i < molekulaVec.size(); i++) {
		//		std::cout << molekulaVec[i] << ' ';
		//	}
		//	std::cout << "\t|\t";
		//	for (int i = 0; i < tekocaMolVec.size(); i++) {
		//		std::cout << tekocaMolVec[i] << ' ';
		//	}
		//	std::cout << '\n';
		//
		//	if (tekocaMolVec == std::vector<std::string>{ "C", "Rn", "F", "Ar" }) {
		//		std::cout << "dela\n";
		//	}
		//}

		


		//if (mestoNaslednje < 0) {
		//
		//	std::cout << "NAPAKA:\tmestoNaslednje izven obmocja seznama\n";
		//	break;
		//}


		int dolzinaTekoceMol = tekocaMolVec.size();

		if (dolzinaTekoceMol == 1) {

			tekocaMolVec.insert(tekocaMolVec.begin(), molekulaVec[mestoNaslednje]);

			mestoNaslednje--;
			continue;
		}


		int steviloNajdenihUjemanj = najdiUjemanje(zamenjave2, tekocaMolVec);

		if (steviloNajdenihUjemanj == 0) {

			tekocaMolVec.pop_back();

			continue;
		}


		bool celotna = najdiCelotno(zamenjave2, tekocaMolVec);

		if (celotna) {

			if (steviloNajdenihUjemanj == 1) {

				resitev++;

				zamenjaj(zamenjave2, molekulaVec, tekocaMolVec, mestoNaslednje + tekocaMolVec.size() + 1);

				tekocaMolVec = { molekulaVec.back() };
				mestoNaslednje = molekulaVec.size() - 1;
				continue;
			}
			else {
				std::cout << "MANJKA:\tif (celotna) && else\n";
				break;
			}
		}
		else {

			tekocaMolVec.insert(tekocaMolVec.begin(), molekulaVec[mestoNaslednje]);

			mestoNaslednje--;
			continue;
		}


		std::cout << "NAPAKA:\tkonec\n";
		break;
	}

	//std::cout << sestaviMolekulo(molekulaVec) << "\n";
	
	return resitev;
}

int levoPravilo(const std::map<std::string, std::vector<std::string>>& zamenjave, std::string& molekula) {

	int resitev = 0;

	std::map<std::string,std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);

	int mestoNaslednje = 1;
	std::vector<std::string> tekocaMolVec({ molekulaVec.front() });

	//for (std::string str : tekocaMol) std::cout << str << '\n';

	while (molekulaVec != std::vector<std::string>{"e"}) {

		//std::cout << molekulaVec.size() << ": ";
		//for (int i = 0; i < (16 < molekulaVec.size() ? molekulaVec.size() : molekulaVec.size()); i++) {
		//	std::cout << molekulaVec[i] << ' ';
		//} std::cout << "\t|\t";
		//for (int i = 0; i < tekocaMolVec.size(); i++) {
		//	std::cout << tekocaMolVec[i] << ' ';
		//} std::cout << '\n';


		if (mestoNaslednje >= molekulaVec.size()) {

			//std::cout << "NAPAKA:\tmestoNaslednje izven obmocja seznama\n";
			return -1;
		}


		int dolzinaTekoceMol = tekocaMolVec.size();

		if (dolzinaTekoceMol == 1) {

			tekocaMolVec.push_back(molekulaVec[mestoNaslednje]);

			mestoNaslednje++;
			continue;
		}


		int steviloNajdenihUjemanj = najdiUjemanje(zamenjave2, tekocaMolVec);

		if (steviloNajdenihUjemanj == 0) {

			tekocaMolVec.erase(tekocaMolVec.begin());

			continue;
		}
		

		bool celotna = najdiCelotno(zamenjave2, tekocaMolVec);

		if (celotna) {

			if (steviloNajdenihUjemanj == 1) {

				resitev++;

				zamenjaj(zamenjave2, molekulaVec, tekocaMolVec, mestoNaslednje);
				
				if (mestoNaslednje - tekocaMolVec.size() == 0) {
					molekula = sestaviMolekulo(molekulaVec);
					return resitev;
				}

				tekocaMolVec = { molekulaVec.front() };
				mestoNaslednje = 1;
				continue;
			}
			else {
				std::cout << "MANJKA:\tif (celotna) && else\n";
				break;
			}
		}
		else {

			tekocaMolVec.push_back(molekulaVec[mestoNaslednje]);

			mestoNaslednje++;
			continue;
		}


		//std::cout << "NAPAKA:\tkonec\n";
		break;
	}

	return -2;
}
int desnoPravilo(const std::map<std::string, std::vector<std::string>>& zamenjave, std::string& molekula) {

	int resitev = 0;

	std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);

	int mestoNaslednje = molekulaVec.size() - 2;
	std::vector<std::string> tekocaMolVec({ molekulaVec.back() });

	//for (std::string str : tekocaMol) std::cout << str << '\n';


	while (molekulaVec != std::vector<std::string>{"e"}) {


		//std::cout << molekulaVec.size() << ": ";
		//for (int i = 0; i < (16 < molekulaVec.size() ? molekulaVec.size() : molekulaVec.size()); i++) {
		//	std::cout << molekulaVec[i] << ' ';
		//}
		//std::cout << "\t|\t";
		//for (int i = 0; i < tekocaMolVec.size(); i++) {
		//	std::cout << tekocaMolVec[i] << ' ';
		//}
		//std::cout << '\n';


		if (mestoNaslednje < 0) {

			//std::cout << "NAPAKA:\tmestoNaslednje izven obmocja seznama\n";
			return -1;
		}


		int dolzinaTekoceMol = tekocaMolVec.size();

		if (dolzinaTekoceMol == 1) {

			tekocaMolVec.insert(tekocaMolVec.begin(), molekulaVec[mestoNaslednje]);

			mestoNaslednje--;
			continue;
		}


		int steviloNajdenihUjemanj = najdiUjemanje(zamenjave2, tekocaMolVec);

		if (steviloNajdenihUjemanj == 0) {

			tekocaMolVec.pop_back();

			continue;
		}


		bool celotna = najdiCelotno(zamenjave2, tekocaMolVec);

		if (celotna) {

			if (steviloNajdenihUjemanj == 1) {

				resitev++;

				int velikostMol = molekulaVec.size();

				zamenjaj(zamenjave2, molekulaVec, tekocaMolVec, mestoNaslednje + tekocaMolVec.size() + 1);

				if (mestoNaslednje + tekocaMolVec.size() + 1 == velikostMol) {

					molekula = sestaviMolekulo(molekulaVec);
					return resitev;
				}

				tekocaMolVec = { molekulaVec.back() };
				mestoNaslednje = molekulaVec.size() - 1;
				continue;
			}
			else {
				std::cout << "MANJKA:\tif (celotna) && else\n";
				break;
			}
		}
		else {

			tekocaMolVec.insert(tekocaMolVec.begin(), molekulaVec[mestoNaslednje]);

			mestoNaslednje--;
			continue;
		}


		//std::cout << "NAPAKA:\tkonec\n";
		break;
	}

	return -2;
}


void najdiOmejenost(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula, std::vector<std::string>& levaOmejenost, std::vector<std::string>& desnaOmejenost) {

	std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);

	std::set<std::string> seznamAtomov(molekulaVec.begin(), molekulaVec.end());
	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++) {
		seznamAtomov.insert(it->first);
	}

	for (const std::string& atom : seznamAtomov) {
		
		std::set<Omejenost> seznamOmejenosti;
		
		for (auto it = zamenjave2.begin(); it != zamenjave2.end(); it++) {


			std::vector<std::string> izhodVec = razdeliMolekula(it->first);

			for (int i = 0; i < izhodVec.size(); i++) if (izhodVec[i] == atom) {
				
				if (i == 0)
					seznamOmejenosti.insert(Omejenost::LEVO);
				else if (i == izhodVec.size() - 1)
					seznamOmejenosti.insert(Omejenost::DESNO);
				else 
					seznamOmejenosti.insert(Omejenost::UMESNO);
			}
		}

		if (seznamOmejenosti.size() == 1) {

			if (*seznamOmejenosti.begin() == Omejenost::LEVO)
				levaOmejenost.push_back(atom);
			else if (*seznamOmejenosti.begin() == Omejenost::DESNO)
				desnaOmejenost.push_back(atom);
		}
	}

	//std::cout << "Leva:\n";
	//for (std::string str : levaOmejenost)
	//	std::cout << str << "\n";
	//
	//std::cout << "Desna:\n";
	//for (std::string str : desnaOmejenost)
	//	std::cout << str << "\n";
}


void simuliraj3(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula) {

	int resitev = 0;

	std::vector<std::string> levaOmejenost;
	std::vector<std::string> desnaOmejenost;

	najdiOmejenost(zamenjave, molekula, levaOmejenost, desnaOmejenost);

	std::map<std::string, std::string> zamenjave2 = obrniZamenjave(zamenjave);
	std::vector<std::string> molekulaVec = razdeliMolekula(molekula);


	for (int i = 0; i < 30; i++) {

		int zadnjaLeva = 0;
		int zadnjaDesna = molekulaVec.size();

		for (int i = 0; i < molekulaVec.size(); i++) {

			if (std::find(levaOmejenost.begin(), levaOmejenost.end(), molekulaVec[i]) != levaOmejenost.end()) {

				zadnjaLeva = i;
			}
			else if (std::find(desnaOmejenost.begin(), desnaOmejenost.end(), molekulaVec[i]) != desnaOmejenost.end()) {

				zadnjaDesna = i + 1;

				break;
			}
		}


		if (zadnjaLeva == 0 && zadnjaDesna == molekulaVec.size()) {
			break;
		}


		std::string vmesnaMol = sestaviMolekulo(molekulaVec.begin() + zadnjaLeva, molekulaVec.begin() + zadnjaDesna);
		std::cout << '\n' << vmesnaMol << ":\n";


		int stLevo = levoPravilo(zamenjave, vmesnaMol);
		std::cout << stLevo << ": " << vmesnaMol << '\n';

		if (stLevo > 0) {
			std::vector<std::string> vmesnaMolVec = razdeliMolekula(vmesnaMol);
			molekulaVec.erase(molekulaVec.begin() + zadnjaLeva, molekulaVec.begin() + zadnjaDesna);
			molekulaVec.insert(molekulaVec.begin() + zadnjaLeva, vmesnaMolVec.begin(), vmesnaMolVec.end());
			resitev += stLevo;
			continue;
		}

		int stDesno = desnoPravilo(zamenjave, vmesnaMol);
		std::cout << stDesno << ": " << vmesnaMol << '\n';

		if (stDesno > 0) {
			std::vector<std::string> vmesnaMolVec = razdeliMolekula(vmesnaMol);
			molekulaVec.erase(molekulaVec.begin() + zadnjaLeva, molekulaVec.begin() + zadnjaDesna);
			molekulaVec.insert(molekulaVec.begin() + zadnjaLeva, vmesnaMolVec.begin(), vmesnaMolVec.end());
			resitev += stDesno;
			continue;
		}

		std::cout << "Nic ni najdeno\n";
	}

	std::cout << "\nresitev: " << resitev << '\n';
}



int main() {

	std::map<std::string, std::vector<std::string>> zamenjave;
	std::string molekula;

	preberiPodatke("2015/19.txt", zamenjave, molekula);


	int resitev1 = steviloMoznihZamenjav(zamenjave, molekula);
	std::cout << "Lahko dobimo " << resitev1 << " razlicnih molekul.\n";

	int resitev2 = desnoPraviloOG(zamenjave, molekula);
	std::cout << "Iskano molekulo dobimo v " << resitev2 << " korakih.\n";


	return 0;
}
