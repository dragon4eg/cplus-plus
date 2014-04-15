
#include "lineSegm.h"

int main ()
{
	Segment tmps;
	SegmentSet db;
	string message;
	do
	{
		cout<<"Enter command and segment or type quit: \n";
		cin>>message;
		if (message[0] == 'q')
		{
			cout<<"Done!\n";
			break;
		}
		switch (message.at(0))
		{
			case 'f': find(message, tmps, db);
				break;
			case 'm': make(message, tmps, db);
				break;
			case 'r': remove(message, tmps, db);
				break;
		}
	}
	while(true);
	db.clear();
	return 0;
}
