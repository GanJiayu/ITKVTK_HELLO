#include "itkImage.h"
#include "itkImageFileReader.h"
#include <iostream>
#include <iterator>
//#include <itk_png.h>
#include <itkPNGImageIO.h>
//#include <itkPNGImageIOFactory.h>
#include <itkImageToVTKImageFilter.h>
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
	itk::PNGImageIO::Pointer imageIO = itk::PNGImageIO::New();

	typedef itk::RGBPixel< unsigned char > PixelType;
	const unsigned int dimension = 2;

	typedef itk::Image<PixelType, dimension> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;

	//SmartPoint
	
	ReaderType::Pointer reader = ReaderType::New();

	reader->SetImageIO(imageIO);
	reader->SetFileName("D:\\SAMPLE.png");
	reader->Update();
	ImageType::Pointer image = reader->GetOutput();
	//ImageType::Pointer image = reader->GetOutput();
	
	const ImageType::IndexType PI = { {1,1} };
	ImageType::PixelType PV = image->GetPixel(PI);
	
	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;

	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();

	vtkSmartPointer<vtkImageViewer> viewer = vtkSmartPointer<vtkImageViewer>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> rwin = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkImageData> img = connector->GetOutput();
	std::cout << "HelloWorld:" << PV << std::endl;
	std::cout << img->GetNumberOfPoints()<<" "<<img->GetExtent() << std::endl;
	
	viewer->SetupInteractor(rwin);
	viewer->SetInputData(connector->GetOutput());
	

	viewer->Render();
	viewer->SetColorLevel(128);
	viewer->SetColorWindow(255);
	rwin->Start();

	//std::cout << "HelloWorld:" << PV <<std::endl;

	std::cin.get();

	return EXIT_SUCCESS;
}