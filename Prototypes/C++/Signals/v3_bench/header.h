struct countable {
	void operator++();
	void operator--();
	volatile int ctr = 0;
};

struct counter {
	virtual void count(countable& ctr) = 0;
};

struct counter_inc : counter {
	void count(countable& ctr) override;
};


struct counter_dec : counter {
	void count(countable& ctr) override;
};


struct vf_counter : countable {
	counter* ctr;

	void invoke();
	
};

extern vf_counter v;


struct st_counter {
	void count(countable& ctr) const;

	mutable volatile int padding[8];
};


#include <functional>
struct fun_counter : countable {
	std::function<void(countable&)> ctr;
	void invoke();
};

extern fun_counter f;


extern volatile int val;


template<typename T, typename Func>
struct delegate;

template<typename T, typename R, typename... Args>
using mem_fn = R(T::*)(Args...);

template<typename T, typename R, typename... Args>
using mem_cfn = R(T::*)(Args...) const;

template<typename T, typename R, typename... Args>
struct delegate<T,R(Args...)> {
	template <mem_fn<T,R,Args...> meth>
	static void call(void* zis, Args&&...args)
	{
		((T*)(zis)->*meth)(std::forward<Args>(args)...);
	}
};

template<typename T, typename R, typename... Args>
struct delegate<T const,R(Args...)> {
	template <mem_cfn<T,R,Args...> meth>
	static void call(void* zis, Args&&...args)
	{
		((T const*)(zis)->*meth)(std::forward<Args>(args)...);
	}
};

struct delectr : countable{
	void* collubl;
	void(*coll)(void*, countable&);
	void invoke();
};

extern delectr d;

#include <chrono>
constexpr unsigned num_iter = 1000000000;
using namespace std::chrono;

double test_vfun(bool arg);
double test_fun(bool arg);
double test_tpl(bool arg);
double test_plain();
