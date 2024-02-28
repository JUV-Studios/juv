#ifndef JUV_EVENT
#define JUV_EVENT

#include "array.hpp"

namespace juv
{
	struct event_token
	{
		std::array<std::byte, sizeof(void(*)(void))> value;
		[[nodiscard]] bool operator==(event_token const& other) const noexcept = default;

#ifdef _WIN32
		using native_type = winrt::event_token;
		event_token(native_type token) noexcept : value{ std::bit_cast<decltype(value)>(token.value) } {}
		operator native_type() noexcept { return std::bit_cast<native_type>(value); }
#endif

		event_token(decltype(value) token = {}) : value{ token } {}
	};


	template <typename... T>
	struct event
	{
		static_assert(sizeof...(T) == 2 || sizeof...(T) == 3);
	};

	template <typename Author, typename Sender, typename Args>
	struct event<Author, Sender, Args>
	{
	private:
		using free_func = std::conditional_t<std::is_void_v<Sender>, void(Args&), void(Sender&, Args&)>;
		using member_func = std::conditional_t<std::is_void_v<Sender>, void(void*, Args&), void(void*, Sender&, Args&)>;

		struct handler_type
		{
			std::shared_ptr<void> m_context;
			union
			{
				free_func* m_free_func;
				member_func* m_member_func;
			};

			event_token get_token() const noexcept
			{
				if (m_context) return std::bit_cast<event_token>(m_context.get());
				else return std::bit_cast<event_token>(m_free_func);
			}
		};

		shared_array<handler_type> m_handlers;
		// TODO
	};
}

#endif