#include <string>

class Utils
{
public:
	static std::string iconv_recode(const std::string& from, const std::string& to, std::string text);
	static std::string recodeUTF8toCP1251(const std::string& text);
};