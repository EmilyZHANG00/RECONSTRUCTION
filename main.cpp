#include "ReconstrutionTest.h"
int  main()
{
	
	ReconstrutionTest  test;
	test.get_data("data/inputfile.txt");
	test.get_reconstruction_result();
	return 0;
}