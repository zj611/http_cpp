#ifndef __Downloader__HttpClient__
#define __Downloader__HttpClient__

#include "Common.h"
#include <curl/curl.h>
#include <curl/easy.h>

class HttpClient
{
public:
    HttpClient();
    HttpClient(string agent);
    virtual ~HttpClient();
    
    string get(string url);
    bool post(string url, string data);
    /**
     Download url into folder
     */
    bool download(string url, string folder);
    /**
     Download url and rename it to satisfy filepath
     */
    bool downloadAs(string url, string filepath);
    
private:
    static size_t writeToFile(void * ptr, size_t size, size_t nmemb, FILE * stream);
    static size_t writeToString(char * ptr, size_t size, size_t nmemb, string * sp);
    
    void initCURL();
    
private:
    CURL * m_curl;
};

#endif /* defined(__Downloader__HttpClient__) */
