# ifndef CppADDetOfMinorIncluded
# define CppADDetOfMinorIncluded

// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin DetOfMinor$$
$spell
	std
	Det
	const
	CppADvector
	namespace
$$

$index DetOfMinor$$
$mindex determinant minor matrix$$
$section Compute the Determinant of a Minor$$

$table
$bold Syntax$$
$cnext $syntax%# include "example/DetOfMinor.h"%$$ 
$rnext
$cnext $syntax%%Type% DetOfMinor( 
	const CppADvector<%Type%> &%A%, 
	size_t %m%, 
	size_t %n%, 
	CppADvector<size_t> &%r%, 
	CppADvector<size_t> &%c%)%$$ 
$tend

$fend 20$$

$head Inclusion$$
The template function $code DetOfMinor$$ is defined in the $code CppAD$$
namespace by including 
the file $code example/DetOfMinor.h$$.
It is only intended for example and testing purposes, 
so it is not automatically included by
$xref/CppAD//CppAD.h/$$.

$head Description$$
This template function
returns the determinant of a minor of the matrix $italic A$$
using expansion by minors.
The minor $italic M$$ of the matrix $italic A$$ is defined by
$math%
	M(i, j) = A( R(i), C(j) )
%$$
where  the functions $math%R(i)%$$ and $math%C(j)%$$ are defined below.
Expansion by minors is chosen as an example because it uses
a lot of floating point operations yet does not require much source code
(on the order of $italic m$$ factorial floating point operations and 
about 75 lines of source code).
This is not an efficient method for computing a determinant;
for example, using an LU factorization would be better.

$head A$$
The $italic Type$$ vector defines 
an $italic m$$ by $italic m$$ matrix by
$math%
	A(i, j) = A[ i + j * m]
%$$

$head m$$
The size of the original matrix $italic A$$.

$head n$$
The size of the minor $italic M$$
(must be less than or equal $italic m$$).

$head r$$
This vector of length $math%m+1%$$ is a linked list that
defines the row indices of $italic A$$
that are included in the minor.
To be specific, we define the function $math%R(i)%$$ 
for $math%i=0, ... , n-1%$$ as follows:
$math%
	R(0)   = r[m]
	R(i+1) =  r[ R(i) ]
%$$
The $th i$$ row of the minor is a sub-vector of row index
$math%R(i)%$$ in $italic A$$.
(All the elements of $italic r$$ must be less than $italic m$$.)
The vector $italic r$$ is modified during the computation, but it is restored
to its original value before the return.

$head c$$
This vector of length $math%m+1%$$ is a linked list that
defines the column indices of $italic A$$
that are included in the minor.
To be specific, we define the function $math%C(i)%$$ 
for $math%i=0, ... , n-1%$$ as follows:
$math%
	C(0)   = c[m]
	C(j+1) =  c[ C(j) ]
%$$
The $th j$$ column of the minor is a sub-vector of column index
$math%C(j)%$$ in $italic A$$.
(All the elements of $italic c$$ must be less than $italic m$$.)
The vector $italic c$$ is modified during the computation, but it is restored
to its original value before the return.

$children%
	example/det_of_minor.cpp
%$$

$head Example$$
The file
$xref/DetOfMinor.cpp/$$ 
contains an example and a test of $code DetOfMinor.h$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file
$xref/DetOfMinor.h/$$ 
contains the source for this template function.


$end
---------------------------------------------------------------------------
$begin DetOfMinor.h$$
$spell
	Cpp
	ifndef
	endif
	Det
	const
$$

$index DetOfMinor$$
$mindex determinant minor matrix$$
$section Compute the Determinant of a Minor: Source Code$$

$comment This file is in the Example subdirectory$$
$code
# ifndef CppADDetOfMinorIncluded
$pre
$$
# define CppADDetOfMinorIncluded

$verbatim%example/DetOfMinor.h%0%// BEGIN PROGRAM%// END PROGRAM%1%$$

# endif
$$

$end
---------------------------------------------------------------------------
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>


// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
Type DetOfMinor(
	const CppADvector<Type> &A, 
	size_t m, 
	size_t n, 
	CppADvector<size_t> &r, 
	CppADvector<size_t> &c)
{	size_t         R0;  // R(0)
	size_t         Cj;  // C(j)
	size_t   previous;  // C(j) = c[previous]
	size_t          j;  // column index in M
	Type         detM;  // determinant of M
	Type         detS;  // determinant of sub-minor of M
	Type          m0j;  // an element of the first row of M
	int            s;   // sign of the current sub-minor in summation

	// indices in A of M(0, 0)
	R0 = r[m];
	Cj = c[m];

	// check for 1 by 1 case
	if( n == 1 )
		return A[ R0 * m + Cj ];

	/*
	compute determinant of M by expanding minors along its first row
	*/

	// initialize summation
	detM = 0;

	// initialize sign of permutation
	s = 1;

	// remove first row from sub-minors of M
	r[m] = r[R0];

	// initialize previous column index
	previous = m;

	// for each column of M
	for(j = 0; j < n; j++)
	{	// M(0, j)
		m0j = A[ R0 * m + Cj ];

		// remove column index j from sub-minor of M
		c[previous] = c[Cj];

		// determinant of subminor
		detS = DetOfMinor(A, m, n - 1, r, c);

		// restore column index j to sub-minor of M
		c[previous] = Cj;

		// include in summation
		if( s > 0 )
			detM = detM + m0j * detS;
		else
			detM = detM - m0j * detS;

		// advance to next column of M
		previous = Cj;
		Cj       = c[Cj];
		s        = -s;		
	}

	// restore column index zero to M
	r[m] = R0;

	return detM;
}

} // END CppAD namespace

// END PROGRAM
# endif