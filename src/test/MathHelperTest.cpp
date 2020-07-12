#include "Tests.hpp"
#include "../MathHelper/MathHelper.h"
#include "../MathHelper/Random.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

void TestFpoats() {
    cout << "TestFpoats:" << endl;
    
    fpoat f1(2, 0);
    fpoat f2(0, 5);
    cout << f1.f() << " = 2.0" << endl;
	cout << f2.f() << " = 0.5" << endl;
	cout << (f1 + f2).f() << " = 2.5" << endl;
	cout << (f2 + f1).f() << " = 2.5" << endl;
	cout << (f1 - f2).f() << " = 1.5" << endl;
	cout << (f2 - f1).f() << " = -1.5" << endl;
    cout << (f1 * f2).f() << " = 1.0" << endl;
    cout << (f2 * f1).f() << " = 1.0" << endl;
    cout << (f1 / f2).f() << " = 4.0" << endl;
    cout << (f2 / f1).f() << " = 0.25" << endl;
    
    fpoat f3(100,5);
    fpoat f4(0,85);
    fpoat f5(0,1);
    cout << (f3 + f4).f() << " = 101.35" << endl;
    cout << (f3 * f4).f() << " = 85.425" << endl; 
    cout << (f3 / f4).f() << " = 118.2352" << endl;
    cout << (f4 / f3).f() << " = 0.0084" << endl;
    cout << (f3 / f5).f() << " = 1005" << endl;

	cout << fpsqrt(fpoat(4, 0)).f() << " = 2" << endl;

	fpoat e1(0, 843);
	fpoat e2(0, 2351);
	cout << fpsqrt(e1 * e1 + e2 * e2).f() << " = .8751" << endl;

	cout << (-fpoat(2, 4215)).f() << " = -2.4215" << endl;

	cout << "Max: " << FixedPoint::MAX.f() << ", Min: " << FixedPoint::MIN.f() << endl;
}

void TestRand() {
    Random r = Random(64);
    
    for (int i = 0; i < 10; i++) {
        cout << r.rand() << endl;
    }
    
    for (int i = 0; i < 100; i++) {
        cout << r.rand(0, 10) << endl;
    }
}
