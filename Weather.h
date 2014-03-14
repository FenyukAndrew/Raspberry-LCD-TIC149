#include <string>

class Weather
{
public:
	Weather(const std::string &url);
	~Weather();
	
	std::string get_HTML(const std::string &url);
	const std::string get_weather(const std::string &parameter);
	
private:
	//int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
	std::string xml_text;
};
