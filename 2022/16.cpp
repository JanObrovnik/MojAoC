/*
	tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>



class Ventil {
public:
	std::string naziv;
	int tok;
	bool odprt;
	std::vector<Ventil*> tuneli;
	int cas;

	Ventil(std::string naziv, int tok) : naziv(naziv), tok(tok), odprt(false), tuneli({}), cas(30) {}
	Ventil(std::string naziv, int tok, int cas) : naziv(naziv), tok(tok), odprt(false), tuneli({}), cas(cas) {}
};

bool operator<(const Ventil& ve1, const Ventil& ve2) {
	if (ve1.odprt != ve2.odprt) return ve1.odprt > ve2.odprt;
	return ve1.tok < ve2.tok;
	//return ve1.tok * ve1.cas < ve2.tok * ve2.cas;
}
bool operator>(const Ventil& ve1, const Ventil& ve2) {
	if (ve1.odprt != ve2.odprt) return ve1.odprt < ve2.odprt;
	return ve1.tok > ve2.tok;
	//return ve1.tok * ve1.cas > ve2.tok * ve2.cas;
}

std::ostream& operator<<(std::ostream& os, const Ventil& ve) {
	os << ve.naziv << ' ' << ve.tok << '\t';
	for (const auto& tun : ve.tuneli)
		os << tun->naziv << ' ';
	return os;
}

struct PotDel1 {
	std::string zacetek;
	std::string konec;

	PotDel1() : zacetek(""), konec("") {}
	PotDel1(std::string zacetek, std::string konec) : zacetek(zacetek), konec(konec) {}

	bool operator==(const PotDel1& pd) const {
		return zacetek == pd.zacetek && konec == pd.konec;
	}

	struct HashFunkcija {
		size_t operator()(const PotDel1& pd) const {
			size_t zacetekHash = std::hash<std::string>()(pd.zacetek);
			size_t konecHash = std::hash<std::string>()(pd.konec) << 1;
			return zacetekHash ^ konecHash;
		}
	};
};

struct PotDel2 {
	int cas;
	int tok;

	PotDel2() : cas(0), tok(0) {}
	PotDel2(int cas, int tok) : cas(cas), tok(tok) {}
};

/*//*
class Izbor {
public:
	std::vector<std::pair<std::string,std::pair<short, short>>> seznam;
	// stevilo razlicnih, trenutno mesto

	Izbor() : seznam({}) {}

	bool povecaj() {

		seznam.rbegin()->second.second++;

		for (auto itr = seznam.rbegin(); itr != seznam.rend(); itr++) {
			if (itr->second.second == itr->second.first) {
				itr->second.second = 0;
				itr++;
				if (itr == seznam.rend())
					return false;
				itr->second.second++;
				itr--;
			}
		}
	}
};
/**/


std::vector<Ventil> preberiPodatki(std::string pot) {

	std::vector<Ventil> resitev{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";
		return resitev;
	}

	std::string vrstica;

	std::vector<std::pair<std::string, int>> seznamParovNazivov{};
	std::vector<std::vector<std::string>> seznamTunelov{};
	std::string naziv;
	int tok;

	while (std::getline(podatki, vrstica)) {

		std::stringstream ss(vrstica);

		ss.ignore(6);
		ss >> naziv;
		ss.ignore(15);
		ss >> tok;
		ss.ignore(24);

		char c;
		std::string tunel;
		std::vector<std::string> sezTunel;

		while (ss.peek() != EOF) {
			ss >> tunel;
			tunel = std::string(tunel.begin(), tunel.begin() + 2);
			sezTunel.push_back(tunel);
		}

		seznamParovNazivov.push_back({ naziv,tok });
		seznamTunelov.push_back(sezTunel);
	}

	podatki.close();

	for (const auto& par : seznamParovNazivov)
		resitev.push_back(Ventil(par.first, par.second));

	for (int i = 0; i < seznamTunelov.size(); i++)
		for (const std::string& naziv : seznamTunelov[i]) {
			auto it(std::find_if(resitev.begin(), resitev.end(), [&](const Ventil& ve) {return ve.naziv == naziv; }));
			resitev[i].tuneli.push_back(&*it);
		}
		

	return resitev;
}


std::vector<Ventil> narediSeznamTock(Ventil* zacetniVentil, short stVentilov, short cas) {

	std::vector<Ventil> resitev;


	cas--;	
	Ventil navidezniVentil = *zacetniVentil;
	navidezniVentil.cas = cas;
	navidezniVentil.tok *= cas;
	
	std::queue<Ventil> cakalnaVrsta;
	cakalnaVrsta.push(navidezniVentil);

	resitev.push_back(navidezniVentil);

	while (resitev.size() != stVentilov /* && cas > 0 */) {

		for (const auto& ventil : cakalnaVrsta.front().tuneli) {

			//std::cout << "vrsta " << cakalnaVrsta.front().naziv << " zmanjsa tlak za " << cakalnaVrsta.front().tok <<
			//	" (" << cakalnaVrsta.front().cas << ")" << ".\n";

			navidezniVentil = *ventil;
			navidezniVentil.cas = cakalnaVrsta.front().cas - 1;
			navidezniVentil.tok *= navidezniVentil.cas;

			//std::cout << "Ventil " << navidezniVentil.naziv << " zmanjsa tlak za " << navidezniVentil.tok <<
			//			 " (" << navidezniVentil.cas << ")" << ".\n";

			auto it = std::find_if(resitev.begin(), resitev.end(), [&](const Ventil& ven) {
				return ven.naziv == navidezniVentil.naziv;
				});
			if (it == resitev.end()) {
				resitev.push_back(navidezniVentil);
				//std::cout << resitev.back() << '\n';
			}

			cakalnaVrsta.push(navidezniVentil);
		}
		cakalnaVrsta.pop();
		//std::cout << '\n';
	}


	return resitev;
}

/**/
void DFS(Ventil* zacetniVentil, std::vector<Ventil> seznamVentilov, int resitev = 0, int cas = 30, int index = 0) {

	//std::cout << "idnex: " << index << '\n';
	index++;

	Ventil* trenutniVentil(zacetniVentil);

	///////////////////// neki narobe s casom
	std::vector<Ventil> seznamTock(narediSeznamTock(trenutniVentil, seznamVentilov.size(), trenutniVentil->cas));
	std::sort(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 > ve2; });
	for (const auto& ve : seznamTock)
		std::cout << "Ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
	int st = std::count_if(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve) {return ve.odprt; });
	std::cout << "Stevilo ze odprtih: " << st << '\n';
	std::cout << '\n';

	auto itPrveNicle(std::find_if(seznamTock.begin(), seznamTock.end(), [&](const Ventil& ve) {return ve.tok < 1; }));
	if (itPrveNicle == seznamTock.begin())
		std::cout << "Postopek koncan.\n";

	std::vector<Ventil> seznamPozitivnihTock(seznamTock.begin(), itPrveNicle);
	//for (const auto& ve : seznamPozitivnihTock)
	//	std::cout << "Poz ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
	//std::cout << '\n';


	for (const Ventil& ventil : seznamPozitivnihTock) {

		cas = ventil.cas;
		int novaResitev = resitev + ventil.tok;
		trenutniVentil = &*std::find_if(seznamVentilov.begin(), seznamVentilov.end(), [&](const Ventil& ve) {
			return ve.naziv == ventil.naziv;
			});
		trenutniVentil->odprt = true;

		DFS(trenutniVentil, seznamVentilov, novaResitev, cas, index);
	}

	std::cout << "resitev: " << resitev << '\n';
}
/**/
/**/
std::vector<Ventil> optimizirajPot(Ventil* zacetniVentil, std::vector<Ventil>& seznamVentilov) {

	Ventil* trenutniVentil(zacetniVentil);


	int resitev(0);
	int cas(30);

	while (cas > 0) {

		std::vector<Ventil> seznamTock(narediSeznamTock(trenutniVentil, seznamVentilov.size(), cas));
		std::sort(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 > ve2; });
		for (const auto& ve : seznamTock)
			std::cout << "Ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";


		auto itPrveNicle(std::find_if(seznamTock.begin(), seznamTock.end(), [&](const Ventil& ve) {return ve.tok < 1; }));
		if (itPrveNicle == seznamTock.begin())
			break;
		
		std::vector<Ventil> seznamPozitivnihTock(seznamTock.begin(), itPrveNicle);
		for (const auto& ve : seznamPozitivnihTock)
			std::cout << "Poz ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
		std::cout << '\n';

		return seznamPozitivnihTock;

		cas = seznamTock.front().cas;
		resitev += seznamTock.front().tok;
		trenutniVentil = &*std::find_if(seznamVentilov.begin(), seznamVentilov.end(), [&](const Ventil& ve) {
			return ve.naziv == seznamTock.front().naziv;
			});
		trenutniVentil->odprt = true;

		std::cout << "resitev: " << resitev << '\n';
	}
}
/**/
/**/

void zapolniBazoPodatkov(std::vector<Ventil>& seznamVentilov, std::unordered_map<PotDel1, PotDel2, PotDel1::HashFunkcija>& baza) {
	
	for (Ventil& ventil : seznamVentilov) {

		if (ventil.tok == 0 && ventil.naziv != "AA")
			continue;

		std::vector<Ventil> seznam(narediSeznamTock(&ventil, seznamVentilov.size(), 30));

		for (const Ventil& notranjiVentil : seznam) {

			if (notranjiVentil.tok == 0)
				continue;

			baza[PotDel1(ventil.naziv, notranjiVentil.naziv)] = PotDel2(notranjiVentil.cas - 30, notranjiVentil.tok / notranjiVentil.cas);
		}
	}
}

/**/
/**/
void izracunajPotek(const std::string& zacetek, const std::string& konec, int& cas, int& tok, const std::unordered_map<PotDel1, PotDel2, PotDel1::HashFunkcija>& baza) {

	PotDel2 resitev = baza.at(PotDel1(zacetek, konec));

	cas += resitev.cas;
	tok += resitev.tok * cas;
}
/**/
/**/
int potujPoPoti(const std::vector<std::string>& seznamNazivov, const std::unordered_map<PotDel1, PotDel2, PotDel1::HashFunkcija>& baza) {

	int cas = 30;
	int tok = 0;
	std::string zacetek = "AA";

	for (const std::string& konec : seznamNazivov) {

		izracunajPotek(zacetek, konec, cas, tok, baza);

		zacetek = konec;
	}

	return tok;
}
/**/
/**/

/**/
/**/
std::vector<std::vector<std::string>> narediSeznamZaporedji(Ventil* zacetniVentil, std::vector<Ventil>& seznamVentilov) {

	std::vector<Ventil> seznam(narediSeznamTock(zacetniVentil, seznamVentilov.size(), 30));
	auto it(std::remove_if(seznam.begin(), seznam.end(), [](Ventil& ventil) {return ventil.tok < 1; }));
	seznam.erase(it, seznam.end());


	std::vector<std::string> nazivi;
	for (const Ventil& ve : seznam)
		nazivi.push_back(ve.naziv);

	std::vector<std::vector<std::string>> zaporedjeNazivov({ {nazivi[0]} });
	//zaporedjeNazivov.push_back({ {nazivi[0]} });

	/////////////////////////// sproti naredi izracun
	while (zaporedjeNazivov[0].size() < nazivi.size()) {

		std::vector<std::vector<std::string>> kopija(zaporedjeNazivov);

		for (int i = 1; i < (zaporedjeNazivov[0].size() + 1); i++)
			for (const std::vector<std::string>& vec : kopija)
				zaporedjeNazivov.push_back(vec);


		int velikostSeznama = zaporedjeNazivov.size();
		int velikostStringa = zaporedjeNazivov[0].size();
		std::string naslednjiKarakter = nazivi[velikostStringa];

		int zamik = 0;
		int meja = velikostSeznama / (velikostStringa + 1);

		for (int i = 0; i < velikostSeznama; i++) {

			zaporedjeNazivov[i].insert(zaporedjeNazivov[i].begin() + zamik, naslednjiKarakter);

			if ((i + 1) % meja == 0)
				zamik++;
		}
	}

	//for (const auto& vec : zaporedjeNazivov) {
	//	for (const auto& str : vec)
	//		std::cout << str << ' ';
	//	std::cout << '\n';
	//}


	//std::cout << '\n';
	std::sort(seznam.begin(), seznam.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 < ve2; });
	int tokMax(seznam.back().tok / 2);
	//for (const Ventil& ve : seznam)
	//	std::cout << ve << '\n';
	//std::cout << '\n';

	std::set<std::string> setMajhnihPretokov;
	for (const Ventil& ve : seznam)
		if (ve.tok < tokMax)
			setMajhnihPretokov.insert(ve.naziv);




	//std::cout << "st: " << zaporedjeNazivov.size() << " | " << setMajhnihPretokov.size() << '\n';

	auto itZ = std::remove_if(zaporedjeNazivov.begin(), zaporedjeNazivov.end(), [&](const auto& str) {
		//return setMajhnihPretokov.find(str[0]) != setMajhnihPretokov.end();
		for (int i = 0; i < setMajhnihPretokov.size(); i++)
			if (setMajhnihPretokov.find(str[i]) != setMajhnihPretokov.end())
				return true;
		return false;
		});
	zaporedjeNazivov.erase(itZ, zaporedjeNazivov.end());

	//std::cout << "st: " << zaporedjeNazivov.size() << '\n';


	//for (const auto& vec : zaporedjeNazivov) {
	//	for (const auto& str : vec)
	//		std::cout << str << ' ';
	//	std::cout << '\n';
	//}

	return zaporedjeNazivov;
}
/**/


int main(int argc, char* argv[]) {

	std::vector<Ventil> seznamVentilov(preberiPodatki("Advent22/16.txt"));

	auto it(std::find_if(seznamVentilov.begin(), seznamVentilov.end(), [&](const Ventil& ve) {
		return ve.naziv == "AA";
		}));


	std::cout << "del 1\n";

	std::unordered_map<PotDel1, PotDel2, PotDel1::HashFunkcija> bazaPodatkov;
	zapolniBazoPodatkov(seznamVentilov, bazaPodatkov);
	//std::cout << bazaPodatkov.size() << '\n';

	//for (auto it = bazaPodatkov.begin(); it != bazaPodatkov.end(); it++)
	//	std::cout << it->first.zacetek << '-' << it->first.konec << ':' << it->second.cas << ',' << it->second.tok << '\n';
	std::cout << "del 2\n";


	std::vector<std::vector<std::string>> seznamZaporedij(narediSeznamZaporedji(&*it, seznamVentilov));
	//for (auto a : seznamZaporedij) {
	//	for (auto b : a)
	//		std::cout << b << ' ';
	//	std::cout << '\n';
	//}


	//std::vector<std::string> sez{ "DD","BB","JJ","HH","EE","CC" };

	std::cout << "del 3\n";


	int maxTok(0);

	for (const std::vector<std::string>& sez : seznamZaporedij) {

		int cas = 30;
		int tok = 0;
		std::string zacetek = "AA";

		for (const std::string& konec : sez) {

			izracunajPotek(zacetek, konec, cas, tok, bazaPodatkov);

			zacetek = konec;
		}

		//std::cout << tok << '\n';

		if (maxTok < tok)
			maxTok = tok;
	}

	std::cout << "Maksimalni tok ki ga lahko dosezemo je " << maxTok << ".\n";


	/*/
	std::unordered_map<PotDel1, PotDel2, PotDel1::HashFunkcija> testMap;
	testMap[PotDel1("AA", "BB")] = PotDel2(27, 20);
	testMap[PotDel1("BB", "CC")] = PotDel2(34, 21);
	testMap[PotDel1("CC", "DD")] = PotDel2(40, 22);
	testMap[PotDel1("DD", "AA")] = PotDel2(41, 23);
	testMap[PotDel1("BB", "AA")] = PotDel2(41, 23);

	for (auto it = testMap.begin(); it != testMap.end(); it++)
		std::cout << it->first.zacetek << '-' << it->first.konec << ':' << it->second.cas << ',' << it->second.tok << '\n';
	/*/

	/// 2
	/*/
	std::string sez("AB");
	for (int i = 0; i < 2; i++) {
		
		std::string kopija(sez);

		if (i == 0) {
			std::cout << kopija << '\n';
			continue;
		}

		std::iter_swap(kopija.begin(), kopija.begin() + 1);
		std::cout << kopija << '\n';
	}
	std::cout << std::endl;
	/**/
	/*
		A

		BA
		AB

		CBA
		CAB
		BCA
		ACB
		BAC
		ABC

		DCBA
		DCAB
		DBCA
		DACB
		DBAC
		DABC
		CDBA
		CDAB
		BDCA
		ADCB
		BDAC
		ADBC
		CBDA
		CADB
		BCDA
		ACDB
		BADC
		ABDC
		CBAD
		CABD
		BCAD
		ACBD
		BACD
		ABCD
	*/
	/*/
	/// 3
	std::string sez = "ABC";
	std::vector<std::string> test({"A"});

	while (test[0].size() < 5) {

		std::vector<std::string> kopija(test);

		for (int i = 1; i < (test[0].size() + 1); i++)
			for (const std::string& str : kopija)
				test.push_back(str);


		int velikostSeznama = test.size();
		int velikostStringa = test[0].size();
		char naslednjiKarakter = test[0][0] + 1;

		int zamik = 0;
		int meja = velikostSeznama / (velikostStringa + 1);
		//std::cout << velikostSeznama << '/' << (velikostStringa + 1) << '=' << meja << '\n';

		for (int i = 0; i < velikostSeznama; i++) {

			test[i].insert(test[i].begin() + zamik, naslednjiKarakter);

			if ((i + 1) % meja == 0)
				zamik++;
		}
	}

	for (std::string str : test)
		std::cout << str << '\n';

		/**/
	//**/
	/////////////////////
	/*/
	std::vector<int> copyTest1;
	copyTest1.push_back(7);
	copyTest1.push_back(6);
	copyTest1.push_back(4);
	copyTest1.push_back(8);
	copyTest1.push_back(1);
	std::vector<int> copyTest2;
	copyTest2.resize(5);

	for (int i : copyTest2)
		std::cout << i << ' ';
	std::cout << '\n';

	std::copy(copyTest1.begin(), copyTest1.end(), copyTest2.begin());

	for (int i : copyTest2)
		std::cout << i << ' ';
	std::cout << '\n';
	/**/


	//DFS(&*it, seznamVentilov);
	
	
	/*/
	int resitev(0);
	int cas(30);

	
	std::vector<Ventil> seznamTock(narediSeznamTock(&*it, seznamVentilov.size(), cas));
	std::sort(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 > ve2; });
	for (const auto& ve : seznamTock)
		std::cout << "Ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
	std::cout << '\n';


	cas = seznamTock.front().cas;
	resitev += seznamTock.front().tok;
	it = std::find_if(seznamVentilov.begin(), seznamVentilov.end(), [&](const Ventil& ve) {
		return ve.naziv == seznamTock.front().naziv;
		});
	it->odprt = true;
	seznamTock = narediSeznamTock(&*it, seznamVentilov.size(), cas);
	std::sort(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 > ve2; });
	for (const auto& ve : seznamTock)
		std::cout << "Ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
	std::cout << '\n';


	cas = seznamTock.front().cas;
	resitev += seznamTock.front().tok;
	it = std::find_if(seznamVentilov.begin(), seznamVentilov.end(), [&](const Ventil& ve) {
		return ve.naziv == seznamTock.front().naziv;
		});
	it->odprt = true;
	seznamTock = narediSeznamTock(&*it, seznamVentilov.size(), cas);
	std::sort(seznamTock.begin(), seznamTock.end(), [](const Ventil& ve1, const Ventil& ve2) {return ve1 > ve2; });
	for (const auto& ve : seznamTock)
		std::cout << "Ventil " << ve.naziv << " zmanjsa tlak za " << ve.tok << " (" << ve.cas << ")" << ".\n";
	std::cout << '\n';
	/**/


	


	//for (const Ventil& ventil : seznamVentilov)
	//	std::cout << ventil << '\n';


	

	return 0;
}