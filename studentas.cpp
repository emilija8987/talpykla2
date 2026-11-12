#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>
#include "studentas.h"
#include "pagalbine.h"

using std::string;
using std::vector;

studentas::studentas(std::istream& is) {
    readStudent(is);
}

double studentas::galBalas(double (*f)(vector<int>)) const {
    double suma = 0;
    for (int p : nd_) {
        suma += p;
    }
    int k = nd_.size();
    double vid = (k > 0) ? suma / k : 0.0;
    double med = f(nd_);
    return 0.4 * vid + 0.6 * egz_;
}

std::istream& studentas::readStudent(std::istream& is) {
    is >> vardas_ >> pavarde_;
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

void printas(const studentas &laik, std::ofstream &R)
{
    R<<std::left<<std::setw(15)<<laik.vardas()
     <<std::left<<std::setw(15)<<laik.pavarde()
     <<std::left<<std::setw(18)<<std::fixed<<std::setprecision(2)<<laik.galutinis_vid()
     <<std::left<<std::setw(18)<<std::fixed<<std::setprecision(2)<<laik.galutinis_med()<<"\n";
}
