#pragma once

#ifndef INCLUDES_H
#define INCLUDES_H

#define CURL_STATICLIB

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#pragma once

#include <sstream>

#include <curl/curl.h>

#ifdef _DEBUG
#pragma comment(lib, "D:/curl-8.11.0/builds/x64 Debug/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib, "D:/curl-8.11.0/builds/x64 Release/lib/libcurl_a.lib")
#endif

#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#include <nlohmann/json.hpp>
using json = nlohmann::json;


class Utils {
public:
    static size_t WriteCallback( void* contents, size_t size, size_t nmemb, void* userp ) {
        ((std::string*)userp)->append( (char*)contents, size * nmemb );
        return size * nmemb;
    }

    static bool checkSkyblockBazaarStatus( ) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl_global_init( CURL_GLOBAL_DEFAULT );
        curl = curl_easy_init( );
        if (curl) {
            curl_easy_setopt( curl, CURLOPT_URL, "https://api.hypixel.net/skyblock/bazaar" );
            curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
            curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );
            res = curl_easy_perform( curl );

            if (res != CURLE_OK) {
                std::cerr << "CURL request failed: " << curl_easy_strerror( res ) << std::endl;
                curl_easy_cleanup( curl );
                return false;
            }

            try {
                nlohmann::json jsonResponse = nlohmann::json::parse( readBuffer );

                curl_easy_cleanup( curl );

                if (jsonResponse.contains( "products" )) {
                    return true;
                }
                else {
                    return false;
                }
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cerr << "Error parsing JSON: " << e.what( ) << std::endl;
                curl_easy_cleanup( curl );
                return false;
            }
        }

        curl_global_cleanup( );
        return false;
    }

    static void InitializeConsole( ) {
        SetConsoleTitleA( "[Skyblock Bazaar] Private Profit Calculator" );

        HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );

        CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
        GetConsoleScreenBufferInfo( hOut, &scrBufferInfo );

        short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
        short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

        short scrBufferWidth = scrBufferInfo.dwSize.X;
        short scrBufferHeight = scrBufferInfo.dwSize.Y;

        COORD newSize = { scrBufferWidth, winHeight };
        if (SetConsoleScreenBufferSize( hOut, newSize ) == 0) {
            exit( EXIT_FAILURE );
        }

        HWND console = GetConsoleWindow( );
        DWORD dwStyle = GetWindowLong( console, GWL_STYLE );

        dwStyle &= ~WS_MAXIMIZEBOX;
        dwStyle &= ~WS_SIZEBOX;

        SetWindowLong( console, GWL_STYLE, dwStyle );
        SetWindowPos( console, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED );

        SMALL_RECT windowSize = { 0, 0, winWidth - 1, winHeight - 1 };
        SetConsoleWindowInfo( hOut, TRUE, &windowSize );
    }

    static void clearConsole( ) {
        HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( hStdOut, &csbi );
        DWORD count;
        FillConsoleOutputCharacter( hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, COORD{ 0, 0 }, &count );
        SetConsoleCursorPosition( hStdOut, COORD{ 0, 0 } );
    }

    static void setConsoleTextColor( int color ) {
        HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        SetConsoleTextAttribute( hConsole, color );
    }
};

#endif