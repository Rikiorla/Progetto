#ifndef BD_BOIDS_HPP
#define BD_BOIDS_HPP

namespace bd {

struct Coord {
  double x;
  double y;

  Coord& operator+=(
      const Coord& p);  // se definita fuori dal tipo, un operatore
                        // deve avere sempre due parametri
};

Coord operator+(Coord const& p1, Coord const& p2);
Coord operator*(Coord const& v, double t);
bool operator>(Coord const& v1, Coord const& v2);

class Boid {
  Coord pos_;
  Coord vel_;

 public:
  Boid(Coord p, Coord v) : pos_{pos_}, vel_{vel_} {}

  Coord getPosition() const { return pos_; }
  Coord getVelocity() const { return vel_; }

  // funzione che calcola la nuova velocità del boid
  // funzione che calcola la nuova posizione del boid in base alla nuova
  // velocità e a deltaT
};

std::vector<Boid> near_boids(
    std::vector<Boid> swarm, Boid crow,
    double ds);  // funzione che preso un boid ti restituisce un vettore di
                 // tutti i boid d vicini a lui
// nel main andrà messa in un for che fa scorrere tutti i boid su cui usare
// questa funzione

double calculate_distance(Boid s, Boid p);

Coord separation(std::vector<Boid>, Boid b, double s, double ds);
Coord allignment(std::vector<Boid>, Boid b, double a);
Coord cohesion(std::vector<Boid> flock, Boid b, double c, double d);
Coord Velocity(std::vector<Boid> flock, Boid b, double s, double c, double d,
               double ds, double a);
double mean_vel(std::vector<Boid> flock);
double mean_pos(std::vector<Boid> flock);

}  // namespace bd

#endif