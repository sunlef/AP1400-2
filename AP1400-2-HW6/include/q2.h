#ifndef Q2_H
#define Q2_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class q2 {
public:
  struct Patient {
    std::string name;
    size_t age;
    size_t smokes;
    size_t area_q;
    size_t alkhol;

    size_t possibility() const {
      return 3 * age + 5 * smokes + 2 * area_q + 4 * alkhol;
    };
  };

  static std::vector<Patient> read_file(std::string filename) {
    std::vector<Patient> res;
    std::ifstream file(filename);
    if (file) {
      std::string line;
      std::getline(file, line);
      std::getline(file, line);
      while (std::getline(file, line)) {
        Patient x;
        std::stringstream ss(line);

        std::string name, surname;
        std::getline(ss, name, ',');
        std::getline(ss, surname, ',');
        while (name.back() == ' ') {
          name.pop_back();
        }
        while (surname.back() == ' ') {
          surname.pop_back();
        }

        x.name = name + " " + surname;
        ss >> x.age;
        ss.ignore();
        ss >> x.smokes;
        ss.ignore();
        ss >> x.area_q;
        ss.ignore();
        ss >> x.alkhol;
        ss.ignore();

        res.push_back(x);
      }
    }
    return res;
  }

  static void sort(std::vector<Patient> &v) {
    std::sort(v.begin(), v.end(), [](Patient const &lhs, Patient const &rhs) {
      return lhs.possibility() > rhs.possibility();
    });
  };
};

#endif // Q2_H