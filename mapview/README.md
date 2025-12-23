This program package is prepared on the MacOS platform.  The
codes may be compiled with any Unix OS platform such as
Linux OS through minor revision fo the CMakeLists.txt.

The program package uses OpenGL+GLUT, glui, OpenCV, CGAL,
Eigen3, FTGL, ShapeLib, GL2PS libraries. You can install
these packages with HomeBrew for MacOS or apt/yum for Linux.

Overall, you can compile the program as follows:

** For macOS (Apple silicon) **

> cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/homebrew

> cmake --build build

** For Linux **

> cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

> cmake --build build -j


Download data files from:

JAEA_niida_integrated_map_040.zip from https://www.dropbox.com/scl/fi/1b7w4uspq2owwp0gzqw20/JAEA_niida_integrated_map_040.zip?rlkey=7o7ianm30vpohr2epm9s58d7p&dl=0

JAEA_date_integrated_map_050.zip from https://www.dropbox.com/scl/fi/5meiuxtimuvpnirz75sww/JAEA_date_integrated_map_050.zip?rlkey=vq4h7u7juxkpexivj0m9gnm75&dl=0

JAEA_nogami_integrated_map_040.zip from https://www.dropbox.com/scl/fi/726br5bu4wvlkprg17y7l/JAEA_nogami_integrated_map_040.zip?rlkey=892w2s3oz8i84w45czw3ucoag&dl=0

Unpack the data files

> unzip JAEA_niida_integrated_map_040.zip

> unzip JAEA_date_integrated_map_050.zip

> unzip JAEA_nogami_integrated_map_040.zip

For example, you can visualize the case study of the area
around the Niida River (Section 3.1) by

> ./build/mapview -c configs/niida-attenuation.conf

| 2011-2013 | 2013-2015 |
|-----------|-----------|
| ![](../images/niida_2011-2013.png) | ![](../images/niida_2013-2015.png) |

The case study of the area in Fukushima City and Date City
in Fukushima Prefecture can be obtained by

> ./build/mapview -c configs/date-attenuation.conf

![Fukushima City and Date City](../images/date_map.png)


You can also analyze the case study of the national forest south of the National Route 288 in Nogame area of Okuma Town by  

> ./build/mapview -c configs/nogami-attenuation.conf

![National Forest in Nogami Area](../images/nogami_map.png)

Note that once the program load the data from the ascii
file, it transforms the data into binary format and wrote
the binary file. Next time, the program directly load the
binary data file and thus you can accelerate the
initialization.



