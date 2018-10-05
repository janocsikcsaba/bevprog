#include "std_lib_facilities.h"

int main(){
	
	double small = 200;
	double large = 0;
	double bb = 0;
	string unit = " ";
	vector<double> v ;
		
  while(unit != "quit"){ //quit
	cout<<"Enter a value : ";
	cin>>bb;
	cout<<"Enter a unit (cm,m,in,ft):";
	cin>>unit;
	if(unit == "m")
	{
		v.push_back(bb);
	}
	else if(unit == "cm")
	{
		v.push_back(bb/100);
	}
	else if(unit == "in")
	{
		v.push_back(bb*0.0254);
	}
	else if(unit == "ft")
	{
		v.push_back(bb*0.3048);
	}
	else
	{
		cout<<"Incorrect unit."<<endl;
	}
		if(bb<small)
		{
			small = bb;
		}
		else if (bb>large)
		{
			large = bb;
		}
	}
	sort(v);
	cout<<"smallest value : "<<small<<endl;
	cout<<"largest value : "<<large<<endl;
	cout<<"size of the vector : "<<v.size()<<endl;
	cout<<"values sorted : "<<endl;
	for(int i = 0;i<v.size();i++)
	{
	cout<<v[i]<<"m"<<endl; //vektor értékeinek kiíratása egyesével, novekvo sorrendben, méterben.
	}
	
	return 0;
}
