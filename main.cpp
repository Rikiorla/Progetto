#include <iostream>
#include <random>

#include "boids.hpp"

int main() {
  std::vector<bd::Boid> flock{};
  std::random_device r;
  std::default_random_engine eng{r()};
  std::uniform_real_distribution<double> dist_pos_x{0., 10.};
  std::uniform_real_distribution<double> dist_pos_y{0., 10.};
  std::uniform_real_distribution<double> dist_vel_x{0., 10.};
  std::uniform_real_distribution<double> dist_vel_y{0., 10.};

  int n;
  std::cin >> n;
  for (int i{0}; i != n; ++i) {
    bd::Coord pos{dist_pos_x(eng), dist_pos_y(eng)};
    bd::Coord vel{dist_vel_x(eng), dist_vel_y(eng)};
    bd::Boid pidgeon{pos, vel};
    flock.push_back(pidgeon);
  };

  double distance;
  double d;
  double ds;
  std::cin >> ds;
  std::cin >> d;

  // copia di flock (const)
  // calcola distanza
  // varie funzioni
  // elimina copia di flock
  // estrapola info da flock'

}