#ifndef _MEMBER_ABSTRACT_H_
#define _MEMBER_ABSTRACT_H_



template<class _Owner, typename _Type>
class Member
{
public:
	using _MemberPtr = _Type(_Owner::*);

	template<_MemberPtr _member>
	class Abstract
	{
	public:
		virtual void apply(_Owner& object) const abstract;
	};

	template<_MemberPtr _member>
	class Const:
		public Abstract<_member>
	{
		_Type value_;

	public:
		Const(_Type value):
			value_{value}
		{
		}

		Const(Const<_member> const& other):
			value_{other.value_}
		{
		}

		virtual void apply(_Owner& object) const override
		{
			(object.*_member) = value_;
		}
	};

	template<_MemberPtr _member>
	class Tracker:
		public Abstract<_member>
	{
		_Owner const* trackedObject_;

	public:
		Tracker(_Owner const* trackedObject):
			trackedObject_{trackedObject}
		{
		}

		Tracker(Tracker<_member> const& other):
			trackedObject_{other.trackedObject_}
		{
		}

		virtual void apply(_Owner& object) const override
		{
			(object.*_member) = (trackedObject_->*_member);
		}
	};

	using _RelationFunc = _Type(*)(_Type const&, _Type const&);

	template<_MemberPtr _member, _RelationFunc _func>
	class Relative:
		public Abstract<_member>
	{
		_Owner const* trackedObject_;

	public:
		Relative(_Owner const* trackedObject):
			trackedObject_{trackedObject}
		{
		}

		Relative(Relative<_member, _func> const& other):
			trackedObject_{other.trackedObject_}
		{
		}

		virtual void apply(_Owner& object) const override
		{
			(object.*_member) = _func((object.*_member),(trackedObject_->*_member));
		}
	};
};

#endif //_MEMBER_ABSTRACT_H
