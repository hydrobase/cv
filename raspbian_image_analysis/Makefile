
CXX = g++

CVPREFIX = /usr/local/include
LIBPREFIX = /usr/local

# If you don't have libtool installed, you can specify the full 
# path to the libtool script in the base directory of your Vision 
# Workbench source tree, e.g. $(HOME)/VisionWorkbench-1.0/libtool
LIBTOOL = libtool

all : ImageAnalyzer 
# Compilation flags:
# -O3   turns on optimization, which you should almost always do
# -g    enables debugging support
# -Wall turns on all compiler warnings
CXXFLAGS = -I$(CVPREFIX) -O3 -Wall


# Linking flags:
LDFLAGS = -L$(LIBPREFIX)/lib -lopencv_calib3d -lopencv_ml -lopencv_features2d -lopencv_highgui -lopencv_video -lopencv_objdetect -lopencv_contrib -lopencv_flann -lopencv_legacy -lopencv_imgproc -lopencv_core -ljpeg -ltiff -lpng -ljasper -lpthread -lm -llapack -lrt -lgtk-x11-2.0

# Object files:
# List the object files needed to build your program here.
OBJECTS = ImageProcessor.o main.o

# Linking rule:
# Duplicate and modify this rule to build multiple programs.
ImageAnalyzer: $(OBJECTS)
	$(CXX) -std=c++11 -c $(CXXFLAGS) main.cpp
	$(LIBTOOL) --mode=link $(CXX) $(LDFLAGS) -o $@ $^ 
	

# Clean-up rule:

clean:
	rm -f *.o *~ \#*
