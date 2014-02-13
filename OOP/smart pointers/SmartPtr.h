#pragma once
using std::cout;
using std::endl;

template <typename T> class SmartPtr
{
	private:		
		T* _pointee;
		unsigned* _count;
		
		void addRef()
		{
		++*_count;
		}
		unsigned decRef()
		{
		return --*_count;
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
				_pointee = NULL;
				cout<<"The pointer you assign to was the last for his object. Old object deleted"<<endl;
			}
			_pointee = smart._pointee; 
			smart.addRef();
			_count = smart._count;
			return *this;
		}
		const T* const showPointee() const
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

