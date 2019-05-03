#ifndef SIGMABASEOPERATOR_H
#define SIGMABASEOPERATOR_H

#include <string>

#include "SigmaBaseTable.h"

// operators: comparison (<, >, =), arithmetic, constant, logical (and, or)
//            also need one like SigmaColumnValueOperator which will take a
//            particular name in the constructor, encode what column it wants, 
//            get that row and send back the string

/*
                    < - comparison
				   / \	
	arithmetic -  +   10 - constant
				 / \
			  hour minute	- column names
			  
					or - logical
				 /       \	
	 		   =           = 
		      / \         / \
		   name "Pear"	name "Raspberry"
*/

class SigmaBaseOperator {
public:
	virtual SigmaBaseTblPtr fabricate() = 0;	
};

#endif