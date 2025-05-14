#ifndef BD_BOIDS_HPP
#define BD_BOIDS_HPP

namespace bd {
struct Position {
  double xp;
  double yp;

  Position operator+=(Position& p);  // se definita fuori dal tipo, un operatore
                                     // deve avere sempre due parametri
};

Position operator+(Position const& p1, Position const& p2);

struct Velocity {
  double xv;
  double yv;
};

Velocity operator+(Velocity const& v1, Velocity const& v2);
Position operator*(Velocity const& v, double t);
bool operator>(Velocity const& v1, Velocity const& v2);

class Boid {
  Position pos_;
  Velocity vel_;

 public:
  Boid(Position p, Velocity v) : pos_{pos_}, vel_{vel_} {}

  Position getPosition() const { return pos_; }
  Velocity getVelocity() const { return vel_; }

  // funzione che prende la posizione del boid
  // funzione che prende la velocità del boid
  // funzione che calcola la nuova velocità del boid
  // funzione che calcola la nuova posizione del boid in base alla nuova
  // velocità e a deltaT
};

std::vector<Boid> calculate_distance(
    Boid, double d);  // funzione che preso un boid ti restituisce un vettore di
                      // tutti i boid d vicini a lui
// nel main andrà messa in un for che fa scorrere tutti i boid su cui usare
// questa funzione

Velocity separation(std::vector<Boid>, Boid, double s);

Velocity allignment(std::vector<Boid>, Boid);
Velocity cohesion(std::vector<Boid>, Boid);

}  // namespace bd

#endif