#include <iostream>
#include <fstream>


class Darilo {
	int width, lenght, height;


	int minPovrsina() const {
		int resitev = width * lenght * height;

		if (width > lenght) {
			if (height > width) resitev /= height;
			else resitev /= width;
		}
		else {
			if (height > lenght) resitev /= height;
			else resitev /= lenght;
		}

		return resitev;
	}

	int minObseg() const {
		int resitev = width + lenght + height;

		if (width > lenght) {
			if (height > width) resitev -= height;
			else resitev -= width;
		}
		else {
			if (height > lenght) resitev -= height;
			else resitev -= lenght;
		}
		resitev *= 2;

		return resitev;
	}

public:
	Darilo() : width(0), lenght(0), height(0) {}
	Darilo(int w, int l, int h) : width(w), lenght(l), height(h) {}

	
	int potrebenPapir() const {
		return 2 * width * lenght + 2 * lenght * height + 2 * height * width + minPovrsina();
	}

	int potrebenTrak() const {
		return width * lenght * height + minObseg();
	}


	friend std::istream& operator>>(std::istream& is, Darilo& da);
};

std::istream& operator>>(std::istream& is, Darilo& da) {
	is >> da.width;
	is.ignore(1);
	is >> da.lenght;
	is.ignore(1);
	is >> da.height;
	return is;
}


std::pair<int, int> preberiPodatke(std::string pot) {

	std::pair<int, int> resitev(0,0);

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "datoteka ni najdena.\n";
		return resitev;
	}

	Darilo darilo;
	while (podatki >> darilo) {
		resitev.first += darilo.potrebenPapir();
		resitev.second += darilo.potrebenTrak();
	}

	podatki.close();


	return resitev;
}


int main() {

	std::pair<int, int> resitev(preberiPodatke("Advent15-2.txt"));
	std::cout << "Potrebujejo " << resitev.first << " cm2 papirja.\n";
	std::cout << "Potrebujejo " << resitev.second << " cm traka.\n";


	return 0;
}