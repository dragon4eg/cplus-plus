template <class T>
class SmartPtr
{
	private:
		T* _pointee;
	public:
		explicit SmartPtr(T* pointee): _pointee(pointee);
		SmartPtr& operator=(const SmartPtr& pointer);
		~SmartPtr();
		T& operator*(); const //return the &object but not change it with this operator;
		}
			return *_pointee;
		}
		T* operator->() cosnt
		{
			return _pointee;
		}
};
