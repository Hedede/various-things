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
	std::unique_lock<std::mutex> lock(mut_start);
	++tasks_started;
	print(name, "tasks_started: " + std::to_string(tasks_started));
	if (tasks_started < 3) {
		while (tasks_started < 3) {
			started.wait(lock);
		}
	} else {
		started.notify_all();
	}
}

void wait_for_rest(std::string const& name)
{
	std::unique_lock<std::mutex> lock(mut_tasks);
	--tasks_left;
	print(name, "tasks_left: " + std::to_string(tasks_left));
	if (tasks_left) {
		while (tasks_left) {
			done.wait(lock);
		}
	} else {
		done.notify_all();
	}
}

bool all_done = false;

void loop(std::string name, int* task)
{
	while (true) {

		wait_start(name);

		{
			std::unique_lock<std::mutex> lock(mut_ready);
			while (!*task) {
				ready.wait(lock);
			}
		}

		if (all_done) {
			return;
		}

		print(name, "work");
		print(name, "work");
		print(name, "work");
		{
			std::unique_lock<std::mutex> lock(mut_ready);
			*task = 0;
		}

		wait_for_rest(name);
	}
}

int main()
{
	std::string name = "main";

	std::thread worker1(loop, "wor1", &task1);
	std::thread worker2(loop, "wor2", &task2);

	for (size_t i = 0; i < 100; ++i) {
		using namespace std::literals;
		// std::this_thread::sleep_for(100ms);

		wait_start(name));

		{
			std::lock_guard<std::mutex> lock1(mut_ready);
			task1 = 1;
			task2 = 1;
		}
		ready.notify_all();

		{
			std::lock_guard<std::mutex> lock2(mut_tasks);
			tasks_left = 3;
		}

		// std::this_thread::sleep_for(100ms);
		wait_for_rest("main");
		tasks_started -= 3;

	}

	all_done = true;

	worker1.join();
	worker2.join();
}
