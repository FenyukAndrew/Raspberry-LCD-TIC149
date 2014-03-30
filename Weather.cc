#include <iostream>
#include <string>
#include <curl/curl.h>
#include "tinyxml.h"

#include "Weather.h"

Weather::Weather(const std::string &url)
{
    xml_text = get_HTML(url);
}

Weather::~Weather()
{
}

int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
/* Get HTML page from URL*/
std::string Weather::get_HTML(const std::string &url)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()  );
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (result == CURLE_OK)
            return buffer;
    }
    return "";
}

int writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    int result = 0;
    if (buffer != NULL)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

/* Get weather from xml and parameter
*     - temperature -- get temperature in Celcius
*     - weather_type -- get weather description
*/
const std::string Weather::get_weather(const std::string &parameter)
{
    TiXmlDocument doc;
    doc.Parse(xml_text.c_str(), 0, TIXML_ENCODING_UTF8);
    TiXmlElement* pForecast = doc.FirstChildElement( "forecast" );
    if ( pForecast )
    {
        // Текущая (фактическая) погода
        TiXmlElement* pFact = pForecast->FirstChildElement("fact");
        if (pFact)
        {
            TiXmlElement* pTemperature = pFact->FirstChildElement(parameter);
            return std::string(pTemperature->GetText());
        }
    }
    return std::string();
}
