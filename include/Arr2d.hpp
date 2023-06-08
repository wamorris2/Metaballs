#ifndef __ARR_2D__
#define __ARR_2D__
#include <vector>
#include <cassert>

template <typename T>
class Arr2d {
private:
	std::size_t width;
  std::size_t height;
	std::vector<T> vec;
public:
  Arr2d() : width(0), height(0), vec(std::vector<T>()) {}
	Arr2d(std::size_t width, std::size_t height) : width(width), height(height), vec(std::vector<T>(width*height)) {}
	T& get_value(std::size_t x, std::size_t y) {
		return this->vec[x + y * this->width];
	}
	void set_value(std::size_t x, std::size_t y, T value) {
		this->vec[x + y * this->width] = value;
	}
  std::size_t get_width() { return this->width; }
  std::size_t get_height() { return this->height; }
};
#endif
