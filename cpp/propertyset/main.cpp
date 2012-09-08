// Examples for using propertyset are containing here

#include<iostream>
#include <boost/bind.hpp>
#include"propertyset.h"

using namespace std;
using namespace cadovvl;

class A : public Property_set<std::string>{
	
	void set(int value){
		cout<< "setting" << endl;
		i = value;
	}
	int get(){
		cout << "getting" << endl;
		return i;
	}
	public:
	int i;
	A(){
		boost::function<void (int) > fs = boost::bind(&A::set , this, _1);
		boost::function<int (void) > fg = boost::bind(&A::get, this);
		add_property("int variable", fs, fg);
	}
};

int main(){
	A a;
	a.set_property("int variable", 2);
	cout << a.get_property<int>("int variable") << endl;

	a.set_property("???", 13.1);
	cout << a.get_property<double>("???") << endl;

	PropertyHelper<string> ph(a);
	
	ph["int variable"] = 5;
	cout << a.get_property<int>("int variable") << endl;
	
	ph["string variable"] = "Sample string";
	string s = ph["string variable"].as<string>();
	cout << s << endl;
	

	return 0;
}
