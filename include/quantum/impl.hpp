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
#ifndef __INCLUDED_QUANTUM_IMPL_HPP__
#define __INCLUDED_QUANTUM_IMPL_HPP__

#include <quantum.hpp>
#include <util/preprocessor.hpp>
#include <quantum/preprocessor.hpp>

// Template for a collective operation on the members of a 
// Quantum object
  
#define Form_1PDM_VEC() \
  if(onePDMScalar != nullptr) onePDM.emplace_back(onePDMScalar); \
  if(onePDMMz != nullptr) onePDM.emplace_back(onePDMMz); \
  if(onePDMMy != nullptr) onePDM.emplace_back(onePDMMy); \
  if(onePDMMx != nullptr) onePDM.emplace_back(onePDMMx); 

#define Quantum_COLLECTIVE_OP(OP_MEMBER,OP_OP) \
  /* Handle densities */\
  OP_OP(T,this,other,memManager,onePDMScalar); \
  OP_OP(T,this,other,memManager,onePDMMz); \
  OP_OP(T,this,other,memManager,onePDMMy); \
  OP_OP(T,this,other,memManager,onePDMMx); \
  \
  /* Handle Member data */\
  OP_MEMBER(this,other,elecDipole); \
  OP_MEMBER(this,other,elecQuadrupole); \
  OP_MEMBER(this,other,elecOctupole);\
  OP_MEMBER(this,other,SExpect); OP_MEMBER(this,other,SSq);



namespace ChronusQ {

  /**
   *  Constructs a Quantum object from another of a another (possibly the same) 
   *  type by copy.
   *
   *  \param [in] other Quantum object to copy
   *  \param [in] dummy Dummy argument to fix calling signature for delegation 
   *    to copy constructor
   */ 
  template <typename T>
  template <typename U>
  Quantum<T>::Quantum(const Quantum<U> &other, int dummy) : 
    Quantum<T>(other.memManager,other.nC,other.iCS,
    (other.onePDMScalar == nullptr) ? 
       0 : std::sqrt(other.memManager.template getSize<U>(other.onePDMScalar)),
    false
    ) {

    #ifdef _QuantumDebug
    std::cout << "Quantum<T>::Quantum(const Quantum<U>&) (this = " << this 
              << ", other = " << &other << ")" << std::endl;
    #endif

    Quantum_COLLECTIVE_OP(COPY_OTHER_MEMBER,COPY_OTHER_MEMBER_OP);
    Form_1PDM_VEC();

  }; // Quantum<T>::Quantum(const Quantum<U> &)
    

  /**
   *  Constructs a Quantum object from another of a another (possibly the same) 
   *  type by move.
   *
   *  \warning Deallocates the passed Quantum object
   *
   *  \param [in] other Quantum object to move
   *  \param [in] dummy Dummy argument to fix calling signature for delegation 
   *    to move constructor
   */ 
  template <typename T>
  template <typename U>
  Quantum<T>::Quantum(Quantum<U> &&other, int dummy) : 
    Quantum<T>(other.memManager,other.nC,other.iCS,
    (other.onePDMScalar == nullptr) ? 
       0 : std::sqrt(other.memManager.template getSize<U>(other.onePDMScalar)),
    false
    ) {

    #ifdef _QuantumDebug
    std::cout << "Quantum<T>::Quantum(Quantum<U>&&) (this = " << this 
              << ", other = " << &other << ")" << std::endl;
    #endif

    Quantum_COLLECTIVE_OP(MOVE_OTHER_MEMBER,MOVE_OTHER_MEMBER_OP);
    Form_1PDM_VEC();
    other.onePDM.clear();

  }; // Quantum<T>::Quantum(Quantum<U> &&)

  // Delagate the copy constructor to the conversion constructors
  template <typename T>
  Quantum<T>::Quantum(const Quantum<T> &other) : Quantum(other,0){ };
  template <typename T>
  Quantum<T>::Quantum(Quantum<T> &&other) : Quantum(std::move(other),0){ };




  /**
   *  Allocates the internal memory a Quantum object
   *
   *  \param [in] N Dimension of density matricies
   */ 
  template <typename T>
  void Quantum<T>::alloc(size_t N) {

    #ifdef _QuantumDebug
    std::cout << "Quantum::alloc (this = " << this << ")" << std::endl;
    #endif

    SPIN_OPERATOR_ALLOC(N,onePDM);

  }; // Quantum<T>::alloc


  /**
   *  Deallocates the internal memory a Quantum object
   */ 
  template <typename T>
  void Quantum<T>::dealloc() {

    #ifdef _QuantumDebug
    std::cout << "Quantum::dealloc (this = " << this << ")" << std::endl;
    #endif

    // Deallocate the 1PDM
    DEALLOC_VEC_OP(memManager,onePDM);

  }; // Quantum<T>::dealloc

}; // namespace ChronusQ

#endif
