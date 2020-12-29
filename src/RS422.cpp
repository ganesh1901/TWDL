#include<RS422.h>



RS422::RS422()
{

}


RS422 *RS422::getRS422Instance()
{
	if(rs422_instance==NULL)
	{
		rs422_instance=new RS422();
	}
	return rs422_instance;
}


void RS422::check()
{
	cout<<"RS422 object created"<<endl;
}

