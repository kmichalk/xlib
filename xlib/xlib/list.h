#ifndef LIST_H
#define LIST_H

namespace x
{
	template<typename _Type>
	class list
	{
		struct ListElem
		{
			_Type value;
			ListElem* next;

			ListElem():
				next{nullptr}
			{
			}

			ListElem(_Type const& value):
				value{value},
				next{nullptr}
			{
			}

			ListElem(_Type&& value):
				value{std::move(value)},
				next{nullptr}
			{
			}

			ListElem(ListElem const& other):
				value{other.value},
				next{new ListElem{*other.next}}
			{
			}

			ListElem(ListElem&& other):
				value{std::move(other.value)},
				next{other.next}
			{
				other.next = nullptr;
			}
		};

		ListElem* first_;
		size_t size_;

	public:
		list():
			first_{nullptr},
			size_{0}
		{
		}

		list(list<_Type> const& other):
			first_{new ListElem{*other.first_}},
			size_{other.size_}
		{
		}

		list(list<_Type>&& other):
			first_{other.first_},
			size_{std::move(other.size_)}
		{
			other.first_ = nullptr;
		}

		inline bool empty() const
		{
			return !bool(size_);
		}

		inline operator bool() const
		{
			return bool(size_);
		}

		list<_Type>& push_back(_Type const& value)
		{

		}


		~list()
		{
			if (first_)	
				delete first_;
		}
	};
}
#endif //LIST_H