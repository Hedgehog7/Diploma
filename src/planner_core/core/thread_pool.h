
#ifndef UltraCore_thread_pool_h
#define UltraCore_thread_pool_h

#include <vector>
#include <thread>

using namespace std;

template<typename T>
class thread_pool
{
	using task_t = T;
	void worker_thread()
	{
		while (!m_finished)
		{
			task_t * p_cur_task;
#if USE_TBB
			if (m_tasks.try_pop(p_cur_task))
#else
			p_cur_task = m_tasks.front();
			m_tasks.pop();
			if(true)
#endif
			{
				++m_busyCount;
				(*p_cur_task)();
				--m_busyCount;
				delete p_cur_task;
			}
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

public:
	thread_pool(int thread_count = 1)
		:m_finished(false), m_busyCount(0)
	{
		for (int i = 0; i < thread_count; ++i)
			m_threads.emplace_back(&thread_pool::worker_thread, this);
	}

	void submit(task_t * new_task)
	{
		m_tasks.push(new_task);
	}

	void wait_all()
	{
		while (!m_tasks.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		
		while (m_busyCount.load() > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
private:
	bool m_finished;
	std::vector<std::thread> m_threads;
	QUEUE_T<task_t*> m_tasks;
	std::atomic<int> m_busyCount;
};

#endif

