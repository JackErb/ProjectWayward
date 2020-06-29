#include "Tests.hpp"
#include "../Game/MathHelper.hpp"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

bool TestFpoats() {
	bool err = false;

	fpoat f1(2, 0);
	cout << f1.f() << " = 2.0" << endl;

	fpoat f2(0, 5);
	cout << f2.f() << " = 0.5" << endl;
	cout << (f1 + f2).f() << " = 2.5" << endl;
	cout << (f2 + f1).f() << " = 2.5" << endl;
	cout << (f1 - f2).f() << " = 1.5" << endl;
	cout << (f2 - f1).f() << " = -0.5" << endl;

	return true;
}