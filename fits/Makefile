TARGET = fits

CXX = g++
CXXFLAGS  = -g
LIBS = -lopencv_core \
	-lopencv_highgui \
	-lopencv_imgcodecs \
	-lopencv_imgproc 

SRC = fits.cc

$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)


