#ifndef OBSERVER_H
#define OBSERVER_H

namespace x
{
	template<typename _Type>
	class Observer
	{
		_Type& var_;
		mutable _Type prev_;

	public:
		Observer() = delete;

		Observer(Observer<_Type> const& other):
			var_{other.var},
			prev_{other.var}
		{
		}

		Observer(Observer<_Type>&& other):
			var_{std::move(other.var_)},
			prev_{std::move(other.var_)}
		{
		}

		Observer(_Type& var):
			var_{var}, prev_{var}
		{
		}

		inline bool operator()() const
		{
			return (var_ != prev_ ? (prev_ = var_, true) : false);
			/*if (var_!=prev_) {
				prev_ = var_;
				return true;
			}
			return false;*/
		}

		inline _Type value() const
		{
			return var_;
		}
	};
}
#endif //OBSERVER_H
