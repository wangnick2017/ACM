#ifndef PAIR_HPP
#define PAIR_HPP
namespace stl
{
	template<class T1, class T2>
	class pair
	{
	public:
		T1 first;
		T2 second;
		pair(T1 _first, T2 _second)
		{
			first = _first;
			second = _second;
		}
	};
}
#endif