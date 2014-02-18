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
		void addRef()
		{
		        ++*count();
		}
		void decRef()
		{
		        return --*count();
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
				delete *count();
				count() = smart.count();
				addRef();
				return;
			}
		}

		//we do not need to delete NULLpointers;
		~SmartPtr()
		{
			if(*count() == 1)
			{
				delete _pointee;
				delete _count;
				_pointee = NULL;
				_count = NULL;
				
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
				cout<<"The pointer you assign to was the last for his object. Old object deleted"<<endl;
			}
			decRef();
			pointee() = smart.pointee(); 
			smart.addRef();
			count() = smart.count();
			return *this;
		}
		const T* const pointee() const
		{
			return _pointee;
		}
		T& operator*() const
		{	
			assert(pointee() == 0);// we can not dereference nullptr
			return *pointee();
		}
		T* operator->() const
		{
			return pointee();
		}
		const bool isNull() cosnt 
		{
			return pointee() == NULL;
		}

};



/*********************************** friend operators **************************************/
/* == != < > >= <= we need just < and == for all others to implement; isNULL sort of...*/
