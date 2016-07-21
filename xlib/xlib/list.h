#ifndef LIST_H
#define LIST_H

namespace x
{
	template<typename T>
	class list
	{
		struct ListElem
		{
			T value;
			ListElem* next;
		};

		ListElem* first_;
		size_t size_;

	public:
		list():
			first_{nullptr},
			size_{0}
		{
		}

		list(list<T> const& other):
			size_{other.size_}
		{
		}
	};
}
#endif //LIST_H