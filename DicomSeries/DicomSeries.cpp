#include "DicomSeries.h"


void PrintMetaData(const char* filepath, int debuglevel) {
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

	std::cout << "Press any key to continue.\n";
	std::cin.get();
}