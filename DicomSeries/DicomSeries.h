#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkResampleImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <itkCenteredEuler3DTransform.h>
#include <itkRayCastInterpolateImageFunction.h>

typedef signed short								InputPixelType;
const unsigned int									Dimension = 3;
typedef itk::Image<InputPixelType, Dimension>			InputImageType;
typedef itk::ImageSeriesReader<InputImageType>		ReaderType;
typedef itk::GDCMImageIO							ImageIOType;
typedef itk::GDCMSeriesFileNames					DIRScanerType;
typedef vtkSmartPointer<vtkImageData>				vtkImageType;
typedef itk::ImageToVTKImageFilter<InputImageType>		ConnectorType;



ReaderType::Pointer SetupSeriesReader(const char* filepath, int debuglevel);
void PrintVTKImageAttributes(ReaderType::Pointer reader);
void GenerateDRR(ReaderType::Pointer reader);
void PrintITKImageMetaData(InputImageType::Pointer image);