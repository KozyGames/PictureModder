#include <iostream>
#include <string>

#include "Picture.h"
#include "Utility.h"

using namespace std;
using namespace Kozy;


int main() {
	string fileName = "resources//dog_bmp.bmp";
	string output = "a";
	//cout << Utility::fileExists(fileName.c_str()) << endl;
	//cout << Utility::fileExists(("resources//"+fileName).c_str()) << endl;

	cout.flush();

	try {
		Picture pic(fileName.c_str());


		pic.save(output.c_str());



	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	cout << "Enter \"e\",\"E\" or \"e\"\n to finish program.";
	for(string s; cin>>s&&s!="e"&&s!="E"&&s!="exit";)
		cout<<"Processing\n";

	return 0;
}