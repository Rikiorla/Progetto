#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "boids.hpp"

#include "doctest.h"

TEST_CASE("testing the operating functions") {
  SUBCASE("testing calculate_distance") {
    using bd::Boid;
    using bd::Coord;

    Boid b1{Coord{0., 0.}, Coord{2., 0.5}};
    Boid b2{Coord{3., 4.}, Coord{1., 0.02}};
    auto d = bd::calculate_distance(b1, b2);
    CHECK(d == doctest::Approx(5));
  }
  SUBCASE("testing near_boids") {
    using bd::Boid;
    using bd::Coord;

    std::vector<Boid> flock{Boid(Coord{0, 0}, Coord{1, 0}),
                            Boid(Coord{1, 1}, Coord{1, 0}),
                            Boid(Coord{5, 5}, Coord{0, 1})};
    Boid b(Coord{0, 0}, Coord{1, 0});

    auto near = bd::near_boids(flock, b, 2.0);
    CHECK(near.size() == 1);
    CHECK(near[0].getPosition().x == doctest::Approx(1));
    CHECK(near[0].getPosition().y == doctest::Approx(1));
  }
}
TEST_CASE("Test new_boids with multiple boids and wrapping") {
  using bd::Boid;
  using bd::Coord;

  double s = 0.1;
  double c = 0.1;
  double d = 0.8;
  double ds = 0.1;
  double a = 0.1;
  double width = 100.0;
  double height = 100.0;
  double MAX_SPEEDX = 3.;
  double MAX_SPEEDY = 3.;

  std::vector<Boid> flock{
      Boid({10.0, 50.0}, {1.0, 0.0}), Boid({20.0, 50.0}, {1.0, 0.5}),
      Boid({30.0, 50.0}, {1.0, -0.5}),
      Boid({99.9, 50.0}, {5.0, 0.0})  // vicino al bordo
  };

  auto updated = bd::new_boids(flock, s, c, d, ds, a, width, height, MAX_SPEEDX,
                               MAX_SPEEDY);

  REQUIRE(updated.size() == flock.size());

  for (std::size_t i = 0; i < updated.size(); ++i) {
    Coord new_pos = updated[i].getPosition();
    Coord new_vel = updated[i].getVelocity();

    // Controllo wrapping
    CHECK(new_pos.x <= width);
    CHECK(new_pos.x >= 0.0);
    CHECK(new_pos.y <= height);
    CHECK(new_pos.y >= 0.0);

    // Velocità limitata
    CHECK(new_vel.x <= 3.0);
    CHECK(new_vel.y <= 3.0);

    // Se vuoi anche una velocità non troppo negativa
    CHECK(new_vel.x >= -3.0);
    CHECK(new_vel.y >= -3.0);
  }

  // Controlla che la distanza minima tra boids sia almeno un certo valore
  // (0.1)
  for (std::size_t i = 0; i < updated.size(); ++i) {
    for (std::size_t j = i + 1; j < updated.size(); ++j) {
      double dist = bd::calculate_distance(updated[i], updated[j]);
      CHECK(dist >= 0.1);
    }
  }
}

TEST_CASE("testing separation") {
  using bd::Boid;
  using bd::Coord;

  std::vector<Boid> flock{Boid(Coord{0, 0}, Coord{0, 0}),
                          Boid(Coord{0.5, 0}, Coord{0, 0}),
                          Boid(Coord{5, 5}, Coord{0, 0})};
  Boid b(Coord{0, 0}, Coord{0, 0});

  Coord sep = bd::separation(flock, b, 0.1, 1.0);
  // separation vector should point away from neighbors close to b
  CHECK(sep.x < 0);
  CHECK(sep.y == doctest::Approx(0.0));
}

TEST_CASE("testing allignment") {
  using bd::Boid;
  using bd::Coord;

  std::vector<Boid> flock{Boid(Coord{0, 0}, Coord{1, 1}),
                          Boid(Coord{1, 1}, Coord{2, 2}),
                          Boid(Coord{2, 2}, Coord{3, 3})};
  Boid b(Coord{0, 0}, Coord{1, 1});

  Coord align = bd::allignment(flock, b, 0.5);
  CHECK(align.x > 0);
  CHECK(align.y > 0);
}

TEST_CASE("testing cohesion") {
  using bd::Boid;
  using bd::Coord;

  std::vector<Boid> flock{Boid(Coord{0, 0}, Coord{0, 0}),
                          Boid(Coord{2, 0}, Coord{0, 0}),
                          Boid(Coord{4, 0}, Coord{0, 0})};
  Boid b(Coord{0, 0}, Coord{0, 0});

  Coord coh = bd::cohesion(flock, b, 0.5, 3.0);
  CHECK(coh.x > 0);
  CHECK(coh.y == doctest::Approx(0.0));
}
TEST_CASE("testing mean velocities and positions") {
  using bd::Boid;
  using bd::Coord;
  std::vector<Boid> flock{Boid{Coord{0, 0}, Coord{3, 4}},
                          Boid{Coord{3, 4}, Coord{12, 5}}};
  double d = bd::meandistance(flock);
  double v = bd::meanvelocity(flock);
  CHECK(d == doctest::Approx(5));
  CHECK(v == doctest::Approx(9));                    
}