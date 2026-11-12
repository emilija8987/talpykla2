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

class Studentas {
private:
  std::string vardas_;
  std::string pavarde_;
  double egzaminas_;
  std::vector<int> nd_;
  double galutinis_vid_;
  double galutinis_med_;

public:
  Studentas() : egzaminas_(0), galutinis_vid_(0), galutinis_med_(0) { }
  Studentas(std::istream& is);

  inline std::string vardas() const { return vardas_; }
  inline std::string pavarde() const { return pavarde_; }
  inline double egz() const { return egzaminas_; }
  inline std::vector<int> nd() const { return nd_; }
  inline double galutinis_vid() const { return galutinis_vid_; }
  inline double galutinis_med() const { return galutinis_med_; }

  double galBalas(double (*) (std::vector<int>) = mediana) const;
  std::istream& readStudent(std::istream&);
};

bool compare(const Studentas&, const Studentas&);
bool comparePagalPavarde(const Studentas&, const Studentas&);
bool comparePagalEgza(const Studentas&, const Studentas&);

#endif










