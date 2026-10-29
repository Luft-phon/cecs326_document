#include <iostream>
using namespace std;

int main() {
int a = 10;
// The pointer 'p' is declared but not used in the original code.
// If you wanted to use it, you would declare it here like this:
int *p = &a;
cout << "Value of a: " << a << endl;
cout << "Address of &a: " << &a << endl;
cout << "Value of *p:" << *p << endl;
cout << "Address of p: " << p << endl;
return 0;
}
