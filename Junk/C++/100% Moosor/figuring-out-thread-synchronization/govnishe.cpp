#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>

#define __(line) print(name, #line); line

void print(std::string const& name, std::string const& str)
{
	static std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::cout << name << ": " << str << std::endl;
}

std::mutex mut_ready;
std::condition_variable ready;
bool buf_ready = false;

std::mutex mut_tasks;
std::condition_variable done;
size_t tasks_left = 99999999999;
void wait_for_rest(std::string const& name)
{
__(	std::unique_lock<std::mutex> lock(mut_tasks);)
__(	--tasks_left;)
__(	if (tasks_left) {)
__(		while (tasks_left) {)
__(			done.wait(lock);)
__(		})
__(	} else {)
__(		done.notify_all();)
__(	})
}

bool all_done = false;

void loop(std::string name)
{
__(	while (true) {)
__(		{)
__(			std::unique_lock<std::mutex> lock(mut_ready);)
__(			while (!buf_ready) {)
__(				ready.wait(lock);)
__(			})
__(		})
__()
__(		if (all_done) {)
__(			return;)
__(		})
__( )
		print(name, "work");
		print(name, "work");
       		print(name, "work");
__()
__(       	wait_for_rest(name);)
__(       })
}

int main()
{
         std::string name = "main";
__(      std::cout << std::this_thread::get_id()  << std::endl;)
__()
__(      std::thread worker1(loop, "wor1");)
__(      std::thread worker2(loop, "wor2");)
__()
__(      for (size_t i = 0; i < 100; ++i) {)
__(      	using namespace std::literals;)
      	// std::this_thread::sleep_for(100ms);
__()
__(      	{)
__(      		std::lock_guard<std::mutex> lock1(mut_ready);)
__(      		buf_ready = true;)
__()
__(      		std::lock_guard<std::mutex> lock2(mut_tasks);)
__(      		tasks_left = 3;)
__(      	})
__()
__(      	ready.notify_all();)
__()
      	// std::this_thread::sleep_for(100ms);
__(      	wait_for_rest("main");)
__()
__(      	{)
__(      		std::lock_guard<std::mutex> lock(mut_ready);)
__(      		buf_ready = false;)
__(      	})
__(      })
__()
__(      all_done = true;)
__()
__(      worker1.join();)
__(      worker2.join();)
}
