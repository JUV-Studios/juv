#ifndef JUV_ATOMIC_REF_COUNT
#define JUV_ATOMIC_REF_COUNT

#include <cstddef>
#include <atomic>

namespace juv
{
	struct atomic_ref_count
	{
	public:
		// Adapted from C++/WinRT.

		atomic_ref_count(size_t count) noexcept : m_count{ count } {}

		atomic_ref_count(atomic_ref_count const& other) : m_count{ other.m_count.load() } {}

		size_t operator=(size_t count) const noexcept { return m_count = count; }

		size_t operator++() const noexcept { return static_cast<size_t>(m_count.fetch_add(1, std::memory_order_relaxed) + 1); }

		size_t operator--() const noexcept
		{
			auto const remaining = m_count.fetch_sub(1, std::memory_order_release) - 1;
			if (remaining == 0) std::atomic_thread_fence(std::memory_order_acquire);
			else if (remaining < 0) abort();
			return static_cast<size_t>(remaining);
		}

		operator size_t() const noexcept { return static_cast<size_t>(m_count); }

		bool try_acquire_unique_ownership() const noexcept
		{
			// TODO Used ChatGPT. Ensure correctness.
			size_t expected = 1;
			size_t exchanged = 0;
			return m_count.compare_exchange_strong(expected, exchanged, std::memory_order_acquire);
		}
	private:
		std::atomic<size_t> mutable m_count;
	};
}

#endif