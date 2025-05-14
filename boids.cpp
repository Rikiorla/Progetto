#include "boids.hpp"

#include <cmath>
#include <vector>

namespace bd {

Position Position::operator+=(Position& p) {};
Position operator+(Position const& p1, Position const& p2) {
  double a = p1.xp + p2.xp;
  double b = p1.yp + p2.yp;
  return Position{a, b};
};
Position operator-(Position const& p1, Position const& p2) {
  double a = p1.xp - p2.xp;
  double b = p1.yp - p2.yp;
  return Position{a, b};
};
Velocity operator+(Velocity const& v1, Velocity const& v2) {
  double a = v1.xv + v2.xv;
  double b = v1.yv + v2.yv;
  return Velocity{a, b};
};
Position operator*(Velocity const& v, double t) {
  double a = v.xv * t;
  double b = v.yv * t;
  return Position{a, b};
};
bool operator>(Velocity const& v1, Velocity const& v2) {
  double mv1 = sqrt(v1.xv * v1.xv + v1.yv * v1.yv);
  double mv2 = sqrt(v2.xv * v2.xv + v2.yv * v2.yv);
  return mv1 > mv2;
};

// swarm è la copia di flock
double calculate_distance(Boid seagull, Boid parrot) {
  Position p_seagull = seagull.getPosition();
  Position p_parrot = parrot.getPosition();

  double distance =
      sqrt((p_seagull.xp - p_parrot.xp) * (p_seagull.xp - p_parrot.xp) +
           (p_seagull.yp - p_parrot.yp) * (p_seagull.yp - p_parrot.yp));
  return distance;
}

std::vector<Boid> near_boids(std::vector<Boid> swarm, Boid crow, double ds) {
  std::vector<Boid> near_boids{};
  int n = swarm.size();
  Position p_crow = crow.getPosition();
  for (int i{}; i < n; ++i) {
    Position p_swarm = swarm[i].getPosition();
    double t = calculate_distance(crow, swarm[i]);
    if (t < ds && t != 0) {
      near_boids.push_back(swarm[i]);
    }
  }
  return near_boids;
}

Velocity separation(std::vector<Boid>, Boid, double s) {};

Velocity allignment(std::vector<Boid>, Boid){};
Velocity cohesion(std::vector<Boid>, Boid){};

}  // namespace bd