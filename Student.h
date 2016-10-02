#ifndef STUDENT_H
#define STUDENT_H
#include <string>
using namespace std;

class Student {
public:
  Student ();
  Student (const string& firstname, char mi, 
           const string& lastName, int score);  
  void setFirstName(const string& s);
  void setmi(char mi);
  void setLastName(const string& s);
  void setScore(int score);
  strinr getFirstName() const;
  char getMi() const;
  string getLastName() const;
  int getScore() const;

private:
  char firstName[25];
  char mi;
  char lastName[25];
  int score;
};
