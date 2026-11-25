#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>
#include "studentas.h"
#include "pagalbine.h"

using std::string;
using std::vector;

studentas::studentas(std::istream& is)
    : egz_(0), galutinis_vid_(0.0), galutinis_med_(0.0) {
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

studentas::~studentas() {}

void studentas::setEgz(int e) {
    egz_ = e;
    galutinis_vid_ = galBalas(vidurkis);
    galutinis_med_ = galBalas(mediana);
}

void studentas::pridetiNd(int pazymys) {
    nd_.push_back(pazymys);
    galutinis_vid_ = galBalas(vidurkis);
    galutinis_med_ = galBalas(mediana);
}

void studentas::setNd(const std::vector<int>& n) {
    nd_ = n;
    galutinis_vid_ = galBalas(vidurkis);
    galutinis_med_ = galBalas(mediana);
}

double studentas::galBalas(double (*f)(std::vector<int>)) const {
    if (nd_.empty()) return 0.6 * egz_;
    return 0.4 * f(nd_) + 0.6 * egz_;
}

std::istream& studentas::readStudent(std::istream& is) {
    return operator>>(is, *this);
}

std::istream& operator>>(std::istream& is, studentas& s) {
    s.nd_.clear();
    s.vardas_.clear();
    s.pavarde_.clear();
    s.egz_ = 0;
    s.galutinis_vid_ = 0.0;
    s.galutinis_med_ = 0.0;

    if (!(is >> std::ws >> s.vardas_ >> s.pavarde_)) {
        return is;
    }

    std::string likutis;
    std::getline(is, likutis);
    std::istringstream eilutes_srautas(likutis);

    std::vector<int> skaiciai;
    int paz;
    while (eilutes_srautas >> paz) {
        if (paz < 1 || paz > 10) {
            is.setstate(std::ios::failbit);
            return is;
        }
        skaiciai.push_back(paz);
    }

    if (!eilutes_srautas.eof() && eilutes_srautas.fail()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (!skaiciai.empty()) {
        s.egz_ = skaiciai.back();
        skaiciai.pop_back();
        s.nd_ = std::move(skaiciai);
    } else {
        s.egz_ = 0;
    }

    s.galutinis_vid_ = s.galBalas(vidurkis);
    s.galutinis_med_ = s.galBalas(mediana);
    return is;
}

std::ostream& operator<<(std::ostream& os, const studentas& s) {
    std::ios::fmtflags senos_veliaveles = os.flags();
    std::streamsize senas_tikslumas = os.precision();

    os << std::left << std::setw(15) << s.vardas_
       << std::left << std::setw(15) << s.pavarde_
       << std::left << std::setw(18) << std::fixed << std::setprecision(2) << s.galutinis_vid_
       << std::left << std::setw(18) << std::fixed << std::setprecision(2) << s.galutinis_med_;

    os.flags(senos_veliaveles);
    os.precision(senas_tikslumas);
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
            f<<std::setw(8)<<atspazymiai(1, 10);
        }
        f<<std::setw(10)<<atspazymiai(1, 10)<<"\n";
    }

    f.close();
    std::cout<<"Atlikta\n\n";
}

void printas(const studentas &laik, std::ofstream &R)
{
    R<<laik<<"\n";
}
