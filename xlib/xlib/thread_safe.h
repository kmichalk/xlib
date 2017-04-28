#ifndef _X_THREAD_SAFE_H_
#define _X_THREAD_SAFE_H_

#include "result.h"
#include "xrnd.h"
#include "byte.h"


namespace x
{
	class lock
	{
	private:
	public:
		static byte* disabler_;

		void* volatile user_;
		bool volatile locked_;

		__forceinline void set_lock_(void* user)
		{
			locked_ = true;
			user_ = user;
		}

		__forceinline void unlock_()
		{
			user_ = nullptr;
			locked_ = false;
		}

		__forceinline void wait_()
		{
			while (locked_) {
			}
		}

		template<class _Func>
		__forceinline void wait_(_Func&& whileWait)
		{
			while (locked_) {
				whileWait();
			}
		}

		/*void wait_(double timeout)
		{
			x::timer<std::chrono::microseconds> waitTimer;
			waitTimer.tic();
			while (locked_ && waitTimer.toc() < timeout) {
			}
		}*/

		__forceinline bool used_by_(void* user)
		{
			return user_ == user;
		}

	public:
		lock():
			user_{nullptr},
			locked_{false}
		{
		}


		lock(void* user):
			user_{user},
			locked_{false}
		{
		}


		lock(lock const& other):
			user_{nullptr},
			locked_{false}
		{
		}


		lock(lock&& other):
			user_{other.user_},
			locked_{other.locked_}
		{
		}


		__forceinline bool is_locked() const
		{
			return locked_;
		}


		void use(void* user)
		{
			//if (!used_by_(user)){ 
			wait_();
			set_lock_(user);
			//}
		}


		template<class _Func>
		void use(void* user, _Func&& whileWait)
		{
			wait_(whileWait);
			set_lock_(user);
		}


		bool try_use(void* user)
		{
			if (!is_locked())
				set_lock_(user);
		}

	
		void unlock(void* user)
		{
			if (used_by_(user))
				unlock_();
		}


		void disable()
		{
			wait_();
			set_lock_(++disabler_);
		}

		
		virtual ~lock()
		{
			disable();
		}
	};


	byte* x::lock::disabler_ = (byte*)x::random<unsigned>();


	///////////////////////////////////////////////////////////////////////////////


	/// <summary>
	/// Provides thread-safe access to data of type specified in template parameter
	/// by preventing multiple objects to access the data at the same time.
	/// It also prevents the deletion of data while it is being used.
	/// The usage is to create a variable or class field wrapping it in thread_safe object.
	/// </summary>
	/// <typeparam name="_Type">Type of stored data.</typeparam>
	template<class _Type>
	class thread_safe
	{
	private:
	protected:
		void* volatile user_;
		_Type value_;

		/*__forceinline void wait_(thread_safe<_Type> const& other)
		{
			while (other.user_) {
			}
		}*/
		__forceinline void lock_(void* user)
		{
			user_ = user;
		}

		__forceinline void unlock_()
		{
			user_ = nullptr;
		}

		__forceinline void wait_()
		{
			while (user_) {
			}
		}

		void wait_(double timeout)
		{
			x::timer<std::chrono::microseconds> waitTimer;
			waitTimer.tic();
			while (user_ && waitTimer.toc() < timeout) {
			}
		}

		__forceinline bool used_by_(void* user)
		{
			return user_ == user;
		}

	public:
		/// <summary>
		/// Default constructor. 
		/// Calls default condtructor of the contained value object.
		/// Sets user to nullptr.
		/// </summary>
		/// <example><code>thread_safe<x::string> safeStr;</code></example>
		thread_safe():
			user_{nullptr},
			value_{}
		{
		}


		/// <summary>
		/// Constructor initializing the value and user which is nullptr by default if not specified.
		/// </summary>
		/// <param name="value">Value to be initialized with.</param>
		/// <param name="user">User to be set.</param>
		/// <example><code>x::thread_safe<x::string> safeStr{""};</code></example>
		thread_safe(_Type const& value, void* user = nullptr):
			value_{value},
			user_{user}
		{
		}


		thread_safe(_Type&& value, void* user = nullptr):
			value_{std::forward<_Type>(value)},
			user_{user}
		{
		}


		thread_safe(thread_safe<_Type> const& other):
			value_{other.value()},
			user_{nullptr}
		{
		}


		thread_safe(thread_safe<_Type>&& other):
			value_{other.value_},
			user_{other.user_}
		{
		}


		/// <summary>
		/// Checks whether data is being currently used by any object. 
		/// </summary>
		/// <returns>True if user is set, otherwise false.</returns>
		bool is_used() const
		{
			return (bool)user_;
		}


		/// <summary>
		/// Forces object to wait until call of release method from current user.
		/// Sets user to specified and returns accesed data on success.
		/// </summary>
		/// <param name="user">User requested.</param>
		/// <returns>Reference to accessed data.</returns>
		/// <example><code>thread_safe<x::string> safeStr{"114.0098"};
		/// auto dot = safeStr.use(this).find('.');</code></example>
		_Type& use(void* user)
		{
			wait_();
			user_ = user;
			return value_;
		}


		/// <summary>
		/// Forces object to wait until call of release method from current user.
		/// While access is being permitted, executes function \a whileWait with \a user as caller.
		/// Sets user to specified and returns accesed data on success.
		/// </summary>
		/// <param name="user"> - user requested.</param>
		/// <param name="whileWait"> - function to be executed repeatedly while waiting for the access </param>
		/// <returns>Reference to accessed data.</returns>
		/// <example><code>thread_safe<x::string> safeStr{"114.0098"};
		/// auto dot = safeStr.use(this, ).find('.');</code></example>
		template<class _User, class _Ret>
		_Type& use(_User* user, _Ret(_User::*whileWait)())
		{
			while (user_) {
				(user->*whileWait)();
			}
			user_ = user;
			return value_;
		}


		/// <summary>
		/// Forces object to wait until call of release method from current user.
		/// While access is being permitted, executes function \a whileWait.
		/// Sets user to specified and returns accesed data on success.
		/// </summary>
		/// <param name="user"> - user requested.</param>
		/// <param name="whileWait"> - function to be executed repeatedly while waiting for the access </param>
		/// <returns>Reference to accessed data.</returns>
		/// <example><code>thread_safe<x::string> safeStr{"114.0098"};
		/// auto dot = safeStr.use(this, ).find('.');</code></example>
		template<class _Func>
		_Type& use(void* user, _Func&& whileWait)
		{
			while (user_) {
				whileWait();
			}
			lock_(user);
			return value_;
		}


		/// <summary>
		/// Validates specified user and releases unlocks the data on success.
		/// Sets user to nullptr.
		/// </summary>
		/// <param name="user">User to be validated.</param>
		void release(void* user)
		{
			if (used_by_(user))
				unlock_();
		}


		/// <summary>
		/// Tries to access data by specified user without waiting for release.
		/// </summary>
		/// <param name="user">User to be validated.</param>
		/// <returns>Reference to data wrapped in x::result object if validation succeeded,
		/// otherwise invalid x::result not containing desired information.</returns>
		/// <example><code>x::thread_safe&lt;int&gt;safeInt{1};
		/// if (auto val = safeInt.access(this)){
		///		++val;
		/// }</code></example>
		result<_Type&> access(void* user)
		{
			if (used_by_(user)) return value_;
			else return result<_Type&>::INVALID;
		}

		template<_capture(_Type)>
		std::enable_if_t<!std::is_fundamental<_Type>::value, 
		_Type> value() const
		{
			wait_();
			lock_(this);
			_Type copy = value_;
			unlock_();
			return copy;
		}


		template<_capture(_Type)>
		std::enable_if_t<std::is_fundamental<_Type>::value,
			_Type> value() const
		{
			wait_();
			return value_;
		}


		/// <summary>
		/// Destructor providing wait until release from current user will have been called.
		/// </summary>
		virtual ~thread_safe()
		{
			wait_();
			lock_(this);
		}
	};


	//class _Synchronized: public thread_safe<int>
	//{
	//public:
	//
	//	template<class _Func>
	//	void operator=(_Func&& func)
	//	{
	//		while(user_){
	//			//displn "";
	//		}
	//		user_ = this;
	//		func();
	//		user_ = nullptr;
	//	}
	//};
}
#endif //_X_THREAD_SAFE_H_
