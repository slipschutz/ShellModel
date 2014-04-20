


#include <iostream>
#include <vector>
#include <string>
#include <map>
//#include "TMatrixD.h"
#include "Operator.h"
using namespace std;

int NumSingleParticleStates=4;


class SingleParticleState {
public:
  SingleParticleState(){LevelNum=-1;Spin=-1;}
  SingleParticleState(int p,int s){
    SetLevel(p);
    SetSpin(s);}
  int GetLevelNum(){return LevelNum;}
  int GetSpin(){return Spin;}
  void SetLevel(int p){if (p<1){cout<<"P must be greater than 1"<<endl;}
    else {LevelNum=p;}
  }
  void SetSpin(int s){if (s==1 || s==-1){Spin=s;}else{cout<<"Spin must be +/-1"<<endl;}}
  bool operator==(const SingleParticleState &rhs){
    if (this->LevelNum==rhs.LevelNum && this->Spin==rhs.Spin){
      return true;
    } else {
      return false;
    }
  }
  void Print(){cout<<"Level is "<<LevelNum<<" Spin is "<<Spin<<endl;}
private:
  int LevelNum;
  int Spin;


};

//
//  Phase Definition will be a+_1- a+_1+...a+_n- a+_n+|0>
//  Plus to the right of minus
class SlaterDet {
public:
  vector <bool> TheStates;
  //  map <SingleParticleState,int> TheMapOfStates;
  int Phase;
  bool AddState(SingleParticleState s);
  bool RemoveState(SingleParticleState s);
  SlaterDet();
  bool operator==(const SlaterDet &rhs){
    bool ret=true;
    for (int i=0;i<TheStates.size();i++){
      if ( TheStates[i]!= rhs.TheStates[i]){
	ret=false;
      }
    }
    return ret;
  }

  vector <SingleParticleState> GetTheState();
  void Print();
private:
  map <int,SingleParticleState> TheMapOfStates;
};
bool SlaterDet::AddState(SingleParticleState s){
  //Spot in Vector should be 2*(Level-1) + 1 for spin +
  //Spot in Vector should be 2*(Level-1) + 0 for spin -
  int num=0;
  if (s.GetSpin()==1){
    num=1;
  }else{
    num=0;
  }
  int index = 2*(s.GetLevelNum()-1) +num;
  if ( TheStates[index]== 1){ //Already occupied
    //    Phase=0;
    return false;
  } else {
    TheStates[index]=1;
    TheMapOfStates[index]=s;
    return true;
  }
    
}

bool SlaterDet::RemoveState(SingleParticleState s){
  //Spot in Vector should be 2*(Level-1) + 1 for spin +
  //Spot in Vector should be 2*(Level-1) + 0 for spin -
  int num=0;
  if (s.GetSpin()==1){
    num=1;
  }else{
    num=0;
  }
  int index = 2*(s.GetLevelNum()-1) +num;
  if ( TheStates[index]== 1){ //Already occupied
    TheStates[index]=0;
    TheMapOfStates.erase(index);
    return true;
  } else { //Nothing there 
    //    Phase=0;
    return false;
  }
    
}

vector <SingleParticleState> SlaterDet::GetTheState(){
  vector <SingleParticleState> retVec;
  for (int i=0;i<TheStates.size();i++){
    if ( TheStates[i]==1 ){ //if there is particle there 
      retVec.push_back(TheMapOfStates[i]);
    }
  }


  return retVec;
}



SlaterDet::SlaterDet(){
  TheStates.resize(NumSingleParticleStates);
  Phase=1;
}
void SlaterDet::Print(){
  int size = TheStates.size();
  for (int i=0;i<size;i++){
    cout<<TheStates[i]<<" ";
  }
  cout<<endl;
}


void BuildListOfSD(vector <SlaterDet> & List){
  SlaterDet temp1;
  temp1.AddState(SingleParticleState(1,1));
  temp1.AddState(SingleParticleState(1,-1));
  
  List.push_back(temp1);

  SlaterDet temp2;
  temp2.AddState(SingleParticleState(2,1));
  temp2.AddState(SingleParticleState(2,-1));
  
  List.push_back(temp2);
} 

int GetPhase (vector <SingleParticleState> & theList){
  int phase =1;



}


double GetMatrixElement(SlaterDet lhs,SlaterDet rhs){
  //Caluculate the 1 body term
  cout<<"______________"<<endl;
  double totalOneBody=0;
  for (int p=1;p<=NumSingleParticleStates;p++){
    for (int s=0;s<=1;s++){
      int sigma=-9;
      if (s==0)
	sigma=-1;
      else if (s==1)
	sigma=1;
	

      SlaterDet rhsCopy=SlaterDet(rhs);
      //Extract the SP states from the SD
      //      vector <SingleParticleState> tempList=rhs.GetTheState();
      
      if(rhsCopy.RemoveState(SingleParticleState(p,sigma))==false){ //ie it failed to remove the state
	//it is zero
      }else if(rhsCopy.AddState(SingleParticleState(p,sigma))==false){ //ie it failed to add the state
	//it is zero
	
      } else{ // it wasn't zero check if it over laps with lhs
	if ( lhs == rhsCopy ){ 
	  //a non-zero term
	  totalOneBody=totalOneBody+(p-1);
	  cout<<" p is "<<p<<endl;
	  cout<<"sigma is "<<sigma<<endl;
	  cout<<"***********"<<endl;
	}
	
      }


    }//end s loop

  }//end p loop


}

int main(){

  vector <SlaterDet> ListOfSD;
  BuildListOfSD(ListOfSD);


  for ( auto i : ListOfSD){
    i.Print();
  }

  

  GetMatrixElement(ListOfSD[0],ListOfSD[0]);
  


  return 0;
}
