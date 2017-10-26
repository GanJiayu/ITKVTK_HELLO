#include <itkImageFileReader.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageToVTKImageFilter.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include "vtkAutoInit.h" 
#include <vtkImageData.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);


int main() {
	//signed short: Routine CT DICOM PixelType
	typedef signed short InputPixelType;
	const unsigned int   InputDimension = 3;

	typedef itk::Image < InputPixelType, InputDimension > InputImageType;
	typedef itk::ImageSeriesReader< InputImageType > ReaderType;
	ReaderType::Pointer reader = ReaderType::New();

	typedef itk::GDCMImageIO           ImageIOType;
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	reader->SetImageIO(gdcmImageIO);


	reader->SetFileName("D:\\DCM\\CT2.16.840.1.113662.2.12.0.3049.1309540672.259.dcm");
	
	reader->Update();

	typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(reader->GetOutput());
	connector->Update();

	vtkSmartPointer<vtkImageViewer> viewer = vtkSmartPointer<vtkImageViewer>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> rwin = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkImageData> img = connector->GetOutput();
	

	viewer->SetupInteractor(rwin);
	viewer->SetInputData(connector->GetOutput());


	viewer->Render();
	viewer->SetColorLevel(128);
	viewer->SetColorWindow(255);
	rwin->Start();




	return EXIT_SUCCESS;
}