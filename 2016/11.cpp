/*
	t = 13:26:47 ur
	zelo tezko (obstaja bliznica, ki je zelo lahka)
	t = 4:50 min
	zelo lahko (zaradi bliznice)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


enum class Vrsta {
	GENERATOR,
	MIKROCHIP,
};
struct Naprava {
	std::string naziv;
	Vrsta vrsta;
	Naprava() : naziv(""), vrsta(Vrsta::GENERATOR) {}
	Naprava(std::string naziv, Vrsta vrsta) : naziv(naziv), vrsta(vrsta) {}
};
bool operator==(const Naprava& na1, const Naprava& na2) {
	return na1.naziv == na2.naziv && na1.vrsta == na2.vrsta;
}

enum class Smer {
	GOR,
	DOL,
};
struct Pomik {
	std::vector<Naprava> seznamNaprav;
	Smer smer;
	Pomik() : seznamNaprav({}), smer(Smer::GOR) {}
	Pomik(std::vector<Naprava> seznamNaprav, Smer smer) : seznamNaprav(seznamNaprav), smer(smer) {}
};
bool obratenPomik(const Pomik& po1, const Pomik& po2) {
	return po1.seznamNaprav == po2.seznamNaprav && po1.smer != po2.smer;
}


bool veljavnaPostavitev(const std::vector<Naprava>& seznamNaprav) {
	
	int stMikrochipov = 0;
	int stGeneratorjev = 0;
	
	for (const Naprava& naprava : seznamNaprav) {
		stMikrochipov += naprava.vrsta == Vrsta::MIKROCHIP;
		stGeneratorjev += naprava.vrsta == Vrsta::GENERATOR;
	}

	if (stMikrochipov == 0)
		return true;
	if (stGeneratorjev == 0)
		return true;
	if (stMikrochipov > stGeneratorjev)
		return false;

	for (const Naprava& mikrochip : seznamNaprav) {
		if (mikrochip.vrsta == Vrsta::MIKROCHIP) {
			auto it = std::find_if(seznamNaprav.begin(), seznamNaprav.end(), [&](const Naprava& generator) {
				return generator.vrsta == Vrsta::GENERATOR && generator.naziv == mikrochip.naziv;
				});
			if (it == seznamNaprav.end())
				return false;
		}
	}

	return true;
}

unsigned int resitev1 = -1;

class Tovarna {
	std::vector<std::vector<Naprava>> nadstropja;
	int dvigalo;

public:
	Tovarna() {
		nadstropja = std::vector<std::vector<Naprava>>(4);
		dvigalo = 0;
	}
	Tovarna(const std::vector<std::vector<Naprava>>& nadstropja, const int& dvigalo) {
		this->nadstropja = nadstropja;
		this->dvigalo = dvigalo;
	}

	void dodajMikrochipe(const std::vector<std::pair<std::string, int>>& seznamMikrochipov) {
		for (const std::pair<std::string, int>& par : seznamMikrochipov) {
			Naprava novaNaprava;
			novaNaprava.naziv = par.first;
			novaNaprava.vrsta = Vrsta::MIKROCHIP;
			nadstropja[par.second].push_back(novaNaprava);
		}
	}
	void dodajGeneratorje(const std::vector<std::pair<std::string, int>>& seznamGeneratorjev) {
		for (const std::pair<std::string, int>& par : seznamGeneratorjev) {
			Naprava novaNaprava;
			novaNaprava.naziv = par.first;
			novaNaprava.vrsta = Vrsta::GENERATOR;
			nadstropja[par.second].push_back(novaNaprava);
		}
	}
	void dodaj2Del() {
		nadstropja.front().push_back(Naprava("elerium", Vrsta::GENERATOR));
		nadstropja.front().push_back(Naprava("elerium", Vrsta::MIKROCHIP));
		nadstropja.front().push_back(Naprava("dilithium", Vrsta::GENERATOR));
		nadstropja.front().push_back(Naprava("dilithium", Vrsta::MIKROCHIP));
	}

	bool konec() {
		for (int i = 0; i < nadstropja.size() - 1; i++)
			if (!nadstropja[i].empty())
				return false;
		return true;
	}

	bool poskusiPomik(const Pomik& pomik, std::vector<std::vector<Naprava>>& novaNadstropja, int& novoDvigalo, bool prikazi = true) {

		auto it = std::remove_if(novaNadstropja[novoDvigalo].begin(), novaNadstropja[novoDvigalo].end(), [&](const Naprava& naprava) {
			for (const Naprava& nap : pomik.seznamNaprav)
				if (nap == naprava)
					return true;
			return false;
			});

		novaNadstropja[novoDvigalo].erase(it, novaNadstropja[novoDvigalo].end());

		if		(pomik.smer == Smer::GOR) novoDvigalo++;
		else if (pomik.smer == Smer::DOL) novoDvigalo--;

		for (const Naprava& naprava : pomik.seznamNaprav) {
			novaNadstropja[novoDvigalo].push_back(naprava);
		}

		if (prikazi) {
			print(novaNadstropja);
			//std::cout << veljavnaPostavitev(novaNadstropja[dvigalo]) << ' ' << veljavnaPostavitev(novaNadstropja[novoDvigalo]) << '\n' << '\n';
		}

		return veljavnaPostavitev(novaNadstropja[dvigalo]) && veljavnaPostavitev(novaNadstropja[novoDvigalo]);
	}

	void preglejVseMoznisti(int korak, Pomik predhodnjiPomik, const int& omejitev) {
		
		if (korak > omejitev)
			return;

		std::vector<Pomik> seznamPomikov;

		if (nadstropja[dvigalo].size() == 1) {
			if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::GOR));
			if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::DOL));
		}
		else {
			for (int i = 0; i < nadstropja[dvigalo].size(); i++) {
				if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::GOR));
				if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::DOL));
			}
			for (int i = 0; i < nadstropja[dvigalo].size() - 1; i++)
				for (int j = i + 1; j < nadstropja[dvigalo].size(); j++) {
					std::vector<Naprava> sez;
					if (dvigalo != 3) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::GOR));
						sez.clear();
					}
					if (dvigalo != 0) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::DOL));
						sez.clear();
					}
				}
		}

		for (const Pomik& pomik : seznamPomikov) {
			
			if (obratenPomik(pomik, predhodnjiPomik))
				continue;

			std::vector<std::vector<Naprava>> novaNadstropja = nadstropja;
			int novoDvigalo = dvigalo;
			
			if (poskusiPomik(pomik, novaNadstropja, novoDvigalo)) {
				
				Tovarna novaTovarna(novaNadstropja, novoDvigalo);
				
				if (novaTovarna.konec()) {
					
					//std::cout << "korak " << korak << ":\n";
					//novaTovarna.print();
					if (korak < resitev1)
						resitev1 = korak;

					return;
				}
				
				novaTovarna.preglejVseMoznisti(korak + 1, pomik, omejitev);
			}
		}
	}

	bool preglejVseMoznisti2(int korak, Pomik predhodnjiPomik, const int& omejitev) {

		bool resitev = false;
		
		if (korak > omejitev)
			return false;


		std::vector<Pomik> seznamPomikov;

		if (nadstropja[dvigalo].size() == 1) {
			if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::GOR));
			if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::DOL));
		}
		else {
			for (int i = 0; i < nadstropja[dvigalo].size(); i++) {
				if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::GOR));
				if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::DOL));
			}
			for (int i = 0; i < nadstropja[dvigalo].size() - 1; i++)
				for (int j = i + 1; j < nadstropja[dvigalo].size(); j++) {
					std::vector<Naprava> sez;
					if (dvigalo != 3) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::GOR));
						sez.clear();
					}
					if (dvigalo != 0) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::DOL));
						sez.clear();
					}
				}
		}


		for (const Pomik& pomik : seznamPomikov) {

			if (obratenPomik(pomik, predhodnjiPomik))
				continue;

			std::vector<std::vector<Naprava>> novaNadstropja = nadstropja;
			int novoDvigalo = dvigalo;

			if (poskusiPomik(pomik, novaNadstropja, novoDvigalo)) {

				Tovarna novaTovarna(novaNadstropja, novoDvigalo);

				if (novaTovarna.konec()) {

					//std::cout << "korak " << korak << ":\n";
					//novaTovarna.print();

					return true;
				}

				if (novaTovarna.preglejVseMoznisti2(korak + 1, pomik, omejitev)) {
					resitev = true;
					break;
				}
			}
		}

		return resitev;
	}

	void preglejVseMoznisti3() {

		std::vector<Pomik> seznamPomikov;

		if (nadstropja[dvigalo].size() == 1) {
			if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::GOR));
			if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo].front() }, Smer::DOL));
		}
		else {
			for (int i = 0; i < nadstropja[dvigalo].size(); i++) {
				if (dvigalo != 3) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::GOR));
				if (dvigalo != 0) seznamPomikov.push_back(Pomik({ nadstropja[dvigalo][i] }, Smer::DOL));
			}
			for (int i = 0; i < nadstropja[dvigalo].size() - 1; i++)
				for (int j = i + 1; j < nadstropja[dvigalo].size(); j++) {
					std::vector<Naprava> sez;
					if (dvigalo != 3) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::GOR));
						sez.clear();
					}
					if (dvigalo != 0) {
						sez.push_back(nadstropja[dvigalo][i]);
						sez.push_back(nadstropja[dvigalo][j]);
						seznamPomikov.push_back(Pomik(sez, Smer::DOL));
						sez.clear();
					}
				}
		}

		for (const Pomik& pomik : seznamPomikov) {

			std::vector<std::vector<Naprava>> novaNadstropja = nadstropja;
			int novoDvigalo = dvigalo;

			if (poskusiPomik(pomik, novaNadstropja, novoDvigalo, false)) {

				std::cout << '-' << (int)pomik.smer << ": ";
				for (const Naprava& naprava : pomik.seznamNaprav) {
					std::cout << '(' << naprava.naziv << ' ' << (int)naprava.vrsta << ") ";
				}
				std::cout << '\n';
			}
		}
	}


	void generatorjiPodDvigalom(int& stGen) {
		for (int i = dvigalo - 1; i >= 0; i--)
			for (const Naprava& naprava : nadstropja[i])
				stGen += naprava.vrsta == Vrsta::GENERATOR;
	}
	void mikrochipiPodDvigalom(int& stMik) {
		for (int i = dvigalo - 1; i >= 0; i--)
			for (const Naprava& naprava : nadstropja[i])
				stMik += naprava.vrsta == Vrsta::MIKROCHIP;
	}
	void popari(int& stParov, int& stOdvecGen, int& stOdvecMik) {
		
		std::vector<Naprava> kopija = nadstropja[dvigalo];
		std::vector<std::string> seznamNazivov;
		
		for (const Naprava& mikrochip : kopija)
			if (mikrochip.vrsta == Vrsta::MIKROCHIP)
				for (const Naprava& generator : kopija)
					if (generator.vrsta == Vrsta::GENERATOR && generator.naziv == mikrochip.naziv)
						seznamNazivov.push_back(generator.naziv);
		stParov = seznamNazivov.size();

		auto it = std::remove_if(kopija.begin(), kopija.end(), [&](const Naprava& naprava) {
			for (const std::string& naziv : seznamNazivov)
				if (naprava.naziv == naziv)
					return true;
			return false;
			});
		kopija.erase(it, kopija.end());

		for (const Naprava& naprava : kopija) {
			if (naprava.vrsta == Vrsta::GENERATOR)
				stOdvecGen++;
			else
				stOdvecMik++;
		}
	}

	void najdiGeneratorZaDol0(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR)
				for (const Naprava& mikrochip : nadstropja[dvigalo - 1])
					if (mikrochip.vrsta == Vrsta::MIKROCHIP && mikrochip.naziv == generator.naziv) {
						pomik.seznamNaprav.push_back(generator);
						return;
					}
	}
	void najdiGeneratorZaDol1(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR) {
				bool ujema = false;
				for (const Naprava& mikrochip : nadstropja[dvigalo])
					if (mikrochip.vrsta == Vrsta::MIKROCHIP && mikrochip.naziv == generator.naziv) {
						ujema = true;
						break;
					}
				if (!ujema) {
					pomik.seznamNaprav.push_back(generator);
					return;
				}
			}
	}
	void najdiGeneratorZaGor0(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR)
				for (const Naprava& mikrochip : nadstropja[dvigalo + 1])
					if (mikrochip.vrsta == Vrsta::MIKROCHIP && mikrochip.naziv == generator.naziv)
						pomik.seznamNaprav.push_back(generator);
	}
	void najdiGeneratorZaGor1(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR) {
				bool ujema = false;
				for (const Naprava& mikrochip : nadstropja[dvigalo])
					if (mikrochip.vrsta == Vrsta::MIKROCHIP && mikrochip.naziv == generator.naziv) {
						ujema = true;
						break;
					}
				if (!ujema) {
					pomik.seznamNaprav.push_back(generator);
					if (pomik.seznamNaprav.size() == 2)
						return;
				}
			}
	}
	void najdiParZaDol(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		const Naprava& naprava = nadstropja[dvigalo].front();
		pomik.seznamNaprav.push_back(Naprava(naprava.naziv, Vrsta::GENERATOR));
		pomik.seznamNaprav.push_back(Naprava(naprava.naziv, Vrsta::MIKROCHIP));
	}
	void najdiParZaDolSamoGen(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		const Naprava& naprava = nadstropja[dvigalo].front();
		pomik.seznamNaprav.push_back(Naprava(naprava.naziv, Vrsta::GENERATOR));
	}
	void najdiVseZaGor(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		pomik.seznamNaprav.push_back(nadstropja[dvigalo].front());
		pomik.seznamNaprav.push_back(nadstropja[dvigalo].back());
	}
	void najdi2GenZaGor(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR) {
				pomik.seznamNaprav.push_back(generator);
				if (pomik.seznamNaprav.size() == 2)
					return;
			}
	}
	void najdi2MikZaDol(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		for (const Naprava& mikrochip : nadstropja[dvigalo])
			if (mikrochip.vrsta == Vrsta::MIKROCHIP) {
				pomik.seznamNaprav.push_back(mikrochip);
				if (pomik.seznamNaprav.size() == 2)
					return;
			}
	}
	void najdi1MikrochipZaGor(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		for (const Naprava& mikrochip : nadstropja[dvigalo])
			if (mikrochip.vrsta == Vrsta::MIKROCHIP)
				for (const Naprava& generator : nadstropja[dvigalo + 1])
					if (generator.vrsta == Vrsta::GENERATOR && generator.naziv == mikrochip.naziv) {
						pomik.seznamNaprav.push_back(mikrochip);
						return;
					}
	}
	void najdiPrviMikrochipZaDol(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		for (const Naprava& mikrochip : nadstropja[dvigalo])
			if (mikrochip.vrsta == Vrsta::MIKROCHIP) {
				pomik.seznamNaprav.push_back(mikrochip);
				std::cout << "dela\n";
				return;
			}
	}
	void najdiEdiniMikrochipZaDol(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		pomik.seznamNaprav.push_back(nadstropja[dvigalo].front());
	}
	void najdi2MikZaGor(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		pomik.seznamNaprav.push_back(nadstropja[dvigalo].front());
		pomik.seznamNaprav.push_back(nadstropja[dvigalo].back());
	}
	void najdi1GenZaDol1(Pomik& pomik) {
		pomik.smer = Smer::DOL;
		for (const Naprava& generator : nadstropja[dvigalo])
			if (generator.vrsta == Vrsta::GENERATOR) {
				pomik.seznamNaprav.push_back(generator);
				return;
			}
	}
	void najdiParZaGor(Pomik& pomik) {
		pomik.smer = Smer::GOR;
		for (const Naprava& mikrochip : nadstropja[dvigalo])
			if (mikrochip.vrsta == Vrsta::MIKROCHIP)
				for (const Naprava& generator : nadstropja[dvigalo])
					if (generator.vrsta == Vrsta::GENERATOR) {
						pomik.seznamNaprav.push_back(mikrochip);
						pomik.seznamNaprav.push_back(generator);
						return;
					}
	}
	void najdiZapolniMikGor(Pomik& pomik) {
		najdiGeneratorZaGor0(pomik);
		najdiGeneratorZaGor1(pomik);
	}

	int PomikGeneratorjev() {

		int resitev = 0;

		while (resitev < 20) {
			
			std::cout << "Korak: " << resitev << '\n';

			preglejVseMoznisti3();
			
			int stParov = 0;
			int stOdvecGen = 0;
			int stOdvecMik = 0;
			popari(stParov, stOdvecGen, stOdvecMik);
			//std::cout << "par gen mik\n" << ' ' << stParov << " | " << stOdvecGen << " | " << stOdvecMik << '\n';

			int stGenPod = 0;
			generatorjiPodDvigalom(stGenPod);
			//std::cout << "st generatorjev pod dvigalom: " << stGen << '\n';

			//if (resitev == 1) {
			//	Pomik pomik;
			//	pomik.smer == Smer::GOR;
			//	pomik.seznamNaprav.push_back(Naprava("strontium", Vrsta::GENERATOR));
			//	pomik.seznamNaprav.push_back(Naprava("plutonium", Vrsta::GENERATOR));
			//	bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
			//	if (!veljavnost)
			//		std::cout << "NAPAKA: PomikGeneratorjev - eksperimetalno\n\n";
			//	resitev++;
			//	continue;
			//}

			if (dvigalo == nadstropja.size() - 2 && nadstropja.back().size() == 1 && nadstropja.back().front().vrsta == Vrsta::MIKROCHIP) {
				Pomik pomik;
				najdiZapolniMikGor(pomik);
				bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
				if (!veljavnost)
					std::cout << "NAPAKA: PomikGeneratorjev - if\n\n";
			}
			else if (stOdvecGen == 0 && stOdvecMik == 0) {
				if (stGenPod) {
					if (stParov > 1) {
						// pomakni par dol
						Pomik pomik;
						najdiParZaDol(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if 1 - if - if\n\n";
					}
					else {
						// pomakni samo gen dol
						Pomik pomik;
						najdiParZaDolSamoGen(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if 1 - if - else\n\n";
					}
				}
				else {
					if (stParov > 1) {
						Pomik pomik;
						najdi2GenZaGor(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if 1 - else - if\n\n";
					}
					else {
						Pomik pomik;
						najdiVseZaGor(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if 1 - else - else\n\n";
					}
				}
			}
			else if (stParov == 0 && stOdvecMik > 0) {
				Pomik pomik;
				najdi1MikrochipZaGor(pomik);
				bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
				if (!veljavnost)
					std::cout << "NAPAKA: PomikGeneratorjev - else if 2\n\n";
			}
			else if (stOdvecGen > 0 && stOdvecMik == 0) {
				if (dvigalo != nadstropja.size() - 1) {
					if (stGenPod) {
						Pomik pomik;
						najdiGeneratorZaDol0(pomik);
						if (pomik.seznamNaprav.empty())
							najdiGeneratorZaDol1(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if 3 - if - if\n\n";
					}
					else {
						if (stParov == 1 && stOdvecGen == 1) {
							Pomik pomik;
							najdi2GenZaGor(pomik);
							bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
							if (!veljavnost)
								std::cout << "NAPAKA: PomikGeneratorjev - else if 3 - if - else - if\n\n";
						}
						else if (stParov == 2) {
							Pomik pomik;
							najdiParZaGor(pomik);
							bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
							if (!veljavnost)
								std::cout << "NAPAKA: PomikGeneratorjev - else if 3 - if - else - else if 1\n\n";
						}
						else if (stOdvecGen > 1 && stParov > 1 && dvigalo > 0) {
							Pomik pomik;
							najdi2MikZaDol(pomik);
							bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
							if (!veljavnost)
								std::cout << "NAPAKA: PomikGeneratorjev - else if 3 - if - else - else if 2\n\n";
						}
						else {
							Pomik pomik;
							//najdiGeneratorZaGor0(pomik);
							//if (pomik.seznamNaprav.empty())
							najdiGeneratorZaGor1(pomik);
							//if (pomik.seznamNaprav.empty())
							//	najdiGeneratorZaDol(pomik);
							bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
							if (!veljavnost)
								std::cout << "NAPAKA: PomikGeneratorjev - else if 3 - if - else - else\n\n";
						}
					}
				}
				else if (stGenPod) {
					if (stParov == 1 && stOdvecGen == 0 && stOdvecMik == 0) {
						Pomik pomik;
						najdi1GenZaDol1(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if - else if - if\n\n";
					}
					else {
						Pomik pomik;
						najdiGeneratorZaDol1(pomik);
						bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
						if (!veljavnost)
							std::cout << "NAPAKA: PomikGeneratorjev - else if - else if - else\n\n";
					}
				}
				else return resitev;
			}

			resitev++;
		}

		return -1;
	}
	int PomikMikrochipov() {

		int resitev = 0;

		while (true) {

			std::cout << "Korak: " << resitev << '\n';

			int stParov = 0;
			int stOdvecGen = 0;
			int stOdvecMik = 0;
			popari(stParov, stOdvecGen, stOdvecMik);
			//std::cout << "par gen mik\n" << ' ' << stParov << " | " << stOdvecGen << " | " << stOdvecMik << '\n';

			int stMikPod = 0;
			mikrochipiPodDvigalom(stMikPod);
			//std::cout << "st mikrochipov pod dvigalom: " << stMikPod << '\n';

			//std::cout << dvigalo << " | " << nadstropja.size() - 1 << '\n';

			if (dvigalo == nadstropja.size() - 1) {
				
				if (konec()) return resitev;
				
				else if (stParov == 0) {
					Pomik pomik;
					najdiGeneratorZaDol0(pomik);
					bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
					if (!veljavnost)
						std::cout << "NAPAKA: PomikMikrochipov - if - else if\n\n";
				}
				else {
					Pomik pomik;
					najdiPrviMikrochipZaDol(pomik);
					bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
					if (!veljavnost)
						std::cout << "NAPAKA: PomikMikrochipov - if - else\n\n";
				}
			}
			else if (stMikPod) {
				if (stParov == 1) {
					Pomik pomik;
					najdiVseZaGor(pomik);
					bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
					if (!veljavnost)
						std::cout << "NAPAKA: PomikMikrochipov - else if - if\n\n";
				}
				else if (stOdvecMik == 1) {
					Pomik pomik;
					najdiEdiniMikrochipZaDol(pomik);
					bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
					if (!veljavnost)
						std::cout << "NAPAKA: PomikMikrochipov - else if - else if 1\n\n";
				}
				else if (stOdvecMik > 1) {
					Pomik pomik;
					najdi2MikZaGor(pomik);
					bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
					if (!veljavnost)
						std::cout << "NAPAKA: PomikMikrochipov - else if - else if 2\n\n";
				}
			}
			else {
				Pomik pomik;
				najdiVseZaGor(pomik);
				bool veljavnost = poskusiPomik(pomik, nadstropja, dvigalo);
				if (!veljavnost)
					std::cout << "NAPAKA: PomikMikrochipov - else\n\n";
			}

			resitev++;
		}

		return -1;
	}

	int simuliraj() {
		
		int resitev = 0;

		int resGen = PomikGeneratorjev();
		if (resGen == -1) return -1;
		resitev += resGen;

		int resMik = PomikMikrochipov();
		if (resMik == -1) return -1;
		resitev += resMik;

		return resitev;
	}


	int minMeja() {

		int resitev = 0;

		for (int i = 0; i < nadstropja.size(); i++)
			for (int j = 0; j < nadstropja[i].size(); j++)
				resitev += (3 - i) * 2;

		resitev -= 3 * 3;

		return resitev;
	}


	void print(std::vector<std::vector<Naprava>> nad = {}) const {
		if (nad.empty()) nad = nadstropja;
		for (int i = nad.size() - 1; i >= 0; i--) {
			if (i == dvigalo) std::cout << i << ":x";
			else std::cout << i << ": ";
			for (const Naprava& naprava : nad[i]) {
				std::cout << '(' << naprava.naziv << ' ' << (int)naprava.vrsta << ") ";
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}


	friend int main();
};


Tovarna preberiPodatke(const std::string& pot) {

	Tovarna resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::vector<std::pair<std::string, int>> seznamMikrochipov, seznamGeneratorjev;
	std::string str1, str2;
	std::string vrstica;

	for (int i = 0; i < 4; i++) {

		std::getline(podatki, vrstica);

		std::stringstream ss(vrstica);

		while (ss.peek() != EOF) {

			str1 = str2;

			ss >> str2;
			if (str2.back() == ',' || str2.back() == '.')
				str2.pop_back();

			if (str2 == "microchip") {
				std::string mikrochip;
				for (const char& c : str1) {
					if (c == '-') break;
					mikrochip.push_back(c);
				}
				seznamMikrochipov.push_back({ mikrochip,i });
			}
			else if (str2 == "generator") {
				seznamGeneratorjev.push_back({ str1,i });
			}
		}
	}

	podatki.close();

	resitev.dodajGeneratorje(seznamGeneratorjev);
	resitev.dodajMikrochipe(seznamMikrochipov);
	//resitev.print();

	//std::cout << "Generatorji:\n";
	//for (const std::pair<std::string, int>& par : seznamGeneratorjev) {
	//	std::cout << par.second << " | " << par.first << '\n';
	//} std::cout << '\n';
	//
	//std::cout << "Mikrochipi:\n";
	//for (const std::pair<std::string, int>& par : seznamMikrochipov) {
	//	std::cout << par.second << " | " << par.first << '\n';
	//} std::cout << '\n';

	return resitev;
}


int main() {

	Tovarna tovarna = preberiPodatke("2016/11.txt");

	int resitev12 = tovarna.minMeja();
	std::cout << "Opravimo po " << resitev12 << " korakih (bliznica).\n";


	tovarna.dodaj2Del();

	int resitev2 = tovarna.minMeja();
	std::cout << "Opravimo po " << resitev2 << " korakih (bliznica).\n";


	return 0;
}
