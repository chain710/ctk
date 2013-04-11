include $(top_srcdir)/profile

CXXFLAGS=$(FLAGS) -D__STDC_FORMAT_MACROS

INCLUDES=-I$(HELPER_DIR)

includedir=$(prefix)/include

include_HEADERS=\
    response.h\
    request.h\
    form.h\
    enviroment.h\
    cookie.h\
    str_aux.h

ctkhttpdir=$(prefix)/lib

ctkhttp_LIBRARIES=libctkhttp.a

libctkhttp_a_SOURCES=\
    cookie.cpp\
    enviroment.cpp\
    form.cpp\
    request.cpp\
    response.cpp\
    str_aux.cpp
