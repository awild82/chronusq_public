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
#ifndef __INCLUDED_SINGLESLATER_SCF_HPP__
#define __INCLUDED_SINGLESLATER_SCF_HPP__

#include <singleslater.hpp>
#include <util/matout.hpp>
#include <cqlinalg/blas1.hpp>
#include <cqlinalg/blasutil.hpp>

// SCF definitions for SingleSlaterBase
#include <singleslater/base/scf.hpp> 

namespace ChronusQ {

  /**
   *  \brief Saves the current state of wave function
   *
   *  Saves a copy of the current AO 1PDM and orthonormal Fock
   */ 
  template <typename T>
  void SingleSlater<T>::saveCurrentState() {


    // Checkpoint if file exists
    if( savFile.exists() ) {

      size_t NB = this->aoints.basisSet().nBasis;
      const std::array<std::string,4> spinLabel =
        { "SCALAR", "MZ", "MY", "MX" };

      // Save Matricies
      for(auto i = 0; i < this->fock.size(); i++) {

        savFile.safeWriteData("SCF/1PDM_" + spinLabel[i],
          this->onePDM[i],{NB,NB});

        savFile.safeWriteData("SCF/FOCK_" + spinLabel[i],
          this->fock[i],{NB,NB});

        savFile.safeWriteData("SCF/1PDM_ORTHO_" + spinLabel[i],
          this->onePDMOrtho[i],{NB,NB});

        savFile.safeWriteData("SCF/FOCK_ORTHO_" + spinLabel[i],
          this->fockOrtho[i],{NB,NB});

      }

      // Save Energies
      savFile.safeWriteData("SCF/TOTAL_ENERGY",&this->totalEnergy,
        {1});
      savFile.safeWriteData("SCF/ONE_BODY_ENERGY",&this->OBEnergy,
        {1});
      savFile.safeWriteData("SCF/MANY_BODY_ENERGY",&this->MBEnergy,
        {1});

      // Save Multipoles
      savFile.safeWriteData("SCF/LEN_ELECTRIC_DIPOLE",&this->elecDipole[0],
        {3});
      savFile.safeWriteData("SCF/LEN_ELECTRIC_QUADRUPOLE",
        &this->elecQuadrupole[0][0], {3,3});
      savFile.safeWriteData("SCF/LEN_ELECTRIC_OCTUPOLE",
        &this->elecOctupole[0][0][0], {3,3,3});

      // Save Spin
      savFile.safeWriteData("SCF/S_EXPECT",&this->SExpect[0],{3});
      savFile.safeWriteData("SCF/S_SQUARED",&this->SSq,{1});
      

    // If file doesnt exist, checkpoint important bits in core
    } else {

      size_t OSize = memManager.template getSize(fock[SCALAR]);

      // Copy over current AO density matrix
      for(auto i = 0; i < this->onePDM.size(); i++)
        std::copy_n(this->onePDM[i],OSize,curOnePDM[i]);

      // Copy the previous orthonormal Fock matrix for damping. It's the 
      // previous Fock since saveCurrentState is called at the beginning 
      // of the SCF loop. 
      if ( scfControls.doExtrap and scfControls.doDamp) {
          
        // Avoid saving the guess Fock for extrapolation
        if (scfConv.nSCFIter > 0) {
          for(auto i = 0; i < this->fockOrtho.size(); i++)
            std::copy_n(this->fockOrtho[i],OSize,prevFock[i]);
        }
      }

    }

  }; // SingleSlater<T>::saveCurrentState()





  /**
   *  \brief Obtain a new set of orbitals given a Fock matrix.
   *
   *  Currently implements the fixed-point SCF procedure.
   */ 
  template <typename T>
  void SingleSlater<T>::getNewOrbitals(EMPerturbation &pert, bool frmFock) {

    bool increment = scfControls.doIncFock and 
                     scfConv.nSCFIter % scfControls.nIncFock != 0 and
                     scfControls.guess != RANDOM;

    // Form the Fock matrix D(k) -> F(k)
    if( frmFock ) formFock(pert,increment);

    // Transform AO fock into the orthonormal basis
    ao2orthoFock();

    // Modify fock matrix if requested
    if( scfControls.doExtrap and frmFock ) modifyFock();

    // Diagonalize the orthonormal fock Matrix
    diagOrthoFock();

    // Form the orthonormal density (in the AO storage)
    formDensity();

    // Copy the AO storage to orthonormal storage and back transform
    // the density into the AO basis. This is because ortho2aoDen
    // requires the onePDMOrtho storage is populated.
    for(auto i = 0; i < this->onePDM.size(); i++)
      std::copy_n(this->onePDM[i],
        memManager.template getSize(onePDMOrtho[i]),
        onePDMOrtho[i]);

    // Transform the orthonormal density to the AO basis
    ortho2aoDen();

  }; // SingleSlater<T>::getNewOrbitals







  /**
   *  \brief Evaluate SCF convergence based on various criteria.
   *
   *  Checks the norm of [F,D], if converged -> SCF converged.
   *
   *  Checks change in energy and density between SCF iterations,
   *    if *both* converged -> SCF converged.
   */ 
  template <typename T>
  bool SingleSlater<T>::evalConver(EMPerturbation &pert) {

    // Check energy convergence
      
    // Save copy of old Energy
    double oldEnergy = this->totalEnergy;

    // Compute new energy (with new Density)
    this->computeProperties(pert);
    scfConv.deltaEnergy = this->totalEnergy - oldEnergy;

    bool energyConv = std::abs(scfConv.deltaEnergy) < scfControls.eneConvTol;


    // Check density convergence

    formDelta(); // Get change in density
    size_t DSize = memManager. template getSize(fock[SCALAR]);
    scfConv.RMSDenScalar = TwoNorm<double>(DSize,deltaOnePDM[SCALAR],1);
    scfConv.RMSDenMag = 0.;
    for(auto i = 1; i < deltaOnePDM.size(); i++)
      scfConv.RMSDenMag += std::pow(TwoNorm<double>(DSize,deltaOnePDM[i],1),2.);
    scfConv.RMSDenMag = std::sqrt(scfConv.RMSDenMag);
    

    bool denConv = scfConv.RMSDenScalar < scfControls.denConvTol;

    // Check FP convergence
    bool FDConv(false);

    bool isConverged = FDConv or (energyConv and denConv);

    // Toggle damping based on energy difference
    if( scfControls.doExtrap ) {
      // TODO: should enable print statements only when print flag is high 
      // enough 
      bool largeEDiff = std::abs(scfConv.deltaEnergy) > scfControls.dampError;
      if( scfControls.doDamp and not largeEDiff and 
          scfControls.dampParam > 0.) {

        if( printLevel > 0 )
          std::cout << 
            "    *** Damping Disabled - Energy Difference Fell Below " <<
            scfControls.dampError << " ***" << std::endl;

        scfControls.dampParam = 0.;

      } else if( scfControls.doDamp and largeEDiff and 
                 scfControls.dampParam <= 0.) {

        if( printLevel > 0 )
          std::cout << "    *** Damping Enabled Due to "<<
            scfControls.dampError << " Oscillation in Energy ***" << std::endl;

        scfControls.dampParam = scfControls.dampStartParam;

      }
    }

    return isConverged;

  }; // SingleSlater<T>::evalConver





  /**
   *  \brief Computes the change in the current wave function
   *
   *  Saves onePDM - curOnePDM in deltaOnePDM
   */ 
  template <typename T>
  void SingleSlater<T>::formDelta() {

    size_t NB = aoints.basisSet().nBasis;

    if( not savFile.exists() )
      for(auto i = 0; i < this->onePDM.size(); i++)
        MatAdd('N','N',NB,NB,T(1.),this->onePDM[i],NB,T(-1.),
          curOnePDM[i],NB,deltaOnePDM[i],NB);
    else {

      T* DENSCR = this->memManager.template malloc<T>(NB*NB);
      const std::array<std::string,4> spinLabel =
        { "SCALAR", "MZ", "MY", "MX" };

      for(auto i = 0; i < this->onePDM.size(); i++) {

        savFile.readData("/SCF/1PDM_" + spinLabel[i],DENSCR);

        MatAdd('N','N',NB,NB,T(1.),this->onePDM[i],NB,T(-1.),
          DENSCR,NB,deltaOnePDM[i],NB);
      }

      this->memManager.free(DENSCR);

    }

  }; // SingleSlater<T>:formDelta

  



  /**
   *  \brief Diagonalize the orthonormal fock matrix
   *
   *  General purpose routine which diagonalizes the orthonormal
   *  fock matrix and stores a set of orthonormal MO coefficients
   *  (in WaveFunction::mo1 and possibly WaveFunction::mo2) and
   *  orbital energies. General for both 1 and 2 spin components
   */ 
  template <typename T>
  void SingleSlater<T>::diagOrthoFock() {

    size_t NB = aoints.basisSet().nBasis * nC;
    size_t NB2 = NB*NB;

    // Copy over the fockOrtho into MO storage
    if(nC == 1 and iCS) 
      std::transform(fockOrtho[SCALAR],fockOrtho[SCALAR] + NB2,this->mo1,
        [](T a){ return a / 2.; }
      );
    else if(nC == 1)
      for(auto j = 0; j < NB2; j++) {
        this->mo1[j] = 0.5 * (fockOrtho[SCALAR][j] + fockOrtho[MZ][j]); 
        this->mo2[j] = 0.5 * (fockOrtho[SCALAR][j] - fockOrtho[MZ][j]); 
      }
    else { 

      SpinGather(NB/2,this->mo1,NB,fockOrtho[SCALAR],NB/2,fockOrtho[MZ],
        NB/2,fockOrtho[MY],NB/2,fockOrtho[MX],NB/2);

    }

    // Diagonalize the Fock Matrix
    int INFO = HermetianEigen('V', 'L', NB, this->mo1, NB, this->eps1, 
      memManager );
    if( INFO != 0 ) CErr("HermetianEigen failed in Fock1",std::cout);

    if(nC == 1 and not iCS) {
      INFO = HermetianEigen('V', 'L', NB, this->mo2, NB, this->eps2, 
        memManager );
      if( INFO != 0 ) CErr("HermetianEigen failed in Fock2",std::cout);
    }

#if 0
    printMO(std::cout);
#endif

  }; // SingleSlater<T>::diagOrthoFock


  /**
   *  \brief Transforms all of the spin components of the AO fock
   *  matrix to the orthonormal basis.
   *
   *  Populates / overwrites fockOrtho storage
   */ 
  template <typename T>
  void SingleSlater<T>::ao2orthoFock() {

    for(auto i = 0; i < fock.size(); i++)
      aoints.Ortho1Trans(fock[i],fockOrtho[i]);

  }; // SingleSlater<T>::ao2orthoFock



  /**
   *  \brief Transforms all of the spin compoenents of the orthonormal
   *  1PDM to the AO basis.
   *
   *  Populates / overwrites onePDM storage
   */ 
  template <typename T>
  void SingleSlater<T>::ortho2aoDen() {

    for(auto i = 0; i < onePDMOrtho.size(); i++)
      aoints.Ortho1Trans(onePDMOrtho[i],this->onePDM[i]);

#if 0
    print1PDMOrtho(std::cout);
#endif

  }; // SingleSlater<T>::ao2orthoFock

  
  template <typename T>
  void SingleSlater<T>::ortho2aoMOs() {

    size_t NB = aoints.basisSet().nBasis;

    T* SCR = this->memManager.template malloc<T>(this->nC*NB*NB);

    // Transform the (top half) of MO1
    Gemm('N','N',NB,this->nC*NB,NB,T(1.),aoints.ortho1,NB,
      this->mo1,this->nC*NB,T(0.),SCR,NB);
    SetMat('N',NB,this->nC*NB,T(1.),SCR,NB,this->mo1,this->nC*NB);
    
    if( this->nC == 2 ) {

      // Transform the bottom half of MO1
      Gemm('N','N',NB,this->nC*NB,NB,T(1.),aoints.ortho1,NB,
        this->mo1 + NB,this->nC*NB,T(0.),SCR,NB);
      SetMat('N',NB,this->nC*NB,T(1.),SCR,NB,this->mo1 + NB,this->nC*NB);

    } else if( not this->iCS ) {

      // Transform MO2
      Gemm('N','N',NB,NB,NB,T(1.),aoints.ortho1,NB,this->mo2,NB,T(0.),SCR,NB);
      SetMat('N',NB,NB,T(1.),SCR,NB,this->mo2,NB);

    }


    this->memManager.free(SCR);

#if 0
    // Check proper orthonormalized wrt overlap

    T* SCR2 = this->memManager.template malloc<T>(this->nC*this->nC*NB*NB);
    T* SCR3 = this->memManager.template malloc<T>(this->nC*this->nC*NB*NB);


    // MO1 inner product
    Gemm('N','N',NB,this->nC*NB,NB,T(1.),aoints.overlap,NB,
      this->mo1,this->nC*NB,T(0.),SCR2,this->nC*NB);
    if(this->nC == 2)
      Gemm('N','N',NB,this->nC*NB,NB,T(1.),aoints.overlap,NB,
        this->mo1+NB,this->nC*NB,T(0.),SCR2+NB,this->nC*NB);
   
    Gemm('C','N',this->nC*NB,this->nC*NB,this->nC*NB,T(1.),this->mo1,
      this->nC*NB,SCR2,this->nC*NB,T(0.),SCR3,this->nC*NB);

    for(auto i = 0; i < this->nC*NB; i++)
      SCR3[i*(this->nC*NB + 1)] -= 1.;


    std::cerr << "Error in orthonormazation of MO1 = " 
      << MatNorm<double>('F',this->nC*NB,this->nC*NB,SCR3,this->nC*NB)
      << std::endl;
             


    if(this->nC == 1 and not this->iCS) {
      Gemm('N','N',NB,NB,NB,T(1.),aoints.overlap,NB,this->mo2,NB,T(0.),SCR2,NB);
      Gemm('C','N',NB,NB,NB,T(1.),this->mo2,NB,SCR2,NB,T(0.),SCR3,NB);

      for(auto i = 0; i < this->nC*NB; i++)
        SCR3[i*(this->nC*NB + 1)] -= 1.;

      std::cerr << "Error in orthonormazation of MO2 = " 
        << MatNorm<double>('F',NB,NB,SCR3,NB) << std::endl;
    }

    this->memManager.free(SCR2,SCR3);

#endif

  }; // SingleSlater<T>::ortho2aoMOs


  /**
   *  \brief Initializes the environment for the SCF caluclation.
   *
   *  Allocate memory for extrapolation and compute the energy
   */ 
  template <typename T>
  void SingleSlater<T>::SCFInit() {

    // Allocate additional storage if doing some type of 
    // extrapolation during the SCF procedure
    if ( scfControls.doExtrap ) allocExtrapStorage();

  }; // SingleSlater<T>::SCFInit




  /**
   *  \brief Finalizes the environment for the SCF caluclation.
   *
   *  Deallocate the memory allocated for extrapolation.
   */ 
  template <typename T>
  void SingleSlater<T>::SCFFin() {

    ortho2aoMOs();

    // Deallocate extrapolation storage
    if ( scfControls.doExtrap ) deallocExtrapStorage();

  }; // SingleSlater<T>::SCFFin

}; // namespace ChronusQ

#endif
