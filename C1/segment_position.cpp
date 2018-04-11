#include <iostream>
using namespace std;

int a = 0;

void func() {
	int b = 1;
	cout << "Stack segment position = " << &b << endl;
}

int main() {

	func();	
	char* p1;
	p1 = (char*)malloc(10);

	cout << "Heap segment position = " << (int*)p1 << endl;
	cout << "Data segment position = " << &a << endl;
	cout << "Code segment position = " << (int*)func << endl;
	return 0;
}
