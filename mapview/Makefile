####################
SHELL =         /bin/sh
CXX =		c++
MAKE =		make
####################
ARCH = $(shell uname -m)
ifeq ($(ARCH),arm64)
     PREFIX = /opt/homebrew
else
     PREFIX = /usr/local
endif     
########## OpenMP
#DEBUG =		-g -Wall -Wno-deprecated -DMULTICPU -fopenmp
#DEBUG =		-O4 -Wno-deprecated -DMULTICPU -fopenmp
########## Normal
#      			-g
#      			-Ofast
#			-D__ARM_FEATURE_FP16_VECTOR_ARITHMETIC=0
#			-DEIGEN_HAS_ARM64_FP16=0
#			-Wdeprecated-copy
DEBUG =			\
			-Ofast \
			-Wno-nan-infinity-disabled \
			-Wno-deprecated -stdlib=libc++ -std=c++17 \
			-Wno-deprecated-copy-with-user-provided-copy \
			-Wno-\#pragma-messages \
			-Wno-unknown-warning-option \
      			-Wno-c++11-extensions  
#DEBUG =		-O2 -Wall -Wno-deprecated -DNDEBUG
#DEBUG =		-g -Wall -Wno-deprecated
##########
#CFLAGS =		$(DEBUG) -m64
CFLAGS =		$(DEBUG)
##########
MACDEFS =	-D__MAC__
##########
#		`pkg-config --cflags flann`
#		`pkg-config --libs flann`
#
MACINC =	-I$(PREFIX)/include \
       		-I$(PREFIX)/freetype2 \
		-I/usr/X11R6/include \
		`pkg-config --cflags ftgl` \
		`pkg-config --cflags opencv4` \
                `pkg-config --cflags eigen3`
##########
# 		-lboost_thread-mt
MACLIB =	-L$(PREFIX)/lib \
		`pkg-config --libs ftgl` \
		`pkg-config --libs opencv4` \
		`pkg-config --libs eigen3` \
		-lglui \
		-framework OpenGL -framework GLUT \
		-lgl2ps \
		-lgmp \
		-lgsl -lgslcblas \
		-lshp \
		-lhdf5 \
		-lm -lpthread
##########
SRC =           \
		Level.cpp \
    		Height.cpp \
		Filter.cpp \
		Mesh.cpp \
		Graph.cpp \
		JCN.cpp \
		Fiber.cpp \
		CSVIO.cpp \
		Coord2.cpp \
		Polyline.cpp \
		Map.cpp \
		LandFlag.cpp \
		LandPlot.cpp \
		LandTile.cpp \
		LandMap.cpp \
		Bridson.cpp \
		main.cpp
HDR =		Vars.h \
		Common.h \
		Delaunay.h \
		LandType.h \
		\
		Level.h \
    		Height.h \
		Filter.h \
		Mesh.h \
		Graph.h \
		JCN.h \
		Fiber.h \
		CSVIO.h \
		Coord2.h \
		Polyline.h \
		Map.h \
		LandFlag.h \
		LandPlot.h \
		LandTile.h \
		LandMap.h \
		Bridson.h \
		Config.h \
		
OBJ =           $(SRC:.cpp=.o)
BIN =           mapview
#
.cpp.o:	$(SRC) $(HDR)
	$(CXX) $(DEFS) $(CFLAGS) $(INC) -c $<

default:
	make mac

mac	:
	make $(OBJ) "DEFS = $(MACDEFS)" "INC = $(MACINC)"
	$(CXX) -o $(BIN) $(CFLAGS) $(MACDEFS) $(OBJ) $(MACLIB)

depend :
	-if [ -f Make-depend ] ; \
       	   then rm -f      Make-depend ; \
	fi
	$(CXX) -MM $(CFLAGS) $(MACDEFS) $(MACINC) $(SRC) > Make-depend

clean:
	rm -f *.o *~* core $(BIN)

#
include Make-depend


