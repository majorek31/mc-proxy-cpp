#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadCount)
	: m_Stop(false)
{
	for (int i = 0; i < threadCount; i++)
	{
		m_Threads.emplace_back([this]()
		{
			while (true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(m_Mutex);
					m_Condition.wait(lock, [this]()
					{
						return m_Stop || !m_Tasks.empty();
					});

					if (m_Stop && m_Tasks.empty())
						return;

					task = std::move(m_Tasks.front());
					m_Tasks.pop();
				}

				task();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Stop = true;
	}

	m_Condition.notify_all();

	for (auto& thread : m_Threads)
		if (thread.joinable()) thread.join();
}
