#ifndef JUV_NULL_TERMINATION

#include "char_classification.hpp"

namespace juv
{
	template <character CharT>
	struct ntstring_in final
	{
	public:
		ntstring_in(CharT const* str = nullptr) noexcept : originally_nt{ true }, m_str{ str } {}

		// TODO constraint, constexpr support
		template <typename T>
		ntstring_in(T const& t) : ntstring_in{ get(t) } {}

		ntstring_in(ntstring_in&& other) noexcept : originally_nt{ std::exchange(other.originally_nt, true) }, m_str{ std::exchange(other.m_str, nullptr) } {}

		operator CharT const* () const noexcept { return m_str; }

		~ntstring_in()
		{
			if (!originally_nt) delete m_str;
		}
	private:
		bool originally_nt;
		CharT const* m_str;

		constexpr ntstring_in(CharT* str) noexcept : originally_nt{ false }, m_str{ str } {}

		template <typename T>
		ntstring_in constexpr get(T const& t)
		{
			if constexpr (requires { { t.c_str() } -> std::convertible_to<CharT const*>; })
			{
				return static_cast<CharT const*>(t.c_str());
			}
			else
			{
				auto const tsize = t.size();
				auto ntarray = new CharT[tsize + 1];
				std::ranges::copy(t, ntarray);
				ntarray[tsize] = 0;
				return ntarray;
			}
		}
	};
}

#endif