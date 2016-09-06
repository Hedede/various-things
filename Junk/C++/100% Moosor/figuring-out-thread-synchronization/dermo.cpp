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

class Barrier {
public:
	explicit Barrier(size_t count)
		: count(count), initial(count)
	{
	}

	void reset()
	{
		std::unique_lock<std::mutex> lock(mutex);
		count = initial;
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(mutex);
		--count;
		my_fucking_cout(std::to_string(count));
		if (count == 0) {
			my_fucking_cout("ALL RAZBLOK");
			cv.notify_all();
		} else {
			my_fucking_cout("EYM WAITIN");
			cv.wait(lock, [this] { return count == 0; });
		}
	}
private:
	std::mutex mutex;
	std::condition_variable cv;

	size_t initial;
	size_t count;
};

class Condition {
public:
	void setReady(bool ready)
	{
		std::unique_lock<std::mutex> lock(mutex);
		this->ready = ready;
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [this] { return ready; });
	}
	
	void notifyAll()
	{
		cv.notify_all();
	}
private:
	std::mutex mutex;
	std::condition_variable cv;

	bool ready = false;
};

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
		/*std::ifstream f("bigfile1");
		std::ofstream o("bigfile" +);
		o << f.rdbuf();*/
		my_fucking_cout("dn" + std::to_string(x));
	}
private:
	int x;
};

class Worker {
public:
	Worker(Condition& cond)
		: cond(cond), cond2(cond2), bar(bar)
	{
		thread = std::thread(&Worker::loop, this);
	}

	~Worker()
	{
		thread.join();
	}

	void setTask(Task& task)
	{
		tsk = &task;
	}

	void loop()
	{
		while (true) {
			cond.wait();

			{
				std::unique_lock<std::mutex> lock(mutex);
				if (done)
					return;

				if (tsk)
					tsk->run();
			}

			my_fucking_cout("FOR FUCKS SAKE");
		}
	}

	void kill()
	{
		std::unique_lock<std::mutex> lock(mutex);
		done = true;
	}
private:
	std::mutex mutex;
	bool done = false;
	Task* tsk = nullptr;

	Condition& cond;

	std::thread thread;
};
 
int main()
{
	Condition cond;
	Condition cond2;
	Barrier bar(3);
	Worker worker1(cond1);
	Worker worker2(cond2);

	for (size_t i = 0; i < 100; ++i) {
		using namespace std::literals;
		my_fucking_cout("main(): ARRRGH!");

		FoelTask ft1(1000+i);
		FoelTask ft2(2000+i);

		worker1.setTask(ft1);
		worker2.setTask(ft2);

		std::this_thread::sleep_for(5000ms);

		cond.setReady(true);
		cond2.setReady(false);
		bar.reset();

		my_fucking_cout("main(): WORK, MY SLAVES!");

		std::this_thread::sleep_for(5000ms);

		cond.notifyAll();

		std::this_thread::sleep_for(5000ms);

		bar.wait();

		std::this_thread::sleep_for(5000ms);

		my_fucking_cout("main(): WORK HARDER!");

		std::this_thread::sleep_for(5000ms);

		cond.setReady(false);
		cond2.setReady(true);

		std::this_thread::sleep_for(5000ms);

		cond2.notifyAll();

		std::this_thread::sleep_for(5000ms);
	}

	std::cout << "main(): COOT FORK!" << std::endl;

	worker1.kill();
	worker2.kill();
}
