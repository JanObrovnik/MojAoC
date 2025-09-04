/*
	1:	Relativno lahka
	2:	Zelo zahtevna (zelo nadlezna)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>



enum smer {
	LEVA = 'L',
	DESNA = 'R'
};


/// <summary>
/// 1 Del
/// </summary>
class Navigacija {
private:
	std::string ukazi;

	std::vector<std::string> lokacija;
	std::vector<std::string> leva;
	std::vector<std::string> desna;

	std::string zacetek = "AAA";
	std::string konec = "ZZZ";
	std::string trenutno = zacetek;

	int stevilo = 0;

public:
	void nastaviUkazi(std::string ukazi) { this->ukazi = ukazi; }

	void dodaj(std::string lokacija, std::string leva, std::string desna) {
		this->lokacija.push_back(lokacija);
		this->leva.push_back(leva);
		this->desna.push_back(desna);
	}

	int potuj() {

		while (true) for (char c : ukazi) {
						
			stevilo++;

			
			int i = std::find(lokacija.begin(), lokacija.end(), trenutno) - lokacija.begin();
			
			if (c == LEVA) trenutno = leva[i];
			else if (c == DESNA) trenutno = desna[i];


			if (trenutno == konec) return stevilo;
		}
	}

	friend class NavigacijaSeznama;
	//friend int main();
};

void operator>>(std::istream& is, Navigacija& navi) {
	std::string loc, l, d;
	char c;
	is >> loc >> c >> c >> l >> d;
	l.erase(3); d.erase(3);
	navi.dodaj(loc, l, d);
}


/// <summary>
/// 2 Del
/// </summary>

std::set<unsigned long long> edinstvenaPrastevila(std::vector<int>& seznam) {

	std::set<unsigned long long> resitev;

	std::for_each(seznam.begin(), seznam.end(), [&](int& stevilo) {
		for (int i = 2; i < stevilo / 2 + 1; i++)
			if (stevilo % i == 0)
				resitev.insert(i);
		});

	return resitev;
}


unsigned long long najmanjsiSkupniVeckratnik(std::vector<int> seznam) {
	
	unsigned long long resitev = 1;

	std::set<unsigned long long> set = edinstvenaPrastevila(seznam);


	for (unsigned long long i : set) resitev *= i;


	return resitev;
}

class NavigacijaSeznama {
private:
	std::string ukazi;

	std::vector<std::string> lokacija;
	std::vector<std::string> leva;
	std::vector<std::string> desna;

	char zacetek = 'A';
	char konec = 'Z';
	std::vector<std::string> trenutno;

	int stevilo = 0;
	std::vector<int> seznamStevil;

public:
	void nastaviUkazi(std::string ukazi) { this->ukazi = ukazi; }

	void dodaj(std::string lokacija, std::string leva, std::string desna) {
		this->lokacija.push_back(lokacija);
		this->leva.push_back(leva);
		this->desna.push_back(desna);
	}

	void prepisiPodatke(Navigacija& navigacija) {
		ukazi = navigacija.ukazi;
		lokacija = navigacija.lokacija;
		leva = navigacija.leva;
		desna = navigacija.desna;
	}


	void narediSeznam() {
		std::for_each(lokacija.begin(), lokacija.end(), [&](std::string str) {if (str[2] == 'A') trenutno.push_back(str); });
		seznamStevil.assign(lokacija.size(), 0);
	}

	int potujMK1() {

		narediSeznam();

		while (true) for (char ukaz : ukazi) {

			stevilo++;


			for (std::string& str : trenutno) {

				int i = std::find(lokacija.begin(), lokacija.end(), str) - lokacija.begin();

				if (ukaz == LEVA) str = leva[i];
				else if (ukaz == DESNA) str = desna[i];
			}

			if (stevilo % 20221 == 0) {
				std::for_each(trenutno.begin(), trenutno.end(), [](std::string str) {std::cout << str << ' '; });
				std::cout << '\n';
			}

			if (std::all_of(trenutno.begin(), trenutno.end(), [&](std::string str) {return str[2] == konec; })) return stevilo;
		}
	}

	unsigned long long potujMK2() {

		narediSeznam();

		std::set<int> setStevil;

		while (true) for (char ukaz : ukazi) {

			stevilo++;


			for (std::string& str : trenutno) {

				int i = std::find(lokacija.begin(), lokacija.end(), str) - lokacija.begin();

				if (ukaz == LEVA) str = leva[i];
				else if (ukaz == DESNA) str = desna[i];
			}

			if (std::any_of(trenutno.begin(), trenutno.end(), [&](std::string str) {return str[2] == konec; })) {
				auto it = std::find_if(trenutno.begin(), trenutno.end(), [&](std::string str) {return str[2] == konec; });

				if (setStevil.find(it - trenutno.begin()) == setStevil.end()) {
					setStevil.insert(it - trenutno.begin());
					seznamStevil.push_back(stevilo);

					//std::cout << stevilo << "\t"; if (setStevil.size() == trenutno.size()) std::cout << '\n';
				}
			}

			///////////////// 'seznamStevil' ima preveliko stevilo stevilk
			

			if (setStevil.size() == trenutno.size()) {
				
				auto it = std::remove_if(seznamStevil.begin(), seznamStevil.end(), [](int i) {return i == 0; });
				seznamStevil.erase(it, seznamStevil.end());


				unsigned long long resitev = najmanjsiSkupniVeckratnik(seznamStevil);

				return resitev;
			}
		}
	}

	//friend int main();
};





int main() {

	/// 1 Del
	Navigacija navigacija;


	std::fstream podatki;
	podatki.open("PodatkiAdvent23-8.txt", std::ios::in);

	if (podatki.is_open()) {

		std::string str;

		podatki >> str;
		navigacija.nastaviUkazi(str);

		while (podatki.peek() != EOF) 
			podatki >> navigacija;
		

		podatki.close();
	}


	std::cout << navigacija.potuj() << '\n';
	std::cout << std::endl;



	/// 2 Del
	NavigacijaSeznama navigacijaSeznama;


	navigacijaSeznama.prepisiPodatke(navigacija);


	//std::cout << navigacijaSeznama.potujMK1() << '\n';
	std::cout << navigacijaSeznama.potujMK2() << '\n';
	std::cout << std::endl;



	return 0;
}
