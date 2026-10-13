/*
	t = 1:18:00 ur
	relativno lahko (precej podobno dnevu 9)
	t = 5:28 min
	zelo lahko (precej lustna naloga drgac)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


void preberiPodatke(const std::string& pot, std::map<std::pair<std::string, std::string>, int>& razmerja, std::vector<std::string>& seznamUdelezencev) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string udelezenec1, udelezenec2, predznakStr;
	int predznak, razmerje;

	while (podatki.peek() != EOF) {
		
		podatki >> udelezenec1;
		podatki.ignore(7);

		if (seznamUdelezencev.empty() || seznamUdelezencev.back() != udelezenec1)
			seznamUdelezencev.push_back(udelezenec1);
		
		podatki >> predznakStr;
		if (predznakStr == "gain") predznak = 1;
		else predznak = -1;
		
		podatki >> razmerje;
		razmerje *= predznak;

		podatki.ignore(36);
		podatki >> udelezenec2;
		udelezenec2.pop_back();
		podatki.ignore(1);

		//std::cout << udelezenec1 << " -> " << udelezenec2 << ": " << razmerje << '\n';

		razmerja[{udelezenec1, udelezenec2}] = razmerje;
	}

	podatki.close();

	return;
}


int izracunajSkupnoRazmerje(const std::map<std::pair<std::string, std::string>, int>& razmerja, const std::vector<std::string>& sedezniRed) {

	int resitev = 0;

	for (int i1 = 0; i1 < sedezniRed.size(); i1++) {
		for (int di : {-1, 1}) {
			
			int i2 = i1 + di;
			if (i2 == -1) i2 = sedezniRed.size() - 1;
			else if (i2 == sedezniRed.size()) i2 = 0;

			resitev += razmerja.at({ sedezniRed[i1],sedezniRed[i2] });
		}
	}

	return resitev;
}

int zamenjajSedezniRed(const std::map<std::pair<std::string, std::string>, int>& razmerja, std::vector<std::string> seznamUdelezencev) {

	static std::vector<std::string> sedezniRed;

	int resitev = 0;

	if (sedezniRed.size() == seznamUdelezencev.size() - 1) {
		sedezniRed.push_back(seznamUdelezencev.front());
		int resitev = izracunajSkupnoRazmerje(razmerja, sedezniRed);
		//for (std::string str : sedezniRed)
		//	std::cout << str << ", ";
		//std::cout << ": " << resitev << '\n';
		sedezniRed.pop_back();
		return resitev;
	}

	for (int i = 1; i < seznamUdelezencev.size(); i++) {
		
		if (std::find(sedezniRed.begin(), sedezniRed.end(), seznamUdelezencev[i]) != sedezniRed.end())
			continue;
			
		sedezniRed.push_back(seznamUdelezencev[i]);

		int vmesnaResitev = zamenjajSedezniRed(razmerja, seznamUdelezencev);

		if (vmesnaResitev > resitev) {
			resitev = vmesnaResitev;
		}

		sedezniRed.pop_back();
	}

	return resitev;
}


void dodajSebe(std::map<std::pair<std::string, std::string>, int>& razmerja, std::vector<std::string>& seznamUdelezencev) {

	for (const std::string& udelezenec : seznamUdelezencev) {
		razmerja[{"jaz", udelezenec}] = 0;
		razmerja[{udelezenec, "jaz"}] = 0;
	}
	seznamUdelezencev.push_back("jaz");
}


int main() {

	std::map<std::pair<std::string, std::string>, int> razmerja;
	std::vector<std::string> seznamUdelezencev;

	preberiPodatke("2015/13.txt", razmerja, seznamUdelezencev);


	int resitev1 = zamenjajSedezniRed(razmerja, seznamUdelezencev);

	std::cout << "Razmerje v druzbi je " << resitev1 << ".\n";


	dodajSebe(razmerja, seznamUdelezencev);

	int resitev2 = zamenjajSedezniRed(razmerja, seznamUdelezencev);

	std::cout << "Razmerje v druzbi je " << resitev2 << ".\n";


	return 0;
}
