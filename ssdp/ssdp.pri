HEADERS += ssdp/brisassdpclient.h
SOURCES += ssdp/brisassdpclient.cpp
           
win32 {
    LIBS += -lwsock32 -lws2_32
}
