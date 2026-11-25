#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <algorithm>
#include "pagalbine.h"

class Zmogus {
protected:
    std::string vardas_;
    std::string pavarde_;

public:
    Zmogus() : vardas_(""), pavarde_("") {}
    Zmogus(const std::string& v, const std::string& p) : vardas_(v), pavarde_(p) {}
    virtual ~Zmogus() = default;

    virtual std::string vardas() const = 0;
    virtual std::string pavarde() const = 0;
    virtual void setVardas(const std::string& v) = 0;
    virtual void setPavarde(const std::string& p) = 0;
    virtual void spausdintiInfo(std::ostream& os) const = 0;
};

class studentas : public Zmogus {
private:
    std::vector<int> nd_;
    int egz_;
    double galutinis_vid_;
    double galutinis_med_;

public:
    studentas() : Zmogus(), egz_(0), galutinis_vid_(0), galutinis_med_(0) {}
    explicit studentas(std::istream& is);

    studentas(const studentas& other);
    studentas& operator=(const studentas& other);
    ~studentas() override;

    inline std::string vardas() const override { return vardas_; }
    inline std::string pavarde() const override { return pavarde_; }
    inline std::vector<int> nd() const { return nd_; }
    inline int egz() const { return egz_; }
    inline double galutinis_vid() const { return galutinis_vid_; }
    inline double galutinis_med() const { return galutinis_med_; }

    void setVardas(const std::string& v) override { vardas_ = v; }
    void setPavarde(const std::string& p) override { pavarde_ = p; }
    void setEgz(int e);
    void pridetiNd(int pazymys);
    void setNd(const std::vector<int>& n);
    void setGalutiniai(double vid, double med) { galutinis_vid_ = vid; galutinis_med_ = med; }

    void spausdintiInfo(std::ostream& os) const override;

    std::istream& readStudent(std::istream& is);
    double galBalas(double (*) (std::vector<int>) = mediana) const;

    friend std::istream& operator>>(std::istream& is, studentas& s);
    friend std::ostream& operator<<(std::ostream& os, const studentas& s);
};


void printas(const studentas &laik, std::ofstream &R);
void generuotiStudentuFaila(const std::string &failoPavadinimas, int kiekis, int nd_kiek = 5);

inline bool palygintiPagalVarda(const studentas &a, const studentas &b) {
    if (a.vardas() != b.vardas()) return a.vardas() < b.vardas();
    return a.pavarde() < b.pavarde();
}

inline bool palygintiPagalPavarde(const studentas &a, const studentas &b) {
    if (a.pavarde() != b.pavarde()) return a.pavarde() < b.pavarde();
    return a.vardas() < b.vardas();
}

inline bool palygintiPagalVidurki(const studentas &a, const studentas &b) {
    if (a.galutinis_vid() != b.galutinis_vid()) {
        return a.galutinis_vid() > b.galutinis_vid();
    }
    if (a.galutinis_med() != b.galutinis_med()) {
        return a.galutinis_med() > b.galutinis_med();
    }
    return a.pavarde() < b.pavarde();
}

inline bool palygintiPagalMediana(const studentas &a, const studentas &b) {
    if (a.galutinis_med() != b.galutinis_med()) {
        return a.galutinis_med() > b.galutinis_med();
    }
    if (a.galutinis_vid() != b.galutinis_vid()) {
        return a.galutinis_vid() > b.galutinis_vid();
    }
    return a.pavarde() < b.pavarde();
}

template <typename C>
typename std::enable_if<std::is_same<C, std::vector<studentas>>::value>::type
pabandykReserve(C &grupe, int kiekis) {
    if (kiekis > 0) grupe.reserve(kiekis);
}

template <typename C>
typename std::enable_if<!std::is_same<C, std::vector<studentas>>::value>::type
pabandykReserve(C &, int) {}

template <typename Container>
bool skaitytiFaila(Container &grupe, const std::string &failas, int kiekis = 0)
{
    std::ifstream f(failas);
    if (!f.is_open())
    {
        std::cout<<"Nepavyko atidaryti failo '"<<failas<<"'\n";
        return false;
    }

    pabandykReserve(grupe, kiekis);

    std::string eilute;
    std::getline(f, eilute);

    while (std::getline(f, eilute))
    {
        if (eilute.empty()) continue;

        std::stringstream ss(eilute);
        studentas laik;
        ss >> laik;

        if (!ss.fail() && !laik.vardas().empty() && !laik.pavarde().empty() && laik.egz() > 0)
        {
            grupe.push_back(laik);
        }
    }
    f.close();
    return true;
}

template <typename Container>
void rusiuotiStudentus(Container &grupe, int kriterijus)
{
    auto comp = [kriterijus](const studentas &a, const studentas &b) {
        if (kriterijus == 1) return palygintiPagalVarda(a, b);
        if (kriterijus == 2) return palygintiPagalPavarde(a, b);
        if (kriterijus == 3) return palygintiPagalVidurki(a, b);
        return palygintiPagalMediana(a, b);
    };

    if constexpr (std::is_same<Container, std::vector<studentas>>::value) {
        std::sort(grupe.begin(), grupe.end(), comp);
    } else {
        grupe.sort(comp);
    }
}

template <typename Container>
void irasytiIFaila(const std::string &failoPavadinimas, const Container &s_grupe)
{
    std::ofstream R(failoPavadinimas);
    if (!R.is_open())
    {
        std::cout << "Klaida: nepavyko sukurti failo " << failoPavadinimas << "\n";
        return;
    }

    R << std::left << std::setw(15) << "Vardas"
      << std::left << std::setw(15) << "Pavarde"
      << std::left << std::setw(18) << "Galutinis (vid.)"
      << std::left << std::setw(18) << "Galutinis (med.)\n";
    R << "-------------------------------------------------------------------\n";

    for (const auto &x : s_grupe) printas(x, R);

    R.close();
}

template <typename Container>
void dalinti1Strategija(const Container &grupe, Container &vargsiukai, Container &kietiakiai)
{
    vargsiukai.clear();
    kietiakiai.clear();

    for (const auto &s : grupe)
    {
        if (s.galutinis_vid() < 5.0)
        {
            vargsiukai.push_back(s);
        }
        else
        {
            kietiakiai.push_back(s);
        }
    }
}

template <typename Container>
void dalinti2Strategija(Container &grupe, Container &vargsiukai, Container &kietiakiai) {
    vargsiukai.clear();
    kietiakiai.clear();

    if constexpr (std::is_same<Container, std::vector<studentas>>::value) {
        for (const auto &s : grupe) {
            if (s.galutinis_vid() < 5.0) {
                vargsiukai.push_back(s);
            }
        }
        grupe.erase(
            std::remove_if(grupe.begin(), grupe.end(), [](const studentas &s) {
                return s.galutinis_vid() < 5.0;
            }),
            grupe.end()
        );
        kietiakiai = std::move(grupe);
    }
    else {
        for (auto it = grupe.begin(); it != grupe.end(); ) {
            if (it->galutinis_vid() < 5.0) {
                vargsiukai.push_back(*it);
                it = grupe.erase(it);
            } else {
                ++it;
            }
        }
        kietiakiai = std::move(grupe);
    }
}

template <typename Container>
void dalinti3Strategija(Container &grupe, Container &vargsiukai, Container &kietiakiai) {
    vargsiukai.clear();
    kietiakiai.clear();

    if constexpr (std::is_same<Container, std::vector<studentas>>::value) {
        auto it = std::stable_partition(grupe.begin(), grupe.end(), [](const studentas &s) {
            return s.galutinis_vid() < 5.0;
        });

        vargsiukai.assign(std::make_move_iterator(grupe.begin()), std::make_move_iterator(it));
        kietiakiai.assign(std::make_move_iterator(it), std::make_move_iterator(grupe.end()));

        grupe.clear();
    }
    else {
        kietiakiai = std::move(grupe);
        vargsiukai = kietiakiai;

        kietiakiai.remove_if([](const studentas &s) { return s.galutinis_vid() < 5.0; });
        vargsiukai.remove_if([](const studentas &s) { return s.galutinis_vid() >= 5.0; });
        grupe.clear();
    }
}
#endif













