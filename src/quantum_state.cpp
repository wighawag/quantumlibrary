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
#include "quantum_state.hpp"

#include <iostream>

using namespace std;

// Constructors

QuantumState::QuantumState( unsigned nbSystem, unsigned nbValue):
m_state(),
m_nbValue(nbValue),
m_nbSystem(nbSystem)
{
  
}

QuantumState::QuantumState(StateMap state, unsigned nbSystem, unsigned nbValue):
m_state(state), // check with state.size() TODO ?
m_nbValue(nbValue),
m_nbSystem(nbSystem) 
{
  
}

QuantumState::QuantumState(const QuantumState & that){
  
  m_state=that.m_state;
  m_nbValue=that.m_nbValue;
  m_nbSystem=that.m_nbSystem;
  
}

// Destructors

QuantumState::~QuantumState(){
  
}


// Other methods

unsigned QuantumState::nbBasisState() const{
  return (int) pow((double)m_nbValue,(double)m_nbSystem);
}

unsigned QuantumState::measure(void){
  double r=(double)(rand()%10000)/10000;
  cout << "random: " << r << endl;
  unsigned state=0;
  double total=0;
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {
    complex<double> result = it->second * conj( it->second );
    total+=result.real(); // result.imag()=0
    cout << "real: " << result.real() << endl;
    if(total>=r){ //TODO 
      state=it->first;
      break;
    }
  }
  m_state.clear();
  m_state[state]=1;
   cout << "chosen state: |"<< state << ">" << endl;
  return state;
}


// Operators

complex<double>& QuantumState::operator[]( const unsigned& key ){
  return m_state[key];
}

QuantumState & QuantumState::operator*=(const QuantumState & that)
{
  QuantumState temp=QuantumState(that.nbBasisState()*that.nbBasisState());
  int i=0;
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {
    for(StateMap::const_iterator it2 = that.m_state.begin(); it2 != that.m_state.end(); it2++) {
      temp[i]= it->second * it2->second;
      i++;
    }
  }
  
  m_state=temp.m_state;
  m_nbValue=temp.m_nbValue;
  m_nbSystem=temp.m_nbSystem;

  return *this;
}

QuantumState & QuantumState::operator=(const QuantumState & that){
  if(&that == this)return *this;
  m_state=that.m_state;
  m_nbValue=that.m_nbValue;
  m_nbSystem=that.m_nbSystem;
  return *this;
}


QuantumState operator*(const QuantumState & qs1, const QuantumState & qs2)
{
    QuantumState result = qs1;
    return result *= qs2;
}

ostream& operator <<(ostream &os, const QuantumState &qs){
  unsigned j=0;
  complex<double> sum=0;
  for(StateMap::const_iterator it = (qs.m_state).begin(); it != (qs.m_state).end(); it++) {
    //os << it->second << " |" << it->first << "/" << qs.nbBasisState() << ">" ;
    os <<  it->second << " |" ;
    sum+=(it->second*conj(it->second));
    for(int i=qs.m_nbSystem-1; i>=0; i--){
      os << ( (unsigned)(it->first) & (unsigned)( (unsigned)pow(2.0, (double)i)) ) / (unsigned)pow(2.0,(double)i);
    }
    os << ">" ;
    if(j+1<(qs.m_state).size())os << " + ";
    j++;
  }
  cout << " = " << sum;
  
  return os;
}


void QuantumState::SWAP(unsigned i, unsigned j){
  //cout << "SWAPING:" << "(" << i << ", " << j << ")" << endl;

  // nb_value ?? TODO // IMPOSSIBLE ??

  i=m_nbSystem-1-i; //natural => to use 2^i
  j=m_nbSystem-1-j; //natural => to use 2^j

  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {

    complex<double> value = it->second;
    unsigned state = it->first;

    unsigned value_i = ( (it->first) & ( (unsigned)pow(2.0, (double)i)) ) / (unsigned)pow(2.0,(double)i);
    unsigned value_j = ( (it->first) & ((unsigned)pow(2.0,(double)j) ) ) / (unsigned)pow(2.0,(double)j);			 
    if (value_i != value_j){
      state ^= (unsigned)pow(2.0,(double)i) + (unsigned)pow(2.0,(double)j);
    }
    tmp[state]=value;  
    
  }
  //m_state.clear();
  m_state=tmp;
}


void QuantumState::F(unsigned test, unsigned i, unsigned j){ // FN CT  / CNOT si test=1
  // TEST for i et j > qs.size ? TODO  
  //cout << "F(" << test << "):" << "(" << i << ", " << j << ")" << endl;
  i=m_nbSystem-1-i; //natural => to use 2^i
  j=m_nbSystem-1-j; //natural => to use 2^j
  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {

    complex<double> value = it->second;
    unsigned state = it->first;

    unsigned value_i = ( (it->first) & ( (unsigned)pow(2.0, (double)i)) ) / (unsigned)pow(2.0,(double)i);
    if (value_i == test){ // CONTROL BIT
      //cout << "CONTROLED" << endl;
      state ^=  (unsigned)pow(2.0,(double)j) ;
    }
    tmp[state]=value;
  }
  m_state=tmp;

  //cout << "F END"  << endl;
}

void QuantumState::F(unsigned i){ 
  // TEST for i > qs.size ? TODO  
  i=m_nbSystem-1-i; //natural => to use 2^i

  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {

    complex<double> value = it->second;
    unsigned state = it->first;

    state ^=  (unsigned)pow(2.0,(double)i) ;
    
    tmp[state]=value;
  }
  m_state=tmp;

  //cout << "F END"  << endl;
}

void QuantumState::transform(vector<complex<double> > matrix, vector<unsigned> whichones ){ 
  // TEST for whichones et matrix et QuantumState TODO

  //cout << "F(" << test << "):" << "(" << i << ", " << j << ")" << endl;
  //cout << "whichones: " ;
  for(vector<unsigned>::iterator it = whichones.begin(); it != whichones.end(); it++){
    //cout << *it << ", ";
    *it=m_nbSystem-1-(*it); //natural => to use 2^i
  }
  //cout << endl;

  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {

    complex<double> value = it->second;
    unsigned state = it->first;
    
    //vector<unsigned> values(whichones.size());
    unsigned total=0;
    //for(int i=whichones.size()-1;i>=0;i--){
    int i=whichones.size()-1;
    for(vector<unsigned>::iterator it2 = whichones.begin(); it2 != whichones.end(); it2++){
      //cout << *it2;
      int values = ( (it->first) & ( (unsigned)pow(2.0, (double)(*it2))) ) / (unsigned)pow(2.0,(double)(*it2));
      total += values *  (unsigned)pow(2.0,(double)i);
      //total+=values[i] * (unsigned)pow(2.0,i);
      state -= values * (unsigned)pow(2.0,(double)(*it2));
      i--;
    }
    //cout << "total: " << total << endl;
    
    for(int i=0;i<sqrt(matrix.size());i++){ //square matrices TODO
      //cout << "i: " <<i << endl;
      if(matrix[total*(unsigned)sqrt((double)matrix.size())+i]!=complex<double>(0,0)){
	//cout <<"i+state= " << i+state << endl;
	if(tmp.find(i+state)==tmp.end()){
	  //	  cout << "matrix[total*(unsigned)sqrt((double)matrix.size())+i]=" << matrix[total*(unsigned)sqrt((double)matrix.size())+i] << endl;
	  tmp[i+state]=matrix[total*(unsigned)sqrt((double)matrix.size())+i]*value;
	}else{
	  //cout << "tmp: " << endl;
	  //cout << tmp[i+state] << endl; 
	  tmp[i+state]+=matrix[total*(unsigned)sqrt((double)matrix.size())+i]*value;
	  //cout << tmp[i+state] << endl;
	}
      }
    }

  }
  m_state=tmp;

  //cout << "F END"  << endl;
}




void QuantumState::A(unsigned test1, unsigned test2, unsigned i, unsigned j, unsigned k){ 
  // TEST for i et j > qs.size ? TODO  

  i=m_nbSystem-1-i; //natural => to use 2^i
  j=m_nbSystem-1-j; //natural => to use 2^j
  k=m_nbSystem-1-k; //natural => to use 2^k
  
  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++) {

    complex<double> value = it->second;
    unsigned state = it->first;

    unsigned value_i = ( (it->first) & ( (unsigned)pow(2.0, (double)i)) ) / (unsigned)pow(2.0,(double)i);
    unsigned value_j = ( (it->first) & ( (unsigned)pow(2.0, (double)j)) ) / (unsigned)pow(2.0,(double)j);
    if (value_i == test1 && value_j==test2){ // CONTROL BIT
      //cout << "CONTROLED" << endl;
      state ^=  (unsigned)pow(2.0,(double)k) ;
    }
    tmp[state]=value;
  }
  m_state=tmp;

  //cout << "F END"  << endl;
}

void QuantumState::I(vector<unsigned> states){ 

  for(vector<unsigned>::iterator it= states.begin(); it!=states.end(); it++){
    //cout << "state: " << *it << endl;
    if(m_state.find(*it)!=m_state.end()){
      //cout << "reverse " << *it << endl;
      m_state[*it]=-m_state[*it];
    }
  }

}

void QuantumState::I(unsigned beginning, unsigned nb_qubits){ // inverrt phases for states starting with beginning
  
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++){
    unsigned total=it->first/(unsigned)pow(2.0,(double)(m_nbSystem-nb_qubits));
    if(total==beginning)it->second=-it->second;
  }

}

void QuantumState::G(bool existing_only){
  //invert all the sates' amplitudes arround the average
  
  complex<double> total=0;
  
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++){
    total+=it->second;
  }
  cout << "total=" << total << endl;
  
  complex<double> average=0;
  if(existing_only){
    cout << "m_state.size=" << m_state.size() << endl;
    average = total / (complex<double>)m_state.size();
  }else{
    cout << "m_nbSystem=" << m_nbSystem << endl;
    average = total / pow(2.0,(double)m_nbSystem);
  }
  cout << "average=" << average << endl;
 
  if(existing_only){
    for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++){
      it->second=average-(it->second-average);
    }
  }else{
    for(unsigned state=0; state<(unsigned)pow(2.0,(double)m_nbSystem); state++){
      m_state[state]=2.0*average-m_state[state];
    }
  }
  
  


}


void QuantumState::keep(unsigned nb_qubits){
  StateMap tmp = StateMap();
  for(StateMap::iterator it = m_state.begin(); it != m_state.end(); it++){
    tmp[it->first/(unsigned)pow(2.0,(double)(m_nbSystem-nb_qubits))]=it->second;
  }
  m_nbSystem=nb_qubits;
  m_state=tmp;
}

void QuantumState::initialize(vector<unsigned> patterns, unsigned nb_neuron){
  unsigned n=nb_neuron; // nb neuron
  unsigned m=patterns.size()/n; // nb pattern
  vector<unsigned> z(m*n+n);
  for(unsigned i=0; i<patterns.size(); i++){
    z[i]=patterns[i];
  }
  for(unsigned i=0; i<n; i++){
    z[m*n+i]=0;;
  }

  // **** ******* ****

  // INITIALIZATION (SUPOSED TRIVIAL) 
  m_nbValue=2;
  m_nbSystem = n + (n-1) + 2;
  m_state.clear();
  m_state[0]=complex<double>(1.0,0.0);         // |000 00 00>
  cout << *this <<endl;

  for(int p=m-1; p>=0;p--){
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
    
    A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n
    for(int k=2; k>=0;k--){
      A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
    }
    F(1,2*n-2,2*n-1); // Gn-1 => 2*n-2; C1 => 2*n-1 
    for(unsigned k=0; k<n;k++){
      A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
    }
    A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n
    
    cout << *this  <<endl;
  }
  cout << "F(2*n);"  <<endl;
  F(2*n); // C2 => 2*n 
  cout << *this  <<endl;
  keep(3); // test: should be not entangled with the deleted  qubits TODO
  cout << *this  <<endl;
  
  
}




