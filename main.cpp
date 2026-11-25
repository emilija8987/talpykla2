#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <list>
#include "studentas.h"
#include "pagalbine.h"

using std::string;
using std::vector;
using std::list;
using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;

int gautiRusiavimoKriteriju()
{
    std::cout << "\nPasirinkite rusiavimo kriteriju:\n";
    std::cout << "1 - Pagal varda\n";
    std::cout << "2 - Pagal pavarde\n";
    std::cout << "3 - Pagal galutini vidurki\n";
    std::cout << "4 - Pagal galutine mediana\n";
    return ivestiSkaiciu("Jusu pasirinkimas (1-4): ", 1, 4);
}

template <typename Container>
void isvestiIEkrana(const Container &grupe)
{
    std::cout << std::left << std::setw(15) << "Vardas"
              << std::left << std::setw(15) << "Pavarde"
              << std::left << std::setw(18) << "Galutinis (vid.)"
              << std::left << std::setw(18) << "Galutinis (med.)\n";
    std::cout << "-------------------------------------------------------------------\n";
    for (const auto &x : grupe)
    {
        std::cout << x << "\n";
    }
}

template <typename Container>
void apdorotiIrIsrasytiGrupe(Container &grupe)
{
    if (grupe.empty())
    {
        std::cout << "Nera duomenu isvedimui.\n";
        return;
    }

    int rusiavimo_kriterijus = gautiRusiavimoKriteriju();
    rusiuotiStudentus(grupe, rusiavimo_kriterijus);

    Container vargsiukai;
    Container kietiakiai;
    dalinti1Strategija(grupe, vargsiukai, kietiakiai);

    std::cout << "\n--- Vargsiukai ---\n";
    isvestiIEkrana(vargsiukai);
    std::cout << "\n--- Kietiakiai ---\n";
    isvestiIEkrana(kietiakiai);

    irasytiIFaila("vargsiukai.txt", vargsiukai);
    irasytiIFaila("kietiakiai.txt", kietiakiai);
    std::cout << "Duomenys irasyti i 'vargsiukai.txt' ir 'kietiakiai.txt'.\n";
}

template <typename Container>
void atliktiAnalize(int strategijos_nr, const string &failo_pavadinimas, int kiekis, int rusiavimo_kriterijus, string konteinerio_pavadinimas)
{
    if (!failasEgzistuoja(failo_pavadinimas))
    {
        generuotiStudentuFaila(failo_pavadinimas, kiekis);
    }

    int testu_kiekis = 3;
    double suma_skaitymas = 0;
    double suma_rusiavimas = 0;
    double suma_dalijimas = 0;
    double suma_isvedimas = 0;
    double suma_bendras = 0;

    for (int i = 0; i < testu_kiekis; i++)
    {
        Container grupe;

        auto t1 = Clock::now();
        skaitytiFaila(grupe, failo_pavadinimas, kiekis);
        auto t2 = Clock::now();
        double t_skaitymas = Duration(t2 - t1).count();

        auto t3 = Clock::now();
        rusiuotiStudentus(grupe, rusiavimo_kriterijus);
        auto t4 = Clock::now();
        double t_rusiavimas = Duration(t4 - t3).count();

        Container vargsiukai;
        Container kietiakiai;

        auto t5 = Clock::now();
        if (strategijos_nr == 1) {
            dalinti1Strategija(grupe, vargsiukai, kietiakiai);
        } else if (strategijos_nr == 2) {
            dalinti2Strategija(grupe, vargsiukai, kietiakiai);
        } else if (strategijos_nr == 3) {
            dalinti3Strategija(grupe, vargsiukai, kietiakiai);
        }
        auto t6 = Clock::now();
        double t_dalijimas = Duration(t6 - t5).count();

        auto t7 = Clock::now();
        irasytiIFaila("vargsiukai.txt", vargsiukai);
        irasytiIFaila("kietiakiai.txt", kietiakiai);
        auto t8 = Clock::now();
        double t_isvedimas = Duration(t8 - t7).count();

        suma_skaitymas += t_skaitymas;
        suma_rusiavimas += t_rusiavimas;
        suma_dalijimas += t_dalijimas;
        suma_isvedimas += t_isvedimas;
        suma_bendras += (t_skaitymas + t_rusiavimas + t_dalijimas + t_isvedimas);
    }

    std::cout << std::left
              << std::setw(13) << konteinerio_pavadinimas << " | "
              << std::setw(23) << failo_pavadinimas << " | "
              << std::setw(10)  << kiekis << " | "
              << std::fixed << std::setprecision(4)
              << std::setw(11) << suma_skaitymas / testu_kiekis << "s | "
              << std::setw(11) << suma_rusiavimas / testu_kiekis << "s | "
              << std::setw(11) << suma_dalijimas / testu_kiekis << "s | "
              << std::setw(11) << suma_isvedimas / testu_kiekis << "s | "
              << std::setw(11) << suma_bendras / testu_kiekis << "s\n";
}

void demonstruotiKlasesVeikima()
{
    std::cout << "\n=== ABSTRAKCIOS KLASES 'Zmogus' IR ISVESTINES 'studentas' DEMONSTRACIJA ===\n";
    std::cout << "Patikra (compile-time): std::is_abstract<Zmogus>::value == "
              << std::boolalpha << std::is_abstract<Zmogus>::value << "\n";
    std::cout << "Tiesioginis objektas 'Zmogus z;' negalimas (kompiliatorius atmeta, nes yra grynai virtualiu metodu).\n";

    std::istringstream eilute("Vardas Pavarde 8 9 10 7 9");
    studentas s1;
    eilute >> s1;
    std::cout << "\n1. Nuskaitytas per >> ir isvestas per << (s1):\n" << s1 << "\n";

    Zmogus* z_ptr = &s1;
    std::cout << "\n1b. Polimorfinis isvedimas per abstrakcios klases rodykle Zmogus*:\n";
    z_ptr->spausdintiInfo(std::cout);
    std::cout << "\n";

    studentas s2(s1);
    std::cout << "\n2. Sukurtas per kopijavimo konstruktoriu s2(s1):\n" << s2 << "\n";

    s2.setPavarde("Pakeistas");
    s2.pridetiNd(10);
    std::cout << "Po s2 pakeitimu:\n";
    std::cout << "  s1 (originalas): " << s1 << "\n";
    std::cout << "  s2 (kopija):     " << s2 << "\n";

    studentas s3;
    s3 = s1;
    std::cout << "\n3. Priskirta per operator= (s3 = s1):\n" << s3 << "\n";

    s3 = s3;
    std::cout << "Po saves priskyrimo (s3 = s3):\n" << s3 << "\n";

    std::cout << "\n4. Lokalus blokas (objektas sunaikinamas isejus is bloko):\n";
    {
        studentas laikinas(s1);
        laikinas.setVardas("Laikinas");
        std::cout << "  Bloko viduje: " << laikinas << "\n";
    }
    std::cout << "  Po lokalaus bloko.\n";

    std::cout << "\n=== DEMONSTRACIJOS PABAIGA ===\n\n";
}

template <typename Container>
void vykdytiRankiniIvedima(string konteinerio_pavadinimas)
{
    Container grupe;
    int n = ivestiSkaiciu("Iveskite studentu kieki: ", 1, 100000);

    for (int j = 0; j < n; j++)
    {
        studentas laik;

        std::cout << "\nPasirinkite kaip ivesti pazymius:\n";
        std::cout << "1 - Ivesti studenta (varda, pavarde, pazymius ir egzamina) per >> operatoriu\n";
        std::cout << "2 - Ivesti pazymius ir egzamina atsitiktinai\n";
        int pasirinkimas = ivestiSkaiciu("Jusu pasirinkimas (1 arba 2): ", 1, 2);

        if (pasirinkimas == 1)
        {
            std::cout << "Iveskite viena eilute: Vardas Pavarde pazymiai... Egzaminas (ir paspauskite Enter):\n";
            std::cin.ignore(1000, '\n');
            while (true)
            {
                std::string eilute;
                std::getline(std::cin, eilute);
                std::istringstream ss(eilute);
                if (ss >> laik && !laik.vardas().empty() && !laik.pavarde().empty() && laik.egz() > 0)
                {
                    break;
                }
                std::cout << "Klaida. Bandykite ivesti dar karta (Vardas Pavarde nd1 ... egz):\n";
            }
        }
        else
        {
            string v, p;
            std::cout << "Iveskite per tarpa studento varda ir pavarde: ";
            std::cin >> v >> p;
            laik.setVardas(v);
            laik.setPavarde(p);

            int nd_kiek = ivestiSkaiciu("Kiek atsitiktiniu pazymiu (1-50)? ", 1, 50);
            std::cout << "Pazymiai: ";
            for (int i = 0; i < nd_kiek; i++)
            {
                int p_paz = atspazymiai(1, 10);
                laik.pridetiNd(p_paz);
                std::cout << p_paz << " ";
            }
            std::cout << "\n";
            int egz_rez = atspazymiai(1, 10);
            laik.setEgz(egz_rez);
            std::cout << "Egzaminas: " << egz_rez << "\n";
        }

        grupe.push_back(laik);
        std::cout << "Ivestas studentas (" << konteinerio_pavadinimas << "): " << grupe.back() << "\n";
        std::cout << "Studento atminties adresas (" << konteinerio_pavadinimas << "): " << &grupe.back() << "\n";
    }
    apdorotiIrIsrasytiGrupe(grupe);
}

int main()
{
    std::cout << "Pasirinkite programos rezima:\n";
    std::cout << "1 - Paleisti AUTOMATINE SPARTOS ANALIZE visiems failams\n";
    std::cout << "2 - Vykdyti iprasta programa (pasirinkti viena faila arba vesti ranka)\n";
    std::cout << "3 - Demonstruoti 'studentas' klases metodus ir operatorius\n";
    int rezimas = ivestiSkaiciu("Jusu pasirinkimas (1-3): ", 1, 3);

    if (rezimas == 1)
    {
        int rusiavimo_kriterijus = gautiRusiavimoKriteriju();

        struct TestData
        {
            string failas;
            int kiekis;
        };

        vector<TestData> testai =
        {
            {"studentai_1000.txt", 1000},
            {"studentai_10000.txt", 10000},
            {"studentai_100000.txt", 100000},
            {"studentai_1000000.txt", 1000000},
           // {"studentai_10000000.txt", 10000000}
        };

        std::cout << "\n========================================= SPARTOS ANALIZE (VIDURKIAI) =======================================================\n";
        std::cout << std::left
                  << std::setw(13) << "Konteineris" << " | "
                  << std::setw(23) << "Failas" << " | "
                  << std::setw(10) << "Kiekis" << " | "
                  << std::setw(12) << "Nuskaitymas" << " | "
                  << std::setw(12) << "Rusiavimas" << " | "
                  << std::setw(12) << "Dalijimas" << " | "
                  << std::setw(12) << "Isvedimas" << " | "
                  << std::setw(12) << "BENDRAS" << "\n";
        std::cout << "=============================================================================================================================\n";

        std::cout << "\n--- 1 STRATEGIJA (std::vector) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::vector<studentas>>(1, t.failas, t.kiekis, rusiavimo_kriterijus, "std::vector");

        std::cout << "\n--- 2 STRATEGIJA (std::vector) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::vector<studentas>>(2, t.failas, t.kiekis, rusiavimo_kriterijus, "std::vector");

        std::cout << "\n--- 3 STRATEGIJA (std::vector) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::vector<studentas>>(3, t.failas, t.kiekis, rusiavimo_kriterijus, "std::vector");

        std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

        std::cout << "\n--- 1 STRATEGIJA (std::list) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::list<studentas>>(1, t.failas, t.kiekis, rusiavimo_kriterijus, "std::list");

        std::cout << "\n--- 2 STRATEGIJA (std::list) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::list<studentas>>(2, t.failas, t.kiekis, rusiavimo_kriterijus, "std::list");

        std::cout << "\n--- 3 STRATEGIJA (std::list) ---\n";
        for (const auto &t : testai) atliktiAnalize<std::list<studentas>>(3, t.failas, t.kiekis, rusiavimo_kriterijus, "std::list");

        std::cout << "========================================================================================================================\n\n";
    }
    else if (rezimas == 2)
    {
        std::vector<studentas> grupe;

        std::cout << "Pasirinkite duomenu ivesties buda:\n";
        std::cout << "1 - Skaityti 'studentai_1000.txt'\n";
        std::cout << "2 - Skaityti 'studentai_10000.txt'\n";
        std::cout << "3 - Skaityti 'studentai_100000.txt'\n";
        std::cout << "4 - Skaityti 'studentai_1000000.txt'\n";
        std::cout << "5 - Skaityti 'studentai_10000000.txt'\n";
        std::cout << "6 - Ivesti kito failo pavadinima\n";
        std::cout << "7 - Ivesti duomenis ranka\n";
        int variantas = ivestiSkaiciu("Jusu pasirinkimas: ", 1, 7);

        string failo_pavadinimas = "";

        int kiekis = 0;

        if (variantas == 1) { failo_pavadinimas = "studentai_1000.txt"; kiekis = 1000; }
        else if (variantas == 2) { failo_pavadinimas = "studentai_10000.txt"; kiekis = 10000; }
        else if (variantas == 3) { failo_pavadinimas = "studentai_100000.txt"; kiekis = 100000; }
        else if (variantas == 4) { failo_pavadinimas = "studentai_1000000.txt"; kiekis = 1000000; }
        else if (variantas == 5) { failo_pavadinimas = "studentai_10000000.txt"; kiekis = 10000000; }

        if (variantas >= 1 && variantas <= 5)
        {
            const string failai[5] = {
                "studentai_1000.txt",
                "studentai_10000.txt",
                "studentai_100000.txt",
                "studentai_1000000.txt",
                "studentai_10000000.txt"
            };
            const int kiekiai[5] = {1000, 10000, 100000, 1000000, 10000000};

            failo_pavadinimas = failai[variantas - 1];
            kiekis = kiekiai[variantas - 1];

            generuotiStudentuFaila(failo_pavadinimas, kiekis);
            if (skaitytiFaila(grupe, failo_pavadinimas, kiekis))
            {
                apdorotiIrIsrasytiGrupe(grupe);
            }
        }

        else if (variantas == 6)
        {
            while (true)
            {
                std::cout << "Iveskite duomenu failo pavadinima: ";
                std::cin >> failo_pavadinimas;

                if (skaitytiFaila(grupe, failo_pavadinimas)) break;
                std::cout << "Bandykite dar karta.\n\n";
            }
            apdorotiIrIsrasytiGrupe(grupe);
        }

        else if (variantas == 7)
        {
            std::cout << "Pasirinkite konteineri rankiniam ivedimui:\n";
            std::cout << "1 - std::vector\n";
            std::cout << "2 - std::list\n";
            int kont_tipas = ivestiSkaiciu("Jusu pasirinkimas (1 arba 2): ", 1, 2);

            if (kont_tipas == 1)
            {
                vykdytiRankiniIvedima<std::vector<studentas>>("vector");
            }
            else
            {
                vykdytiRankiniIvedima<std::list<studentas>>("list");
            }
        }
    }
    else if (rezimas == 3)
    {
        demonstruotiKlasesVeikima();
    }

    return 0;
}

