/*
	precej enostavno
	komplicirana zahtevnost : zelo lahko ali relativno tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


enum Karakteristika {
	NARASCUJOCA,
	PADAJOCA,
	NEUTRALNA,
};


std::vector<std::vector<short>> preberiPodatke(std::string pot) {

	std::vector<std::vector<short>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);


	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena\n";
		return resitev;
	}

	std::string vrstica;
	while (podatki.peek() != EOF) {

		resitev.push_back({});

		std::getline(podatki, vrstica);
		std::stringstream ss(vrstica);

		short vrednost;
		while (ss >> vrednost)
			resitev.back().push_back(vrednost);
	}

	podatki.close();


	return resitev;
}


bool preveriVarnost(std::vector<short>& porocilo) {

	Karakteristika kar;

	if (porocilo[0] > porocilo[1]) kar = PADAJOCA;
	else if (porocilo[0] < porocilo[1]) kar = NARASCUJOCA;
	else return false;

	short maxRazlika = 0;
	short minRazlika = 0;
	for (short i = 0; i < porocilo.size() - 1; i++) {

		short razlika = porocilo[i + 1] - porocilo[i];

		if (razlika == 0) return false;
		else if (razlika > 3 || razlika < -3) return false;
		else if (razlika > maxRazlika) maxRazlika = razlika;
		else if (razlika < minRazlika) minRazlika = razlika;
	}
	if (minRazlika < 0 && maxRazlika > 0) return false;

	return true;
}


Karakteristika dolociKarakteristiko(short st1, short st2) {
	if (st1 > st2) return PADAJOCA;
	if (st1 < st2) return NARASCUJOCA;
	return NEUTRALNA;
}

/*
bool preveriRazsirjenoVarnost(std::vector<short> porocilo, bool obstajaNapaka = false) {

	bool resitev = false;

	Karakteristika kar;

	if (porocilo[0] > porocilo[1]) kar = PADAJOCA;
	else if (porocilo[0] < porocilo[1]) kar = NARASCUJOCA;
	else resitev = false;

	short maxRazlika = 0;
	short minRazlika = 0;
	for (short i = 0; i < porocilo.size() - 1; i++) {

		short razlika = porocilo[i + 1] - porocilo[i];

		if (razlika == 0) resitev = false;
		else if (razlika > 3 || razlika < -3) resitev = false;
		else if (razlika > maxRazlika) maxRazlika = razlika;
		else if (razlika < minRazlika) minRazlika = razlika;
	}
	if (minRazlika < 0 && maxRazlika > 0) resitev = false;


	if (!resitev && !obstajaNapaka) {


	}


	return true;

}*/

/*bool preveriRazsirjenoVarnost(std::vector<short> porocilo, bool obstajaNapaka = false) {

	short stNarascujocih = 0;
	short stPadajocih = 0;
	short stNeutralnih = 0;

	short maxRazlika = 0;

	for (short i = 0; i < porocilo.size() - 1; i++) {
		Karakteristika kar = dolociKarakteristiko(porocilo[i], porocilo[i + 1]);
		if (kar == NARASCUJOCA) stNarascujocih++;
		else if (kar == PADAJOCA) stPadajocih++;
		else if (kar == NEUTRALNA) stNeutralnih++;

		short razlika = abs(porocilo[i] - porocilo[i + 1]);
		if (maxRazlika < razlika) maxRazlika = razlika;
	}


	if (obstajaNapaka) {
		if (stNeutralnih > 0) return false;
		if (stNarascujocih > 0 && stPadajocih > 0) return false;
	}
	if (stNeutralnih == 0 && (stPadajocih == 0 || stNarascujocih == 0) && maxRazlika <= 3)
		return true;


	for (short i = 0; i < porocilo.size() - 1; i++) {
		Karakteristika kar = dolociKarakteristiko(porocilo[i], porocilo[i + 1]);
		if (kar == NEUTRALNA) {
			std::cout << "dela\n";
			porocilo.erase(porocilo.begin() + i);
		}
		else if (kar == NARASCUJOCA && stPadajocih > stNarascujocih || kar == PADAJOCA && stPadajocih < stNarascujocih) {
			porocilo.erase(porocilo.begin() + i + 1);
			std::cout << "dela\n";
		}
		else if (abs(porocilo[i] - porocilo[i + 1]) > 3) {
			porocilo.erase(porocilo.begin() + i + 1);
			std::cout << "dela\n";
		}
	}


	if (!obstajaNapaka) preveriRazsirjenoVarnost(porocilo, true);
	else return true;
}*/

bool preveriRazsirjenoVarnostMK1(std::vector<short> porocilo, bool obstajaNapaka = false) {

	std::vector<short> seznamNapak{};


	short stNarascujocih = 0;
	short stPadajocih = 0;

	for (short i = 0; i < porocilo.size() - 1; i++) {
		Karakteristika kar = dolociKarakteristiko(porocilo[i], porocilo[i + 1]);
		if (kar == NARASCUJOCA) stNarascujocih++;
		else if (kar == PADAJOCA) stPadajocih++;
	}

	Karakteristika splosna = (stNarascujocih > stPadajocih) ? NARASCUJOCA : PADAJOCA;
	
	
	/* Napaka ker vedno vzamemo drugi par */
	for (short i = 0; i < porocilo.size() - 1; i++) {
		Karakteristika kar = dolociKarakteristiko(porocilo[i], porocilo[i + 1]);
		if (kar == NEUTRALNA) { seznamNapak.push_back(i + 1); seznamNapak.push_back(i); }
		else if (kar != splosna) { seznamNapak.push_back(i + 1); seznamNapak.push_back(i); }
		else if (abs(porocilo[i] - porocilo[i + 1]) > 3) { seznamNapak.push_back(i + 1); seznamNapak.push_back(i); }
	}

	if (seznamNapak.size() == 0) return true;
	else {
		if (!obstajaNapaka) {
			std::vector<short> s1 = porocilo, s2 = porocilo;
			s1.erase(s1.begin() + seznamNapak.front());
			s2.erase(s2.begin() + seznamNapak.back());
			return (preveriRazsirjenoVarnostMK1(s1, true) || preveriRazsirjenoVarnostMK1(s2, true));
		}
		else {
			return false;
		}
	}
}

bool preveriRazsirjenoVarnostMK2(std::vector<short>& porocilo) {

	if (preveriVarnost(porocilo)) return true;

	for (short i = 0; i < porocilo.size(); i++) {
		std::vector<short> skrajsan = porocilo;
		skrajsan.erase(skrajsan.begin() + i);
		if (preveriVarnost(skrajsan)) return true;
	}

	return false;
}


int main() {

	std::vector<std::vector<short>> seznamPorocil(preberiPodatke("Advent24-2.txt"));

	//for (auto porocilo : seznamPorocil) {
	//	for (short level : porocilo)
	//		std::cout << level << ' ';
	//	std::cout << '\n';
	//}


	int resitev1 = 0;
	int resitev2 = 0;
	int resitev3 = 0;

	int zap = 0;
	for (auto& porocilo : seznamPorocil) {
		std::cout << ++zap << ':' << preveriVarnost(porocilo) << '|' 
								  << preveriRazsirjenoVarnostMK1(porocilo) << '|'
								  << preveriRazsirjenoVarnostMK2(porocilo) << '\n';

		resitev1 += preveriVarnost(porocilo);
		resitev2 += preveriRazsirjenoVarnostMK1(porocilo);
		resitev3 += preveriRazsirjenoVarnostMK2(porocilo);
	}

	std::cout << "Stevilo varnih je " << resitev1 << ".\n";
	std::cout << "Razsirjeno stevilo varnih MK1 je " << resitev2 << ".\n";
	std::cout << "Razsirjeno stevilo varnih MK2 je " << resitev3 << ".\n";


	return 0;
}
