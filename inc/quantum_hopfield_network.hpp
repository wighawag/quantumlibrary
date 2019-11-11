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
#ifndef QUANTUM_HOPFIELD_NETWORK_HPP
#define QUANTUM_HOPFIELD_NETWORK_HPP

#include "quantum_state.hpp"

class QuantumHopfieldNetwork{
protected:
  unsigned m_nb_neuron;
  QuantumState m_quantum_state;
  double* m_inputs;

public:
  QuantumHopfieldNetwork(int nb_neuron );  // nb inputs, nb system by inputss and nb_value by system
  virtual ~QuantumHopfieldNetwork();

  void set_quantum_state(QuantumState quantum_state);
  QuantumState get_quantum_state(void);

  void set_inputs(double* inputs);

  void compute();
  void compute(double* inputs);

  double learn(double* inputs);

  friend istream& operator >>(istream &is,QuantumHopfieldNetwork &qhs);
  friend ostream& operator <<(ostream &os,const QuantumHopfieldNetwork &qhs);
  

};


#endif

