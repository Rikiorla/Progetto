
#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <random>

#include "boids.hpp"

const float WIDTH = 800;
const float HEIGHT = 600;
const float MAX_SPEEDX = 3.;
const float MAX_SPEEDY = 3.;
int main() {
  std::cout
      << "Hi, please insert the number of boids you want in your flock \n";

  int Num_boids;

  std::cin >> Num_boids;
  assert(!std::cin.fail());
  if (Num_boids < 2) {
    throw std::runtime_error{"the flock is formed by at least two boids"};
  }
  float dist;
  float cr_dist;
  float sep;
  float all;
  float coe;
  std::cout << "Thank you, now please insert flight parameters: \n"
            << "operating distance \n";
  std::cin >> dist;
  std::cout << "critical distance, must be less than operating distance \n";
  std::cin >> cr_dist;
  std::cout << "repulsion parameter, it should be in between (0,1) \n";
  std::cin >> sep;
  std::cout << "allignement parameter, it should be in between (0,1)  \n";
  std::cin >> all;
  std::cout << "cohesion parameter, it should be in between (0,1) \n";
  std::cin >> coe;

  if (dist < cr_dist) {
    throw std::runtime_error{
        "critical distance must be stricly less than operating distance"};
  }

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids SFML");
  window.setFramerateLimit(60);

  std::vector<bd::Boid> flock{};
  std::random_device r;
  std::default_random_engine eng{r()};
  std::uniform_real_distribution<double> dist_pos_x{0., 800.};
  std::uniform_real_distribution<double> dist_pos_y{0., 600.};
  std::uniform_real_distribution<double> dist_vel_x{-10., 10.};
  std::uniform_real_distribution<double> dist_vel_y{-10., 10.};

  for (int i{0}; i != Num_boids; ++i) {
    bd::Coord pos{dist_pos_x(eng), dist_pos_y(eng)};
    bd::Coord vel{dist_vel_x(eng), dist_vel_y(eng)};
    flock.emplace_back(pos, vel);
  };

  sf::CircleShape shape(3);
  shape.setFillColor(sf::Color::White);
  shape.setOrigin(3, 3);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    std::vector<bd::Boid> new_b =
        bd::new_boids(flock, sep, coe, dist, cr_dist, all, WIDTH, HEIGHT);

    window.clear(sf::Color::Black);

    for (auto& b : new_b) {
      bd::Coord p = b.getPosition();
      sf::Vector2f pos(static_cast<float>(p.x), static_cast<float>(p.y));

      shape.setPosition(pos);
      window.draw(shape);
    }
    
    flock = std::move(new_b);
    window.display();
  }
  return 0;
}