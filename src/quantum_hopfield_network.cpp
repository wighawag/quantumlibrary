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
#include "quantum_hopfield_network.hpp"

#include <iostream>
using namespace std;


//class QuantumHopfieldNetwork{
//protected:
//  unsigned m_nb_neuron;
//  QuantumState m_quantum_state;

//public:
QuantumHopfieldNetwork::QuantumHopfieldNetwork(int nb_neuron ):
m_nb_neuron(nb_neuron),
m_quantum_state(nb_neuron*(nb_neuron-1)/2 ,2), // nb_neuron qubits -> nb_neuron*(nb_neuron-1)/2 nb_system , 2 nb_value  // diagonal null and symetric connections
m_inputs(new double[nb_neuron])
{

}

QuantumHopfieldNetwork::~QuantumHopfieldNetwork(){
  delete[] m_inputs;
}

void QuantumHopfieldNetwork::set_quantum_state(QuantumState quantum_state){
  //check size TODO
  m_quantum_state=quantum_state;
}

QuantumState QuantumHopfieldNetwork::get_quantum_state(void){
  return m_quantum_state;
}

void QuantumHopfieldNetwork::set_inputs(double* inputs){
  //check size TODO
  for(unsigned i = 0; i < m_nb_neuron; i++){
    m_inputs[i]=inputs[i];
    cout << "m_inputs["<< i <<"]: " << m_inputs[i] << endl;
  }
}


void QuantumHopfieldNetwork::compute(double* inputs){
  //check size TODO
  //asynchronous:
  for(unsigned i = 0; i < m_nb_neuron; i++){
    m_inputs[i]=inputs[i];
    cout << "m_inputs["<< i <<"]: " << m_inputs[i] << endl;
  }
  compute();
}

void QuantumHopfieldNetwork::compute(){
  int *weights = new int(m_nb_neuron*(m_nb_neuron-1)/2); 
  unsigned i = 0;
  int measurement = m_quantum_state.measure();
  while(i<(m_nb_neuron*(m_nb_neuron-1)/2)){ 
    cout << "measurement = " << measurement<< endl;
    weights[i] = measurement % 2; // (qubit -> nb_value=2, nb_system=1)
    measurement -= weights[i];
    measurement = measurement / 2; 
     
    if(weights[i]==0)weights[i]=-1;
    else weights[i]=1;
    cout << "weights[" <<i << "] = " << weights[i]<< endl;
    //cout << "inputs[" << i << "] = " << inputs[i] << endl;
    i++;
  }

  // weights are set, now compute the new inputs
  for(unsigned i=0; i<m_nb_neuron;i++){
    double tmp=0;
    for(unsigned j=0; j<m_nb_neuron; j++){
      if(i!=j){
	unsigned k,l;
	if(i>j){
	  k=j;l=i; 
	}else{
	  k=i;l=j;
	}
	unsigned weight_number= k*m_nb_neuron+(l-k)-((k+1)*k/2)-1;
	tmp+=m_inputs[j]*weights[weight_number];
	cout << "i = " << i << endl;
	cout << "m_inputs[" << j << "] = " << m_inputs[j] << endl;
	cout << "weights[" << weight_number << "] = " << weights[weight_number] << endl;
	cout << "tmp+=m_inputs[" << j << "]*weights[" << weight_number << "] --> " << tmp << endl;
      }
    }
    if(tmp>0)m_inputs[i]=1;
    else m_inputs[i]=-1;
    cout << "m_inputs[" << i << "] = " << m_inputs[i] <<  endl;
  }
}

double QuantumHopfieldNetwork::learn(double* inputs){
  return 0.0;
}

//istream& operator >>(istream &is,QuantumHopfieldNetwork &qhs){
//  for(unsigned i=0; i< qhs.m_nb_neuron; i++){
//    is << qhs.m_inputs[i] << ", ";
//  }
//  return os;
//}

ostream& operator <<(ostream &os,const QuantumHopfieldNetwork &qhs){
  for(unsigned i=0; i< qhs.m_nb_neuron; i++){
    os << qhs.m_inputs[i];
    if(i==qhs.m_nb_neuron-1) os << endl; 
    else os << ", ";

  }
  return os;
}


 
