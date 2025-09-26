# Multidimensional VRT Builder

## Overview
This utility is designed to facilitate the creation of Multidimensional Virtual Raster Tiles (VRT) using C++. It leverages the GDAL (Geospatial Data Abstraction Library) structure for efficient handling of geospatial data.

## Usage Instructions
1. **Build the Utility**:  To build the utility, you will need to have GDAL installed. Ensure you have the necessary development libraries and headers for GDAL. You can build the project using the following commands:
   
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Run the Utility**: After building the project, you can run the utility with the following command:
   
   ```bash
   ./vrt_builder <input_files> <output_vrt>
   ```
   Replace `<input_files>` with the paths to your input raster files and `<output_vrt>` with the desired output VRT file name.

## Build Info
- **Dependencies**: GDAL libraries
- **C++ Standard**: Ensure your compiler supports at least C++11.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.