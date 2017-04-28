//#ifndef _X_SYNCHRONIZED_H_
//#define _X_SYNCHRONIZED_H_
//
//#include <mutex>
//#include "thread_safe.h"
//
//namespace x
//{
//	class _Lock
//	{
//		friend class _LockHandler;
//
//		volatile bool locked_;
//		void* volatile user_;
//
//		/*_Lock(size_t id):
//			id{id},
//			locked_{false},
//			user_{nullptr}
//		{
//		}*/
//
//	public:
//		_Lock():
//			locked_{false},
//			user_{nullptr}
//		{
//		}
//
//		//const size_t id;
//
//		__forceinline bool isLocked() const volatile
//		{
//			return locked_;
//		}
//
//		void lock(void* user)
//		{
//			while (locked_) {
//			}
//			locked_ = true;
//			user_ = user;
//		}
//
//		bool unlock(void* user)
//		{
//			if (user_ == user) {
//				user_ = nullptr;
//				locked_ = false;
//				return true;
//			}
//			return false;
//		}
//	};
//
//
//
//	template<class _Func>
//	class _ConcreteUser;
//
//	class _SyncBlockData
//	{
//	public:
//		_Lock* lock_;
//		void* user_;
//		void* func_;
//
//		__forceinline _SyncBlockData(_Lock* lock, void* user):
//			lock_{lock},
//			user_{user}
//		{
//		}
//
//		template<class _Func>
//		_ConcreteUser<_Func>* operator+ (_Func&& func)
//		{
//			func_ = &func_;
//			return static_cast<_ConcreteUser<_Func>*>(this);
//		}
//	};
//
//	template<class _Func>
//	class _ConcreteUser: public _SyncBlockData
//	{
//	public:
//		__forceinline void executeFunc()
//		{
//			(*(_Func*)func_)();
//		}
//	};
//
//
//	///////////////////////////////////////////////////////////////////////////////
//
//	
//	///////////////////////////////////////////////////////////////////////////////
//
//	class _LockHandler
//	{
//		static constexpr size_t DEFAULT_LOCKS_SIZE_ = 10;
//		static x::vector<_Lock*> locks_;
//
//	public:
//		static inline _Lock* createLock()
//		{
//			auto newLock = new _Lock;//{locks_.size()};
//			locks_.push_back(newLock);
//			return newLock;
//		}
//	};
//
//	x::vector<_Lock*> _LockHandler::locks_ = x::vector<_Lock*>(_LockHandler::DEFAULT_LOCKS_SIZE_);
//
//	/*__forceinline bool _Lock::isLocked() const
//	{
//		return _LockHandler::isLocked(this);
//	}*/
//
//	///////////////////////////////////////////////////////////////////////////////
//
//#define SYNC_TYPE 0
//
//#if SYNC_TYPE == 0
//
//	class _Synchronized
//	{	
//		std::mutex mutex_;
//
//	public:
//		template<class _Func>
//		__forceinline void operator=(_Func&& func)
//		{
//			//Sleep(1);
//			//std::lock_guard<std::mutex> lock{mutex_};
//			mutex_.lock();
//			func();
//			mutex_.unlock();
//			//mutex_.unlock();
//		}
//
//
//	};
//
//#define synchronized \
//	static x::_Synchronized __syncBlock__; \
//	__syncBlock__ = [&]() 
//
//#elif SYNC_TYPE == 1
//	class _Synchronized
//	{
//
//		static volatile _LockHandler locker_;
//
//	public:
//	/*	template<class _Func>
//		__forceinline void operator=(_Func&& func) volatile
//		{
//			while (locked_) {
//			}
//			locked_ = true;
//			func();
//			locked_ = false;
//		}*/
//
//
//		template<class _Func>
//		void operator=(_ConcreteUser<_Func>* data) volatile
//		{
//			data->lock_->lock(data->user_);
//			data->executeFunc();
//			data->lock_->unlock(data->user_);
//		}
//
//	};
//
//	#define synchronized(_lock) \
//		static volatile x::_Synchronized __syncBlock__; \
//	__syncBlock__ = x::_SyncBlockData{_lock, this} + [&]() 
//
//#endif
//
//
////#define synchronized \
////	static volatile x::_Synchronized __syncBlock__; \
////	if (__syncBlock__.enter()); else 
//
//}
//#endif //_X_SYNCHRONIZED_H_
