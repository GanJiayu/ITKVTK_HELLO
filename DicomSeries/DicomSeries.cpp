#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include "DicomSeries.h"
#include "vtkAutoInit.h" 
#include <vtkRenderWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);


ReaderType::Pointer SetupSeriesReader(const char* filepath, int debuglevel) {
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();

	reader->SetImageIO(dicomIO);

	DIRScanerType::Pointer dirscanner = DIRScanerType::New();
	dirscanner->SetUseSeriesDetails(true);
	dirscanner->SetDirectory(filepath);

	typedef std::vector<std::string>	UIDSeriesContainer;
	typedef std::vector<std::string>	FileNamesContainer;
	const UIDSeriesContainer & seriesUID = dirscanner->GetSeriesUIDs();

	UIDSeriesContainer::const_iterator seriesItr = seriesUID.begin();
	

	if (debuglevel <= 1) {
		UIDSeriesContainer::const_iterator seriesEnd = seriesUID.end();
		std::cout << "SeriesUID:" << std::endl;
			while (seriesItr != seriesEnd) {
				std::cout << seriesItr->c_str() << std::endl;
				++seriesItr;
			}
	}
	

	std::string targetSeriesUID = seriesUID.begin()->c_str();

	FileNamesContainer filenames;

	filenames = dirscanner->GetFileNames(targetSeriesUID);
	if (debuglevel <= 1) {
		std::cout << "FileNames:" << std::endl;
		UIDSeriesContainer::const_iterator fileItr = filenames.begin();
		UIDSeriesContainer::const_iterator fileEnd = filenames.end();

		while (fileItr != fileEnd) {
			std::cout << fileItr->c_str() << std::endl;
			++fileItr;
		}
	}

	reader->SetFileNames(filenames);
	reader->Update();
	if (debuglevel <= 0) {
		std::cout << "Press any key to continue.\n";
		std::cin.get();
	}
	return reader;
}

void PrintVTKImageAttributes(ReaderType::Pointer reader) {
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(reader->GetOutput());
	connector->Update();

	vtkImageType image = vtkImageType::New();
	image->DeepCopy(connector->GetOutput());

	cout << "Metadata:\n" <<
		"ImageExtent:" << image->GetExtent()[0] << " " << image->GetExtent()[1] << " " << image->GetExtent()[2] << " " <<
		image->GetExtent()[3] << " " << image->GetExtent()[4] << " " << image->GetExtent()[5] << "\n" <<
		"ImageSpacing:" << image->GetSpacing()[0] << " " << image->GetSpacing()[1] << " " << image->GetSpacing()[2] << "\n" <<
		"Origin:" << image->GetOrigin()[0] << " " << image->GetOrigin()[1] << " " << image->GetOrigin()[2] << "\n";
	cout << "Press any key to continue.\n";
	//cin.get();
}

void PrintITKImageMetaData(InputImageType::Pointer image) {
	using namespace std;
	InputImageType::SpacingType spacing = image->GetSpacing();
	InputImageType::PointType	origin = image->GetOrigin();
	InputImageType::RegionType	region = image->GetLargestPossibleRegion();
	InputImageType::IndexType	start = region.GetIndex();
	InputImageType::SizeType	extent = region.GetSize();
	cout << "Spacing:" <<
		spacing[0] << " " << spacing[1] << " " << spacing[2] << " " << endl;
	cout << "Origin:" <<
		origin[0] << " " << origin[1] << " " << origin[2] << " " << endl;
	cout << "Init-Index:" <<
		start[0] << " " << start[1] << " " << start[2] << " " << endl;
	cout << "Extent:" <<
		extent[0] << " " << extent[1] << " " << extent[2] << " " << endl;

	itk::Matrix<double> direction = image->GetDirection();
	cout << "Direction:" <<
		direction[0][0] << " " << direction[0][1] <<
		" " << direction[0][2] << " " << endl <<
		direction[1][0] << " " << direction[1][1] <<
		" " << direction[1][2] << " " << endl <<
		direction[2][0] << " " << direction[2][1] <<
		" " << direction[2][2] << " "  << endl;
	//cin.get();
}

InputImageType::Pointer OverrideImageOEDS(InputImageType::Pointer image) {
	InputImageType::PointType origin;
	origin[0] = 0;
	origin[1] = 0;
	origin[2] = 0;

	InputImageType::SpacingType spacing = image->GetSpacing();
	itk::Matrix<double> direction; 
	direction[0][0] = 1;
	direction[0][1] = 0;
	direction[0][2] = 0;
	direction[1][0] = 0;
	direction[1][1] = 1;
	direction[1][2] = 0;
	direction[2][0] = 0;
	direction[2][1] = 0;
	direction[2][2] = 1;

	image->SetOrigin(origin);
	image->SetDirection(direction);
	image->Update();
	return image;
}

void GenerateDRR(ReaderType::Pointer reader) {

	double SDD = 400;
	double bTx = 0.0, bTy = 0.0, bTz = 0.0;
	double bRx = 90.0, bRy = 0.0, bRz = 0.0;
	double cTx = 0.0, cTy = 0.0, cTz = 0.0;
	double dSx = 1.0, dSy = 1.0, dSz = 1.0;
	double dTx = 0.0, dTy = 0.0;
	double Thres = 0;

	typedef   unsigned char								OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension >	OutputImageType;

	InputImageType::Pointer ctImage3d = reader->GetOutput();
	ctImage3d = OverrideImageOEDS(ctImage3d);
	PrintITKImageMetaData(ctImage3d);//verbose print

	InputImageType::Pointer image = InputImageType::New();
	if (true) {
		

		InputImageType::SpacingType spacing;
		spacing[0] = 3.;
		spacing[1] = 3.;
		spacing[2] = 3.;
		image->SetSpacing(spacing);

		InputImageType::PointType origin;
		origin[0] = 0.;
		origin[1] = 0.;
		origin[2] = 0.;
		image->SetOrigin(origin);

		InputImageType::IndexType start;

		start[0] = 0;  // first index on X
		start[1] = 0;  // first index on Y
		start[2] = 0;  // first index on Z

		InputImageType::SizeType  size;

		size[0] = 61;  // size along X
		size[1] = 61;  // size along Y
		size[2] = 61;  // size along Z

		InputImageType::RegionType region;

		region.SetSize(size);
		region.SetIndex(start);

		image->SetRegions(region);
		image->Allocate(true); // initialize to zero.

		image->Update();

		typedef itk::ImageRegionIteratorWithIndex< InputImageType > IteratorType;

		IteratorType iterate(image, image->GetLargestPossibleRegion());

		while (!iterate.IsAtEnd())
		{

			InputImageType::IndexType idx = iterate.GetIndex();

			if ((idx[0] >= 6) && (idx[0] <= 54)
				&& (idx[1] >= 6) && (idx[1] <= 54)
				&& (idx[2] >= 6) && (idx[2] <= 54)

				&& ((((idx[0] <= 11) || (idx[0] >= 49))
					&& ((idx[1] <= 11) || (idx[1] >= 49)))

					|| (((idx[0] <= 11) || (idx[0] >= 49))
						&& ((idx[2] <= 11) || (idx[2] >= 49)))

					|| (((idx[1] <= 11) || (idx[1] >= 49))
						&& ((idx[2] <= 11) || (idx[2] >= 49)))))
			{
				iterate.Set(10);
			}

			else if ((idx[0] >= 18) && (idx[0] <= 42)
				&& (idx[1] >= 18) && (idx[1] <= 42)
				&& (idx[2] >= 18) && (idx[2] <= 42)

				&& ((((idx[0] <= 23) || (idx[0] >= 37))
					&& ((idx[1] <= 23) || (idx[1] >= 37)))

					|| (((idx[0] <= 23) || (idx[0] >= 37))
						&& ((idx[2] <= 23) || (idx[2] >= 37)))

					|| (((idx[1] <= 23) || (idx[1] >= 37))
						&& ((idx[2] <= 23) || (idx[2] >= 37)))))
			{
				iterate.Set(60);
			}

			else if ((idx[0] == 30) && (idx[1] == 30) && (idx[2] == 30))
			{
				iterate.Set(100);
			}

			++iterate;
		}
	}


	typedef itk::ResampleImageFilter<InputImageType, InputImageType > FilterType;

	FilterType::Pointer filter = FilterType::New();

	filter->SetInput(ctImage3d);
	filter->SetDefaultPixelValue(0);

	typedef itk::CenteredEuler3DTransform< double >  TransformType;

	TransformType::Pointer transform = TransformType::New();

	transform->SetComputeZYX(true);

	TransformType::OutputVectorType translation;

	//TRANSLATION
	translation[0] = bTx;
	translation[1] = bTy;
	translation[2] = bTz;
	const double dtr = (std::atan(1.0) * 4.0) / 180.0;
	//ROTATION
	transform->SetTranslation(translation);
	transform->SetRotation(dtr*bRx, dtr*bRy, dtr*bRz);


	InputImageType::PointType   imOrigin = image->GetOrigin();
	InputImageType::SpacingType imRes = image->GetSpacing();

	typedef InputImageType::RegionType     InputImageRegionType;
	typedef InputImageRegionType::SizeType InputImageSizeType;

	InputImageRegionType imRegion = image->GetBufferedRegion();
	InputImageSizeType   imSize = imRegion.GetSize();
	imOrigin[0] += imRes[0] * static_cast<double>(imSize[0]) / 2.0;
	imOrigin[1] += imRes[1] * static_cast<double>(imSize[1]) / 2.0;
	imOrigin[2] += imRes[2] * static_cast<double>(imSize[2]) / 2.0;


	TransformType::InputPointType center;
	center[0] = 0.0 + imOrigin[0];
	center[1] = 0.0 + imOrigin[1];
	center[2] = 0.0 + imOrigin[2];

	transform->SetCenter(center);

	if (true)
	{
		std::cout << "Image size: "
			<< imSize[0] << ", " << imSize[1] << ", " << imSize[2]
			<< std::endl << "   resolution: "
			<< imRes[0] << ", " << imRes[1] << ", " << imRes[2]
			<< std::endl << "   origin: "
			<< imOrigin[0] << ", " << imOrigin[1] << ", " << imOrigin[2]
			<< std::endl << "   center: "
			<< center[0] << ", " << center[1] << ", " << center[2]
			<< std::endl << "Transform: " << transform << std::endl;
	}

	typedef itk::RayCastInterpolateImageFunction<InputImageType, double>
		InterpolatorType;
	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	interpolator->SetTransform(transform);
	interpolator->SetThreshold(Thres);

	
	interpolator->Print(std::cout);

	filter->SetInterpolator(interpolator);
	filter->SetTransform(transform);

	InputImageType::SizeType   size;

	size[0] = 511;  // number of pixels along X of the 2D DRR image
	size[1] = 511;  // number of pixels along Y of the 2D DRR image
	size[2] = 1;   // only one slice

	filter->SetSize(size);

	InputImageType::SpacingType spacing;

	spacing[0] = 1.0;  // pixel spacing along X of the 2D DRR image [mm]
	spacing[1] = 1.0;  // pixel spacing along Y of the 2D DRR image [mm]
	spacing[2] = 1.0; // slice thickness of the 2D DRR image [mm]

	filter->SetOutputSpacing(spacing);

	double origin[Dimension];

	origin[0] = imOrigin[0] - 0 - 1.29*((double)511 - 1.) / 2.;
	origin[1] = imOrigin[1] - 0 - 1.29*((double)511 - 1.) / 2.;
	//origin[0] = 0;
	//origin[1] = 0;
 	origin[2] = imOrigin[2] + SDD / 1000.0;

	filter->SetOutputOrigin(origin);

	InterpolatorType::InputPointType focalpoint;

	focalpoint[0] = imOrigin[0] - imOrigin[0];
	focalpoint[1] = imOrigin[1] - imOrigin[1];
	focalpoint[2] = imOrigin[2] - SDD;

	interpolator->SetFocalPoint(focalpoint);

	filter->Update(); 

	std::cout << "Output image size: "
		<< size[0] << ", "
		<< size[1] << ", "
		<< size[2] << std::endl;

	std::cout << "Output image spacing: "
		<< spacing[0] << ", "
		<< spacing[1] << ", "
		<< spacing[2] << std::endl;

	std::cout << "Output image origin: "
		<< origin[0] << ", "
		<< origin[1] << ", "
		<< origin[2] << std::endl;

	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(filter->GetOutput());
	connector->Update(); 

	vtkSmartPointer<vtkImageViewer> viewer = vtkSmartPointer<vtkImageViewer>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> rwin = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindow> rwin = vtkSmartPointer<vtkRenderWindow>::New();
	viewer->SetRenderWindow(rwin);
	//viewer->SetupInteractor(rwin);
	viewer->SetInputData(connector->GetOutput());

	viewer->Render();
	//viewer->SetColorLevel(128);
	//viewer->SetColorWindow(255);
	rwin->Start();

	for (int i = -100; i < 300; i += 10) {
		Thres = i;
		interpolator->SetThreshold(Thres);


		//interpolator->Print(std::cout);

		filter->SetInterpolator(interpolator);

		
		connector->Update();
		viewer->Render();
		//rwin->Start();
		std::cout << i << std::endl;
	}
	
}