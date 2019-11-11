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

#include "quantum_associative_memory.hpp"

#include <iostream>

using namespace std;

QuantumAssociativeMemory::QuantumAssociativeMemory(vector<unsigned> patterns, int nb_qubits):
  QuantumState(nb_qubits*2+1)
{
  initialize(patterns, nb_qubits);
}

//destructor
QuantumAssociativeMemory::~QuantumAssociativeMemory(){

}

//operators

ostream& operator <<(ostream &os, const QuantumAssociativeMemory &qam){
  unsigned j=0;
  complex<double> sum=0;
  for(StateMap::const_iterator it = (qam.m_state).begin(); it != (qam.m_state).end(); it++) {
    //os << it->second << " |" << it->first << "/" << qam.nbBasisState() << ">" ;
    os <<  it->second << " |" ;
    sum+=(it->second*conj(it->second));
    for(int i=qam.m_nbSystem-1; i>=0; i--){
      os << ( (unsigned)(it->first) & (unsigned)( (unsigned)pow(2.0, (double)i)) ) / (unsigned)pow(2.0,(double)i);
    }
    os << ">" ;
    if(j+1<(qam.m_state).size())os << " + ";
    j++;
  }
  cout << " = " << sum;
  
  return os;

}


//other methods
void QuantumAssociativeMemory::initialize(vector<unsigned> patterns, int nb_qubits){
  n = nb_qubits; // nb neuron
  m = patterns.size()/n; // nb pattern
  
  z = vector<unsigned>(m*n+n);
  for(unsigned i=0; i<patterns.size(); i++){
    //  cout << "patterns[" << i << "]=" << patterns[i] << endl;
    z[i]=patterns[i];
  }
  for(unsigned i=0; i<n; i++){
    z[patterns.size()+i]=0;
  }

  // **** ******* ****

  // INITIALIZATION (SUPOSED TRIVIAL) 
  m_nbValue=2; //qubits
  m_nbSystem = n + (n-1) + 2;
  m_state.clear();
  m_state[0]=complex<double>(1.0,0.0);         // |000 00 00>
  cout << *this <<endl;

  for(int p=m-1; p>=0;p--){
    cout << "turn: " << p << endl;
    for(unsigned i = 0; i < n; i++ ){
      if(z[p*n+i]!=z[(p+1)*n+i]){
	cout << "F(0," << 2*n << "," << i << ")" << endl;
	F(0,2*n,i);  // C2 => 2*n;  Xi => i
	cout << *this  <<endl;
      }
    }
    cout << "*** flip c1" << endl;
    cout << "F(0," << 2*n << "," << 2*n-1 << ")" << endl;
    F(0,2*n,2*n-1);  // C2 => 2*n; C1 => 2*n-1 
    cout << *this  <<endl;      
    vector<complex<double> > S(4*4);
    S[0]=1;   S[1]=0;  S[2]=0;                                 S[3]=0;
    S[4]=0;   S[5]=1;  S[6]=0;                                 S[7]=0;
    S[8]=0;   S[9]=0; S[10]=sqrt((double)(p)/((double)p+1.0)); S[11]=1.0/sqrt((double)p+1.0);
    S[12]=0; S[13]=0; S[14]=1.0/sqrt((double)p+1.0);           S[15]=sqrt((double)(p)/((double)p+1.0));

    vector<unsigned> qubits(2);
    qubits[0]=2*n-1;
    qubits[1]=2*n;
    cout << "transform(S,qubits);" << endl;
    transform(S,qubits);
    cout << *this  <<endl;
    cout << "A(" << z[p*n+0] << "," << z[p*n+1] << ",0,1, "<<n << ")" << endl;
    A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n
    cout << *this  <<endl;
    for(unsigned k=2; k<n;k++){
      cout << "A(" << z[p*n+k] << ",1," << k << "," <<n+k-2 <<","<< n+k-1 << ")" << endl;
      A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
      cout << *this  <<endl;
    }
    cout << "F(1," << 2*n-2 << "," << 2*n-1 << ")" << endl;  
    F(1,2*n-2,2*n-1); // Gn-1 => 2*n-2; C1 => 2*n-1 
    cout << *this  <<endl;
    for(int k=n-1; k>=2;k--){
      cout << "A(" << z[p*n+k] << ",1," << k << "," <<n+k-2 <<","<< n+k-1 << ")" << endl;
      A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
      cout << *this  <<endl;
    }
    cout << "A(" << z[p*n+0] << "," << z[p*n+1] << ",0,1, "<<n << ")" << endl;
    A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n
    cout << *this  <<endl;
  }
  cout << "F(2*n);"  <<endl;
  F(2*n); // C2 => 2*n 
  cout << *this  <<endl;
  keep(n); // should be not entangled with the deleted  qubits TODO
  cout << *this  <<endl;
  
 
}

unsigned QuantumAssociativeMemory::find(vector<unsigned> pattern, bool existing_only){

 
  unsigned nb_known=pattern.size();
  
  cout << m;
  vector<unsigned> patterns(m);
  for(unsigned i=0; i<m; i++){
    unsigned tmp=0;
    for(unsigned j=0;j<n;j++){
      cout << "z[" << i*n+j << "]=" << z[i*n+j] << endl;
      tmp+=z[i*n+j]*(unsigned)pow(2.0,(double)((n-1-j)));
    }
      cout << tmp <<endl;
      patterns[i]=tmp;
  }
  
  unsigned value_pattern=0;
  for(unsigned j=0;j<nb_known;j++){
      value_pattern+=pattern[j]*(unsigned)pow(2.0,(double)((nb_known-1-j)));
  }
  

  cout << "I(" << value_pattern << ","<<  nb_known <<");" <<endl;
  I(value_pattern,nb_known);
  cout << *this  <<endl;
  cout << "G();" <<endl;
  G(existing_only);
  cout << *this  <<endl;
  if(!existing_only){
    cout << "I(patterns);" <<endl;
    I(patterns);
    cout << *this  <<endl;
    cout << "G();" <<endl;
    G(existing_only);
    cout << *this  <<endl;
  }
  int T = (int)((3.14/4.0)*sqrt(pow(2.0,(double)n)) - 2); // (-1)x ????????? ajouter par moi
  //T=10;
  //T=-1;
  //T= (int)(3.14/4.0 * sqrt(m_state.size()))-2;
  //int r0=;
  //int r1=;
  //int N = (unsigned)pow(2.0,(double)n);
  //int a =(2*(m-2*r1))/N;
  //int b =(4*(p+r0))/N;
  //int T = ((3.14/2) - tanh(((4*a-a*b+(r1/(r0+r1)))/(-a*b+(2*a*(N+m-r0-2*r1))/(N-r0-r1)-(m-r1)/(N-r0-r1)))*sqrt((r0+r1)/(N-r0-r1))))/cosh(1-(2*((r0+r1)/N)));
  cout << "T = " << T << endl;
  for(int i=0; i< T; i++){
    cout << "I(value_pattern, nb_qubits);" <<endl;
    I(value_pattern,nb_known);
    cout << *this  <<endl;
    cout << "G();" <<endl;
    G(existing_only);
    cout << *this  <<endl;  
  }
  
  unsigned i = measure();
  cout << *this  <<endl;

  return i;
 
}

