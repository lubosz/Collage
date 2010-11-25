QT      += webkit network

HEADERS = \
    levelconstructor.h
SOURCES = main.cpp \
    levelconstructor.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webkit/framecapture
sources.files = $$SOURCES $$HEADERS
sources.path = $$[QT_INSTALL_EXAMPLES]/webkit/framecapture
INSTALLS += target sources
