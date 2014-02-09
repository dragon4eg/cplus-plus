#pragma once

template <typename T> class SmartPtr
{
	private:
		T* _pointee;
		RefCounter* _count;
	public:
		explicit SmartPtr(): _pointee(0), _count(0)
		{
			_count = new RefCounter;
			_count->addRef();
		}
		explicit SmartPtr(T* pointee = 0): _pointee(pointee), _count(0)
		{
			_count = new RefCounter;
			_count->addRef();
		}
		SmartPtr(const SmartPtr<T>& smart): _pointee(smart._pointee), _count(smart._count)
		{
			_count->addRef();
		}
		~SmartPtr()
		{
			if (_count->Release() == 0)
			{
				delete _pointee;
				delete _count;
			}
		}
		SmartPtr<T>& operator=(const SmartPtr<T>& someSmartPtr)
		{
			//some conditions should exist for allowing freely do the next copying;
			_pointee = someSmartPtr._pointee;
			_count = someSmartPtr._count;
			_count->addRef();
			return *this;
		}

		T& operator*(); const //return the &object but not change it with this operator;
		}
			return *_pointee;
		}
		T* operator->() cosnt
		{
			return _pointee;
		}
};

/*********************************** friend operators **************************************/
/* == != < > >= <= we need just < and == for all others to implement; isNULL sort of...*/


/******************* reference counter holder class ****************************************/
class RefCounter
{
	private:
		size_t _count;
	public:
		size_t count()
		{
			return _count;
		}
		void addRef()
		{
			++count();
			return; 
		}
		size_t Release()
		{
			return --count();
		}
};
