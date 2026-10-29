/*
	t = 3:51:30 ur
	zelo lahko
	t = 8:28 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct Oprema {
	
	short cena, napad, obramba;
	
	Oprema(short cena, short napad, short obramba) : cena(cena), napad(napad), obramba(obramba) {}
};
void operator+=(Oprema& op1, const Oprema& op2) {
	op1.cena += op2.cena;
	op1.napad += op2.napad;
	op1.obramba += op2.obramba;
}

class Borec {
	
	Oprema oprema;
	short HP;

public:
	Borec() : oprema(0, 0, 0), HP(0) {}
	Borec(short HP) : oprema(0, 0, 0), HP(HP) {}

	void napaden(const Borec& borec) {
		short att = (borec.oprema.napad - oprema.obramba);
		if (att < 1) att = 1;
		HP -= att;
	}

	void nastaviOpremo(const Oprema& oprema) {
		this->oprema = oprema;
	}
	void nastaviHP(const int& HP) {
		this->HP = HP;
	}

	Oprema dobiOpremo() const {
		return oprema;
	}
	short dobiHP() const {
		return HP;
	}

	void dodajOpremo(const Oprema& oprema) {
		this->oprema += oprema;
	}

	friend Borec preberiPodatke(const std::string& pot);
};


Borec preberiPodatke(const std::string& pot) {

	Borec resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki.ignore(12);
	podatki >> resitev.HP;
	podatki.ignore(8);
	podatki >> resitev.oprema.napad;
	podatki.ignore(7);
	podatki >> resitev.oprema.obramba;

	podatki.close();

	return resitev;
}

void zapisiPodatke(std::vector<Oprema>& orozja, std::vector<Oprema>& oklepi, std::vector<Oprema>& prstani) {

	orozja.push_back(Oprema(8,  4, 0));
	orozja.push_back(Oprema(10, 5, 0));
	orozja.push_back(Oprema(25, 6, 0));
	orozja.push_back(Oprema(40, 7, 0));
	orozja.push_back(Oprema(74, 8, 0));

	oklepi.push_back(Oprema(13,  0, 1));
	oklepi.push_back(Oprema(31,  0, 2));
	oklepi.push_back(Oprema(53,  0, 3));
	oklepi.push_back(Oprema(75,  0, 4));
	oklepi.push_back(Oprema(103, 0, 5));
	
	prstani.push_back(Oprema(25,  1, 0));
	prstani.push_back(Oprema(50,  2, 0));
	prstani.push_back(Oprema(100, 3, 0));
	prstani.push_back(Oprema(20,  0, 1));
	prstani.push_back(Oprema(40,  0, 2));
	prstani.push_back(Oprema(80,  0, 3));
}


int igraj(Borec jaz, Borec posast) {

	while (true) {

		posast.napaden(jaz);

		if (posast.dobiHP() <= 0)
			return 0;
		//std::cout << "Pos: " << posast.dobiHP() << ", ";

		jaz.napaden(posast);

		if (jaz.dobiHP() <= 0)
			return 1;
		//std::cout << "Jaz: " << jaz.dobiHP() << "\n";
	}
}

std::pair<unsigned short, unsigned short> preglejVse(const Borec& jaz, const Borec& posast, const std::vector<Oprema>& orozja, const std::vector<Oprema>& oklepi, const std::vector<Oprema>& prstani) {

	unsigned short resitev1 = -1;
	unsigned short resitev2 = 0;

	//std::vector<int> vec(8);

	for (int i = 0; i < (int)orozja.size(); i++) {
		for (int j = -1; j < (int)oklepi.size(); j++) {
			for (int k = -1; k < (int)prstani.size(); k++) {
				for (int l = -1; l < (int)prstani.size(); l++) {

					if (k == l)
						continue;

					Borec nov = jaz;

					if (i != -1) nov.dodajOpremo(orozja[i]);
					if (j != -1) nov.dodajOpremo(oklepi[j]);
					if (k != -1) nov.dodajOpremo(prstani[k]);
					if (l != -1) nov.dodajOpremo(prstani[l]);

					if (nov.dobiOpremo().cena < resitev1) {
						if (igraj(nov, posast) == 0) {
							resitev1 = nov.dobiOpremo().cena;
							//vec[0] = i;
							//vec[1] = j;
							//vec[2] = k;
							//vec[3] = l;
						}
					}

					if (nov.dobiOpremo().cena > resitev2) {
						if (igraj(nov, posast) == 1) {
							resitev2 = nov.dobiOpremo().cena;
							//vec[4] = i;
							//vec[5] = j;
							//vec[6] = k;
							//vec[7] = l;
						}
					}
				}
			}
		}
	}
	//for (int i : vec) std::cout << i << ' '; std::cout << '\n';

	return { resitev1, resitev2 };
}


int main() {

	Borec jaz(100);
	Borec posast = preberiPodatke("2015/21.txt");

	std::vector<Oprema> orozja, oklepi, prstani;
	zapisiPodatke(orozja, oklepi, prstani);


	std::pair<unsigned short, unsigned short> resitev = preglejVse(jaz, posast, orozja, oklepi, prstani);

	std::cout << "Potrebujemo zapraviti " << resitev.first << " denarja.\n";
	std::cout << "Lahko zapravimo " << resitev.second << " denarja in se vedno zgubimo.\n";
	

	return 0;
}
