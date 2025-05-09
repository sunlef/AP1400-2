#ifndef Q3_H
#define Q3_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

inline std::string trim(std::string const &s) {
  auto start = s.find_first_not_of(" \t\n\r");
  if (start == std::string::npos) {
    return "";
  }

  auto end = s.find_last_not_of(" \t\n\r");
  return s.substr(start, end - start + 1);
}

inline std::vector<std::string> split(std::string const &s, char ch) {
  std::vector<std::string> res;

  std::vector<int> p;
  for (int i = 0; i < int(s.size()); i++) {
    if (s[i] == ch) {
      p.push_back(i);
    }
  }

  int start = 0;
  for (int i = 0; i < int(p.size()); i++) {
    auto x = trim(s.substr(start, p[i] - start));
    if (!x.empty()) {
      res.push_back(x);
    }
    start = p[i] + 1;
  }
  if (start != s.size()) {
    auto x = trim(s.substr(start));
    if (!x.empty()) {
      res.push_back(x);
    }
  }
  return res;
}

inline int parseTimeToMinutes(std::string s) {
  int h = 0, m = 0;
  auto p = s.find('h');
  if (p != std::string::npos) {
    h = std::stoi(s.substr(0, p));
    s = s.substr(p + 1);
  }

  p = s.find('m');
  if (p != std::string::npos) {
    m = std::stoi(s.substr(0, p));
  }

  return 60 * h + m;
}

class q3 {
public:
  struct Flight {
    std::string flight_number;
    size_t duration;
    size_t connections;
    size_t connection_times;
    size_t price;
    size_t weight() const { return duration + connection_times + 3 * price; }
  };

  static auto gather_flights(std::string filename) {
    auto FlightComparator = [](Flight const &lhs, Flight const &rhs) {
      return lhs.weight() > rhs.weight();
    };

    std::priority_queue<Flight, std::vector<Flight>, decltype(FlightComparator)>
        res;

    std::fstream file(filename);

    if (file) {
      std::string line;
      while (std::getline(file, line)) {
        auto parts = split(line, '-');

        Flight x{};

        for_each(parts.begin(), parts.end(), [&](std::string const &part) {
          auto subpart = split(part, ':');
          if (subpart.size() < 2) {
            return;
          }
          if (subpart[0] == "flight_number") {
            x.flight_number = subpart[1];
          } else if (subpart[0] == "duration") {
            x.duration = parseTimeToMinutes(subpart[1]);
          } else if (subpart[0] == "connections") {
            x.connections = std::stoi(subpart[1]);
          } else if (subpart[0] == "connection_times") {
            auto t = split(subpart[1], ',');
            for_each(t.begin(), t.end(), [&](std::string const &ti) {
              x.connection_times += parseTimeToMinutes(ti);
            });
          } else if (subpart[0] == "price") {
            x.price = std::stoi(subpart[1]);
          }
        });

        res.push(x);
      }
    }
    return res;
  }
};
#endif // Q3_H