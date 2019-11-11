//Copyright (C) 2006  Ronan Sandford
//
//This file is part of Quantum Library
//
//    Quantum Library is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Quantum Library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef QUANTUM_ASSOCIATIVE_MEMORY_HPP
#define QUANTUM_ASSOCIATIVE_MEMORY_HPP

#include "quantum_state.hpp"

#include <complex>
#include <map>
#include <vector>

using namespace std;

class QuantumAssociativeMemory : QuantumState{
protected:
  vector<unsigned> z;
  unsigned m;
  unsigned n;

public:
  //constructors
  //  QuantumAssociativeMemory();
  QuantumAssociativeMemory(vector<unsigned> patterns, int nb_qubits);

  //destructor
  virtual ~QuantumAssociativeMemory();

  //other methods
  void initialize(vector<unsigned> patterns, int nb_qubits);
  unsigned find(vector<unsigned> pattern, bool existing_only=true);


  //operators
  friend ostream& operator <<(ostream &os, const QuantumAssociativeMemory &qam);
  

};

#endif
