
#include "Submodularity.h"

// Put realisation here if nessesary

bool Submodularity::is_correct(const Interval interval){
	if(interval.get<0>().size() != interval.get<1>().size()) return false;
	return interval.get<0>().is_subset_of(interval.get<1>());
}

