
#ifndef BD_BOIDS_HPP
#define BD_BOIDS_HPP
#include <vector>

namespace bd {
struct Coord {
  double x;
  double y;

  Coord& operator+=(const Coord& p);
};

Coord operator+(Coord const& p1, Coord const& p2);
Coord operator*(Coord const& v, double t);
bool operator>(Coord const& v1, Coord const& v2);
bool operator==(Coord const& c1, Coord const& c2);

class Boid {
  Coord pos_;
  Coord vel_;

 public:
  Boid(Coord p, Coord v) : pos_{p}, vel_{v} {}

  Coord getPosition() const { return pos_; }
  Coord getVelocity() const { return vel_; }
};
bool operator==(const Boid& lhs, const Boid& rhs);
std::vector<Boid> near_boids(const std::vector<Boid>& flock, const Boid& b,
                             double ds);
std::vector<Boid> new_boids(const std::vector<Boid>& flock, double s, double c,
                            double d, double ds, double a, double width,
                            double height, double maxspeedx, double maxspeedy);

double calculate_distance(const Boid& s, const Boid& p);

Coord separation(const std::vector<Boid>&, const Boid& b, double s, double ds);
Coord allignment(const std::vector<Boid>&, const Boid& b, double a);
Coord cohesion(const std::vector<Boid>& flock, const Boid& b, double c,
               double d);
Coord newVelocity(const std::vector<Boid>& flock, const Boid& b, double s,
                  double c, double d, double ds, double a);
Coord newPosition(const Boid& b, const Coord& v);

}  // namespace bd

#endif