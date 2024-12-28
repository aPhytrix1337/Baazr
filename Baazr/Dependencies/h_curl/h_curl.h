#ifndef H_CURL_H
#define H_CURL_H

#include <Includes.h>

class H_Curl {
public:
    H_Curl( );
    ~H_Curl( );

    std::string m_request( const std::string& url );

private:
    CURL* curl;
    struct curl_slist* headers;

    static size_t write_callback( void* contents, size_t size, size_t nmemb, void* userp );
};

#endif // H_CURL_H
