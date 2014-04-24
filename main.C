


#include <iostream>
#include <vector>
#include <string>
#include <map>
//#include "TMatrixD.h"
#include "Operator.h"
using namespace std;

int NumSingleParticleStates=4;
double g=1;

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
  vector <int> TheOrderedStates;//things will be pushed on this 
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
  map <int, int> TheOrderedMap;
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
    TheOrderedMap[index]=TheOrderedStates.size();
    TheOrderedStates.push_back(index);
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
    //TheOrderedStates.erase(TheOrderedStates.begin()+TheOrderedMap[index]);

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
  cout<<"____________"<<endl;
  int size = TheStates.size();
  for (int i=0;i<size;i++){
    cout<<TheStates[i]<<" ";
  }
  cout<<endl;
  cout<<"____________"<<endl;
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

int GetPhase (vector <int> list){
  int phase=1;
  bool loop=true;
  while (loop){
    loop=false;

    for (int i=0;i<list.size()-1;i++){
      if ( list[i+1]> list[i] ){//if there is an increase
	int temp=list[i];
	list[i]=list[i+1];
	list[i+1]=temp;
	phase=-1*phase;
	loop=true;
      }
    }
  }

  return phase;
}


double GetMatrixElement(SlaterDet lhs,SlaterDet rhs){
  //Caluculate the 1 body term

  double totalOneBody=0;
  for (int p=1;p<=NumSingleParticleStates/2;p++){
    for (int s=0;s<=1;s++){
      int sigma=-9;
      if (s==0)
	sigma=-1;
      else if (s==1)
	sigma=1;
	
      SlaterDet rhsCopy=SlaterDet(rhs);

      
      if(rhsCopy.RemoveState(SingleParticleState(p,sigma))==false){ //ie it failed to remove the state
	//it is zero
      }else if(rhsCopy.AddState(SingleParticleState(p,sigma))==false){ //ie it failed to add the state
	//it is zero
	
      } else{ // it wasn't zero check if it over laps with lhs
	if ( lhs == rhsCopy ){ 
	  //it overlaps
	  // cout<<"The ORdered State ";
	  // for (int i=0;i<rhsCopy.TheOrderedStates.size();i++){
	  //   cout<<"I = "<<i<<" "<<rhsCopy.TheOrderedStates[i]<<" ";//rhsCopy.TheOrderedStates.size()-1-i];
	  // }cout<<endl;
	  // cout<<"TERM"<<endl;
	  // cout<<"P IS "<<p<<endl;
	  // cout<<"Sigma "<<sigma<<endl;
	  // rhsCopy.Print();
	  int phase = GetPhase(rhsCopy.TheOrderedStates);
	  //	  cout<<"PHASE "<<phase<<endl;
	  totalOneBody=totalOneBody+ (p-1);
	  //	  int t;cin>>t;
	}
	
      }
    }//end s loop
  }//end p loop

  
  ///Calculate the two body term.
  double TotalTwoBody=0;
  for (int p=1;p<=NumSingleParticleStates/2;p++){
    for (int q=1;q<=NumSingleParticleStates/2;q++){
      SlaterDet rhsCopy=SlaterDet(rhs);
      

      //ad_p+ ad_p- a_q- a_q+
  
      if(rhsCopy.RemoveState(SingleParticleState(q,1))==false){ //ie it failed to remove the state
	//it is zero
      }else if(rhsCopy.RemoveState(SingleParticleState(q,-1))==false){ //ie it failed to add the state
	//it is zero
	
      } else if (rhsCopy.AddState(SingleParticleState(p,-1))==false){
	//it is zero
      } else if (rhsCopy.AddState(SingleParticleState(p,1))==false){
	//it is zero
      } else{ // it wasn't zero check if it over laps with lhs
	
      	if ( lhs == rhsCopy ){ 
	  TotalTwoBody=TotalTwoBody -g;
	}
	
      }
    }
  }
  

  // cout<<"Total One body "<<totalOneBody<<endl;
  // cout<<"Two body "<<TotalTwoBody<<endl;
  return totalOneBody+TotalTwoBody;
}

int main(){

  vector <SlaterDet> ListOfSD;
  BuildListOfSD(ListOfSD);


  for ( auto i : ListOfSD){
    //    i.Print();
  }

  
  vector <int> temp;

  
  for (auto i :ListOfSD){
    for (auto j: ListOfSD){
      cout <<GetMatrixElement(i,j)<<"  ";
    }
    cout<<endl;
  }
  


  return 0;
}
