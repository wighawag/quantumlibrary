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
#ifndef QUANTUM_STATE_HPP
#define QUANTUM_STATE_HPP

#include <complex>
#include <map>
#include <vector>

using namespace std;

struct intCmp {
  bool operator()( const int i1, const int i2 ) const {
    return i1<i2;
  }
};

typedef map<unsigned, complex<double>, intCmp> StateMap;

class QuantumState{
protected:
  StateMap m_state;
  unsigned m_nbValue;
  unsigned m_nbSystem;

public:
  //constructors
  QuantumState(const QuantumState & that);
  QuantumState( unsigned nbSystem, unsigned nbValue=2);
  QuantumState(StateMap state, unsigned nbSystem, unsigned nbValue=2);
  
  //destructor
  virtual ~QuantumState();

  // other functions
  unsigned nbBasisState() const;

  unsigned nbSystem();
  unsigned nbValue();

  unsigned measure();

  void transform(vector<complex<double> > matrix, vector<unsigned> whichones ); // any matrices on any qubits !
  void SWAP(unsigned i, unsigned j);
  void F(unsigned test, unsigned i, unsigned j);
  void F(unsigned i); // 
  void A(unsigned test1, unsigned test2, unsigned i, unsigned j, unsigned k); //
  

  void I(vector<unsigned> states); //invert the phase of the specified states (Identity except for states = -1)
  void I(unsigned beginning, unsigned nb_qubits); // inverrt phases for states starting with beginning
  void G(bool existing_only=true);
 
  void keep(unsigned nb_qubits);

  void initialize(vector<unsigned> patterns, unsigned nb_neuron);
  void find(vector<unsigned> pattern);

  //operators
  complex<double>& operator[]( const unsigned& key );
  QuantumState & operator*=(const QuantumState & that); //Tensor Product
  QuantumState & operator=(const QuantumState & that);
  friend QuantumState operator*(const QuantumState & qs1, const QuantumState & qs2); 
  friend ostream& operator <<(ostream &os, const QuantumState &qs);


};

#endif
