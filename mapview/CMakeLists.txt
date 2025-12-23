cmake_minimum_required(VERSION 3.20)
project(Mapview LANGUAGES CXX)

# ---- Options (helpful for Linux portability) ----
option(USE_FTGL  "Enable FTGL (text rendering)" ON)
option(USE_GLUI  "Enable GLUI (UI)"            ON)
option(USE_GL2PS "Enable GL2PS (vector export)" ON)

# ---- executable ----
add_executable(mapview)

# C++ standard
target_compile_features(mapview PRIVATE cxx_std_17)

# Custom headers
target_include_directories(mapview PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- Source files ----
target_sources(mapview PRIVATE
  src/main.cpp
  src/Level.cpp
  src/Height.cpp
  src/Filter.cpp
  src/Mesh.cpp
  src/Graph.cpp
  src/JCN.cpp
  src/Fiber.cpp
  src/CSVIO.cpp
  src/Coord2.cpp
  src/Polyline.cpp
  src/Map.cpp
  src/LandFlag.cpp
  src/LandPlot.cpp
  src/LandTile.cpp
  src/LandMap.cpp
  src/Bridson.cpp
)

# ---- Common warnings/options ----
# Put -Ofast only on Release to keep Debug usable.
target_compile_options(mapview PRIVATE
  $<$<CONFIG:Release>:-Ofast>
  -Wno-deprecated
  -Wno-unknown-warning-option
)

# macOS-only definitions / options
if(APPLE)
  target_compile_definitions(mapview PRIVATE __MAC__)
  target_compile_options(mapview PRIVATE
    -Wno-nan-infinity-disabled
    -Wno-deprecated-copy-with-user-provided-copy
    -Wno-#pragma-messages
    -Wno-c++11-extensions
    -stdlib=libc++
  )
  target_link_options(mapview PRIVATE -stdlib=libc++)
endif()

# ---- Eigen ----
find_package(Eigen3 REQUIRED)
target_link_libraries(mapview PRIVATE Eigen3::Eigen)

# ---- OpenCV ----
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
target_link_libraries(mapview PRIVATE ${OpenCV_LIBS})
target_include_directories(mapview PRIVATE ${OpenCV_INCLUDE_DIRS})

# ---- Boost ----
find_package(Boost REQUIRED COMPONENTS filesystem system program_options)
target_link_libraries(mapview PRIVATE Boost::filesystem Boost::system Boost::program_options)

# ---- CGAL ----
find_package(CGAL REQUIRED COMPONENTS Core)
target_link_libraries(mapview PRIVATE CGAL::CGAL CGAL::CGAL_Core)

# ---- OpenGL + GLUT ----
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(mapview PRIVATE OpenGL::GL GLUT::GLUT)

# ---- Helper: find headers/libs without hardcoding /opt/homebrew ----
include(GNUInstallDirs)

function(find_and_link_simple target name header libnames)
  # header: e.g. shapefil.h
  # libnames: list, e.g. shp;libshp
  find_path(${name}_INCLUDE_DIR NAMES ${header})
  find_library(${name}_LIBRARY NAMES ${libnames})

  if(NOT ${name}_INCLUDE_DIR OR NOT ${name}_LIBRARY)
    message(FATAL_ERROR "${name} not found (need ${header} and ${libnames}).")
  endif()

  target_include_directories(${target} PRIVATE ${${name}_INCLUDE_DIR})
  target_link_libraries(${target} PRIVATE ${${name}_LIBRARY})
endfunction()

# ---- Shapelib ----
find_and_link_simple(mapview SHPLIB shapefil.h "shp;libshp")

# ---- FTGL ----
if(USE_FTGL)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(FTGL REQUIRED ftgl)
  target_include_directories(mapview PRIVATE ${FTGL_INCLUDE_DIRS})
  target_link_directories(mapview PRIVATE ${FTGL_LIBRARY_DIRS})
  target_link_libraries(mapview PRIVATE ${FTGL_LIBRARIES})
  target_compile_definitions(mapview PRIVATE HAVE_FTGL=1)
else()
  target_compile_definitions(mapview PRIVATE HAVE_FTGL=0)
endif()

# ---- GLUI ----
if(USE_GLUI)
  find_path(GLUI_INCLUDE_DIR NAMES glui.h PATH_SUFFIXES GL GLUI)
  find_library(GLUI_LIBRARY NAMES glui)
  if(NOT GLUI_INCLUDE_DIR OR NOT GLUI_LIBRARY)
    message(FATAL_ERROR "GLUI not found (need glui.h and libglui). Set -DUSE_GLUI=OFF to build without it.")
  endif()
  target_include_directories(mapview PRIVATE ${GLUI_INCLUDE_DIR})
  target_link_libraries(mapview PRIVATE ${GLUI_LIBRARY})
  target_compile_definitions(mapview PRIVATE HAVE_GLUI=1)
else()
  target_compile_definitions(mapview PRIVATE HAVE_GLUI=0)
endif()

# ---- GL2PS ----
if(USE_GL2PS)
  find_path(GL2PS_INCLUDE_DIR NAMES gl2ps.h)
  find_library(GL2PS_LIBRARY NAMES gl2ps)
  if(NOT GL2PS_INCLUDE_DIR OR NOT GL2PS_LIBRARY)
    message(FATAL_ERROR "GL2PS not found. Set -DUSE_GL2PS=OFF to build without it.")
  endif()
  target_include_directories(mapview PRIVATE ${GL2PS_INCLUDE_DIR})
  target_link_libraries(mapview PRIVATE ${GL2PS_LIBRARY})
  target_compile_definitions(mapview PRIVATE HAVE_GL2PS=1)
else()
  target_compile_definitions(mapview PRIVATE HAVE_GL2PS=0)
endif()

# ---- Runtime data ----
foreach(data_dir IN ITEMS map fonts satellite configs)
  add_custom_command(TARGET mapview POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${data_dir}
            $<TARGET_FILE_DIR:mapview>/${data_dir}
    COMMENT "Copying ${data_dir} -> output dir"
  )
endforeach()
