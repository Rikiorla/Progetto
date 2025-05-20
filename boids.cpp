#include "boids.hpp"

#include <cmath>
#include <numeric>
#include <vector>

#include "algorithm"

namespace bd {

Coord& Coord::operator+=(const Coord& p) {
  x += p.x;
  y += p.y;
  return *this;
};
Coord operator+(Coord const& c1, Coord const& c2) {
  double a = c1.x + c2.x;
  double b = c1.y + c2.y;
  return Coord{a, b};
};
Coord operator-(Coord const& c1, Coord const& c2) {
  double a = c1.x - c2.x;
  double b = c1.y - c2.y;
  return Coord{a, b};
};
Coord operator*(Coord const& c, double t) {
  double a = c.x * t;
  double b = c.y * t;
  return Coord{a, b};
};
bool operator>(Coord const& c1, Coord const& c2) {
  double mc1 = sqrt(c1.x * c1.x + c1.y * c1.y);
  double mc2 = sqrt(c2.x * c2.x + c2.y * c2.y);
  return mc1 > mc2;
};

double calculate_distance(Boid s, Boid p) {
  Coord pos_s = s.getPosition();
  Coord pos_p = p.getPosition();

  double distance = sqrt((pos_s.x - pos_p.x) * (pos_s.x - pos_p.x) +
                         (pos_s.y - pos_p.y) * (pos_s.y - pos_p.y));
  return distance;
}  // da modificare il comportamento ai bordi

std::vector<Boid> near_boids(std::vector<Boid> flock, Boid b, double d) {
  std::vector<Boid> near_boids{};
  int n = flock.size();
  Coord pos_b = b.getPosition();
  for (int i{}; i < n; ++i) {
    Coord pos_flock = flock[i].getPosition();
    double t = calculate_distance(b, flock[i]);
    if (t < d && t != 0) {
      near_boids.push_back(flock[i]);
    }
  }
  return near_boids;
}

Coord separation(std::vector<Boid> flock, Boid b, double s, double ds) {
  std::vector<Boid> neighbours = near_boids(flock, b, ds);
  int n = neighbours.size();
  Coord sum_pos = std::accumulate(neighbours.begin(), neighbours.end(),
                                  Coord{0., 0.}, [](Coord pos, Boid h) {
                                    Coord pos_h = h.getPosition();
                                    pos += pos_h;
                                  });
  Coord pos_b = b.getPosition();
  Coord total_pos =
      sum_pos - pos_b * n;  // formula ottenuta tramite calcoli sulla sommatoria
  Coord v1 = total_pos * (-s);
  return v1;
};

Coord allignment(std::vector<Boid> flock, Boid b, double a) {
  int n = flock.size();
  Coord sum_vel = std::accumulate(flock.begin(), flock.end(), Coord{0., 0.},
                                  [](Coord vel, Boid h) {
                                    Coord vel_h = h.getVelocity();
                                    vel += vel_h;
                                  });
  Coord vel_b = b.getVelocity();
  Coord total_vel =
      (sum_vel - vel_b * n) *
      (1 / (n - 1));  // formula ottenuta tramite calcoli sulla sommatoria
  Coord v2 = total_vel * a;
  return v2;
};

Coord cohesion(std::vector<Boid> flock, Boid b, double c, double d) {
  std::vector<Boid> neighbours = near_boids(flock, b, d);
  int n = neighbours.size();
  Coord sum_pos = std::accumulate(neighbours.begin(), neighbours.end(),
                                  Coord{0., 0.}, [](Coord pos, Boid h) {
                                    Coord pos_h = h.getPosition();
                                    pos += pos_h;
                                  });
  Coord c_mass =
      sum_pos * (1 / n);  // /n poichè near_boids non contiene il boid stesso
  Coord pos_b = b.getPosition();
  Coord v3 = (c_mass - pos_b) * c;
  return v3;
};

Coord Velocity(std::vector<Boid> flock, Boid b, double s, double c, double d,
               double ds, double a) {
  Coord vel_b = b.getVelocity();
  Coord v1 = separation(flock, b, s, ds);
  Coord v2 = allignment(flock, b, a);
  Coord v3 = cohesion(flock, b, c, d);
  Coord v = vel_b + v1 + v2 + v3;
  return v;
};

double mean_vel(std::vector<Boid> flock) {

};
double mean_pos(std::vector<Boid> flock) {};
}  // namespace bd