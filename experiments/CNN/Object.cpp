#include <stdio.h>
#include <string>
#include <vector>

class Object
{
	public: 
		Object();
		Object(int c[4], std::string new_label);
		void set_name(std::string new_label);	
		
		void set_bnd_box(int c[4]);			// coordinates for the bounding box of the object
											// (Xmin, Ymin, Xmax, Ymax) == (x0, y0, x1, y1)
	private: 
		int x0;
		int y0; 

		int x1;
		int y1;

		std::string label;	// name of object

};

Object::Object(void)
{
	std::cout << "Empty Object created" << std::endl;
}

Object::Object(int c[4], std::string new_label)
{
	label = new_label;
	x0 = c[0];
	y0 = c[1];
	x1 = c[2];
	y1 = c[3];

	printf("Object created with values");
	printf("Label : %s\n", label.c_str());
	printf("(Xmin, Ymin) - (Xmax, Ymax) : (%d, %d) - (%d, %d)", x0, y0, x1, y1);
}

void Object::set_name(std::string new_label)
{
	label = new_label;
}

void Object::set_bnd_box(int c[4])
{
	x0 = c[0];
	y0 = c[1];
	x1 = c[2];
	y1 = c[3];
}