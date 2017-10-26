#include "DicomSingleSlice.h"
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <iostream>

//Necessary before activate vtkrenderwindow
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

vtkImage DcmReadSingleSlice(const char *filepath) {

	ReaderType::Pointer reader = ReaderType::New();

	typedef itk::GDCMImageIO           ImageIOType;
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	
	reader->SetFileName(filepath);
	reader->SetImageIO(gdcmImageIO);
	reader->Update();

	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(reader->GetOutput());
	connector->Update();

	vtkImage image = vtkImage::New();
	image->DeepCopy(connector->GetOutput());
	//DcmShowSingleSlice(image,connector);
	return image;
}

void DcmPrintMetaData(vtkImage img) {
	using namespace std;
	cout << "Metadata:\n" <<
		"ImageExtent:" << img->GetExtent()[0] << " " << img->GetExtent()[1] << " " << img->GetExtent()[2] << " " <<
		img->GetExtent()[3] << " " << img->GetExtent()[4] << " " << img->GetExtent()[5] << "\n" <<
		"ImageSpacing:" << img->GetSpacing()[0] << " " << img->GetSpacing()[1] << " " << img->GetSpacing()[2] << "\n" <<
		"Origin:" << img->GetOrigin()[0] << " " << img->GetOrigin()[1] << " " << img->GetOrigin()[2] << "\n";
	//cout << "Press any key to continue.\n";
	//cin.get();
}

void DcmShowSingleSlice(vtkImage img) {

	vtkSmartPointer<vtkImageViewer> viewer = vtkSmartPointer<vtkImageViewer>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> rwin = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	viewer->SetupInteractor(rwin);
	viewer->SetInputData(img);

	viewer->Render();
	//viewer->SetColorLevel(128);
	//viewer->SetColorWindow(255);
	rwin->Start();
}
