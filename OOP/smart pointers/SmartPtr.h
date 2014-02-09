#pragma once

template <typename T> class SmartPtr
{
	private:
		T* _pointee;
	public:
		explicit SmartPtr(T* pointee = 0): _pointee(pointee)
		{
			return;
		}
		SmartPtr(const SmartPtr<T>& smart): _pointee(smart._pointee) 
		{
		//я вже поняв шо ХУЙНЯ	
			smart._pointee = 0;
		}
		~SmartPtr()
		{
			delete _pointee;
		}
		SmartPtr<T>& operator=(const SmartPtr<T>& smart)
		{
			_pointee = smart._pointee;
			smart._pointee = 0;
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

