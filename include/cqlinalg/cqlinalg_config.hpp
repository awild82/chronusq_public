/* 
 *  This file is part of the Chronus Quantum (ChronusQ) software package
 *  
 *  Copyright (C) 2014-2017 Li Research Group (University of Washington)
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *  
 *  Contact the Developers:
 *    E-Mail: xsli@uw.edu
 *  
 */
#ifndef __INCLUDED_CQLINALG_CONFIG_HPP__
#define __INCLUDED_CQLINALG_CONFIG_HPP__

#include <chronusq_sys.hpp>

// Choose linear algebra headers
#ifdef _CQ_MKL
  #define MKL_Complex16 dcomplex // Redefine MKL complex type
  #include <mkl.h> // MKL
#else
  #define lapack_complex_double dcomplex // Redefine OpenBLAS complex type
  #include <f77blas.h>
  #include <lapacke.h> // OpenBLAS
#endif

#include <memmanager.hpp>
#include <Eigen/Core>

#endif
