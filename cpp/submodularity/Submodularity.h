
#ifndef __SUBMODULARITY__
#define __SUBMODULARITY__

#include <boost/dynamic_bitset.hpp>
#include <boost/exception/all.hpp>
#include <string>
#include <map>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace Submodularity{


	typedef boost::dynamic_bitset<> Set;
	typedef boost::tuple< Set, Set > Interval;
	typedef boost::tuple< Set, Set > Chain;
	typedef boost::dynamic_bitset<>::size_type Index;

	//! Check in interval [S,T] is S \subset T
	bool is_correct(const Interval interval);

	namespace CalculationPolicies{
		typedef boost::error_info<struct tag_my_info,std::string> submodularity_info;
		struct submodularity_error: virtual boost::exception, virtual std::exception { }; 
	
		struct DoNothing{
			static void error_behaviour(std::string s){
			}
		};
		struct ThrowAnException{
			static void error_behaviour(std::string s){
				throw submodularity_error() << submodularity_info(s);
			}
		};
		struct WriteToCerr{
			static void error_behaviour(std::string s){
				std::cerr << s << std::endl;
			}
		};
		struct WriteToCerrAndThrowAnException{
			static void error_behaviour(std::string s){
				WriteToCerr::error_behaviour(s);
				ThrowAnException::error_behaviour(s);
			}
		};
	};

	//! d_j^+	
	/** 
	* It is possible to speed up an execution
	* by changing "Set s" argument on "Set& s",
	* but we should take care about setting s[j] to 0 again
	*/
	template< class CalculationPolicy = CalculationPolicies::WriteToCerrAndThrowAnException ,
	class T , class SubmodularFunction >
	T incremental_value(Set s, Index j, SubmodularFunction submodularFunction){
		if(s[j]) CalculationPolicy::error_behaviour( "Error while calculating incremental value");
		T value( submodularFunction(s) );
		s[j] = 1;
		value = submodularFunction(s) - value;
		return value;
	}
	
	template < class CalculationPolicy = CalculationPolicies::WriteToCerrAndThrowAnException ,
	class T> 
	T incremental_value(Set s , Index i, T (* submodularFunction) (const Set& )){
		typedef T (* SMF_t) (const Set& );
		return incremental_value<CalculationPolicy, T, SMF_t>(s , i , submodularFunction);
	}
	
	
	//! d_j^-	
	/** 
	* It is possible to speed up an execution
	* by changing "Set s" argument on "Set& s",
	* but we should take care about setting s[j] to 1 again
	*/
	template< class CalculationPolicy = CalculationPolicies::WriteToCerrAndThrowAnException,
	class T , class SubmodularFunction >
	T decremental_value(Set s, Index j, SubmodularFunction submodularFunction){
		// alternative realisation
		/*
		if(s[j] == 0) CalculationPolisy :: error_behaviour( "Error while calculating decremental value");
		s[j] = 0;
		return (-1)*incremental_value(s,j,submodularFunction);
		*/
		if(s[j] == 0) CalculationPolicy :: error_behaviour( "Error while calculating decremental value");
		T value ( submodularFunction(s) );
		s[j] = 0;
		value = submodularFunction(s) - value;
		return value;
	}
	
	template < class CalculationPolicy = CalculationPolicies::WriteToCerrAndThrowAnException,
	class T> 
	T decremental_value(Set s , Index i, T (* submodularFunction) (const Set& )){
		typedef T (* SMF_t) (const Set& );
		return decremental_value< CalculationPolicy, T, SMF_t>(s , i , submodularFunction);
	}
	
	
	//! check if S is a local maximum
	template<class SubmodularFunction, class T>
	bool is_local_maximum(Set s, SubmodularFunction submodularFunction){
		T value = submodularFunction(s);
		Index end = s.size();
		for (Index i = 0; i < end; ++i){
			s[i] ^= 1;
			if(value < submodularFunction(s)) return false;
			s[i] ^= 1;
		}
		return true;
	}
	
	template <class T>
	bool is_local_maximum(Set s, T (*submodularFunction)(const Set&)){
		typedef T (* SMF_t) (const Set& );
		return is_local_maximum<T, SMF_t>(s,submodularFunction);
	}

	

};

#endif // __SUBMODULARITY__
