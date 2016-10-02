#include <iostream>
#include<fstream>
#include "student.h"
using namespace std;

void displayStudent(const Student& student) {
  cout << student.getFirstName() << "";
  cout << student.getMi() << "";
  cout << student.getLastName() << "";
  cout << student.getScore() << endl;
}

int main () {
  fstream binaryio; // Create stram object

  // Open file for input and output
  binaryio.open("student.dat", ios::in | ios:: out | ios::binary);

  Student student1;
  binaryio.seekg(sizeof(Student));
  binaryio.read(reinterpret_cast<char*>(&student1), sizeof(student));
  displayStudent(student1);

  student1.setLastName("Yao");
  binaryio.seekp(sizeof(Student));
  binaryio.write(reinterpret_cast<char*>(&student), sizeof(student));

  student student2;
  binaryio.seekg(sizeof(Student));
  binaryio.read(reinterpret_cast<char*>(&student2), sizeof(student));
  displayStudent(student2);

  binaryio.close();

  return 0;
}
