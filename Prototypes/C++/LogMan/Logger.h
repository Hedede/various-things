#include <vector>
#include <string>
#include <algorithm>
struct Message {
	std::string source;
	// ...other stuff...
	std::string text;
};

struct Logger {
	virtual void log(Message msg) = 0;
};

struct MultiLogger;

struct LogBook {
	LogBook(MultiLogger& logger);
	virtual ~LogBook();

	virtual void log(Message msg) = 0;
private:
	MultiLogger& logger;
};

struct MultiLogger : Logger {
	void add(LogBook& log)
	{
		receivers.push_back(&log);
	}

	void remove(LogBook& log)
	{
		auto to_erase = std::remove(begin(receivers), end(receivers), &log);
		if (to_erase != end(receivers))
			receivers.erase(to_erase);
	}

	void log(Message msg) override
	{
		for (auto rec : receivers)
			rec->log(msg);
	}

private:
	std::vector<LogBook*> receivers;
};

LogBook::LogBook(MultiLogger& logger)
	: logger(logger)
{
	logger.add(*this);
}

LogBook::~LogBook()
{
	logger.remove(*this);
}

struct LogFront {
	Logger* logger;

	void log(Message msg)
	{
		if (logger)
			logger->log(msg);
	}
};
