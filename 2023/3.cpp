#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include <set>
#include <string>


class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}
};


int main() {

	std::vector<std::string> engineScematic;


	std::fstream podatki;
	podatki.open("Advent23/3.txt", std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";

		engineScematic = { "467..114..",
						   "...*......",
						   "..35..633.",
						   "......#...",
						   "617*......",
						   ".....+.58.",
						   "..592.....",
						   "......755.",
						   "...$.*....",
						   ".664.598.."
		};
	}

	std::string vrstica;
	while (std::getline(podatki, vrstica))
		engineScematic.push_back(vrstica);
	std::vector<std::string>& es = engineScematic;
	
	podatki.close();


	const char char0 = '0' - 1;
	const char char9 = '9' + 1;


	std::vector<std::vector<Tocka>> seznamTock;

	for (int i = 0; i < es.size(); i++) {
		bool vecmetnoStevilo = false;

		for (int j = 0; j < es[i].size(); j++) {

			if (es[i][j] > char0 && es[i][j] < char9) {

				if (!vecmetnoStevilo) {
					seznamTock.push_back({});
					vecmetnoStevilo = true;
				}
				seznamTock[seznamTock.size() - 1].push_back(Tocka(j, i));
			}
			else vecmetnoStevilo = false;
		}
	}


	std::vector<std::vector<Tocka>> seznamSosednjihTock;

	for (int i = 0; i < seznamTock.size(); i++) {
		seznamSosednjihTock.push_back({});
		std::vector<Tocka>& set = seznamSosednjihTock[seznamSosednjihTock.size() - 1];
	
		for (int j = 0; j < seznamTock[i].size(); j++) {
			int& x = seznamTock[i][j].x;
			int& y = seznamTock[i][j].y;

			if (j == 0) {
				set.push_back(Tocka(x - 1, y - 1));
				set.push_back(Tocka(x - 1, y + 0));
				set.push_back(Tocka(x - 1, y + 1));
			}
			else if (j == seznamTock[i].size() - 1) {
				set.push_back(Tocka(x + 1, y - 1));
				set.push_back(Tocka(x + 1, y + 0));
				set.push_back(Tocka(x + 1, y + 1));
			}
			set.push_back(Tocka(x + 0, y - 1));
			set.push_back(Tocka(x + 0, y + 1));
		}

		auto it = std::remove_if(set.begin(), set.end(), [&es](Tocka tocka) {
			return tocka.x < 0 || tocka.y < 0 || tocka.x > es[0].size() - 1 || tocka.y > es.size() - 1;
		});

		set.assign(set.begin(), it);
	}


	std::set<int> seznamZadetkov;

	for (int i = 0; i < seznamSosednjihTock.size(); i++) {
		for (int j = 0; j < seznamSosednjihTock[i].size(); j++) {
			int& x = seznamSosednjihTock[i][j].x;
			int& y = seznamSosednjihTock[i][j].y;
			
			if (es[y][x] != '.') seznamZadetkov.insert(i);
			
			//std::cout << x << " | " << y << '\n';
		}
		//std::cout << std::endl;
	}
	//std::cout << std::endl;


	std::list<int> seznamMehanskihDelov;

	for (const int& zadetek : seznamZadetkov) {
		std::string str = "";

		std::for_each(seznamTock[zadetek].begin(), seznamTock[zadetek].end(), [&es, &str](Tocka tocka) {str += es[tocka.y][tocka.x]; });

		seznamMehanskihDelov.push_back(std::stoi(str));
	}


	//std::cout << "Mehanski deli: ";
	//std::for_each(seznamMehanskihDelov.begin(), seznamMehanskihDelov.end(), [](int i) {std::cout << i << ", "; });
	
	int vsotaMehanskihDelov = std::accumulate(seznamMehanskihDelov.begin(), seznamMehanskihDelov.end(), 0);
	
	std::cout << "\nVsota mahanskih delov: ";
	std::cout << vsotaMehanskihDelov << std::endl;



	return 0;
}
