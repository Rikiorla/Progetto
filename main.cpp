#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <random>
#include <sstream>      // Per stringstream
#include <iomanip>      // Per setprecision

#include "boids.hpp"

const float WIDTH = 800;
const float HEIGHT = 600;
const float MAX_SPEEDX = 3.;
const float MAX_SPEEDY = 3.;

int main() {
  std::cout << "Hi, please insert the number of boids you want in your flock \n";

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

  // Caricamento font
  sf::Font font;
  if (!font.loadFromFile("OpenSans-Regular.ttf")) {
    std::cerr << "Errore nel caricamento del font\n";
    return -1;
  }

  sf::Text infoText;
  infoText.setFont(font);
  infoText.setCharacterSize(16);
  infoText.setFillColor(sf::Color::White);
  infoText.setStyle(sf::Text::Regular);
  infoText.setPosition(WIDTH - 250.f, 10.f);

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
      if (event.type == sf::Event::Closed)
        window.close();
    }

    std::vector<bd::Boid> new_b =
        bd::new_boids(flock, sep, coe, dist, cr_dist, all, WIDTH, HEIGHT,
                      MAX_SPEEDX, MAX_SPEEDY);

    window.clear(sf::Color::Black);

    // Disegna boids
    for (auto& b : new_b) {
      bd::Coord p = b.getPosition();
      sf::Vector2f pos(static_cast<float>(p.x), static_cast<float>(p.y));
      shape.setPosition(pos);
      window.draw(shape);
    }

    // Calcola i valori da mostrare
    double meanDist = bd::meandistance(new_b);
    double stdDist = bd::dev_stddistance(new_b);
    double meanVel = bd::meanvelocity(new_b);
    double stdVel = bd::dev_stdvelocity(new_b);

    // Formatta i valori con 2 cifre decimali
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Mean Distance: " << meanDist << "\n";
    ss << "Std Dev Dist: " << stdDist << "\n";
    ss << "Mean Velocity: " << meanVel << "\n";
    ss << "Std Dev Vel: " << stdVel;

    infoText.setString(ss.str());

    // Disegna testo (dopo i boids)
    window.draw(infoText);

    window.display();

    flock = std::move(new_b);
  }
  return 0;
}