#include "gdal_priv.h"
#include "gdal_mdarray.h"
#include <vector>
#include <string>
#include <iostream>

// Conceptual example: create a multidimensional VRT from multiple NetCDFs stacked along time
int main()
{
    GDALAllRegister();

    // List of input NetCDF files (replace with your own)
    std::vector<std::string> inputFiles = {
        "ts_20200101.nc",
        "ts_20200102.nc",
        "ts_20200103.nc"
    };

    // Open first file to get variable info
    GDALDataset* poFirstDS = (GDALDataset*) GDALOpenEx(inputFiles[0].c_str(), GDAL_OF_MULTIDIM_RASTER, nullptr, nullptr, nullptr);
    if (!poFirstDS) {
        std::cerr << "Failed to open: " << inputFiles[0] << std::endl;
        return 1;
    }
    GDALGroup* poRootGroup = poFirstDS->GetRootGroup();
    GDALMDArray* poVar = poRootGroup->GetMDArray("varname"); // <-- EDIT: use your variable name
    std::vector<GUInt64> varShape = poVar->GetDimensionsSize();
    GDALClose(poFirstDS);

    // Create a new multidimensional VRT dataset
    GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("VRT");
    GDALDataset* poVRTDS = poDriver->CreateMultiDimensional("stack.vrt", nullptr);
    GDALGroup* poVRTGroup = poVRTDS->GetRootGroup();

    // Define dimensions: time, y, x
    std::vector<std::string> dimNames = {"time", "y", "x"};
    std::vector<GUInt64> dimSizes = {inputFiles.size(), varShape[0], varShape[1]};
    std::vector<GDALDimension*> dimensions;
    for (size_t i = 0; i < dimNames.size(); ++i) {
        dimensions.push_back(poVRTGroup->CreateDimension(dimNames[i].c_str(), nullptr, nullptr, dimSizes[i]));
    }

    // Create MDArray for stacked variable
    GDALMDArray* poStackArray = poVRTGroup->CreateMDArray("varname", dimensions, GDT_Float32, nullptr);

    // For each input file, attach the appropriate slice
    for (size_t t = 0; t < inputFiles.size(); ++t) {
        GDALDataset* poSrcDS = (GDALDataset*) GDALOpenEx(inputFiles[t].c_str(), GDAL_OF_MULTIDIM_RASTER, nullptr, nullptr, nullptr);
        if (!poSrcDS) {
            std::cerr << "Failed to open: " << inputFiles[t] << std::endl;
            continue;
        }
        GDALGroup* poSrcGroup = poSrcDS->GetRootGroup();
        GDALMDArray* poSrcVar = poSrcGroup->GetMDArray("varname"); // <-- EDIT: use your variable name

        // This is conceptual: actual API for VRTMDArraySource may differ
        // The intention is to reference poSrcVar as a source for [t,:,:] in poStackArray

        // poStackArray->SetMDArraySource(VRTMDArraySource::CreateFromMDArray(poSrcVar, {t, 0, 0}, {varShape[0], varShape[1]}));

        GDALClose(poSrcDS);
    }

    GDALClose(poVRTDS);

    std::cout << "Created multidimensional stack.vrt." << std::endl;
    return 0;
}
