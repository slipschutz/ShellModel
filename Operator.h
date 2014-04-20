#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include "TMatrixD.h"
using namespace std;


class Operator {

public:
  Operator();
  //  virtual double Apply(vector<bool>& ){cout<<"Nope"<<endl;}
  virtual vector <string> GetSubScript(){return theSubScripts;}
  virtual string GetType()=0;
  virtual void Print(){cout<<"nope"<<endl;}
  virtual void PrintLatex(){cout<<"nope"<<endl;}
  virtual void SetSubScript(vector <string> s);
  virtual void SetSubScript(string s){theSubScripts.clear();theSubScripts.push_back(s);}
  virtual void SetSubScript(string s1,string s2){theSubScripts.clear();
    theSubScripts.push_back(s1);theSubScripts.push_back(s2);}

  vector <string> theSubScripts;
  int Phase;
};

Operator::Operator() 
{Phase=1;theSubScripts.clear();}

void Operator::SetSubScript(vector <string> s){
  theSubScripts.clear();
  theSubScripts=s;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class Raise : public Operator{
public:
  Raise();
  Raise(string s,int p){SetSubScript(s);Phase=p;}
  Raise(string s1,string s2,int p){SetSubScript(s1,s2);Phase=p;}
  Raise(vector <string> vec){SetSubScript(vec);}
  void Print(){cout<<"R_"<<theSubScripts[0]<<" ";}
  void PrintLatex(){cout<<"\\RO{";copy(theSubScripts.begin(),theSubScripts.end(),ostream_iterator<string>(cout,""));
    cout<<"} ";}

  string GetType(){return "raise";}
  
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class Lower : public Operator{
public:
  Lower();
  Lower(vector <string> vec){SetSubScript(vec);}
  Lower(string s,int p){SetSubScript(s);Phase=p;}
  Lower(string s1,string s2,int p){SetSubScript(s1,s2);Phase=p;}
  void Print(){cout<<"L_"<<theSubScripts[0]<<" ";}
  void PrintLatex(){cout<<"\\LO{";copy(theSubScripts.begin(),theSubScripts.end(),ostream_iterator<string>(cout,""));
    cout<<"} ";}

  string GetType(){return "lower";}
};

Lower::Lower()
{}

Raise::Raise()
{}
