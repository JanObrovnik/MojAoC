/*
	t = 1:45 ur
	relativno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Tocka {
public:
	long long x, y;

	Tocka() : x(0), y(0) {}
	Tocka(long long x, long long y) : x(x), y(y) {}

	long long povrsina() const {
		return (abs(x) + 1) * (abs(y) + 1);
	}
};

std::ostream& operator<<(std::ostream& os, const Tocka& to) {
	return os << to.x << ',' << to.y;
}
std::istream& operator>>(std::istream& is, Tocka& to) {
	is >> to.x;
	is.ignore(1);
	is >> to.y;
	return is;
}

bool operator<=(const Tocka& to1, const Tocka& to2) {
	return to1.x <= to2.x && to1.y <= to2.y;
}
bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}
bool operator>=(const Tocka& to1, const Tocka& to2) {
	return to1.x >= to2.x && to1.y >= to2.y;
}

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}
Tocka operator-(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x - to2.x, to1.y - to2.y);
}

Tocka minKot(const Tocka& to1, const Tocka& to2) {
	return Tocka(std::min(to1.x, to2.x), std::min(to1.y, to2.y));
}
Tocka maxKot(const Tocka& to1, const Tocka& to2) {
	return Tocka(std::max(to1.x, to2.x), std::max(to1.y, to2.y));
}


std::vector<Tocka> preberiPodatke(const std::string& pot) {

	std::vector<Tocka> resitev;
	
	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	Tocka tocka;

	while (podatki >> tocka) {
		resitev.push_back(tocka);
	}

	podatki.close();

	return resitev;
}


long long izracunajPovrsinoMedTockama(const Tocka& to0, const Tocka& to1) {
	return (to0 - to1).povrsina();
}

long long poisciRobove(const std::vector<Tocka>& seznamTock) {

	Tocka minXminY = seznamTock.front();
	Tocka minYminX = seznamTock.front();
	Tocka maxXmaxY = seznamTock.front();
	Tocka maxYmaxX = seznamTock.front();
	Tocka minXmaxY = seznamTock.front();
	Tocka maxYminX = seznamTock.front();
	Tocka maxXminY = seznamTock.front();
	Tocka minYmaxX = seznamTock.front();

	for (auto it = seznamTock.cbegin() + 1; it != seznamTock.cend(); it++) {
		
		if (it->x < minXminY.x) { minXminY = *it; minXmaxY = *it; }
		else if (it->x == minXminY.x) {
			if (it->y < minXminY.y) { minXminY = *it; }
			if (it->y > minXmaxY.y) { minXmaxY = *it; }
		}

		if (it->x > maxXmaxY.x) { maxXmaxY = *it; maxXminY = *it; }
		else if (it->x == maxXmaxY.x) {
			if (it->y < maxXminY.y) { maxXminY = *it; }
			if (it->y > maxXmaxY.y) { maxXmaxY = *it; }
		}

		if (it->y < minYminX.y) { minYminX = *it; minYmaxX = *it; }
		else if (it->y == minYminX.y) {
			if (it->x < minYminX.x) { minYminX = *it; }
			if (it->x > minYmaxX.x) { minYmaxX = *it; }
		}

		if (it->y > maxYmaxX.y) { maxYmaxX = *it; maxYminX = *it; }
		else if (it->y == maxYmaxX.y) {
			if (it->x < maxYminX.x) { maxYminX = *it; }
			if (it->x > maxYmaxX.x) { maxYmaxX = *it; }
		}
	}

	//std::cout << "minXminY: " << minXminY << '\n';
	//std::cout << "minYminX: " << minYminX << '\n';
	//std::cout << "maxXmaxY: " << maxXmaxY << '\n';
	//std::cout << "maxYmaxX: " << maxYmaxX << '\n';
	//std::cout << "minXmaxY: " << minXmaxY << '\n';
	//std::cout << "maxYminX: " << maxYminX << '\n';
	//std::cout << "maxXminY: " << maxXminY << '\n';
	//std::cout << "minYmaxX: " << minYmaxX << '\n';
	//std::cout << '\n';


	Tocka minXminYmeja0 = minKot(minXminY, minYminX);
	Tocka minXminYmeja1 = maxKot(minXminY, minYminX);
	Tocka maxXmaxYmeja0 = minKot(maxXmaxY, maxYmaxX);
	Tocka maxXmaxYmeja1 = maxKot(maxXmaxY, maxYmaxX);
	Tocka minXmaxYmeja0 = minKot(minXmaxY, maxYminX);
	Tocka minXmaxYmeja1 = maxKot(minXmaxY, maxYminX);
	Tocka maxXminYmeja0 = minKot(maxXminY, minYmaxX);
	Tocka maxXminYmeja1 = maxKot(maxXminY, minYmaxX);

	//std::cout << "minXminYmeja0: " << minXminYmeja0 << '\n';
	//std::cout << "minXminYmeja1: " << minXminYmeja1 << '\n';
	//std::cout << "maxXmaxYmeja0: " << maxXmaxYmeja0 << '\n';
	//std::cout << "maxXmaxYmeja1: " << maxXmaxYmeja1 << '\n';
	//std::cout << "minXmaxYmeja0: " << minXmaxYmeja0 << '\n';
	//std::cout << "minXmaxYmeja1: " << minXmaxYmeja1 << '\n';
	//std::cout << "maxXminYmeja0: " << maxXminYmeja0 << '\n';
	//std::cout << "maxXminYmeja1: " << maxXminYmeja1 << '\n';
	//std::cout << '\n';


	std::vector<Tocka> minXminYobmocje;
	std::vector<Tocka> maxXmaxYobmocje;
	std::vector<Tocka> minXmaxYobmocje;
	std::vector<Tocka> maxXminYobmocje;

	for (const Tocka& to : seznamTock) {
		if (to >= minXminYmeja0 && to <= minXminYmeja1) minXminYobmocje.push_back(to);
		if (to >= maxXmaxYmeja0 && to <= maxXmaxYmeja1) maxXmaxYobmocje.push_back(to);
		if (to >= minXmaxYmeja0 && to <= minXmaxYmeja1) minXmaxYobmocje.push_back(to);
		if (to >= maxXminYmeja0 && to <= maxXminYmeja1) maxXminYobmocje.push_back(to);
	}

	//for (const Tocka& to : minXminYobmocje) std::cout << to << " | "; std::cout << '\n';
	//for (const Tocka& to : maxXmaxYobmocje) std::cout << to << " | "; std::cout << '\n';
	//for (const Tocka& to : minXmaxYobmocje) std::cout << to << " | "; std::cout << '\n';
	//for (const Tocka& to : maxXminYobmocje) std::cout << to << " | "; std::cout << '\n';
	//std::cout << '\n';


	long long resitev = 0;

	for (const Tocka& to0 : minXminYobmocje)
		for (const Tocka& to1 : maxXmaxYobmocje) {
			const long long& novaPovrsina = izracunajPovrsinoMedTockama(to0, to1);
			if (izracunajPovrsinoMedTockama(to0, to1) > resitev)
				resitev = izracunajPovrsinoMedTockama(to0, to1);
			//std::cout << to0 << " | " << to1 << " : " << izracunajPovrsinoMedTockama(to0, to1) << '\n';
		}
	//std::cout << '\n';
	for (const Tocka& to0 : minXmaxYobmocje)
		for (const Tocka& to1 : maxXminYobmocje) {
			const long long& novaPovrsina = izracunajPovrsinoMedTockama(to0, to1);
			if (izracunajPovrsinoMedTockama(to0, to1) > resitev)
				resitev = izracunajPovrsinoMedTockama(to0, to1);
			//std::cout << to0 << " | " << to1 << " : " << izracunajPovrsinoMedTockama(to0, to1) << '\n';
		}
	//std::cout << '\n';

	return resitev;
}


int main() {

	std::vector<Tocka> seznamTocka = preberiPodatke("2025/9.txt");
	//for (auto to : seznamTocka) {
	//	std::cout << to << '\n';
	//} std::cout << '\n';

	long long resitev0 = poisciRobove(seznamTocka);
	std::cout << "Najvecja povrsina je " << resitev0 << ".\n";


	return 0;
}
