#include <SFML/Graphics.hpp>
#include <cassert>
#include <iomanip>  // Per setprecision
#include <iostream>
#include <random>
#include <sstream>  // Per stringstream

#include "boids.hpp"

const float WIDTH = 800;
const float HEIGHT = 600;
const float MAX_SPEEDX = 250.;
const float MAX_SPEEDY = 250.;

int main() {
  try {
    std::cout
        << "Hi, please insert the number of boids you want in your flock \n";

    int Num_boids;
    std::cin >> Num_boids;
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
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
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
    std::cout << "critical distance, must be less than operating distance \n";
    std::cin >> cr_dist;
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
    std::cout << "repulsion parameter, it should be in between (0,1] \n";
    std::cin >> sep;
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
    if (sep <= 0 || sep > 1) {
      throw std::runtime_error{"Separation parameter out of range "};
    }
    std::cout << "allignement parameter, it should be in between (0,1]  \n";
    std::cin >> all;
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
    if (all <= 0 || all > 1) {
      throw std::runtime_error{"Allignement parameter out of range "};
    }
    std::cout << "cohesion parameter, it should be in between (0,1] \n";
    std::cin >> coe;
    if (std::cin.fail()) {
      throw std::runtime_error{"Input error: invalid character"};
    }
    if (coe <= 0 || coe > 1) {
      throw std::runtime_error{"Cohesion parameter out of range "};
    }

    if (dist < cr_dist) {
      throw std::runtime_error{
          "critical distance must be stricly less than operating distance"};
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids SFML");
    window.setFramerateLimit(60);

    // Caricamento font
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
      throw std::runtime_error{"Errore nel caricamento del font\n"};
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
    std::uniform_real_distribution<double> dist_vel_x_1{-200., -100.};
    std::uniform_real_distribution<double> dist_vel_x_2{100., 200.};
    std::uniform_real_distribution<double> dist_vel_y_1{-200., -100.};
    std::uniform_real_distribution<double> dist_vel_y_2{100., 200.};
    std::uniform_int_distribution<int> choose_interval(0, 1);



    for (int i{0}; i != Num_boids; ++i) {
      bd::Coord pos{dist_pos_x(eng), dist_pos_y(eng)};
      double vx =
          (choose_interval(eng) == 0) ? dist_vel_x_1(eng) : dist_vel_x_2(eng);
      double vy =
          (choose_interval(eng) == 0) ? dist_vel_y_1(eng) : dist_vel_y_2(eng);
      bd::Coord vel{vx, vy};
      flock.emplace_back(pos, vel);
    }

    sf::CircleShape shape(3);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(3, 3);

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
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

      // Disegna testo
      window.draw(infoText);

      window.display();

      flock = std::move(new_b);
    }
    return 0;
  } catch (std::exception const& e) {
    std::cerr << "Caught exeption: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}