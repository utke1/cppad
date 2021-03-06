/* $Id: sparse_hessian.cpp 3136 2014-03-02 11:54:07Z bradbell $ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/vector.hpp>

/*
$begin sacado_sparse_hessian.cpp$$
$spell
	boolsparsity
	const
	Sacado
	bool
	CppAD
$$

$section Sacado Speed: Sparse Hessian$$

$codep */
// A sacado version of this test is not yet implemented 
extern bool link_sparse_hessian(
        size_t                           size       ,
        size_t                           repeat     ,
        const CppAD::vector<size_t>&      row       ,
        const CppAD::vector<size_t>&      col       ,
        CppAD::vector<double>&            x         ,
        CppAD::vector<double>&            hessian   ,
        size_t&                           n_sweep
)
{
	return false;
}
/* $$
$end
*/
