#include "h_curl.h"
#include <iostream>

H_Curl::H_Curl( ) : curl( curl_easy_init( ) ), headers( nullptr ) {
    if (curl) {
        headers = curl_slist_append( headers, "Content-Type: application/json" );
    }
}

H_Curl::~H_Curl( ) {
    if (curl) {
        curl_easy_cleanup( curl );
    }
    if (headers) {
        curl_slist_free_all( headers );
    }
}

size_t H_Curl::write_callback( void* contents, size_t size, size_t nmemb, void* userp ) {
    ((std::string*)userp)->append( (char*)contents, size * nmemb );
    return size * nmemb;
}

std::string H_Curl::m_request( const std::string& url ) {
    if (!curl) return {};

    std::string response;

    curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
    curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &response );

    CURLcode res = curl_easy_perform( curl );
    if (res != CURLE_OK) {
        return "";
    }

    return response;
}
