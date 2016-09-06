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
int task1 = 0;
int task2 = 0;

std::mutex mut_start;
std::condition_variable started;
size_t tasks_started = 0;

void wait_start(std::string const& name)
{
__(	std::unique_lock<std::mutex> lock(mut_start);)
__(	++tasks_started;)
print(name, "tasks_started: " + std::to_string(tasks_started));
__(	if (tasks_started < 3) {)
__(		while (tasks_started < 3) {)
__(			started.wait(lock);)
__(		})
__(	} else {)
__(		started.notify_all();)
__(	})
}

void wait_for_rest(std::string const& name)
{
__(	std::unique_lock<std::mutex> lock(mut_tasks);)
__(	--tasks_left;)
print(name, "tasks_left: " + std::to_string(tasks_left));
__(	if (tasks_left) {)
__(		while (tasks_left) {)
__(			done.wait(lock);)
__(		})
__(	} else {)
__(		done.notify_all();)
__(	})
}

bool all_done = false;

void loop(std::string name, int* task)
{
__(	while (true) {)
__()
__(		wait_start(name));
__()
__(		{)
__(			std::unique_lock<std::mutex> lock(mut_ready);)
__(			while (!*task) {)
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
__(		{)
__(			std::unique_lock<std::mutex> lock(mut_ready);)
__(			*task = 0;)
__(		})
__()
__(       	wait_for_rest(name);)
__(       })
}

int main()
{
         std::string name = "main";
__()
__(      std::thread worker1(loop, "wor1", &task1);)
__(      std::thread worker2(loop, "wor2", &task2);)
__()
__(      for (size_t i = 0; i < 100; ++i) {)
__(      	using namespace std::literals;)
      	// std::this_thread::sleep_for(100ms);
__()
__(		wait_start(name));
__()
__(      	{)
__(      		std::lock_guard<std::mutex> lock1(mut_ready);)
__(      		task1 = 1;)
__(      		task2 = 1;)
__(      	})
__(      	ready.notify_all();)
__()
__(		{)
__(      		std::lock_guard<std::mutex> lock2(mut_tasks);)
__(      		tasks_left = 3;)
__(      	})
__()
      	// std::this_thread::sleep_for(100ms);
__(      	wait_for_rest("main");)
__(      		tasks_started -= 3;)
__()
__(      })
__()
__(      all_done = true;)
__()
__(      worker1.join();)
__(      worker2.join();)
}
