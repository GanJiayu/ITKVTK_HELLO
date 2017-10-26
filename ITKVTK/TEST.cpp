#include "DicomSingleSlice.h"

int main() {
	using namespace std;
	const char *fp = "D:\\DCM\\CT2.16.840.1.113662.2.12.0.3049.1309540672.259.dcm";

	vtkSmartPointer<vtkImageData> oneslice = vtkImage::New();

	oneslice = DcmReadSingleSlice(fp);
	//ConnectorType::Pointer vtkPlug = DcmGetPlugToVTK(fp);
	DcmPrintMetaData(oneslice);
	DcmShowSingleSlice(oneslice);

	return EXIT_SUCCESS;
}
