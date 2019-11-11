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
#include <iostream>
#include <complex>

#include "quantum_state.hpp"

#include "xor_neuron.hpp"

#include "quantum_hopfield_network.hpp"

#include "quantum_associative_memory.hpp"

using namespace std;


int main (int argc, char **argv){

  srand(time(NULL));

  cout << "test?(1->quantum_state)(2->xor_network)(3->hopfield)(4->OPERATOR FOR GOAL)(5->Quantum Associative Memory)(6)(7)(8)";
  int answer=6;
  cin >> answer;

  if(answer==1){
    cout << "**** quantum state ****" << endl;
    
    QuantumState qs = QuantumState(1,2);  
 
    qs[0]=complex<double>(1.0,0.0);

    cout << qs << endl;
    vector<complex<double> > hadamard(4);
    hadamard[0]=complex<double>(1.0/sqrt(2.0),0.0); hadamard[1]=complex<double>(1.0/sqrt(2.0),0.0);
    hadamard[2]=complex<double>(1.0/sqrt(2.0),0.0); hadamard[3]=complex<double>(-1.0/sqrt(2.0),0.0);
    vector<unsigned> qubits(1);
    qubits[0]=0;
    cout << "hadamard " << endl;
    qs.transform(hadamard, qubits); // Hadamard
    cout << qs << endl;
    cout << "measure:" << qs.measure() << endl;
    cout << qs << endl;

    cout << "hadamard " << endl;
    qs.transform(hadamard, qubits); // Hadamard
    cout << qs << endl;
    cout << "measure:" << qs.measure() << endl;
    cout << qs << endl;

    cout << "modyfing " << endl;
    qs[0]=0; // TODO should delete the state (probability equal to 0)
    qs[1]=1.0;

    cout << qs << endl;
    cout << "measure:" << qs.measure() << endl;
    cout << qs << endl;

    
  }else if(answer==2){
    
    cout << "**** neuron xor *****" << endl;
    
    XorNeuron xn = XorNeuron(2,1,2); 
    QuantumState qs = QuantumState(2,2);

    qs[1]=complex<double>(sqrt(0.5),0.0); //XOR
    qs[2]=complex<double>(sqrt(0.5),0.0);
    
    //qs[3]=complex<double>(1.0,0.0); // OR
    
   
    cout << qs << endl;
    xn.set_quantum_state(qs);
    cout << " - --  - xn set" << endl;

    vector<double> inputs(2);
    inputs[0]=0;
    inputs[1]=0;
    cout << "xn.compute(0,0) = " << xn.compute(inputs) << endl;
    xn.set_quantum_state(qs);
    
    inputs[0]=0;
    inputs[1]=1;
    cout << "xn.compute(0,1) = " << xn.compute(inputs) << endl;
    xn.set_quantum_state(qs);
    
    inputs[0]=1;
    inputs[1]=0;
    cout << "xn.compute(1,0) = " << xn.compute(inputs) << endl;
    xn.set_quantum_state(qs);
    
    inputs[0]=1;
    inputs[1]=1;
    cout << "xn.compute(1,1) = " << xn.compute(inputs) << endl;
    
    cout << "75% de réussites" << endl;


  }else if(answer==3){
    cout << "**** Quantum Hopfield Network *****" << endl;
    unsigned nb_neuron;
    cout << "how many neurons? ";
    cin >> nb_neuron;
 
    QuantumHopfieldNetwork qhn = QuantumHopfieldNetwork(nb_neuron); 
    QuantumState qs = QuantumState(nb_neuron*(nb_neuron-1)/2 ,2);
    for(unsigned i=0; i<nb_neuron*(nb_neuron-1)/2;i++){
      qs[i]=complex<double>(0.0,0.0);
    }
    unsigned whichone;
    cout << " which one to 1 ? ";
    cin >> whichone;
    qs[whichone]=complex<double>(1.0,0.0);
    
    qhn.set_quantum_state(qs);

    cout << "before set the inputs: " << qhn;
    
    double* inputs = new double[nb_neuron];
    for(unsigned i=0; i< nb_neuron; i++){
      inputs[i]=-1;
    }
    qhn.set_inputs(inputs);
    cout << "before compute: "<< qhn;
    qhn.compute(inputs);
    cout << "after compute: " << qhn;
    // cout << "AGAIN: " << endl;
//     qhn.compute();
//     cout << "after compute: " << qhn;
//     cout << "AGAIN: " << endl;
//     qhn.compute();
//     cout << "after compute: " << qhn;
//     cout << "AGAIN: " << endl;
//     qhn.compute();
//     cout << "after compute: " << qhn;


    delete[] inputs;
  }else if(answer==4){
    cout << "**** Quantum Associative Memory" << endl;
 
    // **** EXEMPLE ****
    unsigned n=3; // nb neuron
    unsigned m=3; // nb pattern
    vector<unsigned> z(m*n+n);
    z[0]=1; z[1]=0; z[2]=0;
    z[3]=0; z[4]=1; z[5]=0;
    //z[6]=0; z[7]=0; z[8]=0;
    z[6]=0; z[7]=1; z[8]=1;
    z[9]=0;z[10]=0;z[11]=0;
    // **** ******* ****

    // INITIALIZATION (SUPOSED TRIVIAL) 
    QuantumState qs = QuantumState(n + (n-1) + 2);
    qs[0]=complex<double>(1.0,0.0);         // |000 00 00>
    qs[0]=complex<double>(1/sqrt(2),0.0);
    qs[6]=complex<double>(1/sqrt(2),0.0);
    cout << qs  <<endl;

    qs.SWAP(6,5);
    
    cout << qs  <<endl;

   
    cout << "**** ALGO ***" << endl;
    qs = QuantumState(n + (n-1) + 2);
    qs[0]=complex<double>(1.0,0.0);         // |000 00 00>
    //qs[0]=complex<double>(1/sqrt(2),0.0);
    //qs[6]=complex<double>(1/sqrt(2),0.0);
    cout << qs  <<endl;
    
    for(int p=m-1; p>=0;p--){
      cout << "p=" << p << endl;
      for(unsigned i = 0; i < n; i++ ){
	//cout << "z[" << p << "*" << n << "+" << i << "] = " << z[p*n+i] << endl;
	//cout << "z[" << p+1 << "*" << n << "+" << i << "] = " << z[(p+1)*n+i] << endl;
	if(z[p*n+i]!=z[(p+1)*n+i]){
	  cout << "qs.F(0," << 2*n << "," << i << ")" << endl;
	  qs.F(0,2*n,i);  // C2 => 2*n;  Xi => i
	  cout << qs  <<endl;
	  //  int r;
	  //cin >> r; 
	}
      }
      cout << "*** flip c1" << endl;
      cout << "qs.F(0," << 2*n << "," << 2*n-1 << ")" << endl;
      qs.F(0,2*n,2*n-1);  // C2 => 2*n; C1 => 2*n-1 
      cout << qs  <<endl;      
      vector<complex<double> > S(4*4);
      S[0]=1;   S[1]=0;  S[2]=0;                                 S[3]=0;
      S[4]=0;   S[5]=1;  S[6]=0;                                 S[7]=0;
      S[8]=0;   S[9]=0; S[10]=sqrt((double)(p)/((double)p+1.0)); S[11]=1.0/sqrt((double)p+1.0);
      S[12]=0; S[13]=0; S[14]=1.0/sqrt((double)p+1.0);           S[15]=sqrt((double)(p)/((double)p+1.0));
      //for(int i = 0; i< 4*4; i++){
      //cout << "S[" << i << "]=" << S[i] << endl;
      //}
      vector<unsigned> qubits(2);
      qubits[0]=2*n-1;
      qubits[1]=2*n;
      cout << "qs.transform(S,qubits);" << endl;
      qs.transform(S,qubits);
      //qs.transform(S,qubits);
      qs.A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n
      for(int k=2; k>=0;k--){
	qs.A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
      }
      qs.F(1,2*n-2,2*n-1); // Gn-1 => 2*n-2; C1 => 2*n-1 
      for(unsigned k=0; k<n;k++){
	qs.A(z[p*n+k],1, k,n+k-2,n+k-1); // Xk => k; Gk-2 => n+k-2; Gk-1 => n+k-1
      }
      qs.A(z[p*n+0],z[p*n+1], 0,1,n); // X1 => 0; X2 => 1; G1 => n

      cout << qs  <<endl;
    }
    cout << "qs.F(2*n);"  <<endl;
    qs.F(2*n); // C2 => 2*n 
    cout << qs  <<endl;
    qs.keep(3); // should be not entangled with the deleted  qubits TODO
    cout << qs  <<endl;
    
    cout << "*** FIND a PATTERN beginning by 10 ****" << endl;
    unsigned pattern=2;
    unsigned nb_qubits=2;

    vector<unsigned> patterns(m);
    for(unsigned i=0; i< m; i++){
      unsigned tmp=0;
      for(unsigned j=0;j<n;j++){
	tmp+=z[i*m+j]*(unsigned)pow(2.0,(double)((n-1-j)));
      }
      cout << tmp <<endl;
      patterns[i]=tmp;
    }

    cout << "qs.I(pattern, nb_qubits);" <<endl;
    qs.I(pattern,nb_qubits);
    cout << qs  <<endl;
    cout << "qs.G();" <<endl;
    qs.G();
    cout << qs  <<endl;
    cout << "qs.I(patterns);" <<endl;
    qs.I(patterns);
    cout << qs  <<endl;
    cout << "qs.G();" <<endl;
    qs.G();
    cout << qs  <<endl;
    cout << "T = " << (unsigned)((3.14/4.0)*sqrt(pow(2.0,(double)n)) - 2) << endl;
    for(unsigned i=0; i< (unsigned)((3.14/4.0)*sqrt(pow(2.0,(double)n)) - 2); i++){
      cout << "qs.I(pattern, nb_qubits);" <<endl;
      qs.I(pattern,nb_qubits);
      cout << qs  <<endl;
      cout << "qs.G();" <<endl;
      qs.G();
      cout << qs  <<endl;

    }

    qs.measure();
    cout << qs  <<endl;

  }else if(answer==5){
     cout << "**** Quantum Associative Memory 5" << endl;

     
     vector<unsigned> patterns(9);
     patterns[0]=1; patterns[1]=0; patterns[2]=0;
     patterns[3]=0; patterns[4]=1; patterns[5]=0;
     //patterns[6]=0; patterns[7]=0; patterns[8]=0;
     patterns[6]=0; patterns[7]=1; patterns[8]=1;

     QuantumAssociativeMemory qam(patterns, 3);
 
     cout << qam << endl;
     vector<unsigned> pattern(2);
     pattern[0]=1; pattern[1]=0;

     cout << "answer: " << qam.find(pattern) <<endl;
     cout << qam << endl;



  }else if(answer==6){
     cout << "**** Quantum Associative Memory 6" << endl;

     
     vector<unsigned> patterns(9*4);
     patterns[0]=1; patterns[1]=0; patterns[2]=0;
     patterns[3]=0; patterns[4]=1; patterns[5]=0;
     patterns[6]=0; patterns[7]=1; patterns[8]=1;

     patterns[9]=1; patterns[10]=1; patterns[11]=1;
     patterns[12]=0; patterns[13]=1; patterns[14]=0;
     patterns[15]=0; patterns[16]=1; patterns[17]=0;

     patterns[18]=1; patterns[19]=0; patterns[20]=0;
     patterns[21]=1; patterns[22]=1; patterns[23]=0;
     patterns[24]=1; patterns[25]=1; patterns[26]=1;

     patterns[27]=0; patterns[28]=0; patterns[29]=0;
     patterns[30]=0; patterns[31]=1; patterns[32]=0;
     patterns[33]=1; patterns[34]=0; patterns[35]=1;

     QuantumAssociativeMemory qam(patterns, 9);
 
     cout << qam << endl;

     vector<unsigned> pattern(6);
     pattern[0]=1; pattern[1]=0; pattern[2]=0;
     pattern[3]=0; pattern[4]=1; pattern[5]=0;
     

     cout << "answer: " << qam.find(pattern) <<endl;
     
     cout << qam << endl;



  }else if(answer==7){
     cout << "**** Quantum Associative Memory 7" << endl;

     
     vector<unsigned> patterns(4*3);
     patterns[0]=1; patterns[1]=1; patterns[2]=0; patterns[3]=0; 

     patterns[4]=1; patterns[5]=0; patterns[6]=0; patterns[7]=1; 

     patterns[8]=1; patterns[9]=1; patterns[10]=1; patterns[11]=1;

     QuantumAssociativeMemory qam(patterns, 4);
 
     cout << qam << endl;

     vector<unsigned> pattern(2);
     pattern[0]=1; pattern[1]=1; //pattern[2]=0;     

     cout << "answer: " << qam.find(pattern,true) <<endl;
     
     cout << qam << endl;

  }else if(answer==8){
     cout << "**** Quantum Associative Memory 8 (word completion) " << endl;

     
     vector<string> words(10);
     words[0]="hell";
     words[1]="leaa";
     words[2]="lall";
     words[3]="ahaa";
     words[4]="laaa";
     words[5]="alah";
     words[6]="hall";
     words[7]="elle";
     words[8]="llee";
     words[9]="ehaa";


     vector<unsigned> patterns(10*4*2);
     int k=0; //aehl
     cout << "patterns : " << endl;
     for(vector<string>::iterator it = words.begin(); it!= words.end(); it++){
       cout << (*it) << endl;
       for(unsigned i=0; i<8; i+=2){
	 if((*it)[i/2]=='a'){
	   patterns[k*8+i]=0;patterns[k*8+i+1]=0;
	 }
	 if((*it)[i/2]=='e'){
	   patterns[k*8+i]=0;patterns[k*8+i+1]=1;
	 }
	 if((*it)[i/2]=='h'){
	   patterns[k*8+i]=1;patterns[k*8+i+1]=0;
	 }
	 if((*it)[i/2]=='l'){
	   patterns[k*8+i]=1;patterns[k*8+i+1]=1;
	 }
	 cout << patterns[k*8+i] << patterns[k*8+i+1];
       }
       cout << endl;
	 k++;
     }
     // cout << "recapitulation " << endl;
//      for(unsigned i=0; i<patterns.size(); i++){
//        cout << "patterns[" << i << "]=" << patterns[i] << endl;
//      }
     

     QuantumAssociativeMemory qam(patterns, 8);
 
     cout << qam << endl;

     for(vector<string>::iterator it = words.begin(); it!= words.end(); it++){
       cout << (*it) << endl;
     }
     cout << "word? (2 letters)";
     string word;
     cin >> word;
     
     vector<unsigned> pattern(4);
     cout << "look for: " ;
     for(unsigned i=0; i<4; i+=2){
	 if(word[i/2]=='a'){	  
	   pattern[i]=0;pattern[i+1]=0;
	 }
	 if(word[i/2]=='e'){
	   pattern[i]=0;pattern[i+1]=1;
	 }
	 if(word[i/2]=='h'){
	   pattern[i]=1;pattern[i+1]=0;
	 }
	 if(word[i/2]=='l'){
	   pattern[i]=1;pattern[i+1]=1;
	 }
	 cout << pattern[i] << pattern[i+1];
     }
     cout << endl;
     
     unsigned response;
     response = qam.find(pattern,true);

     string s="    ";
     unsigned tmp=response;
     for(int i=3; i>=0; i--){
	 if(tmp % 4==0){
	   s[i]='a';
	 }
	 if(tmp % 4==1){
	   s[i]='e';
	 }
	 if(tmp % 4==2){
	   s[i]='h';
	 }
	 if(tmp % 4==3){
	   s[i]='l';
	 }
	 tmp = tmp/4;
     }
     
     cout << "answer: " << response << " => " << s << endl;
     
     cout << qam << endl;



  }

  return 0;
}

