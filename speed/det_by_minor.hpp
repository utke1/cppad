# ifndef CPPAD_DET_BY_MINOR_INCLUDED
# define CPPAD_DET_BY_MINOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin DetByMinor$$
$spell
	typedef
	const
	hpp
	Det
	CppADvector
	namespace
$$

$section Determinant Using Expansion by Minors$$

$head Syntax$$
$syntax%# include <speed/det_by_minor.hpp>
%$$
$syntax%DetByMinor<%Scalar%, %Vector%> %det%(%n%)
%$$
$syntax%%d% = %det%(%matrix%)
%$$

$head Inclusion$$
The template class $code DetByMinor$$ is defined in the $code CppAD$$
namespace by including 
the file $code speed/det_by_minor.hpp$$
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$xref/cppad//CppAD.h/$$.

$head Constructor$$
The syntax
$syntax%
	DetByMinor<%Scalar%, %Vector%> %det%(%n%)
%$$
constructs the object $italic det$$ which can be used for 
evaluating the determinant of $italic n$$ by $italic n$$ matrices
using expansion by minors.

$head Scalar$$
The type $italic Scalar$$ can be any
$cref/NumericType/$$

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$

$head det$$
The syntax
$syntax%
	%d% = %det%(%matrix%)
%$$
returns the determinant of $italic matrix$$ using expansion by minors.
The argument $italic matrix$$ has prototype
$syntax%
	const %Vector% &%matrix%
%$$
It must be a $italic Vector$$ with length $latex n * n$$ and with
elements of type $italic Scalar$$.
The return value $italic d$$ has prototype
$syntax%
	%Scalar% %d%
%$$

$head Vector$$
If $italic y$$ is a $italic Vector$$ object, 
it must support the syntax
$syntax%
	%y%[%i%]
%$$
where $italic i$$ has type $code size_t$$ with value less than $latex n * n$$.
This must return a $italic Scalar$$ value corresponding the $th i$$
element of the vector $italic y$$.
This is the only requirement of the type $italic Vector$$.

$children%
	speed/example/det_by_minor.cpp
%$$


$head Example$$
The file
$xref/DetByMinor.cpp/$$ 
contains an example and test of $code det_by_minor.hpp$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file
$xref/det_by_minor.hpp/$$ 
contains the source for this template function.


$end
---------------------------------------------------------------------------
$begin det_by_minor.hpp$$
$spell
	Cpp
	ifndef
	endif
	Det
	const
$$

$index DetByMinor$$
$mindex determinant minor matrix$$
$section Determinant using Expansion by Minors: Source Code$$

$code
# ifndef CPPAD_DET_BY_MINOR_INCLUDED
$pre
$$
# define CPPAD_DET_BY_MINOR_INCLUDED

$verbatim%speed/det_by_minor.hpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$

# endif
$$

$end
---------------------------------------------------------------------------

*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <speed/det_of_minor.hpp>

// BEGIN CppAD namespace
namespace CppAD {

template <class Scalar, class Vector>
class DetByMinor {
public:
	DetByMinor(size_t m) : m_(m) , r_(m + 1) , c_(m + 1), a_(m * m)
	{
		size_t i;

		// values for r and c that correspond to entire matrix
		for(i = 0; i < m; i++)
		{	r_[i] = i+1;
			c_[i] = i+1;
		}
		r_[m] = 0;
		c_[m] = 0;
	}

	inline Scalar operator()(const Vector &x) const
	{	size_t i = m_ * m_;
		while(i--)
			a_[i] = x[i];
		return det_of_minor(a_, m_, m_, r_, c_); 
	}

private:
	size_t              m_;

	// made mutable because modified and then restored
	mutable std::vector<size_t> r_;
	mutable std::vector<size_t> c_;
	// make mutable because its value does not matter
	mutable std::vector<Scalar> a_;
};

} // END CppAD namespace

// END PROGRAM
# endif