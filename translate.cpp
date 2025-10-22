\
    #include "translate.hpp"
    #include <curl/curl.h>
    #include <iostream>
    #include <sstream>
    #include <iomanip>
    #include <string>

    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    // URL-encode simple helper
    static std::string url_encode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (char c : value) {
            // Keep alphanumeric and a few safe characters unescaped
            if (isalnum((unsigned char)c) || c=='-' || c=='_' || c=='.' || c=='~') {
                escaped << c;
            } else {
                escaped << '%' << std::setw(2) << int((unsigned char)c);
            }
        }
        return escaped.str();
    }

    std::string translate_text(const std::string& text, const std::string& target) {
        std::string api_key = "YOUR_API_KEY_HERE"; // <-- replace with your Google Cloud API key
        std::string encoded = url_encode(text);
        std::string url = "https://translation.googleapis.com/language/translate/v2?key=" + api_key + "&q=" + encoded + "&target=" + target;

        CURL* curl = curl_easy_init();
        if (!curl) return "{}";

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Perform
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        return response;
    }
