#pragma once
using std::cout;
using std::endl;

class RefCounter
{
	private:
		unsigned _count;
	public:
		void addRef()
		{
			++_count;
		}
		unsigned decRef()
		{
			return --_count;
		}
};

template <typename T> class SmartPtr
{
	private:		
		T* _pointee;
		RefCounter* _count;
	public:
		explicit SmartPtr(T* pointee = 0): _pointee(pointee)
		{
			_count=new RefCounter;
			_count->addRef();
			return;
		}
		SmartPtr(SmartPtr<T>& smart): _pointee(smart._pointee) 
		{
			_count->addRef();
		}
		~SmartPtr()
		{
			if(_count->decRef() == 0)
			{
				delete _pointee;
				delete _count;
			}
			//_count->_count = _count->decRef();
		}
		SmartPtr<T>& operator=(SmartPtr<T>& smart)
		{
			if (this == &smart)
			{
				return *this;
			}
			if (_count->decRef() == 0)
			{
				delete _pointee;
				cout<<"The pointer you assign to was the last for his object. Old object deleted"<<endl;
			}
			_pointee = smart._pointee; 
			smart._count->addRef();
			_count = smart._count;
			return *this;
		}
		const T* showPointee() const
		{
			return _pointee;
		}
		T& operator*() const //return the &object but not change it with this operator;
		{	
			return *_pointee;
		}
		T* operator->() const
		{
			return _pointee;
		}
};


/*********************************** friend operators **************************************/
/* == != < > >= <= we need just < and == for all others to implement; isNULL sort of...*/

