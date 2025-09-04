/*
	Zelo zahtevna
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>


enum Vrsta {
	NAPAKA = '#',
	DELUJOC = '.',
	NEZNAN = '?'
};

class Podatek {
private:
	std::string zapis;
	std::vector<short> seznamVrednosti;

	struct Skupina {
		short mesto, velikost;
		Vrsta vrsta;
		Skupina(short mesto, short velikost, Vrsta vrsta) : mesto(mesto), velikost(velikost), vrsta(vrsta) {}
	};

public:
	Podatek(std::string zapis) : zapis(zapis), seznamVrednosti({}) {}

	void ovrednoti() const;
	bool ustrezen(const std::string& poskusniZapis) const;
	bool ustrezen(const int& bitStevilo) const;

	int vrniSteviloNeznanih() const;
	int vrniSteviloNeznanihNapak() const;

	friend std::istream& operator>>(std::istream& is, Podatek& po);
	friend std::ostream& operator<<(std::ostream& os, const Podatek& po);
};

std::istream& operator>>(std::istream& is, Podatek& po) {
	int i;
	is >> i;
	po.seznamVrednosti.push_back(i);
	return is;
}
std::ostream& operator<<(std::ostream& os, const Podatek& po) {
	os << po.zapis << ' ';
	for (const short& i : po.seznamVrednosti) os << i << ',';
	return os;
}


std::list<Podatek> preberiPodatke(std::string pot) {

	std::list<Podatek> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		size_t mestoPreloma = vrstica.find(' ');

		resitev.push_back(Podatek(std::string(vrstica.begin(), vrstica.begin() + mestoPreloma)));

		std::stringstream ss(vrstica);
		ss.ignore(mestoPreloma);

		while (!ss.eof()) {
			ss >> resitev.back();
			ss.ignore(1);
		}
	}

	podatki.close();


	return resitev;
}


int resitev1 = 0;

void testFun(const int& bitStevilo, const int& velikostZanke, const int& steviloEnk, const Podatek& podatek) {

	if (steviloEnk == 0) {
		resitev1 += podatek.ustrezen(bitStevilo);
		//std::cout << std::bitset<16>(bitStevilo) << '\n';
		return;
	}

	for (int i = 0; i < velikostZanke; i++) {

		int notranjeBitStevilo = bitStevilo;
		notranjeBitStevilo |= (1 << i);

		testFun(notranjeBitStevilo, i, steviloEnk - 1, podatek);
	}
}




int main(int argc, char* argv[]) {

	std::list<Podatek> seznamPodatkov(preberiPodatke("Advent23/12.txt"));

	for (const Podatek& podatek : seznamPodatkov) {
		testFun(0, podatek.vrniSteviloNeznanih(), podatek.vrniSteviloNeznanihNapak(), podatek);
		std::cout << "naredilo\n";
	}

	std::cout << "stevilo vseh moznih kombinacij je " << resitev1 << ".\n";


	//std::cout << std::boolalpha;

	return 0;
}




/*/
enum Simboli {
	NAPAKA = '#',
	DELUJOC = '.',
	NEZNAN = '?'
};


//				  1
//	            1   1
//	          1   2   1
//	        1   3   3   1
//	      1   4   6   4   1
//	    1   5   10  10  5   1
//	  1   6   15  20  15  6   1
//	1   7   21  35  35  21  7   1

int distribucija(int steviloNeznank, int steviloPreostalihNapak) {

	std::vector<int> dist = {1};
	
	for (int i = 0; i < steviloNeznank; i++) {

		std::vector<int> distPomozna = {};

		int* zacetek = &dist[0];
		int* konec = zacetek + dist.size();

		for (int* it = zacetek; it != konec + 1; it++) {
			int dodaj = 0;
			if (it > zacetek) dodaj += *(it - 1);
			if (it < konec) dodaj += *it;
			distPomozna.push_back(dodaj);
		}

		dist = distPomozna;
	}
	//std::for_each(dist.begin(), dist.end(), [&](int i) {std::cout << i << ' '; });

	return dist[steviloPreostalihNapak];
}

int steviloMoznih(int steviloNeznank, int steviloPreostalihNapak) {

	int resitev;

	std::string moznost(steviloNeznank, '.');
	char* zacetek = &moznost[0];
	char* konec = zacetek + steviloNeznank;


	for (int i = 0; i < steviloPreostalihNapak; i++) {

		for (char* it = zacetek + i; it != konec - steviloPreostalihNapak + i; it++) {

			*it = '#';
		}
	}


	int razlicica = 0;
	int steviloRazlicnih = distribucija(steviloNeznank, steviloPreostalihNapak);

	while 

	//for (char* it = zacetek; it != konec - (steviloPreostalihNapak - 1); it++) {
	//
	//	*it = '#';
	//
	//	for (char* it2 = it + 1; it2 != )
	//}



	for (int i = 0; i < steviloRazlicnih; i++) {
		
	}


	

	return resitev;
}


class Linija {
private:
	std::string gezirji;
	std::list<short> logika;

public:
	Linija(std::string gezirji, std::list<short> logika) : gezirji(gezirji), logika(logika) {}


	int steviloResitev() {

		int resitev = 0;

		int stVsehNapak = std::accumulate(logika.begin(), logika.end(), 0);
		int stNeznank = std::count(gezirji.begin(), gezirji.end(), '?');
		int stNapak = std::count(gezirji.begin(), gezirji.end(), '#');
		int razlika = stVsehNapak - stNapak;
		std::cout << stVsehNapak << '\t' << stNapak << '\t' << razlika << '\t' << stNeznank << '\n';

		resitev += pravilnost();


		return resitev;
	}

	bool pravilnost() {

		std::list<short> preveri;


		short dolzina = 0;

		for (char c : gezirji) {

			if (c == NAPAKA)
				dolzina++;

			else {
				if (dolzina != 0)
					preveri.push_back(dolzina);
				dolzina = 0;
			}
		}

		if (dolzina != 0)
			preveri.push_back(dolzina);


		return preveri == logika;
	}


	void isci() {

		std::string kopija = gezirji;

		int stVsehNapak = std::accumulate(logika.begin(), logika.end(), 0);
		int stNeznank = std::count(gezirji.begin(), gezirji.end(), '?');
		int stNapak = std::count(gezirji.begin(), gezirji.end(), '#');
		int razlika = stVsehNapak - stNapak;

		char* zacetek = &kopija[0];
		char* konec = zacetek + kopija.size();

		//for (int i = 0; i < )
	}


	friend class Porocilo;
};

class Porocilo {
private:
	std::list<Linija> seznam;

public:
	Porocilo() = default;




	int vsotaMoznihKombinacij() {
		int resitev = 0;

		std::cout << "S\t#\tD\t?\n--------------------------\n";
		std::for_each(seznam.begin(), seznam.end(), [&resitev](Linija lin) { resitev += lin.steviloResitev(); });
		std::cout << '\n';

		return resitev;
	}


	void dodajPodatek(Linija linija) {
		seznam.push_back(linija);
	}

	void tiskaj() const {
		std::for_each(seznam.begin(), seznam.end(), [](Linija lin) {
			for (int j : lin.logika) std::cout << j << ' ';
			std::cout << "   " << lin.gezirji << '\n';
			});
	}
};


void preberiDatoteko(Porocilo& porocilo, std::string pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (podatki.is_open()) {

		while (podatki.peek() != EOF) {

			std::string vrstica;
			std::getline(podatki, vrstica);
			std::stringstream ss(vrstica);

			std::string gezirji;
			ss >> gezirji;

			std::list<short> logika;
			short vrednost;
			char c;

			while (ss >> vrednost) {

				logika.push_back(vrednost);
				ss >> c;
			}

			porocilo.dodajPodatek(Linija(gezirji, logika));
		}

	}
	else std::cout << "Datoteka ni najdena.\n";
}


int main() {

	Porocilo porocilo;

	preberiDatoteko(porocilo, "PodatkiAdvent23-12.txt");

	porocilo.tiskaj();
	std::cout << std::endl;

	std::cout << "Vsota moznih kombinacij je " << porocilo.vsotaMoznihKombinacij() << ".\n";
	std::cout << std::endl;


	distribucija(8, 4);


	return 0;
}
/**/

void Podatek::ovrednoti() const {


	bool predhodnaJeNapaka = false;
	bool predhodnaJeNeznana = false;
	std::vector<Skupina> seznamSkupin;

	for (int i = 0; i < zapis.size(); i++) {

		if (zapis[i] == '#') {
			if (!predhodnaJeNapaka) {
				seznamSkupin.push_back(Skupina(i, 1, NAPAKA));
			}
			else {
				seznamSkupin.back().velikost++;
			}
			predhodnaJeNapaka = true;
			predhodnaJeNeznana = false;
		}
		else if (zapis[i] == '?') {
			if (!predhodnaJeNeznana) {
				seznamSkupin.push_back(Skupina(i, 1, NEZNAN));
			}
			else {
				seznamSkupin.back().velikost++;
			}
			predhodnaJeNeznana = true;
			predhodnaJeNapaka = false;
		}
		else {
			predhodnaJeNeznana = false;
			predhodnaJeNapaka = false;
		}
	}
	///
	///std::cout << seznamSkupin.size() << '\t';
	///


	std::vector<std::pair<short, short>> seznamDotikanja;

	for (int i = 0; i < seznamSkupin.size() - 1; i++)
		if (seznamSkupin[i].mesto + seznamSkupin[i].velikost == seznamSkupin[i + 1].mesto)
			seznamDotikanja.push_back({ i,i + 1 });
	///
	///std::cout << seznamDotikanja.size() << '\n';
	///
	

}

bool Podatek::ustrezen(const std::string& poskusniZapis) const {
	
	std::vector<short> poskusniSeznamVrednosti;

	bool predhodnaJeNapaka = false;

	for (int i = 0; i < poskusniZapis.size(); i++) {

		if (poskusniZapis[i] == '#') {
			if (!predhodnaJeNapaka) {
				poskusniSeznamVrednosti.push_back(1);
			}
			else {
				poskusniSeznamVrednosti.back()++;
			}
			predhodnaJeNapaka = true;
		}
		else
			predhodnaJeNapaka = false;
	}

	if (poskusniSeznamVrednosti == seznamVrednosti)
		return true;
	else
		return false;
}

bool Podatek::ustrezen(const int& bitStevilo) const {
	
	std::string poskusniZapis = zapis;
	
	int stevilo = 0;
	int mesto;

	for (auto itr = poskusniZapis.rbegin(); itr != poskusniZapis.rend(); itr++) {
		if (*itr == '?') {

			mesto = 1 << stevilo++;

			(bitStevilo & mesto) == mesto ? *itr = '#' : '.';
		}
	}

	return ustrezen(poskusniZapis);
}

int Podatek::vrniSteviloNeznanih() const {
	return std::count(zapis.begin(), zapis.end(), '?');
}

int Podatek::vrniSteviloNeznanihNapak() const {
	return std::accumulate(seznamVrednosti.begin(), seznamVrednosti.end(), 0)
		   - std::count(zapis.begin(), zapis.end(), '#');
}
