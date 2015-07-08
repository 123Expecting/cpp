#include <iostream>
#include <typeinfo>
#include "smart.h"
#include "t.h"
#include <stdexcept>
#include <memory>
using namespace std;

class simpleException {
public:
  simpleException() {
    cout << "init exception" << endl;
  }
  ~simpleException() {
    cout << "Dtor of an exception" << endl;
  }
  void what() const {
    cout << "WTF?" << endl;
  }
};

// Regenerate throw
bool isFixed = false;

void letstry() {
  cout << "Before try" << endl;
  try {
    cout << "Inside try: begin" << endl;
    if (!isFixed) {
      throw simpleException();
    } else {
      cout << "Fixed!" << endl;
    }
    cout << "Inside try: end" << endl;
  } catch(const simpleException& e) {
    e.what();
    cout << "Generated: " << typeid(e).name() << endl;
    if (!isFixed) {
      isFixed = true;
      throw;
    }
  }
}

void challenge1() {
  try {
    letstry();
  } catch (...) {
    if (isFixed) {
      cout << "Everything is okay" << endl;
    } else {
      cout << "Still not fixed! terminate()" << endl;
    }
  }
}

void lestMakeLeak2_1(int i) {
  T* p = new T();
  if(i == 0) {
    throw std::logic_error("Ruine challenge2");
  }
  delete p;
}

void lestMakeLeak2_2(int i) {
  smart<T> p(new T());
   if (i == 0) {
    throw std::logic_error("Ruine challenge2");
  }
}

// Check if T call dtor
void challenge2() {
  try {
    // Dtor called
    lestMakeLeak2_1(10);
    // Dtor could not be called => memory leak
    lestMakeLeak2_1(0);
  } catch (...) {
    cout << "Catched!" << endl;
  }
  // using smart pointer
  try {
    // Dtor called
    lestMakeLeak2_2(10);
    // Dtor also called
    lestMakeLeak2_2(0);
  } catch (...) {
    cout << "Catched!" << endl;
  }
}

// Test auto_ptr
void challenge3() {
  auto_ptr<T> p1(new T());
  auto_ptr<T> p2 = p1;
  // Segmentation Fault
  // cout << p1.get()->getID() << endl;
  // p2 owned new T(), p1 == NULL
  cout << p2.get()->getID() << endl;
  const auto_ptr<T> cp1(new T());
  // auto_ptr<T> cp2(cp1); // - compilation error
  // cp1.release(); // - compilation error
}

// Exceptions in ctors
void challenge4() {
  try {
    A obj(T(1));
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
// Exceptions in dtors
void challenge5() {
  try {
    // std::unexpected()
    // B obj(T(1));
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

/* -------------------- */
int main() {
  // challenge1();
  // challenge2();
  // challenge3();
  // challenge4();
  challenge5();
  return 0;
}
