#ifndef SHALLOW_DELETE
#define SHALLOW_DELETE
#include <type_traits>

#define byte char

template<size_t size>
class shallow_destructible
{
	byte fake_data__[size];
public:
	~shallow_destructible(){}
};

#define enable_shallow_delete shallow_destructible
#define shallow(ptr) reinterpret_cast<\
	shallow_destructible<sizeof(decltype(*ptr))>*>(ptr)

//template<typename T>
//struct is_shallow_destructible
//{
//	static constexpr bool value =
//		std::is_base_of<T, shallow_destructible>::value ||
//		std::is_fundamental<T>::value;
//};

#undef byte
#endif //SHALLOW_DELETE