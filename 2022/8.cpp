/*
	Lahka
	Lahka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>



std::vector<std::string> preberiPodatke(std::string pot) {

	std::vector<std::string> resitev{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev.push_back(vrstica);
	
	podatki.close();


	return resitev;
}


bool vidnostLevo(std::vector<std::string>& zemljevidVisine, short xx, short yy) {
	//std::cout << "Levo\n";
	for (short x = 0; x < xx; x++)
		if (zemljevidVisine[yy][x] >= zemljevidVisine[yy][xx])
			return false;

	return true;
}

bool vidnostGor(std::vector<std::string>& zemljevidVisine, short xx, short yy) {
	//std::cout << "Gor\n";
	for (short y = 0; y < yy; y++)
		if (zemljevidVisine[y][xx] >= zemljevidVisine[yy][xx])
			return false;

	return true;
}

bool vidnostDesno(std::vector<std::string>& zemljevidVisine, short xx, short yy) {
	//std::cout << "Desno\n";
	for (short x = xx + 1; x < zemljevidVisine[yy].size(); x++)
		if (zemljevidVisine[yy][x] >= zemljevidVisine[yy][xx])
			return false;

	return true;
}

bool vidnostDol(std::vector<std::string>& zemljevidVisine, short xx, short yy) {
	//std::cout << "Dol\n";
	for (short y = yy + 1; y < zemljevidVisine.size(); y++)
		if (zemljevidVisine[y][xx] >= zemljevidVisine[yy][xx])
			return false;

	return true;
}

bool vidnost(std::vector<std::string>& zemljevidVisine, short xx, short yy) {
	// Optimizira logicna vrata, tako da, ce je ena od zahtev 'true', ne izvrsi drugih
	return vidnostDol(zemljevidVisine, xx, yy) || vidnostGor(zemljevidVisine, xx, yy) ||
		   vidnostLevo(zemljevidVisine, xx, yy) || vidnostDesno(zemljevidVisine, xx, yy);
}

std::vector<std::string> narediZemljevidVidnosti(std::vector<std::string>& zemljevidVisine) {

	std::vector<std::string> resitev(zemljevidVisine);
	
	for (short y = 0; y < zemljevidVisine.size(); y++)
		for (short x = 0; x < zemljevidVisine[y].size(); x++)
			resitev[y][x] = (vidnost(zemljevidVisine, x, y)) ? '#' : '.';
		
	return resitev;
}

int steviloVidnihDreves(std::vector<std::string>& zemljevidVidnosti) {

	int resitev(0);

	for (const std::string& vrstica : zemljevidVidnosti)
		for (const char& karakter : vrstica)
			if (karakter == '#')
				resitev++;

	return resitev;
}

int vrednostDrevesaLevo(std::vector<std::string>& zemljevidVisine, short xx, short yy) {

	int resitev(0);

	for (short x = xx - 1; x >= 0; x--) {
		resitev++;
		if (zemljevidVisine[yy][x] >= zemljevidVisine[yy][xx])
			break;
	}

	return resitev;
}

int vrednostDrevesaGor(std::vector<std::string>& zemljevidVisine, short xx, short yy) {

	int resitev(0);

	for (short y = yy - 1; y >= 0; y--) {
		resitev++;
		if (zemljevidVisine[y][xx] >= zemljevidVisine[yy][xx])
			break;
	}

	return resitev;
}

int vrednostDrevesaDesno(std::vector<std::string>& zemljevidVisine, short xx, short yy) {

	int resitev(0);

	for (short x = xx + 1; x < zemljevidVisine[yy].size(); x++) {
		resitev++;
		if (zemljevidVisine[yy][x] >= zemljevidVisine[yy][xx])
			break;
	}

	return resitev;
}

int vrednostDrevesaDol(std::vector<std::string>& zemljevidVisine, short xx, short yy) {

	int resitev(0);

	for (short y = yy + 1; y < zemljevidVisine.size(); y++) {
		resitev++;
		if (zemljevidVisine[y][xx] >= zemljevidVisine[yy][xx])
			break;
	}

	return resitev;
}

int vrednostDrevesa(std::vector<std::string>& zemljevidVisine, short xx, short yy) {

	return vrednostDrevesaDol(zemljevidVisine, xx, yy) * vrednostDrevesaGor(zemljevidVisine, xx, yy) *
		   vrednostDrevesaLevo(zemljevidVisine, xx, yy) * vrednostDrevesaDesno(zemljevidVisine, xx, yy);
}

int vrednostNajboljsegaDrevesa(std::vector<std::string>& zemljevidVisine) {

	int resitev(0);

	for (short y = 1; y < zemljevidVisine.size() - 1; y++)
		for (short x = 1; x < zemljevidVisine[y].size() - 1; x++) {
			int vrednost(vrednostDrevesa(zemljevidVisine, x, y));
			if (vrednost > resitev)
				resitev = vrednost;
		}

	return resitev;
}



int main() {

	std::vector<std::string> zemljevidVisine(preberiPodatke("Advent22/8.txt"));
	std::vector<std::string> zemljevidVidnosti(narediZemljevidVidnosti(zemljevidVisine));
	int resitev(steviloVidnihDreves(zemljevidVidnosti));

	//for (const auto& vr : zemljevidVisine)
	//	std::cout << vr << '\n';
	//std::cout << '\n';
	
	for (const auto& vr : zemljevidVidnosti)
		std::cout << vr << '\n';
	std::cout << '\n';

	std::cout << "Stevilo vidnih dreves od roba je " << resitev << ".\n";


	int resitev2(vrednostNajboljsegaDrevesa(zemljevidVisine));
	std::cout << "Najvecje stevilo dreves, ki jih lahko vidis iz drugega drevesa je " << resitev2 << ".\n";


	return 0;
}