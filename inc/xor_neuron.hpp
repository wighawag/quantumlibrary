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
#ifndef XOR_NEURON_HPP
#define XOR_NEURON_HPP

#include <vector>

#include "quantum_state.hpp"

class XorNeuron{
protected:
  unsigned m_nb_inputs;
  unsigned m_nb_value;
  unsigned m_nb_system; // system per inputs(weights)
 
  QuantumState m_quantum_state;


public:
  XorNeuron(unsigned nb_inputs = 2, unsigned nb_system = 2, unsigned nb_value = 2);  // nb inputs, nb system by inputss and nb_value by system
  virtual ~XorNeuron();

  void set_quantum_state(QuantumState quantum_state);
  QuantumState get_quantum_state(void);

  double compute(vector<double> inputs);

  double learn(vector<double> inputs, double expected_output);


};

#endif
