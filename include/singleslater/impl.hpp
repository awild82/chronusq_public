/* 
 *  This file is part of the Chronus Quantum (ChronusQ) software package
 *  
 *  Copyright (C) 2014-2017 Li Research Group (University of Washington)
 *  
 *  This program is free software; you ca redistribute it and/or modify
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
#ifndef __INCLUDED_SINGLESLATER_IMPL_HPP__
#define __INCLUDED_SINGLESLATER_IMPL_HPP__

#include <singleslater.hpp>
#include <util/preprocessor.hpp>
#include <quantum/preprocessor.hpp>

// Template for a collective operation on the members of a 
// SingleSlater object
  
#define SingleSlater_COLLECTIVE_OP(OP_OP,OP_VEC_OP) \
  /* Handle Operators */\
  OP_VEC_OP(T,this,other,this->memManager,fock); \
  OP_VEC_OP(T,this,other,this->memManager,fockOrtho); \
  OP_OP(T,this,other,this->memManager,JScalar); \
  OP_VEC_OP(T,this,other,this->memManager,K); \
  OP_VEC_OP(T,this,other,this->memManager,PT);\
  OP_VEC_OP(T,this,other,this->memManager,onePDMOrtho);

namespace ChronusQ {

  /**
   *  Constructs a SingleSlater object to another of the same type by copy.
   *
   *  \param [in] other SingleSlater object to copy
   */ 
  template <typename T>
  SingleSlater<T>::SingleSlater(const SingleSlater<T> &other) : 
    WaveFunction<T>(dynamic_cast<const WaveFunction<T>&>(other)) {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater<T>::SingleSlater(const SingleSlater<T>&) " 
              << this << std::endl;
#endif

    SingleSlater_COLLECTIVE_OP(COPY_OTHER_MEMBER_OP, COPY_OTHER_MEMBER_VEC_OP);

  }; // SingleSlater<T>::SingleSlater(const SingleSlater<T> &)


  /**
   *  Constructs a SingleSlater object to another of a different type by copy.
   *
   *  \param [in] other SingleSlater object to copy
   */ 
  template <typename T>
  template <typename U>
  SingleSlater<T>::SingleSlater(const SingleSlater<U> &other) : 
    WaveFunction<T>(dynamic_cast<const WaveFunction<U>&>(other)) {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater<T>::SingleSlater(const SingleSlater<U>&) " 
              << this << std::endl;
#endif

    SingleSlater_COLLECTIVE_OP(COPY_OTHER_MEMBER_OP, COPY_OTHER_MEMBER_VEC_OP);

  }; // SingleSlater<T>::SingleSlater(const SingleSlater<U> &)



  /**
   *  Constructs a SingleSlater object to another of the same type by move.
   *
   *  \warning Deallocates the passed SingleSlater object
   *
   *  \param [in] other SingleSlater object to move
   */ 
  template <typename T>
  SingleSlater<T>::SingleSlater(SingleSlater<T> &&other) : 
    WaveFunction<T>(dynamic_cast<WaveFunction<T>&&>(other)) {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater<T>::SingleSlater(SingleSlater<T>&&) " << this 
              << std::endl;
#endif

    SingleSlater_COLLECTIVE_OP(MOVE_OTHER_MEMBER_OP, MOVE_OTHER_MEMBER_VEC_OP);

  }; // SingleSlater<T>::SingleSlater(SingleSlater<T> &&)
    

  /**
   *  Constructs a SingleSlater object to another of a different by move.
   *
   *  \warning Deallocates the passed SingleSlater object
   *
   *  \param [in] other SingleSlater object to move
   */ 
  template <typename T>
  template <typename U>
  SingleSlater<T>::SingleSlater(SingleSlater<U> &&other) : 
    WaveFunction<T>(dynamic_cast<WaveFunction<U>&&>(other)) {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater<T>::SingleSlater(SingleSlater<U>&&) " << this 
              << std::endl;
#endif

    SingleSlater_COLLECTIVE_OP(MOVE_OTHER_MEMBER_OP, MOVE_OTHER_MEMBER_VEC_OP);

  }; // SingleSlater<T>::SingleSlater(SingleSlater<U> &&)


  /**
   *  Allocates the internal memory a SingleSlater object
   */ 
  template <typename T>
  void SingleSlater<T>::alloc() {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater::alloc " << this << std::endl;
#endif

    size_t NB = this->aoints.basisSet().nBasis;

    SPIN_OPERATOR_ALLOC(NB,fock);
    SPIN_OPERATOR_ALLOC(NB,fockOrtho);
    SPIN_OPERATOR_ALLOC(NB,K);
    SPIN_OPERATOR_ALLOC(NB,PT);
    SPIN_OPERATOR_ALLOC(NB,onePDMOrtho);

    // J only has a scalar component
    JScalar = this->memManager.template malloc<T>(NB*NB);


  }; // SingleSlater<T>::alloc


  /**
   *  Deallocates the internal memory a SingleSlater object
   */ 
  template <typename T>
  void SingleSlater<T>::dealloc() {

#ifdef _SingleSlaterDebug
    std::cout << "SingleSlater::dealloc " << this << std::endl;
#endif

    SingleSlater_COLLECTIVE_OP(DEALLOC_OP_5, DEALLOC_VEC_OP_5);

  }; // SingleSlater<T>::dealloc

}; // namespace ChronusQ
#endif