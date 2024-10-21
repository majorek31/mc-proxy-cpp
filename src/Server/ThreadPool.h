#pragma once
#include <future>
#include <queue>
#include <functional>
#include <future>
#include <condition_variable>


class ThreadPool
{
public:
	ThreadPool(int threadCount);
	~ThreadPool();

	template<typename T>
	auto AddTask(T&& task) -> std::future<typename std::invoke_result_t<T>>;
private:
	std::vector<std::thread> m_Threads;
	std::queue<std::function<void()>> m_Tasks;
	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	bool m_Stop = false;
};

template<typename T>
inline auto ThreadPool::AddTask(T&& task) -> std::future<typename std::invoke_result_t<T>>
{
	using return_type = std::invoke_result_t<T>;

	auto packaged_task = std::make_shared<std::packaged_task<return_type()>>(std::forward<T>(task));
	std::future<return_type> result = packaged_task->get_future();

	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Tasks.emplace([packaged_task]() { (*packaged_task)(); });
	}

	m_Condition.notify_one();
	return result;
}