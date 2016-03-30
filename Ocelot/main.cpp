#include <iostream>

#include "shunting-yard.h"
#include "interp.h"
#include "variable.h"
using namespace std;

#include <string>
#include <fstream>
#include <streambuf>

int main(int argc, char** argv) 
{

	std::ifstream t("test.ocl");
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());


	Interpreter interp(str);
	interp.exec();


	FloatVariable f(3.99f);
	FloatVariable f2(4.0f);
	FloatVariable f3(3.99f);
	DoubleVariable dv(4.5);
	StringVariable sv("Hello world");
	StringVariable sv2("Hello world");



	cout << "float with double? " << f.compatibleWith(dv) << "\n";
	cout << "float with string? " << f.compatibleWith(sv) << "\n";
	cout << "type name: " << f.getTypeInfo().getTypeName() << "\n";
	cout << "type name: " + sv.getTypeInfo().getTypeName() << "\n";
	cout << "3.99 == 4.0 ? " << f.equalTo(f2) << "\n";
	cout << "3.99 == 3.99 ? " << f.equalTo(f3) << "\n";
	cout << "4.0 == 3.99 ? " << f2.equalTo(f) << "\n";
	cout << "3.99 == \"Hello world\" ? " << f.equalTo(sv) << "\n";
	cout << "\"Hello world\" == \"Hello world\" ? " << sv.equalTo(sv2) << "\n";
	

	system("pause");
	return 0;
}
