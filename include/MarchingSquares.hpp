#ifndef _MARCHINGSQUARES_H_
#define _MARCHINGSQUARES_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <iostream>

#include "Arr2d.hpp"
#include "blob.hpp"
#include "random_number.hpp"
#include "constants.hpp"
#include "imgui.h"
#include "imgui-SFML.h"


class MarchingSquares {
private:
	int width, height;
	sf::ContextSettings settings;
  std::shared_ptr<sf::RenderWindow> window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Clock clock;
  float deltaTime;
	bool paused;

  Arr2d<float> field;
  std::vector<sf::VertexArray> polygons;
  sf::Image image;
  sf::Texture texture;
	std::vector<blob> blobs;

	
	void reset_field();
  void set_field_val(unsigned int, unsigned int);
  float get_field_val(float, float);
  void set_pixel_val(unsigned int, unsigned int);
  void fill_polygon(unsigned int, unsigned int);
  void outline_polygon(unsigned int, unsigned int);
	void calculate_field();

	blob random_blob();

  void imgui_setup();

  void update_blobs();

	void init_variables();
	void init_window();
	void poll_events();

public:
	MarchingSquares();

	inline bool running() { return this->window->isOpen(); }
	void update();
	void render();
  void shutdown();
};

#endif
