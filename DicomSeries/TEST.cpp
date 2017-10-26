#include "DicomSeries.h"


//Debug Level:
//1.
//2.

int main() {
	//PrintVTKImageAttributes(SetupSeriesReader("D:\\DCM", 2));
	//PrintITKImageMetaData(SetupSeriesReader("D:\\DCM", 2)->GetOutput());
	GenerateDRR(SetupSeriesReader("D:\\DCM", 2));
	cin.get();
	return EXIT_SUCCESS;
}