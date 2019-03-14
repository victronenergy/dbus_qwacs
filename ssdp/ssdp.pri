HEADERS += ssdp/brisassdpclient.h ssdp/httprequestheader.h
SOURCES += ssdp/brisassdpclient.cpp ssdp/httprequestheader.cpp
           
win32 {
    LIBS += -lwsock32 -lws2_32
}
