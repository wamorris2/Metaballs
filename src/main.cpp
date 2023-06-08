#include "MarchingSquares.hpp"
#include <thread>
#include <atomic>
#include <mutex>

/*std::mutex rendering_mutex;
std::atomic<unsigned long long int> square;

void thread_func(std::shared_ptr<MarchingSquares> ms, int x) {
  auto field = ms->get_update_image().getSize();
  int buffer = 0;
  while (ms->running()) {
    auto pixel = square.fetch_add(1, std::memory_order_relaxed);
    if (pixel >= field.x * field.y) {
      if (rendering_mutex.try_lock()) {
        ms->render();
      } else {
        
      }
      ms.flip_buffer();
      ms.render_buffer();
    }
    pixel %= field.x * field.y;
    auto y = pixel % field.x;
    auto x = pixel / field.x + 1;
    ms->thread_func(x, y);
    
  }
}*/

int main()
{
  //const auto processor_count = std::thread::hardware_concurrency();
	auto ms = std::make_shared<MarchingSquares>();
  while (ms->running()) {
    ms->update();
    ms->render();
  }

  ms->shutdown();
  /*std::vector<std::thread> threads;
  for (int i = 0; i < processor_count; i++) {
    threads.push_back(std::thread(thread_func, ms, i));
  }
  
	for (int i=0; i<processor_count; i++) {
    threads[i].join();
  }*/
	return 0;
}
