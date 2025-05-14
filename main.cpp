#include <iostream>
#include <random>

#include "boids.hpp"

int main() {
  std::vector<bd::Boid> flock{};
  std::random_device r;
  std::default_random_engine eng{r()};
  std::uniform_real_distribution<double> dist_xp{0., 10.};
  std::uniform_real_distribution<double> dist_yp{0., 10.};
  std::uniform_real_distribution<double> dist_xv{0., 10.};
  std::uniform_real_distribution<double> dist_yv{0., 10.};

  int n;
  std::cin >> n;
  for (int i{0}; i != n; ++i) {
    bd::Position pos{dist_xp(eng), dist_yp(eng)};
    bd::Velocity vel{dist_xv(eng), dist_yv(eng)};
    bd::Boid pigeon{pos, vel};
    flock.push_back(pigeon);
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

  if (distance < ds) {
    bd::Velocity separation();
  }
  if (distance < d) {
    bd::Velocity allignment();
    bd::Velocity coesion();
  }
}