#include <iostream>
#include <string>

#include "Picture.h"
#include "Utility.h"
#include "Exception_Handling.h"

#include "Picture_Algorithms.h"

using namespace std;
using namespace Kozy;


int main() {
	const string fileName = "resources//dog_jpg.bmp";
	const string output1 = "output//a_1";
	const string output2 = "output//a_2";
	const string output3 = "output//a_3";
	const string output4 = "output//a_4";

	//cout << Utility::fileExists(fileName.c_str()) << endl;
	//cout << Utility::fileExists(("resources//"+fileName).c_str()) << endl;

	cout.flush();




	try {
		// Copies data of picture into this data structure
		// original picture data is safe!
		Picture pic(fileName.c_str());


		//uses an algorithm on the picture, defined in Picture_Algorithm
		Algorithm::changeToColorFlair(pic, 255);
		pic.save(output1.c_str());

		const Picture pic_copy = pic; // copy of pic. It is a const Object and can not be modified


		/*
		// Algorithm::changeToColorFlair(pic, 255); is equal to:
		
		for (unsigned long line = 0; line <= pic.getRes().height; line+=2) {
			for (unsigned ppos = 0; ppos != pic.getRes().width; ++ppos) 
				pic[line][ppos].setRGB(255);
		}
		*/
		

		// same as the above, but uses a different color for each pixel
		unsigned color[3] = { 0,200,100 };
		Algorithm::changeToColorFlair(pic, color);
		pic.save(output2.c_str()); // I forbid overwritting existing files. That is why we need to use a different name. 
		// Advanced users can use "save_Picture_BMP", which is hidden in the namespace "FileParsing". That function allows to overwrite existing files
		

		color[0] = 200; color[1] = 0; color[2] = 0; // red

		Algorithm::changeToColorFlair(pic, color);
		pic.setPictureOrientation(!pic.getOrientation()); // flips the picture
		pic.save(output3.c_str());


		pic_copy.save(output4.c_str()); // copy of a previous version of the picture. Should look like output1 picture

	}
	catch (const Invalid_Argument_obj& e) {
		cout << e.what() << endl;
	}
	catch (const Runtime_Error_obj& e) {
		cout << e.what() << endl;
	}
	catch (const Exception_obj& e) {
		cout << e.what() << endl;
	}

	cout << "Enter \"e\",\"E\" or \"e\" to finish program: ";
	for(string s; cin>>s 
		&& s!="e" 
		&&s!="E"
		&&s!="exit";)
		cout<<"Processing\n";

	return 0;
}