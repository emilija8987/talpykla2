#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>
#include "studentas.h"
#include "pagalbine.h"

using std::string;
using std::vector;

Studentas::Studentas(std::istream& is) {
    readStudent(is);
}

double Studentas::galBalas(double (*mediana_func)(std::vector<int>)) const {
    double suma = 0;
    for (int pazymys : nd_) {
        suma += pazymys;
    }
    int k = nd_.size();
    double vid = (k > 0) ? suma / k : 0.0;
    double med = mediana_func(nd_);

    return 0.4 * med + 0.6 * egzaminas_;
}

std::istream& Studentas::readStudent(std::istream& is) {
    string v, p;
    if (is >> v >> p) {
        vardas_ = v;
        pavarde_ = p;

        vector<int> skaiciai;
        int paz;
        while (is >> paz) {
            skaiciai.push_back(paz);
        }

        if (!skaiciai.empty()) {
            egzaminas_ = skaiciai.back();
            skaiciai.pop_back();
            nd_ = skaiciai;

            double suma = 0;
            for (int pazymys : nd_) {
                suma += pazymys;
            }

            int k = nd_.size();
            double vid = (k > 0) ? suma / k : 0.0;
            double med = mediana(nd_);

            galutinis_vid_ = 0.4 * vid + 0.6 * egzaminas_;
            galutinis_med_ = 0.4 * med + 0.6 * egzaminas_;
        }
    }
    return is;
}

void generuotiStudentuFaila(const string &failoPavadinimas, int kiekis, int nd_kiek)
{
    if (failasEgzistuoja(failoPavadinimas))
    {
        std::cout << "Failas '"<<failoPavadinimas<<"' jau yra. Generavimas praleidziamas.\n";
        return;
    }

    std::cout << "Generuojamas failas '"<<failoPavadinimas<<"' ("<<kiekis<<" irasu)... ";

    std::ofstream f(failoPavadinimas);
    if (!f.is_open())
    {
        std::cout<<"\nKlaida: nepavyko sukurti failo "<<failoPavadinimas<<"\n";
        return;
    }

    //vector<char> buffer(1024 * 1024);
    //f.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    f<<std::left<<std::setw(15)<<"Vardas"<<std::setw(15)<<"Pavarde";
    for (int i=1; i<=nd_kiek; i++)
    {
        f<<std::setw(8)<<("ND" + std::to_string(i));
    }
    f<<std::setw(10)<<"Egz."<<"\n";

    for (int i=1; i<=kiekis; i++)
    {
        f << std::left << std::setw(15) << ("Vardas" + std::to_string(i))
          << std::setw(15) << ("Pavarde" + std::to_string(i));

        for (int j=0; j< nd_kiek; j++)
        {
            f<<std::setw(8)<<dist(gen);
        }
        f<<std::setw(10)<<dist(gen)<<"\n";
    }

    f.close();
    std::cout<<"Atlikta\n\n";
}

void printas(const Studentas &laik, std::ofstream &R)
{
    R<<std::left<<std::setw(15)<<laik.vardas()
     <<std::left<<std::setw(15)<<laik.pavarde()
     <<std::left<<std::setw(18)<<std::fixed<<std::setprecision(2)<<laik.galutinis_vid()
     <<std::left<<std::setw(18)<<std::fixed<<std::setprecision(2)<<laik.galutinis_med()<<"\n";
}
