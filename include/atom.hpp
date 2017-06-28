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
#ifndef __INCLUDED_ATOM_HPP__
#define __INCLUDED_ATOM_HPP__

#include <chronusq_sys.hpp>

namespace ChronusQ {

  /**
   *  \brief The Atom struct. Contains pertinant information for the
   *  nuclear structure as it relates to the construction of a Molecule
   *  object.
   */ 
  struct Atom {
    size_t atomicNumber; ///< Atomic Number (# of protons)
    size_t massNumber;   ///< Mass Number
    double atomicMass;   ///< Atomic Mass (in a.u.)
    double slaterRadius; ///< Slater radius (in Bohr)

    std::array<double,3> coord; ///< X,Y,Z coordinates


    /**
     *  Coordinate array constructor (Default)
     *
     *  \param [in] AN   Atomic Number
     *  \param [in] MN   Mass Number
     *  \param [in] MASS Atomic Mass (A.U.)
     *  \param [in] RAD  Slater radium (Bohr)
     *  \param [in] XYZ  Cartesian atomic coordinates (X,Y,Z)
     */
    Atom(const size_t AN = 0, const size_t MN = 0, const double MASS = 0., 
      const double RAD = 0., std::array<double,3> XYZ = {0.,0.,0.}) :
      atomicNumber(AN), massNumber(MN), atomicMass(MASS), slaterRadius(RAD),
      coord(std::move(XYZ)){ };

    /**
     *  X,Y,Z constructor
     *
     *  \param [in] AN   Atomic Number
     *  \param [in] MN   Mass Number
     *  \param [in] MASS Atomic Mass (A.U.)
     *  \param [in] RAD  Slater radium (Bohr)
     *  \param [in] X    X cartesian coordinate of atomic coordinates
     *  \param [in] Y    Y cartesian coordinate of atomic coordinates
     *  \param [in] Z    Z cartesian coordinate of atomic coordinates
     */
    Atom(const size_t AN, const size_t MN, const double MASS, 
      const double RAD, double X, double Y, double Z) :
      Atom(AN,MN,MASS,RAD,{X,Y,Z}){ };

    /**
     *  Symbol + coordinate array constructor
     *
     *  \param [in] symb Atomic sybmol
     *  \param [in] XYZ  Cartesian atomic coordinates (X,Y,Z)
     */ 
    Atom(std::string symb, std::array<double,3> XYZ);

    /**
     *  Symbol + coordinate array constructor
     *
     *  \param [in] symb Atomic sybmol
     *  \param [in] X    X cartesian coordinate of atomic coordinates
     *  \param [in] Y    Y cartesian coordinate of atomic coordinates
     *  \param [in] Z    Z cartesian coordinate of atomic coordinates
     */ 
    Atom(std::string symb, double X = 0., double Y = 0., double Z = 0.) :
      Atom(symb,{X,Y,Z}){ };



    /**
     *  Copy constructor
     *
     *  Copies one Atom object to another
     */ 
    Atom(const Atom &) = default;

    /**
     *  Move constructor
     *
     *  Moves one Atom object to another
     */ 
    Atom(Atom &&)      = default;


    /**
     *  Copy assignment operator
     *
     *  Assigns one Atom object to another through a copy
     */ 
    Atom& operator=(const Atom &) = default;

    /**
     *  Move assignment operator
     *
     *  Assigns one Atom object to another through a move (rvalue reference)
     */ 
    Atom& operator=(Atom &&)      = default;

  }; // Atom struct


  // Map atomic symbol to predefined Atom objects. Support
  // for non-standard isotopes forthcoming
  //
  // FIXME: Need to specify isotope label for non-default isotopes
  static std::map<std::string,Atom> atomicReference(
    {
      { "H"    , { 1  , 1    , 1.0078250321   , 1.058  } }, 
      { "D"    , { 1  , 2    , 2.0141017780   , 1.058  } }, 
      { "T"    , { 1  , 3    , 3.0160492675   , 1.058  } }, 
      { "HE-3" , { 2  , 3    , 3.0160293097   , 0.310  } },
      { "HE"   , { 2  , 4    , 4.0026032497   , 0.310  } },
      { "LI-6" , { 3  , 6    , 6.0151223      , 1.450  } },
      { "LI"   , { 3  , 7    , 7.0160040      , 1.450  } },
      { "BE"   , { 4  , 9    , 9.0121821      , 1.050  } },
      { "B"    , { 5  , 10   , 10.012937      , 0.85   } },
      { "B-11" , { 5  , 11   , 11.0093055     , 0.85   } },
      { "C"    , { 6  , 12   , 12.0000000     , 0.70   } },
      { "C-13" , { 6  , 13   , 13.0033548378  , 0.70   } },
      { "C-14" , { 6  , 14   , 14.003241988   , 0.70   } },
      { "N"    , { 7  , 14   , 14.0030740052  , 0.65   } },
      { "N-15" , { 7  , 15   , 15.0001088984  , 0.65   } },
      { "O"    , { 8  , 16   , 15.9949146221  , 0.60   } },
      { "O-17" , { 8  , 17   , 16.99913150    , 0.60   } },
      { "O-18" , { 8  , 18   , 17.9991604     , 0.60   } },
      { "F"    , { 9  , 19   , 18.9984032     , 0.50   } },
      { "NE"   , { 10 , 20   , 19.9924401759  , 0.38   } },
      { "NE-"  , { 10 , 21   , 20.99384674    , 0.38   } },
      { "NE-"  , { 10 , 22   , 21.99138551    , 0.38   } },
      { "NA"   , { 11 , 23   , 22.98976967    , 1.80   } },
      { "MG"   , { 12 , 24   , 23.98504190    , 1.50   } },
      { "MG-"  , { 12 , 25   , 24.98583702    , 1.50   } },
      { "MG-"  , { 12 , 26   , 25.98259304    , 1.50   } },
      { "AL"   , { 13 , 27   , 26.98153844    , 1.25   } },
      { "SI"   , { 14 , 28   , 27.9769265327  , 1.10   } },
      { "SI-"  , { 14 , 29   , 28.97649472    , 1.10   } },
      { "SI-"  , { 14 , 30   , 29.97377022    , 1.10   } },
      { "P"    , { 15 , 31   , 30.97376151    , 1.00   } },
      { "S"    , { 16 , 32   , 31.97207069    , 1.00   } },
      { "S-"   , { 16 , 33   , 32.97145850    , 1.00   } },
      { "S-"   , { 16 , 34   , 33.96786683    , 1.00   } },
      { "S-"   , { 16 , 36   , 35.96708088    , 1.00   } },
      { "CL"   , { 17 , 35   , 34.96885271    , 1.00   } },
      { "CL-"  , { 17 , 37   , 36.96590260    , 1.00   } },
      { "AR-"  , { 18 , 36   , 35.96754628    , 0.71   } },
      { "AR-"  , { 18 , 38   , 37.9627322     , 0.71   } },
      { "AR"   , { 18 , 40   , 39.962383123   , 0.71   } },
      { "K"    , { 19 , 39,    38.9637069     , 2.20   } },
      { "K"    , { 19 , 40,    39.96399867    , 2.20   } },
      { "K"    , { 19 , 41,    40.96182597    , 2.20   } },
      { "CA"   , { 20 , 40,    39.9625912     , 1.80   } },
      { "CA"   , { 20 , 42,    41.9586183     , 1.80   } },
      { "CA"   , { 20 , 43,    42.9587668     , 1.80   } },
      { "CA"   , { 20 , 44,    43.9554811     , 1.80   } },
      { "CA"   , { 20 , 46,    45.9536928     , 1.80   } },
      { "CA"   , { 20 , 48,    47.952534      , 1.80   } },
      { "SC"   , { 21 , 45,    44.9559102     , 1.60   } },
      { "TI"   , { 22 , 46,    45.9526295     , 1.40   } },
      { "TI"   , { 22 , 47,    46.9517638     , 1.40   } },
      { "TI"   , { 22 , 48,    47.9479471     , 1.40   } },
      { "TI"   , { 22 , 49,    48.9478708     , 1.40   } },
      { "TI"   , { 22 , 50,    49.9447921     , 1.40   } },
      { "V"    , { 23 , 50,    49.9471628     , 1.35   } },
      { "V"    , { 23 , 51,    50.9439637     , 1.35   } },
      { "CR"   , { 24 , 50,    49.9460496     , 1.40   } },
      { "CR"   , { 24 , 52,    51.9405119     , 1.40   } },
      { "CR"   , { 24 , 53,    52.9406538     , 1.40   } },
      { "CR"   , { 24 , 54,    53.9388849     , 1.40   } },
      { "MN"   , { 25 , 55,    54.9380496     , 1.40   } },
      { "FE"   , { 26 , 54,    53.9396148     , 1.40   } },
      { "FE"   , { 26 , 56,    55.9349421     , 1.40   } },
      { "FE"   , { 26 , 57,    56.9353987     , 1.40   } },
      { "FE"   , { 26 , 58,    57.9332805     , 1.40   } },
      { "CO"   , { 27 , 59,    58.9332002     , 1.35   } },
      { "NI"   , { 28 , 58,    57.9353479     , 1.35   } },
      { "NI"   , { 28 , 60,    59.9307906     , 1.35   } },
      { "NI"   , { 28 , 61,    60.9310604     , 1.35   } },
      { "NI"   , { 28 , 62,    61.9283488     , 1.35   } },
      { "NI"   , { 28 , 64,    63.9279696     , 1.35   } },
      { "CU"   , { 29 , 63,    62.9296011     , 1.35   } },
      { "CU"   , { 29 , 65,    64.9277937     , 1.35   } },
      { "ZN"   , { 30 , 64,    63.9291466     , 1.35   } },
      { "ZN"   , { 30 , 66,    65.9260368     , 1.35   } },
      { "ZN"   , { 30 , 67,    66.9271309     , 1.35   } },
      { "ZN"   , { 30 , 68,    67.9248476     , 1.35   } },
      { "ZN"   , { 30 , 70,    69.925325      , 1.35   } },
      { "GA"   , { 31 , 69,    68.925581      , 1.30   } },
      { "GA"   , { 31 , 71,    70.9247050     , 1.30   } },
      { "GE"   , { 32 , 70,    69.9242504     , 1.25   } },
      { "GE"   , { 32 , 72,    71.9220762     , 1.25   } },
      { "GE"   , { 32 , 73,    72.9234594     , 1.25   } },
      { "GE"   , { 32 , 74,    73.9211782     , 1.25   } },
      { "GE"   , { 32 , 76,    75.9214027     , 1.25   } },
      { "AS"   , { 33 , 75,    74.9215964     , 1.15   } },
      { "SE"   , { 34 , 74,    73.9224766     , 1.15   } },
      { "SE"   , { 34 , 76,    75.9192141     , 1.15   } },
      { "SE"   , { 34 , 77,    76.9199146     , 1.15   } },
      { "SE"   , { 34 , 78,    77.9173095     , 1.15   } },
      { "SE"   , { 34 , 80,    79.9165218     , 1.15   } },
      { "SE"   , { 34 , 82,    81.9167000     , 1.15   } },
      { "BR"   , { 35 , 79,    78.9183376     , 1.15   } },
      { "BR"   , { 35 , 81,    80.916291      , 1.15   } },
      { "KR"   , { 36 , 78,    77.920386      , 0.88   } },
      { "KR"   , { 36 , 80,    79.916378      , 0.88   } },
      { "KR"   , { 36 , 82,    81.9134846     , 0.88   } },
      { "KR"   , { 36 , 83,    82.914136      , 0.88   } },
      { "KR"   , { 36 , 84,    83.911507      , 0.88   } },
      { "KR"   , { 36 , 86,    85.9106103     , 0.88   } },
      { "RB"   , { 37 , 85,    84.9117893     , 2.35   } },
      { "RB"   , { 37 , 87,    86.9091835     , 2.35   } },
      { "SR"   , { 38 , 84,    83.913425      , 2.00   } },
      { "SR"   , { 38 , 86,    85.9092624     , 2.00   } },
      { "SR"   , { 38 , 87,    86.9088793     , 2.00   } },
      { "SR"   , { 38 , 88,    87.9056143     , 2.00   } },
      { "Y"    , { 39 , 89,    88.9058479     , 1.80   } },
      { "ZR"   , { 40 , 90,    89.9047037     , 1.55   } },
      { "ZR"   , { 40 , 91,    90.9056450     , 1.55   } },
      { "ZR"   , { 40 , 92,    91.9050401     , 1.55   } },
      { "ZR"   , { 40 , 94,    93.9063158     , 1.55   } },
      { "ZR"   , { 40 , 96,    95.908276      , 1.55   } },
      { "NB"   , { 41 , 93,    92.9063775     , 1.45   } },
      { "MO"   , { 42 , 92,    91.906810      , 1.45   } },
      { "MO"   , { 42 , 94,    93.9050876     , 1.45   } },
      { "MO"   , { 42 , 95,    94.9058415     , 1.45   } },
      { "MO"   , { 42 , 96,    95.9046789     , 1.45   } },
      { "MO"   , { 42 , 97,    96.9060210     , 1.45   } },
      { "MO"   , { 42 , 98,    97.9054078     , 1.45   } },
      { "MO"   , { 42 , 100  , 99.907477      , 1.45   } },
      { "TC"   , { 43 , 97,    96.906365      , 1.35   } },
      { "TC"   , { 43 , 98,    97.907216      , 1.35   } },
      { "TC"   , { 43 , 99,    98.9062546     , 1.35   } },
      { "RU"   , { 44 , 96,    95.907598      , 1.30   } },
      { "RU"   , { 44 , 98,    97.905287      , 1.30   } },
      { "RU"   , { 44 , 99,    98.9059393     , 1.30   } },
      { "RU"   , { 44 , 100  , 99.9042197     , 1.30   } },
      { "RU"   , { 44 , 101  , 100.9055822    , 1.30   } },
      { "RU"   , { 44 , 102  , 101.9043495    , 1.30   } },
      { "RU"   , { 44 , 104  , 103.905430     , 1.30   } },
      { "RH"   , { 45 , 103  , 102.905504     , 1.35   } },
      { "PD"   , { 46 , 102  , 101.905608     , 1.40   } },
      { "PD"   , { 46 , 104  , 103.904035     , 1.40   } },
      { "PD"   , { 46 , 105  , 104.905084     , 1.40   } },
      { "PD"   , { 46 , 106  , 105.903483     , 1.40   } },
      { "PD"   , { 46 , 108  , 107.903894     , 1.40   } },
      { "PD"   , { 46 , 110  , 109.905152     , 1.40   } },
      { "AG"   , { 47 , 107  , 106.905093     , 1.60   } },
      { "AG"   , { 47 , 109  , 108.904756     , 1.60   } },
      { "CD"   , { 48 , 106  , 105.906458     , 1.55   } },
      { "CD"   , { 48 , 108  , 107.904183     , 1.55   } },
      { "CD"   , { 48 , 110  , 109.903006     , 1.55   } },
      { "CD"   , { 48 , 111  , 110.904182     , 1.55   } },
      { "CD"   , { 48 , 112  , 111.9027572    , 1.55   } },
      { "CD"   , { 48 , 113  , 112.9044009    , 1.55   } },
      { "CD"   , { 48 , 114  , 113.9033581    , 1.55   } },
      { "CD"   , { 48 , 116  , 115.904755     , 1.55   } },
      { "IN"   , { 49 , 113  , 112.904061     , 1.55   } },
      { "IN"   , { 49 , 115  , 114.903878     , 1.55   } },
      { "SN"   , { 50 , 112  , 111.904821     , 1.45   } },
      { "SN"   , { 50 , 114  , 113.902782     , 1.45   } },
      { "SN"   , { 50 , 115  , 114.903346     , 1.45   } },
      { "SN"   , { 50 , 116  , 115.901744     , 1.45   } },
      { "SN"   , { 50 , 117  , 116.902954     , 1.45   } },
      { "SN"   , { 50 , 118  , 117.901606     , 1.45   } },
      { "SN"   , { 50 , 119  , 118.903309     , 1.45   } },
      { "SN"   , { 50 , 120  , 119.9021966    , 1.45   } },
      { "SN"   , { 50 , 122  , 121.9034401    , 1.45   } },
      { "SN"   , { 50 , 124  , 123.9052746    , 1.45   } },
      { "SB"   , { 51 , 121  , 120.9038180    , 1.45   } },
      { "SB"   , { 51 , 123  , 122.9042157    , 1.45   } },
      { "TE"   , { 52 , 120  , 119.904020     , 1.40   } },
      { "TE"   , { 52 , 122  , 121.9030471    , 1.40   } },
      { "TE"   , { 52 , 123  , 122.9042730    , 1.40   } },
      { "TE"   , { 52 , 124  , 123.9028195    , 1.40   } },
      { "TE"   , { 52 , 125  , 124.9044247    , 1.40   } },
      { "TE"   , { 52 , 126  , 125.9033055    , 1.40   } },
      { "TE"   , { 52 , 128  , 127.9044614    , 1.40   } },
      { "TE"   , { 52 , 130  , 129.9062228    , 1.40   } },
      { "I"    , { 53 , 127  , 126.904468     , 1.40   } },
      { "XE"   , { 54 , 124  , 123.9058958    , 1.08   } },
      { "XE"   , { 54 , 126  , 125.904269     , 1.08   } },
      { "XE"   , { 54 , 128  , 127.9035304    , 1.08   } },
      { "XE"   , { 54 , 129  , 128.9047795    , 1.08   } },
      { "XE"   , { 54 , 130  , 129.9035079    , 1.08   } },
      { "XE"   , { 54 , 131  , 130.9050819    , 1.08   } },
      { "XE"   , { 54 , 132  , 131.9041545    , 1.08   } },
      { "XE"   , { 54 , 134  , 133.9053945    , 1.08   } },
      { "XE"   , { 54 , 136  , 135.907220     , 1.08   } },
      { "CS"   , { 55 , 133  , 132.905447     , 2.60   } },
      { "BA"   , { 56 , 130  , 129.906310     , 2.15   } },
      { "BA"   , { 56 , 132  , 131.905056     , 2.15   } },
      { "BA"   , { 56 , 134  , 133.904503     , 2.15   } },
      { "BA"   , { 56 , 135  , 134.905683     , 2.15   } },
      { "BA"   , { 56 , 136  , 135.904570     , 2.15   } },
      { "BA"   , { 56 , 137  , 136.905821     , 2.15   } },
      { "BA"   , { 56 , 138  , 137.905241     , 2.15   } },
      { "LA"   , { 57 , 138  , 137.907107     , 1.95   } },
      { "LA"   , { 57 , 139  , 138.906348     , 1.95   } },
      { "CE"   , { 58 , 136  , 135.907140     , 1.85   } },
      { "CE"   , { 58 , 138  , 137.905986     , 1.85   } },
      { "CE"   , { 58 , 140  , 139.905434     , 1.85   } },
      { "CE"   , { 58 , 142  , 141.909240     , 1.85   } },
      { "PR"   , { 59 , 141  , 140.907648     , 1.85   } },
      { "ND"   , { 60 , 142  , 141.907719     , 1.85   } },
      { "ND"   , { 60 , 143  , 142.909810     , 1.85   } },
      { "ND"   , { 60 , 144  , 143.910083     , 1.85   } },
      { "ND"   , { 60 , 145  , 144.912569     , 1.85   } },
      { "ND"   , { 60 , 146  , 145.913112     , 1.85   } },
      { "ND"   , { 60 , 148  , 147.916889     , 1.85   } },
      { "ND"   , { 60 , 150  , 149.920887     , 1.85   } },
      { "PM"   , { 61 , 145  , 144.912744     , 1.85   } },
      { "PM"   , { 61 , 147  , 146.915134     , 1.85   } },
      { "SM"   , { 62 , 144  , 143.911995     , 1.85   } },
      { "SM"   , { 62 , 147  , 146.914893     , 1.85   } },
      { "SM"   , { 62 , 148  , 147.914818     , 1.85   } },
      { "SM"   , { 62 , 149  , 148.917180     , 1.85   } },
      { "SM"   , { 62 , 150  , 149.917271     , 1.85   } },
      { "SM"   , { 62 , 152  , 151.919728     , 1.85   } },
      { "SM"   , { 62 , 154  , 153.922205     , 1.85   } },
      { "EU"   , { 63 , 151  , 150.919846     , 1.85   } },
      { "EU"   , { 63 , 153  , 152.921226     , 1.85   } },
      { "GD"   , { 64 , 152  , 151.919788     , 1.80   } },
      { "GD"   , { 64 , 154  , 153.920862     , 1.80   } },
      { "GD"   , { 64 , 155  , 154.922619     , 1.80   } },
      { "GD"   , { 64 , 156  , 155.922120     , 1.80   } },
      { "GD"   , { 64 , 157  , 156.923957     , 1.80   } },
      { "GD"   , { 64 , 158  , 157.924101     , 1.80   } },
      { "GD"   , { 64 , 160  , 159.927051     , 1.80   } },
      { "TB"   , { 65 , 159  , 158.925343     , 1.75   } },
      { "DY"   , { 66 , 156  , 155.924278     , 1.75   } },
      { "DY"   , { 66 , 158  , 157.924405     , 1.75   } },
      { "DY"   , { 66 , 160  , 159.925194     , 1.75   } },
      { "DY"   , { 66 , 161  , 160.926930     , 1.75   } },
      { "DY"   , { 66 , 162  , 161.926795     , 1.75   } },
      { "DY"   , { 66 , 163  , 162.928728     , 1.75   } },
      { "DY"   , { 66 , 164  , 163.929171     , 1.75   } },
      { "HO"   , { 67 , 165  , 164.930319     , 1.75   } },
      { "ER"   , { 68 , 162  , 161.928775     , 1.75   } },
      { "ER"   , { 68 , 164  , 163.929197     , 1.75   } },
      { "ER"   , { 68 , 166  , 165.930290     , 1.75   } },
      { "ER"   , { 68 , 167  , 166.932045     , 1.75   } },
      { "ER"   , { 68 , 168  , 167.932368     , 1.75   } },
      { "ER"   , { 68 , 170  , 169.935460     , 1.75   } },
      { "TM"   , { 69 , 169  , 168.934211     , 1.75   } },
      { "YB"   , { 70 , 168  , 167.933894     , 1.75   } },
      { "YB"   , { 70 , 170  , 169.934759     , 1.75   } },
      { "YB"   , { 70 , 171  , 170.936322     , 1.75   } },
      { "YB"   , { 70 , 172  , 171.9363777    , 1.75   } },
      { "YB"   , { 70 , 173  , 172.9382068    , 1.75   } },
      { "YB"   , { 70 , 174  , 173.9388581    , 1.75   } },
      { "YB"   , { 70 , 176  , 175.942568     , 1.75   } },
      { "LU"   , { 71 , 175  , 174.9407679    , 1.75   } },
      { "LU"   , { 71 , 176  , 175.9426824    , 1.75   } },
      { "HF"   , { 72 , 174  , 173.940040     , 1.55   } },
      { "HF"   , { 72 , 176  , 175.9414018    , 1.55   } },
      { "HF"   , { 72 , 177  , 176.9432200    , 1.55   } },
      { "HF"   , { 72 , 178  , 177.9436977    , 1.55   } },
      { "HF"   , { 72 , 179  , 178.9458151    , 1.55   } },
      { "HF"   , { 72 , 180  , 179.9465488    , 1.55   } },
      { "TA"   , { 73 , 180  , 179.947466     , 1.45   } },
      { "TA"   , { 73 , 181  , 180.947996     , 1.45   } },
      { "W"    , { 74 , 180  , 179.946706     , 1.35   } },
      { "W"    , { 74 , 182  , 181.948206     , 1.35   } },
      { "W"    , { 74 , 183  , 182.9502245    , 1.35   } },
      { "W"    , { 74 , 184  , 183.9509326    , 1.35   } },
      { "W"    , { 74 , 186  , 185.954362     , 1.35   } },
      { "RE"   , { 75 , 185  , 184.9529557    , 1.35   } },
      { "RE"   , { 75 , 187  , 186.9557508    , 1.35   } },
      { "OS"   , { 76 , 184  , 183.952491     , 1.30   } },
      { "OS"   , { 76 , 186  , 185.953838     , 1.30   } },
      { "OS"   , { 76 , 187  , 186.9557479    , 1.30   } },
      { "OS"   , { 76 , 188  , 187.9558360    , 1.30   } },
      { "OS"   , { 76 , 189  , 188.9581449    , 1.30   } },
      { "OS"   , { 76 , 190  , 189.958445     , 1.30   } },
      { "OS"   , { 76 , 192  , 191.961479     , 1.30   } },
      { "IR"   , { 77 , 191  , 190.960591     , 1.35   } },
      { "IR"   , { 77 , 193  , 192.962924     , 1.35   } },
      { "PT"   , { 78 , 190  , 189.959930     , 1.35   } },
      { "PT"   , { 78 , 192  , 191.961035     , 1.35   } },
      { "PT"   , { 78 , 194  , 193.962664     , 1.35   } },
      { "PT"   , { 78 , 195  , 194.964774     , 1.35   } },
      { "PT"   , { 78 , 196  , 195.964935     , 1.35   } },
      { "PT"   , { 78 , 198  , 197.967876     , 1.35   } },
      { "AU"   , { 79 , 197  , 196.966552     , 1.35   } },
      { "HG"   , { 80 , 196  , 195.965815     , 1.50   } },
      { "HG"   , { 80 , 198  , 197.966752     , 1.50   } },
      { "HG"   , { 80 , 199  , 198.968262     , 1.50   } },
      { "HG"   , { 80 , 200  , 199.968309     , 1.50   } },
      { "HG"   , { 80 , 201  , 200.970285     , 1.50   } },
      { "HG"   , { 80 , 202  , 201.970626     , 1.50   } },
      { "HG"   , { 80 , 204  , 203.973476     , 1.50   } },
      { "TL"   , { 81 , 203  , 202.972329     , 1.90   } },
      { "TL"   , { 81 , 205  , 204.974412     , 1.90   } },
      { "PB"   , { 82 , 204  , 203.973029     , 1.90   } },
      { "PB"   , { 82 , 206  , 205.974449     , 1.90   } },
      { "PB"   , { 82 , 207  , 206.975881     , 1.90   } },
      { "PB"   , { 82 , 208  , 207.976636     , 1.90   } },
      { "BI"   , { 83 , 209  , 208.980383     , 1.60   } },
      { "PO"   , { 84 , 209  , 208.982416     , 1.90   } },
      { "PO"   , { 84 , 210  , 209.982857     , 1.90   } },
      { "AT"   , { 85 , 210  , 209.987131     , 1.90   } },
      { "AT"   , { 85 , 211  , 210.987481     , 1.90   } },
      { "RN"   , { 86 , 211  , 210.990585     , 2.01   } },
      { "RN"   , { 86 , 220  , 220.0113841    , 2.01   } },
      { "RN"   , { 86 , 222  , 222.0175705    , 2.01   } },
      { "FR"   , { 87 , 223  , 223.0197307    , 1.90   } },
      { "RA"   , { 88 , 223  , 223.018497     , 1.90   } },
      { "RA"   , { 88 , 224  , 224.0202020    , 1.90   } },
      { "RA"   , { 88 , 226  , 226.0254026    , 2.15   } },
      { "RA"   , { 88 , 228  , 228.0310641    , 2.15   } },
      { "AC"   , { 89 , 227  , 227.0277470    , 1.95   } },
      { "TH"   , { 90 , 230  , 230.0331266    , 1.80   } },
      { "TH"   , { 90 , 232  , 232.0380504    , 1.80   } },
      { "PA"   , { 91 , 231  , 231.0358789    , 1.80   } },
      { "U"    , { 92 , 233  , 233.039628     , 1.75   } },
      { "U"    , { 92 , 234  , 234.0409456    , 1.75   } },
      { "U"    , { 92 , 235  , 235.0439231    , 1.75   } },
      { "U"    , { 92 , 236  , 236.0455619    , 1.75   } },
      { "U"    , { 92 , 238  , 238.0507826    , 1.75   } },
      { "NP"   , { 93 , 237  , 237.0481673    , 1.75   } },
      { "NP"   , { 93 , 239  , 239.0529314    , 1.75   } },
      { "PU"   , { 94 , 238  , 238.0495534    , 1.75   } },
      { "PU"   , { 94 , 239  , 239.0521565    , 1.75   } },
      { "PU"   , { 94 , 240  , 240.0538075    , 1.75   } },
      { "PU"   , { 94 , 241  , 241.0568453    , 1.75   } },
      { "PU"   , { 94 , 242  , 242.0587368    , 1.75   } },
      { "PU"   , { 94 , 244  , 244.064198     , 1.75   } },
      { "AM"   , { 95 , 241  , 241.0568229    , 1.75   } },
      { "AM"   , { 95 , 243  , 243.0613727    , 1.75   } },
      { "CM"   , { 96 , 243  , 243.0613822    , 2.01   } },
      { "CM"   , { 96 , 244  , 244.0627463    , 2.01   } },
      { "CM"   , { 96 , 245  , 245.0654856    , 2.01   } },
      { "CM"   , { 96 , 246  , 246.0672176    , 2.01   } },
      { "CM"   , { 96 , 247  , 247.070347     , 2.01   } },
      { "CM"   , { 96 , 248  , 248.072342     , 2.01   } },
      { "BK"   , { 97 , 247  , 247.070299     , 2.01   } },
      { "BK"   , { 97 , 249  , 249.074980     , 2.01   } },
      { "CF"   , { 98 , 249  , 249.074847     , 2.01   } },
      { "CF"   , { 98 , 250  , 250.0764000    , 2.01   } },
      { "CF"   , { 98 , 251  , 251.079580     , 2.01   } },
      { "CF"   , { 98 , 252  , 252.081620     , 2.01   } },
      { "ES"   , { 99 , 252  , 252.082970     , 2.01   } },
      { "FM"   , { 100, 257  , 257.095099     , 2.01   } },
      { "MD"   , { 101, 256  , 256.094050     , 2.01   } },
      { "MD"   , { 101, 258  , 258.098425     , 2.01   } },
      { "NO"   , { 102, 259  , 259.10102      , 2.01   } },
      { "LR"   , { 103, 262  , 262.10969      , 2.01   } },
      { "RF"   , { 104, 261  , 261.10875      , 2.01   } },
      { "DB"   , { 105, 262  , 262.11415      , 2.01   } },
      { "SG"   , { 106, 266  , 266.12193      , 2.01   } },
      { "BH"   , { 107, 264  , 264.12473      , 2.01   } },
      { "MT"   , { 109, 268  , 268.13882      , 2.01   } } 
    }
  ); ///< Reference map for atomic lookup 

  // Map atomic symbol to atomic number
  static std::unordered_map<std::string,int> atomicNumMap(
    {
      { "H"    ,  1   }, 
      { "He"   ,  2   },
      { "Li"   ,  3   },
      { "Be"   ,  4   },
      { "B"    ,  5   },
      { "C"    ,  6   },
      { "N"    ,  7   },
      { "O"    ,  8   },
      { "F"    ,  9   },
      { "Ne"   ,  10  },
      { "Na"   ,  11  },
      { "Mg"   ,  12  },
      { "Al"   ,  13  },
      { "Si"   ,  14  },
      { "P"    ,  15  },
      { "S"    ,  16  },
      { "Cl"   ,  17  },
      { "Ar"   ,  18  },
      { "K"    ,  19  },
      { "Ca"   ,  20  },
      { "Sc"   ,  21  },
      { "Ti"   ,  22  },
      { "V"    ,  23  },
      { "Cr"   ,  24  },
      { "Mn"   ,  25  },
      { "Fe"   ,  26  },
      { "Co"   ,  27  },
      { "Ni"   ,  28  },
      { "Cu"   ,  29  },
      { "Zn"   ,  30  },
      { "Ga"   ,  31  },
      { "Ge"   ,  32  },
      { "As"   ,  33  },
      { "Se"   ,  34  },
      { "Br"   ,  35  },
      { "Kr"   ,  36  },
      { "Rb"   ,  37  },
      { "Sr"   ,  38  },
      { "Y"    ,  39  },
      { "Zr"   ,  40  },
      { "Nb"   ,  41  },
      { "Mo"   ,  42  },
      { "Tc"   ,  43  },
      { "Ru"   ,  44  },
      { "Rh"   ,  45  },
      { "Pd"   ,  46  },
      { "Ag"   ,  47  },
      { "Cd"   ,  48  },
      { "In"   ,  49  },
      { "Sn"   ,  50  },
      { "Sb"   ,  51  },
      { "Te"   ,  52  },
      { "I"    ,  53  },
      { "Xe"   ,  54  },
      { "Cs"   ,  55  },
      { "Ba"   ,  56  },
      { "La"   ,  57  },
      { "Ce"   ,  58  },
      { "Pr"   ,  59  },
      { "Nd"   ,  60  },
      { "Pm"   ,  61  },
      { "Sm"   ,  62  },
      { "Eu"   ,  63  },
      { "Gd"   ,  64  },
      { "Tb"   ,  65  },
      { "Dy"   ,  66  },
      { "Ho"   ,  67  },
      { "Er"   ,  68  },
      { "Tm"   ,  69  },
      { "Yb"   ,  70  },
      { "Lu"   ,  71  },
      { "Hf"   ,  72  },
      { "Ta"   ,  73  },
      { "W"    ,  74  },
      { "Re"   ,  75  },
      { "Os"   ,  76  },
      { "Ir"   ,  77  },
      { "Pt"   ,  78  },
      { "Au"   ,  79  },
      { "Hg"   ,  80  },
      { "Tl"   ,  81  },
      { "Pb"   ,  82  },
      { "Bi"   ,  83  },
      { "Po"   ,  84  },
      { "At"   ,  85  },
      { "Rn"   ,  86  },
      { "Fr"   ,  87  },
      { "Ra"   ,  88  },
      { "Ac"   ,  89  },
      { "Th"   ,  90  },
      { "Pa"   ,  91  },
      { "U"    ,  92  },
      { "Np"   ,  93  },
      { "Pu"   ,  94  },
      { "Am"   ,  95  },
      { "Cm"   ,  96  },
      { "Bk"   ,  97  },
      { "Cf"   ,  98  },
      { "Es"   ,  99  },
      { "Fm"   ,  100 },
      { "Md"   ,  101 },
      { "No"   ,  102 },
      { "Lr"   ,  103 },
      { "Rf"   ,  104 },
      { "Db"   ,  105 },
      { "Sg"   ,  106 },
      { "Bh"   ,  107 },
      { "Mt"   ,  109 } 
    }
  ); ///< Reference map for atomic number 


  
  // Define Symbol + coordinate array constructor
  inline Atom::Atom(std::string symb, std::array<double,3> XYZ) :
    Atom(atomicReference[symb]){ coord = XYZ; };

}; // namespace ChronusQ

#endif
