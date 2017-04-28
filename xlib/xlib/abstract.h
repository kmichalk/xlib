#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "dynamic_ptr.h"
#include "typeenum.h"
#include "realcall.h"
#include "static_if.h"
#include "xstring.h"

namespace x
{
	template<class _Base>
	class Abstract
	{
		static_assert(
			std::is_class<_Base>::value,
			"Cannot apply Abstract wrapper. Specified base type is not class.");

		class TypeEraser
		{
		protected:
			template<class> friend class Abstract; 

			virtual TypeEraser* copy() const abstract;
			virtual _Base* copyObject(_Base const * object) const abstract;
			virtual bool is(unsigned type) const abstract;
			virtual void* cast(unsigned type, _Base* object) const abstract;
			virtual size_t offset() const abstract;
			virtual size_t size() const abstract;
			virtual x::string className() const abstract;


			virtual ~TypeEraser() = default;
		};

		template<class _Type>
		class Concrete: public TypeEraser
		{
		protected:
			virtual bool is(unsigned type) const override
			{
				return type == typenum(_Type);
			}

			virtual size_t size() const override
			{
				return sizeof(_Type);
			}

			virtual x::string className() const override
			{
				return typeid(_Type).name();
			}
		};

		template<class _Type> 
		class StaticConcrete: public Concrete<_Type>
		{
		protected:
			virtual TypeEraser* copy() const override
			{
				return new StaticConcrete<_Type>{};
			}

			virtual _Base* copyObject(_Base const * object) const override
			{
				return new _Type{*reinterpret_cast<_Type const*>(object)};
			}

			virtual void* cast(unsigned type, _Base* object) const override
			{
				return (realcall(this, is)(type) ? object : nullptr);
			}

			virtual size_t offset() const override
			{
				return 0;
			}

			virtual ~StaticConcrete() = default;
		};

		template<class _Type>
		class DynamicConcrete: public Concrete<_Type>
		{
		protected:
			size_t const offset_;

			inline _Type const* applyOffset_(_Base const* ptr) const
			{
				//std::cout<<"in DynamicConcrete applyOffset_: "<<ptr<<'\n';
				return reinterpret_cast<_Type const*>(
					reinterpret_cast<byte const*>(ptr) - offset_);
			}

			inline size_t calcOffset_(_Type const* ptr) const
			{
				return reinterpret_cast<byte const*>(static_cast<_Base const*>(ptr)) -
					reinterpret_cast<byte const*>(ptr);
			}

			virtual TypeEraser* copy() const override
			{
				return new DynamicConcrete<_Type>{*this};
			}

			virtual _Base* copyObject(_Base const * object) const override
			{
				//return new _Type{*applyOffset_(object)};
				return new _Type{*dynamic_cast<_Type const*>(object)};
			}

			virtual void* cast(unsigned type, _Base* object) const override
			{
				//std::cout<<"in DynamicConcrete cast: "<<object<<'\n';
				return (realcall(this, is)(type) ? 
					x::rem_const(applyOffset_(object)) : 
					nullptr);
			}

			virtual size_t offset() const override
			{
				return offset_;
			}

		public:
			DynamicConcrete() = delete;

			DynamicConcrete(DynamicConcrete const& other):
				offset_{other.offset_}
			{
				//std::cout<<"offset: "<<offset_<<'\n';
			}

			DynamicConcrete(_Type const* ptr):
				offset_{calcOffset_(ptr)}
			{
				//std::cout<<"dynamic concrete: "<<ptr<<" "<<offset_<<std::endl;
			}

			virtual ~DynamicConcrete() = default;
		};

		template<class>
		friend class Abstract;

		template<class _Type>
		static constexpr bool IS_SAME_OR_DERIVED_ =
			std::is_same<_Base, _Type>::value ||
			std::is_base_of<_Base, _Type>::value;

		template<class _Type>
		static constexpr bool IS_VOID_ =
			std::is_same<_Type, void>::value;


		_Base* object_;
		TypeEraser* typeEraser_;

		template<class _Type>
		static inline TypeEraser* matchEraser_(_Type const* ptr)
		{
			//std::cout<<"match eraser\n";
			return (ptr == reinterpret_cast<_Type const*>(static_cast<_Base const*>(ptr)) ? 
				static_cast<TypeEraser*>(new StaticConcrete<_Type>{}) : 
				static_cast<TypeEraser*>(new DynamicConcrete<_Type>{ptr}));
		}

		Abstract(_Base* ptr, TypeEraser* eraser)
			:
			object_		{ptr},
			typeEraser_	{eraser}
		{
		}

	public:
		Abstract():
			object_{nullptr},
			typeEraser_{nullptr}
		{
			//std::cout<<"A default "<<this<<"\n";
		}

		Abstract(Abstract<_Base> const& other)
			:
			object_{other.copy()},
			typeEraser_{other.typeEraser_->copy()}
		{
			//std::cout<<"A copy "<<this<<" \n"<<object_<<" "<<other.object_<<std::endl;
		}

		/*template<class _OtherBase>
		Abstract(Abstract<_OtherBase> & other)
		:
		object_		{other.copy()},
		typeEraser_	{other.typeEraser_->copy()}
		{
		//std::cout<<"A t copy\n"<<object_<<" "<<other.object_<<//std::endl;
		static_assert(
		IS_SAME_OR_DERIVED_<_OtherBase>,
		"Copied Abstract object must be of the same or derived class.");
		}*/

		template<class _OtherBase>
		Abstract(Abstract<_OtherBase> const & other)
			:
			object_{other.copy()},
			typeEraser_{other.typeEraser_->copy()}
		{
			//std::cout<<"A t copy c "<<this<<" \n";
			static_assert(
				IS_SAME_OR_DERIVED_<_OtherBase>,
				"Copied Abstract object must be of the same or derived class.");
		}

		Abstract(Abstract<_Base> && other)
			:
			object_{other.object_},
			typeEraser_{other.typeEraser_}
		{
			//std::cout<<"A move "<<this<<" \n";
			other.object_ = nullptr;
			other.typeEraser_ = nullptr;
		}

		template<class _OtherBase>
		Abstract(Abstract<_OtherBase> && other)
			:
			object_{other.object_},
			typeEraser_{other.typeEraser_}
		{
			//std::cout<<typeid(_OtherBase).name()<<"\n";
			//std::cout<<"A t move "<<this<<" \n";
			other.object_ = nullptr;
			other.typeEraser_ = nullptr;

			static_assert(
				IS_SAME_OR_DERIVED_<_OtherBase>,
				"Copied Abstract object must be of the same or derived class.");
		}

		template<class _Type>
		Abstract(x::dynamic_ptr<_Type> ptr)
			:
			object_{static_cast<_Base*>(ptr)},
			typeEraser_{matchEraser_(static_cast<_Type*>(ptr))}
		{
			//std::cout<<typeid(_Type).name()<<"\n";
			//std::cout<<"A dynamic_ptr "<<this<<" \n";
		}

	/*	template<class _Type, 
			_concept<!x::same_template<_Type,x::dynamic_ptr<_Type>>::value &&
				!x::same_template<_Type, Abstract<_Base>>::value>>*/
		template<class _Type, 
			_concept<IS_SAME_OR_DERIVED_<_Type>>>
		Abstract(_Type const& object)
			:
			object_{new _Type{object}},
			typeEraser_{matchEraser_(static_cast<_Type*>(object_))}
		{
			//std::cout<<typeid(_Type).name()<<"\n";
			//typeEraser_ = matchEraser_(static_cast<_Type*>(object_));
			//std::cout<<"A object const & ctor "<<this<<" \n";
		}

		//template<class _Type,
		//	_concept<IS_SAME_OR_DERIVED_<_Type>>>
		//	Abstract(_Type const& object)			
		//{
		//	object_ = static_cast<_Base*>(x::New<_Type>{object});
		//	typeEraser_ = matchEraser_(static_cast<_Type*>(object_));
		//	//std::cout<<typeid(_Type).name()<<"\n";
		//	//typeEraser_ = matchEraser_(static_cast<_Type*>(object_));
		//	//std::cout<<"A object const & ctor "<<this<<" \n";
		//}

		Abstract<_Base>& operator=(Abstract<_Base> const& other)
		{
			//std::cout<<"A operator= A\n";

			if (typeEraser_) delete typeEraser_;
			typeEraser_ = other.typeEraser_->copy();
			if (object_) delete object_;
			object_ = other.copy();

			return *this;
		}

		template<class _Assigned>
		Abstract<_Base>& operator=(Abstract<_Assigned> const& other)
		{
			//std::cout<<"A operator= A t\n";
			static_assert(
				IS_SAME_OR_DERIVED_<_Assigned>,
				"Assigned Abstract object must be of the same or derived class.");

			if (typeEraser_) delete typeEraser_;
			typeEraser_ = other.typeEraser_->copy();
			if (object_) delete object_;
			object_ = other.copy();

			return *this;
		}

		template<class _Assigned,
			_concept< !x::same_template<_Assigned, Abstract<_Base>>::value>>
			Abstract<_Base>& operator=(_Assigned const& object)
		{
			//std::cout<<"A operator= T\n";
			static_assert(
				IS_SAME_OR_DERIVED_<_Assigned>,
				"Assigned object must be of the same or derived class.");

			if (typeEraser_) delete typeEraser_;
			typeEraser_ = new StaticConcrete<_Assigned>{};
			if (object_) delete object_;
			object_ = new _Assigned{object};

			return *this;
		}

		template<class _Assigned>
		Abstract<_Base>& operator=(x::dynamic_ptr<_Assigned> object)
		{
			//std::cout<<"A operator= dp\n";
			static_assert(
				IS_SAME_OR_DERIVED_<_Assigned>,
				"Assigned pointer must be of the same or derived class.");

			if (typeEraser_) delete typeEraser_;
			typeEraser_ = new StaticConcrete<_Assigned>{};
			if (object_) delete object_;
			object_ = static_cast<_Assigned*>(object);

			return *this;
		}

		/*inline x::dynamic_ptr<_Base> copy() const
		{
			return //std::move(force_dynamic_(typeEraser_->copyObject(object_)));
		}*/

		/*inline Abstract<_Base> copy() const
		{
			return Abstract<_Base>{typeEraser_->copyObject(object_), typeEraser_->copy()};
		}*/

		inline x::dynamic_ptr<_Base> copy() const
		{
			/*//std::cout<<"copy "<<typeid(_Base).name()<<" " <<this<<'\n';
			if (//std::is_same<_Base, game::Object>::value) {
				//std::cout<<((game::Object*)object_)->shape_->getPosition().x <<","<<((game::Object*)object_)->shape_->getPosition().y<<'\n';
			}*/
			return x::dynamic_ptr<_Base>::force_dynamic(typeEraser_->copyObject(object_));
		}

		template<class _Checked>
		inline bool is() const
		{
			return (typeEraser_ ? typeEraser_->is(typenum(_Checked)) : false);
		}

		template<class _To>
		inline _To* cast()
		{
			//std::cout<<"cast: "<<object_<<'\n';
			return reinterpret_cast<_To*>(typeEraser_->cast(typenum(_To), object_));
		}

		inline size_t offset() const
		{
			return typeEraser_->offset();
		}

		inline size_t size() const
		{
			return typeEraser_->size();
		}

		inline x::string className() const
		{
			return typeEraser_->className();
		}

		/*inline unsigned typenum() const
		{
			return typeEraser_->typenum();
		}*/

		//template<_capture<_Base>, _concept<!IS_VOID_<_Base>>>
		inline _Base* operator->()
		{
			//std::cout<<"A ->\n";
			return object_;
		}

		//template<_capture<_Base>, _concept<!IS_VOID_<_Base>>>
		inline _Base const* operator->() const
		{
			//std::cout<<"A -> c\n";
			return object_;
		}

		//template<_capture<_Base>, _concept<!IS_VOID_<_Base>>>
		inline _Base& operator* ()
		{
			//std::cout<<"A *\n";
			return *object_;
		}

		//template<_capture<_Base>, _concept<!IS_VOID_<_Base>>>
		inline _Base const& operator* () const
		{
			//std::cout<<"A * c\n";
			return *object_;
		}

		template<class _Conv>
		inline operator _Conv* ()
		{
			//std::cout<<"A operator _Conv\n";
			return static_cast<_Conv*>(object_);
		}

		template<class _Conv>
		inline operator _Conv const* () const
		{
			//std::cout<<"A operator _Conv c\n";
			return static_cast<_Conv const*>(object_);
		}

		inline operator bool() const
		{
			return bool(object_);
		}

		~Abstract()
		{
			//std::cout<<"A destr "<<object_<< " "<<typeEraser_<<std::endl;
			if (object_) delete object_;
			if (typeEraser_) delete typeEraser_;
		}
	};

	/*template<class _Base>
	template<class _OtherBase>
	__forceinline Abstract<_Base>::TypeEraser::operator Abstract<_OtherBase>::TypeEraser * ()
	{
	return this;
	}*/
	//template<class _Base>
	//template<class _OtherBase>
	//inline Abstract<_Base>::TypeEraser::operator Abstract<_OtherBase>::TypeEraser*()
	//{
	//}*/
}
#endif //ABSTRACT_H
