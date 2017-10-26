#pragma once
//#include <itkImageFileReader.h>
//#include <itkRescaleIntensityImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageToVTKImageFilter.h>
//#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
//#include <vtkImageViewer.h>
//#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

typedef signed short									InputPixelType;
const unsigned int										InputDimension = 2;
typedef itk::Image < InputPixelType, InputDimension >	InputImageType;
typedef itk::ImageSeriesReader< InputImageType >		ReaderType;
typedef itk::ImageToVTKImageFilter<InputImageType>		ConnectorType;
typedef vtkSmartPointer<vtkImageData>					vtkImage;

//Interfaces:
vtkImage DcmReadSingleSlice(const char*);
void DcmShowSingleSlice(vtkImage);
void DcmPrintMetaData(vtkImage);


