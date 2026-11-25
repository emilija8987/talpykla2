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

studentas::studentas(const studentas& other)
    : vardas_(other.vardas_),
      pavarde_(other.pavarde_),
      nd_(other.nd_),
      egz_(other.egz_),
      galutinis_vid_(other.galutinis_vid_),
      galutinis_med_(other.galutinis_med_) {}

studentas& studentas::operator=(const studentas& other) {
    if (this == &other) return *this;

    vardas_ = other.vardas_;
    pavarde_ = other.pavarde_;
    nd_ = other.nd_;
    egz_ = other.egz_;
    galutinis_vid_ = other.galutinis_vid_;
    galutinis_med_ = other.galutinis_med_;
    return *this;
}

studentas::~studentas() {
    nd_.clear();
    nd_.shrink_to_fit();
}

double studentas::galBalas(double (*f)(std::vector<int>)) const {
    if (nd_.empty()) return 0.6 * egz_;

    double suma = 0;
    for (int p : nd_) {
        suma += p;
    }
    double vid = suma / nd_.size();
    return 0.4 * vid + 0.6 * egz_;
}

std::istream& studentas::readStudent(std::istream& is) {
    is >> vardas_ >> pavarde_;
    return is;
}

std::istream& operator>>(std::istream& is, studentas& s) {
    s.nd_.clear();
    is >> s.vardas_ >> s.pavarde_;

    int paz;
    while (is >> paz) {
        s.nd_.push_back(paz);
    }

    if (!s.nd_.empty()) {
        s.egz_ = s.nd_.back();
        s.nd_.pop_back();
    } else {
        s.egz_ = 0;
    }

    double suma = 0;
    for (int p : s.nd_) suma += p;
    double vid = s.nd_.empty() ? 0.0 : suma / s.nd_.size();
    double med = mediana(s.nd_);

    s.galutinis_vid_ = 0.4 * vid + 0.6 * s.egz_;
    s.galutinis_med_ = 0.4 * med + 0.6 * s.egz_;
    return is;
}

std::ostream& operator<<(std::ostream& os, const studentas& s) {
    os << std::left << std::setw(15) << s.vardas_
       << std::left << std::setw(15) << s.pavarde_
       << std::left << std::setw(18) << std::fixed << std::setprecision(2) << s.galutinis_vid_
       << std::left << std::setw(18) << std::fixed << std::setprecision(2) << s.galutinis_med_;
    return os;
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
    R<<laik<<"\n";
}
