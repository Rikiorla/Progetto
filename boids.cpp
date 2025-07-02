
#include "boids.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace bd {

Coord& Coord::operator+=(const Coord& p) {
  x += p.x;
  y += p.y;
  return *this;
}
Coord operator+(Coord const& c1, Coord const& c2) {
  double a = c1.x + c2.x;
  double b = c1.y + c2.y;
  return Coord{a, b};
}
Coord operator-(Coord const& c1, Coord const& c2) {
  double a = c1.x - c2.x;
  double b = c1.y - c2.y;
  return Coord{a, b};
}
Coord operator*(Coord const& c, double t) {
  double a = c.x * t;
  double b = c.y * t;
  return Coord{a, b};
}
bool operator>(Coord const& c1, Coord const& c2) {
  double mc1 = sqrt(c1.x * c1.x + c1.y * c1.y);
  double mc2 = sqrt(c2.x * c2.x + c2.y * c2.y);
  return mc1 > mc2;
}
bool operator==(Coord const& c1, Coord const& c2) {
  return c1.x == c2.x && c1.y == c2.y;
}
bool operator==(const Boid& lhs, const Boid& rhs) {
  return lhs.getPosition() == rhs.getPosition() &&
         lhs.getVelocity() == rhs.getVelocity();
}
double calculate_velocity(const Boid& s) {
  Coord v = s.getVelocity();
  return sqrt((v.x * v.x) + (v.y * v.y));
}
double calculate_distance(const Boid& s, const Boid& p) {
  Coord pos_s = s.getPosition();
  Coord pos_p = p.getPosition();

  return sqrt((pos_s.x - pos_p.x) * (pos_s.x - pos_p.x) +
              (pos_s.y - pos_p.y) * (pos_s.y - pos_p.y));
}

std::vector<Boid> near_boids(const std::vector<Boid>& flock, const Boid& b,
                             double d) {
  std::vector<Boid> near_boids{};
  std::size_t n = flock.size();

  for (std::size_t i{}; i < n; ++i) {
    double t = calculate_distance(b, flock[i]);
    if (t < d && t != 0) {
      near_boids.push_back(flock[i]);
    }
  }
  return near_boids;
}

Coord separation(const std::vector<Boid>& flock, const Boid& b, double s,
                 double ds) {
  const std::vector<Boid> neighbours = near_boids(flock, b, ds);
  std::size_t n = neighbours.size();
  if (n == 0) return Coord{0., 0.};
  const Coord sum_pos = std::accumulate(neighbours.begin(), neighbours.end(),
                                        Coord{0., 0.}, [](Coord pos, Boid h) {
                                          const Coord pos_h = h.getPosition();
                                          pos += pos_h;
                                          return pos;
                                        });
  const Coord pos_b = b.getPosition();
  const Coord total_pos =
      sum_pos -
      pos_b * static_cast<double>(
                  n);  // formula ottenuta tramite calcoli sulla sommatoria
  const Coord v1 = total_pos * (-s);
  return v1;
}

Coord allignment(const std::vector<Boid>& flock, const Boid& b, double a) {
  std::size_t n = flock.size();
  if (n < 2) return Coord{0., 0.};
  const Coord sum_vel = std::accumulate(flock.begin(), flock.end(),
                                        Coord{0., 0.}, [](Coord vel, Boid h) {
                                          const Coord vel_h = h.getVelocity();
                                          vel += vel_h;
                                          return vel;
                                        });
  const Coord vel_b = b.getVelocity();
  const Coord total_vel =
      (sum_vel - vel_b * static_cast<double>(n)) *
      (1. / (static_cast<double>(n) -
             1.));  // formula ottenuta tramite calcoli sulla sommatoria
  const Coord v2 = total_vel * a;
  return v2;
}

Coord cohesion(const std::vector<Boid>& flock, const Boid& b, double c,
               double d) {
  const std::vector<Boid> neighbours = near_boids(flock, b, d);
  std::size_t n = neighbours.size();
  if (n == 0) return Coord{0., 0.};
  const Coord sum_pos = std::accumulate(neighbours.begin(), neighbours.end(),
                                        Coord{0., 0.}, [](Coord pos, Boid h) {
                                          const Coord pos_h = h.getPosition();
                                          pos += pos_h;
                                          return pos;
                                        });
  const Coord c_mass =
      sum_pos *
      (1. / static_cast<double>(
                n));  // n poichè near_boids non contiene il boid stesso
  const Coord pos_b = b.getPosition();
  const Coord v3 = (c_mass - pos_b) * c;
  return v3;
}
Coord predatorseparation(const Boid& b, const Boid& pr, double ds, double s) {
  if (calculate_distance(b, pr) > ds) {
    return Coord{0., 0.};
  } else {
    Coord bpos = b.getPosition();
    Coord prpos = pr.getPosition();

    Coord v4 = (bpos - prpos) * (s);
    return v4;
  }
}
Coord newVelocity(const std::vector<Boid>& flock, const Boid& b, const Boid& pr,
                  double s, double c, double d, double ds, double a) {
  const Coord vel_b = b.getVelocity();
  const Coord v1 = separation(flock, b, s, ds);
  const Coord v2 = allignment(flock, b, a);
  const Coord v3 = cohesion(flock, b, c, d);
  const Coord v4 = predatorseparation(b, pr, ds, s);
  const Coord v = vel_b + v1 + v2 + v3 + (v4 * 5);
  return v;
}

Coord newPosition(const Boid& b, const Coord& v) {
  const Coord pos_b = b.getPosition();
  const Coord newpos = pos_b + (v * (1. / 30));
  return newpos;
}

std::vector<Boid> new_boids(const std::vector<Boid>& flock, const Boid& pr,
                            double s, double c, double d, double ds, double a,
                            double width, double height, double maxspeedx,
                            double maxspeedy) {
  std::vector<Boid> new_boids{};
  std::size_t n = flock.size();
  for (std::size_t i{}; i < n; ++i) {
    Coord v = newVelocity(flock, flock[i], pr, s, c, d, ds, a);

    if (v.x > maxspeedx) {
      v.x = maxspeedx;
    }
    if (v.y > maxspeedy) {
      v.y = maxspeedy;
    }
    if (v.x < -maxspeedx) {
      v.x = -maxspeedx;
    }
    if (v.y < -maxspeedy) {
      v.y = -maxspeedy;
    }
    Coord p = newPosition(flock[i], v);
    if (p.x < 0.) p.x += width;
    if (p.x > width) p.x -= width;
    if (p.y < 0.) p.y += height;
    if (p.y > height) p.y -= height;

    new_boids.emplace_back(p, v);
  }
  return new_boids;
}
// è la funzione che aggiorna il flock con tutti i parametri dati dall'user e
// sistema il comportamento ai bordi
double meanboiddistance(const std::vector<Boid>& flock, const Boid& b) {
  std::size_t count = 0;
  double sum_d = std::accumulate(flock.begin(), flock.end(), 0.0,
                                 [&](double acc, const Boid& h) {
                                   if (!(h == b)) {
                                     ++count;
                                     return acc + calculate_distance(h, b);
                                   }
                                   return acc;
                                 });
  return count > 0 ? sum_d / static_cast<double>(count) : 0.0;
}
double meandistance(const std::vector<Boid>& flock) {
  std::size_t n = flock.size();
  double sum_dist = std::accumulate(flock.begin(), flock.end(), double{0.},
                                    [&](double d, const Boid& h) {
                                      double mbd = meanboiddistance(flock, h);
                                      d += mbd;
                                      return d;
                                    });
  return sum_dist / static_cast<double>(n);
}
double meanboiddistancesquared(const std::vector<Boid>& flock, const Boid& b) {
  double sq_dis = (meanboiddistance(flock, b) * meanboiddistance(flock, b));
  return sq_dis;
}
double dev_stddistance(const std::vector<Boid>& flock) {
  std::size_t n = flock.size();
  double st_dev = std::accumulate(
      flock.begin(), flock.end(), double{0.}, [&](double d, const Boid& h) {
        double mbds = meanboiddistancesquared(flock, h);
        d += mbds;
        return d;
      });
  return sqrt((st_dev / static_cast<double>(n)) -
              (meandistance(flock) * meandistance(flock)));
}
double meanvelocity(const std::vector<Boid>& flock) {
  std::size_t n = flock.size();
  double vel = std::accumulate(flock.begin(), flock.end(), double{0.},
                               [](double v, const Boid& h) {
                                 double vel_h = calculate_velocity(h);
                                 v += vel_h;
                                 return v;
                               });
  return vel / static_cast<double>(n);
}
double dev_stdvelocity(const std::vector<Boid>& flock) {
  std::size_t n = flock.size();
  double st_dev = std::accumulate(flock.begin(), flock.end(), double{0.},
                                  [&](double v, const Boid& h) {
                                    double vel_h = calculate_velocity(h);
                                    v += (vel_h * vel_h);
                                    return v;
                                  });
  return sqrt((st_dev / static_cast<double>(n)) -
              (meanvelocity(flock) * meanvelocity(flock)));
}
Coord newpredatorvelocity(const Boid& pr, const std::vector<Boid> flock,
                          double d, double c) {
  Coord v1 = cohesion(flock, pr, c, d);
  std::size_t n = flock.size();
  Coord vpr = pr.getVelocity();
  if (n == 0) {
    return vpr + v1;
  } else {
    Boid min = *std::min_element(
        flock.begin(), flock.end(), [&](const Boid& h, const Boid& i) {
          return calculate_distance(h, pr) < calculate_distance(i, pr);
        });

    Coord vmin = min.getVelocity();
    Coord v2 = (vpr - vmin) * 2.;
    Coord v = vpr + v1 + v2;
    return v;
  }
}
Boid newpredator(const Boid& pr, const std::vector<Boid> flock, double d,
                 double c, double width, double height, double maxspeedx,
                 double maxspeedy) {
  Coord vp = newpredatorvelocity(pr, flock, d, c);
  if (vp.x > maxspeedx) {
    vp.x = maxspeedx;
  }
  if (vp.y > maxspeedy) {
    vp.y = maxspeedy;
  }
  if (vp.x < -maxspeedx) {
    vp.x = -maxspeedx;
  }
  if (vp.y < -maxspeedy) {
    vp.y = -maxspeedy;
  }
  Coord p = newPosition(pr, vp);
  if (p.x < 0.) p.x += width;
  if (p.x > width) p.x -= width;
  if (p.y < 0.) p.y += height;
  if (p.y > height) p.y -= height;
  Boid new_p = {p, vp};
  return new_p;
}

}  // namespace bd
// namespace bd