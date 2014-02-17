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

		T* pointee()
                {
                        return _pointee;
                }
                unsigned* count()
                {
                        return _count;
                }
		void addRef()
		{
		        ++*count();
		}
		unsigned decRef()
		{
		        return --*count();
		}
	public:
		explicit SmartPtr( T* const pointee = 0): _pointee(pointee), _count(new unsigned(0))
		{
			addRef();
			return;
		}
		SmartPtr(const SmartPtr<T>& smart): _pointee(smart._pointee) 
		{
			addRef();
                        assert(smart.pointee() == NULL);
		}
		~SmartPtr()
		{
			if(decRef() == 0)
			{
				delete _pointee;
				delete _count;
				_pointee = NULL;
				_count = NULL;
				
			}
		}
		SmartPtr<T>& operator=(SmartPtr<T>& smart)
		{
			if (this == &smart)
			{
				return *this;
			}
			if (decRef() == 0)
			{
				delete _pointee;
				pointee() = NULL;
				cout<<"The pointer you assign to was the last for his object. Old object deleted"<<endl;
			}
			pointee() = smart.pointee(); 
			smart.addRef();
			count() = smart.count();
			return *this;
		}
                const 
		const T* const pointee() const
		{
			return _pointee;
		}
		T& operator*() const
		{	
			return *pointee();
		}
		T* operator->() const
		{
			return pointee();
		}

};



/*********************************** friend operators **************************************/
/* == != < > >= <= we need just < and == for all others to implement; isNULL sort of...*/
