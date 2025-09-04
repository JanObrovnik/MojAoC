/*
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>


struct Povezava {
	std::string uporabnik1 = "";
	std::string uporabnik2 = "";

	std::string poisciDrugo(std::string uporabnik) {
		if (uporabnik == uporabnik1) return uporabnik2;
		if (uporabnik == uporabnik2) return uporabnik1;
		return "";
	}
};

std::istream& operator>>(std::istream& is, Povezava& po) {
	std::string vrstica;
	is >> vrstica;
	po.uporabnik1 = std::string(vrstica.begin(), vrstica.begin() + 2);
	po.uporabnik2 = std::string(vrstica.begin() + 3, vrstica.begin() + 5);
	return is;
}

std::istream& operator>>(std::istream& is, std::vector<Povezava>& se) {
	Povezava po;
	is >> po;
	se.push_back(po);
	return is;
}


struct LAN {
	std::string uporabnik1 = "";
	std::string uporabnik2 = ""; 
	std::string uporabnik3 = "";
	
	bool obstajaT() {
		return uporabnik1.front() == 't' ||
			   uporabnik2.front() == 't' ||
			   uporabnik3.front() == 't';
	}
};

std::ostream& operator<<(std::ostream& os, LAN la) {
	return os << la.uporabnik1 << '-' << la.uporabnik2 << '-' << la.uporabnik3;
}


std::vector<Povezava> preberiPodatke(std::string pot) {

	std::vector<Povezava> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce najti.\n";
		return resitev;
	}

	while (podatki.peek() != EOF)
		podatki >> resitev;

	podatki.close();


	return resitev;
}

std::set<std::string> zapisiUporabnike(std::vector<Povezava>& seznamPovezav) {

	std::set<std::string> resitev;

	for (Povezava& pov : seznamPovezav) {
		resitev.insert(pov.uporabnik1);
		resitev.insert(pov.uporabnik2);
	}

	return resitev;
}



int main() {

	std::vector<Povezava> seznamPovezav(preberiPodatke("Advent24-23.txt"));

	std::set<std::string> seznamUporabnikov(zapisiUporabnike(seznamPovezav));

	std::map<std::string, std::set<std::string>> povezave;
	for (const std::string& str : seznamUporabnikov)
		povezave[str] = std::set<std::string>({});

	std::cout << "dela...\n";

	for (Povezava& pov : seznamPovezav) {

		povezave[pov.uporabnik1].insert(pov.uporabnik2);
		povezave[pov.uporabnik2].insert(pov.uporabnik1);
	}

	std::cout << "dela...\n";

	std::vector<LAN> seznamLAN;

	for (Povezava& pov : seznamPovezav) { ////////////// trikrat zapise vse LAN-e
		std::multiset<std::string> seznamSkupnih(povezave[pov.uporabnik1].begin(), povezave[pov.uporabnik1].end());
		seznamSkupnih.insert(povezave[pov.uporabnik2].begin(), povezave[pov.uporabnik2].end());

		for (std::string str : povezave[pov.uporabnik1])
			if (seznamSkupnih.count(str) > 1)
				seznamLAN.push_back({ pov.uporabnik1,pov.uporabnik2,str });
	}

	std::cout << "dela...\n";

	auto it = std::remove_if(seznamLAN.begin(), seznamLAN.end(), [](LAN& lan) {return !lan.obstajaT(); });
	seznamLAN.erase(it, seznamLAN.end());
	
	//for (LAN& lan : seznamLAN)
	//	std::cout << lan << '\n';

	std::cout << "Stevilo vseh LAN-ov je " << seznamLAN.size() / 3 << " (" << seznamLAN.size() << ").\n";





	return 0;
}