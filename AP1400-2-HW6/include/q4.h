#ifndef Q4_H
#define Q4_H

#include <numeric>
#include <vector>

class q4 {
public:
  struct Vector2D {
    double x{};
    double y{};
  };

  struct Sensor {
    Vector2D pos;
    double accuracy;
  };

  static Vector2D kalman_filter(std::vector<Sensor> sensors) {
    double weighted_x = std::transform_reduce(
        sensors.begin(), sensors.end(), double{0}, std::plus<>(),
        [](Sensor const &x) { return x.pos.x * x.accuracy; });

    double weighted_y = std::transform_reduce(
        sensors.begin(), sensors.end(), double{0}, std::plus<>(),
        [](Sensor const &x) { return x.pos.y * x.accuracy; });

    double accuracy_sum = std::transform_reduce(
        sensors.begin(), sensors.end(), double{0}, std::plus<>(),
        [](Sensor const &x) { return x.accuracy; });

    return Vector2D{weighted_x / accuracy_sum, weighted_y / accuracy_sum};
  }
};
#endif // Q4_H