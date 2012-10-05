
#include "Submodularity.h"
#include <iostream>


using namespace std;
using namespace Submodularity;

int test_func(const Set& s){
	return s.find_first();
}

/*
namespace A{
	enum B{
		size = 100
	};
};
*/
int main(){

	Set set(2);
	int j = 0;
	set[0] = 1;
	cout << incremental_value<CalculationPolicies::DoNothing>(set,j,&test_func) << endl;
	cout << decremental_value(set,j,&test_func) << endl;
	cout << is_correct(tie(set,set)) << endl;
	
	cout << tie(set,set) << endl;
	
	//int a[A::B::size];
	
	return 0;
}




