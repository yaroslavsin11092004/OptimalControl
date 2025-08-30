#ifndef ITERATOR_H
#define ITERATOR_H
#include <cstddef>
#include <type_traits>
namespace stdext
{
	template<typename T>
	struct checked_array_iterator
	{
		private:
			T _val;
			size_t _len;
		public:
			typedef typename std::remove_pointer<T>::type value_type;
			checked_array_iterator(T val, size_t len): _val(val), _len(len){}
			value_type& operator[](int index){	return *(_val += index);};
	};
}
#endif
