/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# ifndef CPPAD_EXTERN_VALUE_INCLUDED
# define CPPAD_EXTERN_VALUE_INCLUDED

template <class Type> 
class extern_value  {
private:
	Type value_;
public:
	extern_value(Type value);
	void set(Type value);
	Type get(void);
};

# endif
