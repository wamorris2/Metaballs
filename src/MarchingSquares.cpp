#include "MarchingSquares.hpp"

static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float crit_val = 1.0f;
static float intensity = 50.0f;
static float speed = 40.0f;
static int cellsize = 10;
static int draw_points = 0;
static int draw_fill = 1;
static int draw_blobs = 0;

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float ratio)
{
	return ratio * (b - a) + a;
}

void MarchingSquares::imgui_setup() {
  ImGui::Begin("Options", 0, ImGuiWindowFlags_NoResize);
  ImGui::SliderFloat("Critical Value", &crit_val, 0.0f, 5.0f);
  ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1000.0f);
  ImGui::SliderFloat("Blob Speed", &speed, 0.0f, 500.0f);
  if (ImGui::SliderInt("Cellsize", &cellsize, 1, 50)) {
    this->reset_field();
  }
  if (ImGui::Button("Draw Points")) {
    draw_points = !draw_points;
  }
  ImGui::SameLine();
  if (ImGui::Button(draw_fill ? "Draw Lines" : "Draw Fill")) {
    draw_fill = !draw_fill;
  }
  ImGui::SameLine();
  if (ImGui::Button("Draw Blobs")) {
    draw_blobs = !draw_blobs;
  }
  ImGui::ColorEdit3("Blob Color", color);
  ImGui::End();
}

void MarchingSquares::init_variables()
{
	// this->width = 1002;
	// this->height = 902;
  this->width = sf::VideoMode::getDesktopMode().width;
  this->height = sf::VideoMode::getDesktopMode().height;
	this->videoMode = sf::VideoMode(width, height);
  this->image = sf::Image();
  this->image.create(this->width, this->height);
  this->texture = sf::Texture();
  this->texture.create(this->width, this->height);
  

	this->blobs = std::vector<blob>();
  for (int i = 0; i < 3; i++)
    this->blobs.push_back(this->random_blob());
	
	this->reset_field();
	this->paused = false;
}

void MarchingSquares::init_window()
{
	this->settings.antialiasingLevel = 8u;
	this->window = std::make_shared<sf::RenderWindow>(this->videoMode, "Marching Squares", sf::Style::Default ^ sf::Style::Resize, this->settings);
	this->window->setVerticalSyncEnabled(true);
}

MarchingSquares::MarchingSquares()
{
  this->init_variables();
	this->init_window();

  while (!ImGui::SFML::Init(*this->window)) 
    std::cout << "ImGui Failed to Initialize" << std::endl;
}

void MarchingSquares::poll_events()
{
	while (this->window->pollEvent(this->ev))
	{
    ImGui::SFML::ProcessEvent(this->ev);
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			switch (this->ev.key.code)
			{
			case sf::Keyboard::Escape:
				this->window->close();
				break;
			case sf::Keyboard::Space:
				paused = !paused;
				break;
			case sf::Keyboard::A:
				if (this->blobs.size() <= 10)
					this->blobs.push_back(this->random_blob());
				break;
			case sf::Keyboard::D:
				if (this->blobs.size() > 0)
					this->blobs.pop_back();
				break;
			case sf::Keyboard::K:
				if (paused)
				{
					this->update_blobs();
          this->calculate_field();
				}
				break;
			case sf::Keyboard::J:
				if (paused)
				{
					this->update_blobs();
          this->calculate_field();
				}
				break;
      default:
        break;
			}
			break;
    default:
      break;
		}
	}
}




void MarchingSquares::reset_field()
{
	this->field = Arr2d<float>(width / cellsize + 1, height / cellsize + 1);
  this->polygons = std::vector<sf::VertexArray>();
	this->calculate_field();
}

sf::Color get_image_color(double field_val) {
  return sf::Color(
      (int)(0 * field_val) % 256,
      (int)(123 * field_val) % 256,
      (int)(255 * field_val) % 256
  );
}

blob MarchingSquares::random_blob() {
  float angle = random_float_uni() * 2 * PI;
	return blob(50.0f, sf::Vector2f(random_float_uni() * width, random_float_uni() * height), sf::Vector2f(cos(angle), sin(angle)));
}

void MarchingSquares::update_blobs() {
  for (blob& b : blobs)
	{
    // update blob direction
		if (b.position.x > width - b.radius)
		{
			b.position.x = width - b.radius;
			b.velocity.x = -b.velocity.x;
		}
		else if (b.position.x < b.radius)
		{
			b.position.x = b.radius;
			b.velocity.x = -b.velocity.x;
		}

		if (b.position.y > height - b.radius)
		{
			b.position.y = height - b.radius;
			b.velocity.y = -b.velocity.y;
		}
		else if (b.position.y < b.radius)
		{
			b.position.y = b.radius;
			b.velocity.y = -b.velocity.y;
		}
    
    // update blob position
    b.position += speed * b.velocity * this->deltaTime;   
  }
}

void MarchingSquares::calculate_field()
{
  this->polygons.clear();
  /*for (unsigned int x = 0; x < this->width; x++)
    for (unsigned int y = 0; y < this->height; y++)
    {
      this->set_pixel_val(x, y);
      if (x % (int)cellsize == 0 && y % (int)cellsize == 0) {
        unsigned int xi = x / (int)cellsize, yi = y / (int)cellsize;
        this->set_field_val(xi, yi);
        if (xi > 0 && yi > 0) {
          if (draw_fill)
            this->fill_polygon(xi, yi);
          else
            this->outline_polygon(xi, yi);
        }
      }
    }*/
  for (unsigned int i = 0; i < this->field.get_width(); i++) {
    for (unsigned int j = 0; j < this->field.get_height(); j++) {
      this->set_field_val(i, j);
      if (i > 0 && j > 0 && (this->field.get_value(i, j) >= crit_val || this->field.get_value(i-1,j-1) >= crit_val || this->field.get_value(i,j-1) >= crit_val || this->field.get_value(i-1,j) >= crit_val)) { // calculate polygons
        if (draw_fill)
          this->fill_polygon(i, j);
        else
          this->outline_polygon(i, j);
      }
    }
  }
}

void MarchingSquares::set_field_val(unsigned int x, unsigned int y) {
  double field_val = this->get_field_val(x*cellsize, y*cellsize);
  this->field.set_value(x, y, field_val);
}

float MarchingSquares::get_field_val(float x, float y) {
  float field_val = 0.0;
  for (const blob& b : blobs) {
    field_val += intensity / distance(sf::Vector2f(x, y), b.position);
  }
  return field_val;
}


void MarchingSquares::set_pixel_val(unsigned int x, unsigned int y) {
  this->image.setPixel(x, y, get_image_color(this->get_field_val((double)x, (double)y) / crit_val));
}

void MarchingSquares::fill_polygon(unsigned int x, unsigned int y) {
  sf::Vector2i points[4] = { 
    sf::Vector2i(x-1, y-1), 
    sf::Vector2i(x-1, y), 
    sf::Vector2i(x, y), 
    sf::Vector2i(x, y-1) 
  };
  sf::Vector2f center = sf::Vector2f((float)x + 0.5, (float)y + 0.5);
  float cf = this->get_field_val(center.x, center.y);
  sf::Color col = sf::Color((int)(color[0]*255),(int)(color[1]*255),(int)(color[2]*255));
  sf::VertexArray zone(sf::TriangleFan, 6);
  int vertex = 0;
  // go clockwise around the points and check if they are above the critical value
  int state = 0b0000;
  for (int k = 0; k < 4; ++k)
  {
    sf::Vector2i p = points[k], np = points[(k+1)%4];
    sf::Vector2f point = sf::Vector2f(p * cellsize), next_point = sf::Vector2f(np * cellsize);
    float f = this->field.get_value(p.x, p.y), nf = this->field.get_value(np.x, np.y);
    int above_crit = (f >= crit_val);
    state |= above_crit << k;
    if (above_crit)
      zone[vertex++] = sf::Vertex(point, col);
    // the critical value of the next point is different from the current one, add the lerped vertex to the vertex array
    if ((nf >= crit_val) != above_crit)
      zone[vertex++] = sf::Vertex(lerp(point, next_point, (crit_val-f)/(nf-f)), col);
  }
  state |= (cf >= crit_val) << 4;
  sf::VertexArray zone1(sf::TriangleFan, 3);
  sf::VertexArray zone2(sf::TriangleFan, 3);
  switch (state) {
    case 0b01010:
      zone1[0] = zone[0];
      zone1[1] = zone[1];
      zone1[2] = zone[2];
      this->polygons.push_back(zone1);
      zone2[0] = zone[3];
      zone2[1] = zone[4];
      zone2[2] = zone[5];
      this->polygons.push_back(zone2);
      break;
    case 0b00101:
      zone1[0] = zone[0];
      zone1[1] = zone[1];
      zone1[2] = zone[5];
      this->polygons.push_back(zone1);
      zone2[0] = zone[2];
      zone2[1] = zone[3];
      zone2[2] = zone[4];
      this->polygons.push_back(zone2);
      break;
    default:
      if (vertex) {
        zone.resize(vertex);
        this->polygons.push_back(zone);
      }
      break;
  }
}

void MarchingSquares::outline_polygon(unsigned int x, unsigned int y) {
  sf::Vector2i points[4] = { 
    sf::Vector2i(x-1, y-1), 
    sf::Vector2i(x-1, y), 
    sf::Vector2i(x, y), 
    sf::Vector2i(x, y-1) 
  };
  sf::Color col = sf::Color((int)(color[0]*255),(int)(color[1]*255),(int)(color[2]*255));
  sf::Vector2f center = sf::Vector2f((float)x + 0.5, (float)y + 0.5);
  float cf = this->get_field_val(center.x, center.y);
  sf::VertexArray zone(sf::Lines, 4);
  int vertex = 0;
  int state = 0b0000;
  for (int k = 0; k < 4; ++k)
  {
    sf::Vector2i p = points[k], np = points[(k+1)%4];
    sf::Vector2f point = sf::Vector2f(p*cellsize), next_point = sf::Vector2f(np*cellsize);
    float f = this->field.get_value(p.x, p.y), nf = this->field.get_value(np.x, np.y);
    int above_crit = (f >= crit_val);
    sf::Vector2f lerp_point = lerp(point, next_point, (crit_val-f)/(nf-f));
    if ((nf >= crit_val) != (f >= crit_val))
      zone[vertex++] = sf::Vertex(lerp_point, col);
    state |= (above_crit << k);
  }
  state |= (cf >= crit_val) << 4;

  sf::Vertex temp(zone[3].position, zone[3].color);
  switch (state) {
    case 0b00101: case 0b11010:
      zone[3] = zone[2];
      zone[2] = zone[1];
      zone[1] = zone[0];
      zone[0] = temp;
      break;
    default:
      break;
  }
  if(vertex) {
    zone.resize(vertex);
    this->polygons.push_back(zone);
  }
}

void MarchingSquares::update()
{
	this->poll_events();
	if (paused)
		return;
  this->update_blobs();
  this->calculate_field();
  auto restart = this->clock.restart();
  this->deltaTime = restart.asSeconds();
  ImGui::SFML::Update(*this->window, restart);
  this->imgui_setup();
}

void MarchingSquares::render()
{
	this->window->clear();
  
  this->texture.update(this->image);
  sf::Sprite spr;
  spr.setTexture(this->texture);
  this->window->draw(spr);
  
  for (const auto& polygon : polygons)
    this->window->draw(polygon);


  if (draw_points)
    for (std::size_t i = 0; i < this->field.get_width() - 1; i++)
      for (std::size_t j = 0; j < this->field.get_height() - 1; j++)
      {
        sf::CircleShape circ;
        circ.setRadius(2.0f);
        circ.setOrigin(1.0f, 1.0f);
        
        if (this->field.get_value(i, j) >= crit_val)
          circ.setFillColor(sf::Color::Green);
        else
          circ.setFillColor(sf::Color::Red);
        circ.setPosition(i * cellsize, j * cellsize);
        this->window->draw(circ);
      }

  
	if (draw_blobs)
	{
    sf::CircleShape circ;
    circ.setFillColor(sf::Color::Blue);
    circ.setRadius(20.0f);
    circ.setOrigin(10.0f, 10.0f);
    for (const blob& b : blobs)
    {
      circ.setPosition(b.position);
      this->window->draw(circ);
    }
	}

  ImGui::SFML::Render(*this->window);
	this->window->display();
	//this->deltaTime = this->clock.restart().asSeconds();
}

void MarchingSquares::shutdown() {
  ImGui::SFML::Shutdown();
}

