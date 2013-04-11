include $(top_srcdir)/profile

CXXFLAGS=$(FLAGS)

INCLUDES=-I$(HELPER_DIR) -I$(HTTP_DIR)

includedir=$(prefix)/include

include_HEADERS=\
    fast_cgi_wrapper.h\
    std_cgi_wrapper.h

ctkcgidir=$(prefix)/lib

ctkcgi_LIBRARIES=libctkcgiwrapper.a

libctkcgiwrapper_a_SOURCES=\
    std_cgi_wrapper.cpp\
    fast_cgi_wrapper.cpp
