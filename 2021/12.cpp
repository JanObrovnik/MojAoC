/*
	t = 50:00 min (priblizno)
	nekoliko lako
	t = 6:17 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


enum TipJame {
	zacetek,
	konec,
	majhna,
	velika
};

class Jama {
public:
	std::string naziv;
	TipJame tip;
	std::vector<std::string> seznamPovezav;

	Jama(std::string naziv) : naziv(naziv) {
		
		if (naziv == "start") tip = zacetek;
		else if (naziv == "end") tip = konec;
		else if (naziv[0] >= 'a' && naziv[0] <= 'z') tip = majhna;
		else tip = velika;
	}

	void dodajPovezavo(const std::string& povezava) {
		seznamPovezav.push_back(povezava);
	}
};


std::map<std::string, std::vector<std::string>> preberiPodtke(const std::string& pot) {

	std::map<std::string, std::vector<std::string>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::string naziv1, naziv2;

	while (podatki.peek() != EOF) {

		std::getline(podatki, naziv1, '-');
		std::getline(podatki, naziv2);

		resitev[naziv1].push_back(naziv2);
		resitev[naziv2].push_back(naziv1);
	}

	podatki.close();

	return resitev;
}


bool majhnaJama(const std::string& jama) {
	const char& c = jama.front();
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

int potuj1(const std::string& tocka, std::vector<std::string> seznamZeObiskanih, const std::map<std::string, std::vector<std::string>>& zemljevid) {

	static int resitev = 0;

	if (tocka == "end") {
		resitev++;
		return resitev;
	}

	auto it = std::find(seznamZeObiskanih.begin(), seznamZeObiskanih.end(), tocka); /////////////////
	if (it != seznamZeObiskanih.end()) {
		if (majhnaJama(tocka) || tocka == "start")
			return -1;
	}

	seznamZeObiskanih.push_back(tocka);

	for (const std::string& novaTocka : zemljevid.at(tocka))
		potuj1(novaTocka, seznamZeObiskanih, zemljevid);


	return resitev;
}



int potuj2(const std::string& tocka, std::vector<std::string> seznamZeObiskanih, bool dvojniObisk, const std::map<std::string, std::vector<std::string>>& zemljevid) {

	static int resitev = 0;

	if (tocka == "end") {
		resitev++;
		return resitev;
	}

	auto it = std::find(seznamZeObiskanih.begin(), seznamZeObiskanih.end(), tocka); /////////////////
	if (it != seznamZeObiskanih.end()) {
		if (tocka == "start")
			return -1;
		if (majhnaJama(tocka)) {
			if (dvojniObisk == false)
				dvojniObisk = true;
			else
				return -1;
		}
	}

	seznamZeObiskanih.push_back(tocka);

	for (const std::string& novaTocka : zemljevid.at(tocka))
		potuj2(novaTocka, seznamZeObiskanih, dvojniObisk, zemljevid);


	return resitev;
}


int main() {

	std::map<std::string, std::vector<std::string>> zemljevid(preberiPodtke("2021/12.txt"));
	//for (auto it = zemljevid.begin(); it != zemljevid.end(); it++) {
	//	std::cout << it->first << " : ";
	//	for (const std::string& str : it->second)
	//		std::cout << str << ", ";
	//	std::cout << '\n';
	//}
		
	int resitev1(potuj1("start", {}, zemljevid));

	std::cout << "Stevilo razlicnih poti je " << resitev1 << ".\n"; // 5212



	int resitev2(potuj2("start", {}, false, zemljevid));

	std::cout << "Stevilo razlicnih poti z dvojnim obiskom male jame je " << resitev2 << ".\n"; // 134862


	return 0;
}
