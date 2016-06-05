#ifndef FN_H
#define FN_H

#define enable_if std::enable_if_t
#define decay std::decay_t

template<typename...>
class Fn;

template<typename R, typename... A>
class Fn<R(A...)>
{
	struct eraser_
	{
		virtual R operator()(A...) const abstract;
	};
	template<typename F>
	struct concrete_ final: public eraser_
	{
		F fn;

		template<typename G>
		concrete_(G&& fn): fn{std::forward<G>(fn)} {}

		R operator()(A... args) const final override {
			return fn(args...);
		}
	};

	eraser_* fn_;

public:
	template<typename F/*,
		typename = enable_if<std::is_convertible<
		decltype((*fn_)(std::declval<A>()...)),
		R>::value>*/>
	Fn(F&& fn):
		fn_{new concrete_<decay<F>>{std::forward<F>(fn)}} {}

	__forceinline R operator()(A... args) const 
	{
		return (*fn_)(std::forward<A>(args)...);
	}

	~Fn()
	{
		delete fn_;
	}
};

#endif //FN_H
