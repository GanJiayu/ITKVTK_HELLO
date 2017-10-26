
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>


typedef signed short								PixelType;
const unsigned int									Dimension = 3;
typedef itk::Image<PixelType, Dimension>			InputImageType;
typedef itk::ImageSeriesReader<InputImageType>		ReaderType;
typedef itk::GDCMImageIO							ImageIOType;
typedef itk::GDCMSeriesFileNames					DIRScanerType;





void PrintMetaData(const char* filepath, int debuglevel);
