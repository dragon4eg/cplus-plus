#include "Reporter.h"
#include "SmartPtr.h"
#include <iostream>
using std::cout;
using std::endl;


int main()
{
	SmartPtr<Reporter>spr(new Reporter);
	cout<<sprshowPointee()<<endl;
	Reporter::report();
	cout<<endl;
	{
		cout<<"scope_act_1:"<<endl;
		SmartPtr<Reporter> sprInScope(new Reporter);
		cout<<endl;
		cout<<sprInScope.showPointee()<<endl;
		cout<<endl;
		cout<<"scope_act_2:"<<endl;
		SmartPtr<Reporter> sprInScopeCopy(sprInScope);
		cout<<sprInScope.showPointee()<<' '<<sprInScopeCopy.showPointee()<<endl;
		cout<<endl;
		cout<<"scope_act_3:"<<endl;
		sprInScopeCopy = sprInScope;//here's a situation with the memory leak
		cout<<sprInScope.showPointee()<<' '<<sprInScopeCopy.showPointee()<<endl;
		cout<<endl;
		cout<<"scope_act_4:"<<endl;
		spr = sprInScope;
		cout<<spr.showPointee()<<' '<<sprInScope.showPointee()<<endl;
		cout<<endl;

		Reporter::report();
		cout<<"scopeend:"<<endl;
		cout<<endl;
	}

Reporter::report();
Reporter::report(false);

return 0;
}

