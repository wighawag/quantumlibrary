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
#include "xor_neuron.hpp"


#include <vector>

#include <iostream>
using namespace std;

XorNeuron::XorNeuron(unsigned nb_inputs, unsigned nb_system, unsigned nb_value): 
  m_nb_inputs(nb_inputs),
  m_nb_value(nb_value),
  m_nb_system(nb_system),
  m_quantum_state(nb_system*nb_inputs,nb_value)
{
  
}

XorNeuron::~XorNeuron(){

}
  
void XorNeuron::set_quantum_state(QuantumState quantum_state){
  m_quantum_state=quantum_state; // cehck for nb_value et nb_system*nb_inputs TODO
}     

QuantumState XorNeuron::get_quantum_state(){
  return m_quantum_state; 
}     


double XorNeuron::compute(vector<double> inputs){
  // check nb_inputs with size of inputs
  //cout << "vector<int> weights;" << endl;
  vector<int> weights(m_nb_inputs); 
  //cout << "unsigned i = 0;" << endl;
  unsigned i = 0;
  //cout << "unsigned measurement = m_quantum_state.measure();" << endl;
  int measurement = m_quantum_state.measure();
  //cout << "double result=0" << endl;
  double result=0;
  //cout << "while(i<m_nb_inputs){ " << endl;
  while(i<m_nb_inputs){ 
    //cout << "measurement = " << measurement<< endl;
    weights[i] = measurement % (int)pow((double)m_nb_value,(double)m_nb_system);
    measurement -= weights[i];
    //cout << "measurement = " << measurement<< endl;
    measurement = measurement / (int)pow((double)m_nb_value,(double)m_nb_system); //nb_system per weights
    //cout << "measurement = " << measurement<< endl;     
    if(m_nb_system!=1){
      weights[i]-=(int)pow((double)m_nb_value,(double)m_nb_system)/2;
    }else{
      if(weights[i]==0)weights[i]=-1;
    }
    cout << "weights[" << i << "] = "<< weights[i] << endl;
    //cout << "inputs[" << i << "] = " << inputs[i] << endl;
    result+= inputs[i]*weights[i];
    i++;
  }
		 
  return result-0;

}
