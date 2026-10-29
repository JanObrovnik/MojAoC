/*
	t = 2:50:50 ur
	malo lahko
	t = 5:25 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


enum Vrsta {
	Missile,
	Drain,
	Shield,
	Poison,
	Recharge,
	Size,
};

struct Ucinek {

	short cas;
	short moc;
	short mana;
	Vrsta vrsta;

	Ucinek() : cas(0), moc(0), mana(0), vrsta(Vrsta::Missile) {}
	Ucinek(short cas, short moc, short mana, Vrsta vrsta) : cas(cas), moc(moc), mana(mana), vrsta(vrsta) {}
};


void preberiPodatke(const std::string& pot, short& HP, short& damage) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	podatki.ignore(12);
	podatki >> HP;
	podatki.ignore(8);
	podatki >> damage;

	podatki.close();

	return;
}

void zapisiPodatke(short& HP, short& mana, std::map<Vrsta, Ucinek>& seznamUcinkov) {

	HP = 50;
	mana = 500;

	seznamUcinkov[Missile] = Ucinek(-1, 4, 53, Vrsta::Missile);
	seznamUcinkov[Drain] = Ucinek(-1, 2, 73, Vrsta::Drain);
	seznamUcinkov[Shield] = Ucinek(6, 7, 113, Vrsta::Shield);
	seznamUcinkov[Poison] = Ucinek(6, 3, 173, Vrsta::Poison);
	seznamUcinkov[Recharge] = Ucinek(5, 101, 229, Vrsta::Recharge);
}


std::pair<int, int> igraj(short posastHP, const short& posastDamage, short jazHP, short jazMana, const std::map<Vrsta, Ucinek>& seznamUcinkov, const std::vector<Vrsta>& zaporedjeUcinkov, const int& del) {

	int mana = 0;

	Ucinek trajanjeShield = seznamUcinkov.at(Shield);
	trajanjeShield.cas = 0;
	Ucinek trajanjePoison = seznamUcinkov.at(Poison);
	trajanjePoison.cas = 0;
	Ucinek trajanjeRecharge = seznamUcinkov.at(Recharge);
	trajanjeRecharge.cas = 0;
	
	//for (Vrsta vr : zaporedjeUcinkov) std::cout << vr; std::cout << '\n';
	for (Vrsta vr : zaporedjeUcinkov) {

		if (del == 2) {
			jazHP--;
			if (jazHP <= 0)
				return { 1,mana };
		}

		//if (zaporedjeUcinkov == std::vector<Vrsta>{Shield, Poison, Missile, Missile, Missile}) {
		//	std::cout << posastHP << ", " << jazHP << ", " << jazMana << '\n';
		//}

		if (trajanjeShield.cas > 0) {
			trajanjeShield.cas--;
		}
		if (trajanjePoison.cas > 0) {
			trajanjePoison.cas--;
			posastHP -= seznamUcinkov.at(Poison).moc;
		}
		if (trajanjeRecharge.cas > 0) {
			trajanjeRecharge.cas--;
			jazMana += seznamUcinkov.at(Recharge).moc;
		}

		if (posastHP <= 0)
			return { 0,mana };

		if (vr == Missile) {
			mana += seznamUcinkov.at(Missile).mana;
			jazMana -= seznamUcinkov.at(Missile).mana;
			posastHP -= seznamUcinkov.at(Missile).moc;
		}
		else if (vr == Drain) {
			mana += seznamUcinkov.at(Drain).mana;
			jazMana -= seznamUcinkov.at(Drain).mana;
			posastHP -= seznamUcinkov.at(Drain).moc;
			jazHP += seznamUcinkov.at(Drain).moc;
		}
		else if (vr == Shield) {
			if (trajanjeShield.cas > 0)
				return { -1,mana };
			mana += seznamUcinkov.at(Shield).mana;
			jazMana -= seznamUcinkov.at(Shield).mana;
			trajanjeShield.cas = seznamUcinkov.at(Shield).cas;
		}
		else if (vr == Poison) {
			if (trajanjePoison.cas > 0)
				return { -1,mana };
			mana += seznamUcinkov.at(Poison).mana;
			jazMana -= seznamUcinkov.at(Poison).mana;
			trajanjePoison.cas = seznamUcinkov.at(Poison).cas;
		}
		else if (vr == Recharge) {
			if (trajanjeRecharge.cas > 0)
				return { -1,mana };
			mana += seznamUcinkov.at(Recharge).mana;
			jazMana -= seznamUcinkov.at(Recharge).mana;
			trajanjeRecharge.cas = seznamUcinkov.at(Recharge).cas;
		}

		if (jazMana <= 0)
			return { 1,mana };
		if (posastHP <= 0)
			return { 0,mana };


		if (trajanjeShield.cas > 0) {
			trajanjeShield.cas--;
		}
		if (trajanjePoison.cas > 0) {
			trajanjePoison.cas--;
			posastHP -= seznamUcinkov.at(Poison).moc;
		}
		if (trajanjeRecharge.cas > 0) {
			trajanjeRecharge.cas--;
			jazMana += seznamUcinkov.at(Recharge).moc;
		}

		if (posastHP <= 0)
			return { 0,mana };

		short damage = posastDamage;
		if (trajanjeShield.cas > 0) {
			damage -= seznamUcinkov.at(Shield).moc;
			if (damage < 1)
				damage = 1;
		}
		jazHP -= damage;

		if (jazHP <= 0)
			return { 1,mana };
	}

	return { -2,mana };
}

inline void povecaj(std::vector<Vrsta>& zaporedjeUcinkov) {
	zaporedjeUcinkov.back() = Vrsta(zaporedjeUcinkov.back() + 1);
	for (int i = zaporedjeUcinkov.size() - 1; i >= 0; i--) {
		if (zaporedjeUcinkov[i] == Vrsta::Size) {
			zaporedjeUcinkov[i] = Vrsta::Missile;
			if (i > 0)
				zaporedjeUcinkov[i - 1] = Vrsta(zaporedjeUcinkov[i - 1] + 1);
			else
				zaporedjeUcinkov.back() = Vrsta(zaporedjeUcinkov.back() + 1);
		}
	}
}

unsigned int simuliraj(const short& posastHP, const short& posastDamage, const short& jazHP, const short& jazMana, const std::map<Vrsta, Ucinek>& seznamUcinkov, const int& del) {

	unsigned int resitev = -1;

	int steviloPotez = 0;

	while (resitev == (unsigned int)-1) {

		steviloPotez++;
		//std::cout << steviloPotez << '\n';

		std::vector<Vrsta> zaporedjeUcinkov(steviloPotez, Vrsta::Missile);
		//for (Vrsta vr : zaporedjeUcinkov) std::cout << vr << '\n';

		for (int i = 0; i < pow(int(Vrsta::Size), steviloPotez); i++) {

			//std::pair<int, int> rezultat = igraj(14, 8, 10, 250, seznamUcinkov, zaporedjeUcinkov);
			std::pair<int, int> rezultat = igraj(posastHP, posastDamage, jazHP, jazMana, seznamUcinkov, zaporedjeUcinkov, del);

			if (rezultat.first == -2) {
				//std::cout << "Nihce ni zmagal: "; for (Vrsta vr : zaporedjeUcinkov) std::cout << vr; std::cout << '\n';
			}
			else if (rezultat.first == -1) {
				//std::cout << "Prepovedana poteza\n";
			}
			else if (rezultat.first == 0) {
				//std::cout << "o\n";
				if (rezultat.second < resitev) {
					//for (Vrsta vr : zaporedjeUcinkov) std::cout << vr; std::cout << '\n';
					resitev = rezultat.second;
				}
			}
			else if (rezultat.first == 1) {
				//std::cout << "x\n";
			}

			povecaj(zaporedjeUcinkov);
		}
	}

	return resitev;
}


int main() {

	short posastHP, posastDamage;
	preberiPodatke("2015/22.txt", posastHP, posastDamage);

	short jazHP, jazMana;
	std::map<Vrsta, Ucinek> seznamUcinkov;
	zapisiPodatke(jazHP, jazMana, seznamUcinkov);


	unsigned int resitev1 = simuliraj(posastHP, posastDamage, jazHP, jazMana, seznamUcinkov, 1);
	std::cout << "Najmanj mane, ki jo rabimo za zmago je " << resitev1 << ".\n";

	unsigned int resitev2 = simuliraj(posastHP, posastDamage, jazHP, jazMana, seznamUcinkov, 2);
	std::cout << "Najmanj mane, ki jo rabimo za zmago na visji tezavnosti je " << resitev2 << ".\n";


	return 0;
}
