#pragma once
#include <cstddef>
#include <cassert>
using std::cout;
using std::endl;

template <typename T> class SmartPtr
{
	private:	
		T* _pointee;
		unsigned* _count;

		T* pointee(){ return _pointee; }
		unsigned* count(){ return _count; }
		void addRef(){ ++*count(); }
		void decRef(){ --*count(); }
		const bool checkEqualCondition(const SmartPtr<T>& lhs, SmartPtr<T>& rhs)
		{
			return (lhs.pointee()==rhs.pointee() && lhs.count()==rhs.count());
		}
		void setFromTo(SmartPtr<T>& lhs, SmartPtr<T>& rhs)
		{
			rhs.pointee() = lhs.pointee();
			rhs.count() = lhs.count();
			lhs.addRef();
		}
	public:
		explicit SmartPtr( T* const pointee = NULL): _pointee(pointee), _count(new unsigned(0))
		{
			if (isNull())
			{
				return;
			}
			else
			{
				addRef();
				return;
			}
		}
		SmartPtr(const SmartPtr<T>& smart): _pointee(smart._pointee), _count(new unsigned(0)) 
		{	
			if (smart.isNull())
			{
				return;
			}	
			else
			{
				delete _count;
				setFromTo(smart, *this);
				assert(checkEqualCondition(*this, smart));
				return;
			}
		}

		//we do not need to delete NULLpointers;
		~SmartPtr()
		{
			if(*_count == 1)//if this is the last smart pointer to some object
			{
				delete _pointee;
				delete _count;
			}
			else
			{
				decRef();
			}
		}
		SmartPtr<T>& operator=(SmartPtr<T>& smart)
		{
			if (this == &smart)
			{
				return *this;
			}
			if (count() == 1)
			{
				delete pointee();
				pointee() = NULL;
				#ifdef DEBUG
					cout<<"The pointer you assign to was the last for his object. Old object deleted"<<endl;
				#endif
			}
			if(count() == 0)
			{
				assert(smart.isNull());
				setFromTo(smart,*this);
				return;
			}
			decRef();
			setFromTo(smart, *this);
			assert(checkEqualCondition(*this, smart));
			return *this;
		}

		const T* const pointee() const
		{
			return _pointee;
		}
		T& operator*() const
		{	
			assert(!isNull());// we can not dereference nullptr
			return *pointee();
		}
		T* operator->() const
		{
			return pointee();
		}
		const bool isNull() const
		{
			return pointee() == NULL;
		}
};

template <typename T>
const bool operator==(SmartPtr<T>& lhs, SmartPtr<T>& rhs)
{
	return lhs.pointee() == rhs.pointee();
}

template <typename T>
const bool operator!=(SmartPtr<T>& lhs, SmartPtr<T>& rhs)
{
	return !(lhs == rhs);
}
/*->"	 " - this is  tabulation symbol for pasting on ipad*/
