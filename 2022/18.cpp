/*
	precej lahko
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <queue>



class Tocka {
public:
	short x, y, z;

	Tocka() : x(0), y(0), z(0) {}
	Tocka(short x, short y, short z) : x(x), y(y), z(z) {}


	bool operator==(const Tocka& tocka) const {
		return x == tocka.x && y == tocka.y && z == tocka.z;
	}

	struct HashFunkcija {
		size_t operator()(const Tocka& tocka) const {
			size_t hashX = std::hash<short>()(tocka.x);
			size_t hashY = std::hash<short>()(tocka.y) << 1;
			size_t hashZ = std::hash<short>()(tocka.z) << 2;
			return hashX ^ hashY ^ hashZ;
		}
	};
};

void operator+=(Tocka& to1, const Tocka& to2) {
	to1.x += to2.x;
	to1.y += to2.y;
	to1.z += to2.z;
}


struct Meje {
	short minX, maxX;
	short minY, maxY;
	short minZ, maxZ;

	Meje(short x, short y, short z) : minX(x), maxX(x), minY(y), maxY(y), minZ(z), maxZ(z) {}

	void izpis() const {
		std::cout << "\tX\tY\tZ\n";
		std::cout << "min\t" << minX << '\t' << minY << '\t' << minZ << '\n';
		std::cout << "max\t" << maxX << '\t' << maxY << '\t' << maxZ << '\n';
	}

	bool izven(const Tocka& tocka) const {
		if (tocka.x < minX || tocka.x > maxX ||
			tocka.y < minY || tocka.y > maxY ||
			tocka.z < minZ || tocka.z > maxZ)
			return true;
		return false;
	}

	short velikost() const {
		return (maxX - minX + 1) * (maxY - minY + 1) * (maxZ - minZ + 1);
	}
};



class Kapljica {
private:
	std::unordered_set<Tocka, Tocka::HashFunkcija> seznamTock;
	const std::vector<Tocka> seznamSosed{ Tocka(0,0,1), Tocka(0,1,0), Tocka(1,0,0), Tocka(0,0,-1), Tocka(0,-1,0), Tocka(-1,0,0) };
	int skupnoSteviloPloskev;


public:
	Kapljica() : seznamTock({}), skupnoSteviloPloskev(0) {}

	void dodajTocko(const Tocka& tocka) {

		int steviloPloskev = 6;

		for (Tocka soseda : seznamSosed) {

			soseda += tocka;

			if (seznamTock.find(soseda) != seznamTock.end())
				steviloPloskev -= 2;
		}

		seznamTock.insert(tocka);
		skupnoSteviloPloskev += steviloPloskev;
	}


	int dobiCelotnoPovrsino() const {
		return skupnoSteviloPloskev;
	}

	size_t dobiVelikost() const {
		return seznamTock.size();
	}


	int dobijZunanjoPovrsino(const Meje& meje) const {

		std::unordered_set<Tocka, Tocka::HashFunkcija> seznamTockZraka{};

		std::queue<Tocka> vrstaTock({ Tocka(meje.minX, meje.minY, meje.minZ) });

		while (!vrstaTock.empty()) {

			Tocka sledilec(vrstaTock.front());

			for (Tocka soseda : seznamSosed) {
				
				soseda += sledilec;

				if (seznamTock.find(soseda) != seznamTock.end())
					continue;
				if (seznamTockZraka.find(soseda) != seznamTockZraka.end())
					continue;
				if (meje.izven(soseda))
					continue;

				vrstaTock.push(soseda);
				seznamTockZraka.insert(soseda);
			}

			vrstaTock.pop();
		}


		Kapljica kapljica = *this;

		for (short x = meje.minX + 1; x < meje.maxX - 1; x++)
			for (short y = meje.minY + 1; y < meje.maxY - 1; y++)
				for (short z = meje.minZ + 1; z < meje.maxZ - 1; z++) {
					
					Tocka tocka(x, y, z);

					if (seznamTockZraka.find(tocka) != seznamTockZraka.end())
						continue;
					if (seznamTock.find(tocka) != seznamTock.end())
						continue;

					kapljica.dodajTocko(tocka);
				}


		return kapljica.dobiCelotnoPovrsino();
	}
};


Meje preberiPodatke(const std::string& pot, Kapljica& kapljica) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return Meje(0,0,0);
	}

	short x, y, z;
	char ch;

	podatki >> x >> ch >> y >> ch >> z;
	kapljica.dodajTocko({ x,y,z });
	
	Meje meje(x,y,z);

	while (podatki.peek() != EOF) {
		podatki >> x >> ch >> y >> ch >> z;
		kapljica.dodajTocko({ x,y,z });

		if (x > meje.maxX) meje.maxX = x;
		else if (x < meje.minX) meje.minX = x;		
		if (y > meje.maxY) meje.maxY = y;
		else if (y < meje.minY) meje.minY = y;		
		if (z > meje.maxZ) meje.maxZ = z;
		else if (z < meje.minZ) meje.minZ = z;
	}
	meje.maxX++;
	meje.maxY++;
	meje.maxZ++;
	meje.minX--;
	meje.minY--;
	meje.minZ--;

	podatki.close();


	return meje;
}


int main() {

	Kapljica kapljica;

	Meje meje(preberiPodatke("Advent22/18.txt", kapljica));
	//meje.izpis();
	int resitev1(kapljica.dobiCelotnoPovrsino());

	std::cout << "Povrsina je enaka " << resitev1 << " ploskev.\n";


	int resitev2(kapljica.dobijZunanjoPovrsino(meje));

	std::cout << "Zunanja povrsina je enaka " << resitev2 << " ploskev.\n";



	return 0;
}