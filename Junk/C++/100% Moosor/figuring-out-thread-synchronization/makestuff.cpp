#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

void my_fucking_cout(std::string str)
{
	static std::mutex mudex;
	std::unique_lock<std::mutex> lock(mudex);
	std::cout << std::this_thread::get_id() << ":" << str << std::endl;
}

class Task {
public:
	virtual void run() = 0;
};

class FoelTask : public Task {
public:
	FoelTask(int fileid)
		: x(fileid)
	{
	};
	virtual void run()
	{
		my_fucking_cout("wrk" + std::to_string(x));
		std::ifstream f("bigfile1");
		std::ofstream o("bigfile" + std::to_string(1 + x/1000));
		o << f.rdbuf();
		my_fucking_cout("dn" + std::to_string(x));
	}
private:
	int x;
};

class Worker {
public:
	Worker()
	{
		thread = std::thread(&Worker::loop, this);
	}

	~Worker()
	{
		if (!done)
			kill();
		thread.join();
	}

	void start(Task& task)
	{
		assert(!done);

		{
			std::lock_guard<std::mutex> lock(mutex);
			this->task = &task;
		}
		cond.notify_one();
	}
	
	bool wait()
	{
		assert(!done);

		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] {return !task;});
	}

	void kill()
	{
		assert(!done && "Attempted to kill thread twice.");

		{
			std::lock_guard<std::mutex> lock(mutex);
			done = true;
		}
		cond.notify_one();
	}
private:
	void loop()
	{
		assert(!done);

		while (true) {
			std::unique_lock<std::mutex> lock(mutex);
			cond.wait(lock, [this] {return !done && task;});

			if (done)
				return;

			task->run();
			task = nullptr;

			lock.unlock();
			cond.notify_one();
		}
	}

	std::mutex mutex;
	std::condition_variable cond;

	bool done = false;
	Task* task = nullptr;

	std::thread thread;
};
 
int main()
{
	std::condition_variable cond;
	std::condition_variable cond2;
	Worker worker1;
	Worker worker2;

	for (size_t i = 0; i < 100; ++i) {
		my_fucking_cout("main(): WORK, MY SLAVES!");

		FoelTask ft1(1000+i);
		FoelTask ft2(2000+i);

		worker1.start(ft1);
		worker2.start(ft2);

		worker1.wait();
		worker2.wait();
	}

	std::cout << "main(): COOT FORK!" << std::endl;

	worker1.kill();
	worker2.kill();
}
