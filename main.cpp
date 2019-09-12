#include "Phano.h"

int show_usage() {

	std::cout << "Usage: Phano [OPTIONS] input [output]";
	std::cout << "  The default action is to encode input file.";
	std::cout << "  -d\tDecode file.";
	std::cout << std::endl;
	std::cout << "Examples:";
	std::cout << "  Phano input.txt";
	std::cout << "  Phano input.txt encoded.bin";
	std::cout << "  Phano -d encoded.txt";
	std::cout << std::endl;
	exit(0);
}

int main(int argc, char **argv)
{
	int i = 1;
	int dFlag = 0;
	char inputFilename[128];
	char outputFilename[128];
	if (i == argc) 
		show_usage();
	if (strcmp(argv[i], "-d") == 0) {
		dFlag = 1;
		i++;
		if (i == argc) show_usage();
	}
	strcpy_s(inputFilename, argv[i]);
	i++;
	if (i < argc) {
		strcpy_s(outputFilename, argv[i]);
	}
	else {
		if (dFlag) {
			strcpy_s(outputFilename, "decoded.txt");
		}
		else {
			strcpy_s(outputFilename, "encoded.bin");
		}
	}
	//  Calling encoding or decoding subroutine
	//
	Phano a;
	if (!dFlag) {
		a.load(inputFilename);
		a.encoding(outputFilename);
	}
	else {
		a.decoding(inputFilename, outputFilename);
	}
	return 0;
}
