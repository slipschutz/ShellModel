


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "TMath.h"

#include "TMatrixD.h"
//#include "Operator.h"
using namespace std;

int NumSingleParticleStates=8;
int NumParticles =4;
double g=1;
vector <int> Sort(vector <int> );

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
  void Clear(){TheMapOfStates.clear();TheOrderedMap.clear();TheStates.clear();TheOrderedStates.clear();}
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
  
  int NumPLevels=NumSingleParticleStates/2;
  int NumParticlePairs=2;
  map<vector<int>,bool> tempMap;
  
  double NumSD = TMath::Factorial(NumPLevels)/( TMath::Factorial(NumParticlePairs)*TMath::Factorial(NumPLevels-NumParticlePairs));
  cout<<"There should be "<<NumSD<<" Slater Dets"<<endl;

 
  for (int p=1;p<=NumPLevels;p++){
    SlaterDet temp;
    temp.AddState(SingleParticleState(p,1));
    temp.AddState(SingleParticleState(p,-1));
    for (int i=0;i<NumParticlePairs-1;i++){
      int count=0;
      vector <int> t; 
      for (int j=1;j<=NumPLevels;j++){


	cout<<"IN J LOOP"<<endl;
	SlaterDet copy=SlaterDet(temp);
	bool b1=copy.AddState(SingleParticleState(j,1));
	bool b2=copy.AddState(SingleParticleState(j,-1));
	//check to see if we tried to add a state twice
	if (b1 == true && b2 == true ){//only if both succeeded
	  //j != p at this point
	  temp.AddState(SingleParticleState(j,1));
	  temp.AddState(SingleParticleState(j,-1));
	
	  t.push_back(p);
	  t.push_back(j); //these were the P-levels added 
	  t=Sort(t);
	  if (tempMap.count(t)==0){
	    //Victory
	    cout<<"Victory"<<endl;
	    List.push_back(temp);
	    count++;
	    temp.Print();	    
	    tempMap[t]=true;
	    temp.Clear();
	  }//end count if
	}//end b1 b2 if	
	if (count==1){
	  j=NumPLevels+3;
	  count=0;
	}
      }//end j loop
    }//end i loop
  } //end p loop
  

}//end


void BuildListOfSD2(vector <SlaterDet> & List){
  
  int NumPLevels=NumSingleParticleStates/2;
  int NumParticlePairs=NumParticles/2;
  map<vector<int>,bool> tempMap;
  
  double NumSD = TMath::Factorial(NumPLevels)/( TMath::Factorial(NumParticlePairs)*TMath::Factorial(NumPLevels-NumParticlePairs));
  cout<<"There should be "<<NumSD<<" Slater Dets"<<endl;


  if (NumParticlePairs== 1){
    for (int p=1;p<=NumPLevels;p++){
      SlaterDet temp;
      temp.AddState(SingleParticleState(p,1));
      temp.AddState(SingleParticleState(p,-1));
      List.push_back(temp);
    }
  } else if (NumParticlePairs==2){
    for (int p1=1;p1<=NumPLevels;p1++){
      for (int p2=1;p2<=NumPLevels;p2++){
	SlaterDet temp;
	vector<int> t;
	bool b1=temp.AddState(SingleParticleState(p1,1));
	temp.AddState(SingleParticleState(p1,-1));

	bool b2=temp.AddState(SingleParticleState(p2,1));
	temp.AddState(SingleParticleState(p2,-1));
	if (b1 && b2){
	  t.push_back(p1);
	  t.push_back(p2);
	  t=Sort(t);

	  if (tempMap.count(t)==0){
	    List.push_back(temp);
	    tempMap[t]=true;
	  }
	}
      }
    }

  }else if (NumParticlePairs ==3 ){
    for (int p1=1;p1<=NumPLevels;p1++){
      for (int p2=1;p2<=NumPLevels;p2++){
	for (int p3=1;p3<=NumPLevels;p3++){

	  SlaterDet temp;
	  vector<int> t;
	  bool b1=temp.AddState(SingleParticleState(p1,1));
	  temp.AddState(SingleParticleState(p1,-1));

	  bool b2=temp.AddState(SingleParticleState(p2,1));
	  temp.AddState(SingleParticleState(p2,-1));

	  bool b3=temp.AddState(SingleParticleState(p3,1));
	  temp.AddState(SingleParticleState(p3,-1));


	  if (b1 && b2 && b3){
	    t.push_back(p1);
	    t.push_back(p2);
	    t.push_back(p3);
	    t=Sort(t);

	    if (tempMap.count(t)==0){
	      List.push_back(temp);
	      tempMap[t]=true;
	    }
	  }
	}
      }
    }
  }else if (NumParticlePairs==4){
    for (int p1=1;p1<=NumPLevels;p1++){
      for (int p2=1;p2<=NumPLevels;p2++){
	for (int p3=1;p3<=NumPLevels;p3++){
	  for (int p4=1;p4<=NumPLevels;p4++){
	    SlaterDet temp;
	    vector<int> t;
	    bool b1=temp.AddState(SingleParticleState(p1,1));
	    temp.AddState(SingleParticleState(p1,-1));
	    
	    bool b2=temp.AddState(SingleParticleState(p2,1));
	    temp.AddState(SingleParticleState(p2,-1));
	    
	    bool b3=temp.AddState(SingleParticleState(p3,1));
	    temp.AddState(SingleParticleState(p3,-1));
	    
	    bool b4=temp.AddState(SingleParticleState(p4,1));
	    temp.AddState(SingleParticleState(p4,-1));
	    
	    

	    if (b1 && b2 && b3 &&b4){
	      t.push_back(p1);
	      t.push_back(p2);
	      t.push_back(p3);
	      t.push_back(p4);
	      t=Sort(t);

	      if (tempMap.count(t)==0){
		List.push_back(temp);
		tempMap[t]=true;
	      }
	    }
	  }
	}
      }
    }
  } else if (NumParticlePairs==5){
    throw "Not Done Yet";
  } else if (NumParticlePairs==6){
    throw "Not Done Yet";

  }

}


vector <int> Sort(vector <int> input){
  vector <int> temp(100,0);
  vector <int> temp2;
  for (auto i : input){
    temp[i]=i;
  }
  for (auto j : temp ){
    if (j!=0){
      temp2.push_back(j);
    }
  }
  return temp2;
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
#include "TVectorT.h"
int main(int argc,char** argv){

  if (argc ==2){
    g=atof(argv[1]);
      cout<<"G is "<<g<<endl;
      cout<<"Num Particles? "<<endl;
      cin>>NumParticles;
      cout<<"Num SP?"<<endl;
      cin>>NumSingleParticleStates;

  } else if (argc == 4){
    g=atof(argv[1]);
    NumParticles=atoi(argv[2]);
    NumSingleParticleStates=atoi(argv[3]);
  } else {
    cout<<"Usage: ./main g"<<endl;
    cout<<"Usage: ./main g NumParticles NumSPStates"<<endl;
    return -1;
  }
      

  vector <SlaterDet> ListOfSD;
  


  BuildListOfSD2(ListOfSD);


  for ( auto i : ListOfSD){
    i.Print();
  }

  

  TMatrixD theMatrix(ListOfSD.size(),ListOfSD.size());
  
  for (int i=0;i<ListOfSD.size();i++){
    for (int j=0;j<ListOfSD.size();j++){
      theMatrix[i][j]=GetMatrixElement(ListOfSD[i],ListOfSD[j]);
      
    }
  }
  theMatrix.Print();
  
  TVectorT <double> eigenValues;
  TMatrixD result=theMatrix.EigenVectors(eigenValues);

  result.Print();
  
  eigenValues.Print();

  return 0;

  for (int i=0;i< theMatrix.GetNrows();i++){
    cout<<"{";
    for (int j=0;j<theMatrix.GetNcols();j++){
      if (j<theMatrix.GetNcols()-1){
	cout<<theMatrix[i][j]<<",";
      } else {
	cout<<theMatrix[i][j];
      }
    }
    cout<<"},";
  }

}
