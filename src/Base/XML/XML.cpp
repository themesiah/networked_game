#include <string>

#include "XML.h"
#include "Utils\Types.h"
//#include "Logger/Logger.h"

namespace tinyxml2
{
#define QUERY_AND_RETURN_BASE_TYPE( type, QueryFnc ) \
type lRet;                                       \
XMLError eResult = QueryFnc( name, &lRet );      \
return (eResult == XML_SUCCESS) ? lRet : defval;

    template<> int XMLElement::GetAttribute<int>(const char* name, const int& defval) const
    {
        QUERY_AND_RETURN_BASE_TYPE(int, QueryIntAttribute);
    }

    template<> float XMLElement::GetAttribute<float>(const char* name, const float& defval) const
    {
        QUERY_AND_RETURN_BASE_TYPE(float, QueryFloatAttribute);
    }

    template<> bool XMLElement::GetAttribute<bool>(const char* name, const bool& defval) const
    {
        QUERY_AND_RETURN_BASE_TYPE(bool, QueryBoolAttribute);
    }
#undef QUERY_AND_RETURN_BASE_TYPE

	template<> uint32 XMLElement::GetAttribute<uint32>(const char* name, const uint32& defval) const
	{
		uint32 l_uint32;
		const char* pszRet = nullptr;

		pszRet = Attribute(name);
		if (pszRet == nullptr)
			l_uint32 = defval;
		else
			sscanf_s(pszRet, "%ud", &l_uint32);

		return l_uint32;
	}

	template<> uint16 XMLElement::GetAttribute<uint16>(const char* name, const uint16& defval) const
	{
		uint32 l_uint16;
		const char* pszRet = nullptr;

		pszRet = Attribute(name);
		if (pszRet == nullptr)
			l_uint16 = defval;
		else
			sscanf_s(pszRet, "%uhd", &l_uint16);

		return (uint16)l_uint16;
	}

	template<> int16 XMLElement::GetAttribute<int16>(const char* name, const int16& defval) const
	{
		int32 l_int16;
		const char* pszRet = nullptr;

		pszRet = Attribute(name);
		if (pszRet == nullptr)
			l_int16 = defval;
		else
			sscanf_s(pszRet, "%hd", &l_int16);

		return (int16)l_int16;
	}

	template<> uint8 XMLElement::GetAttribute<uint8>(const char* name, const uint8& defval) const
	{
		uint32 l_uint8;
		const char* pszRet = nullptr;

		pszRet = Attribute(name);
		if (pszRet == nullptr)
			l_uint8 = defval;
		else
			sscanf_s(pszRet, "%ud", &l_uint8);

		return (uint8)l_uint8;
	}


	template<> wchar_t XMLElement::GetAttribute<wchar_t>(const char* name, const wchar_t& defval) const
	{

		wchar_t l_wchat_t;
		const char* pszRet = nullptr;

		pszRet = Attribute(name);
		if (pszRet == nullptr)
			l_wchat_t = defval;
		else
			sscanf_s(pszRet, "%hu", &l_wchat_t);

		return l_wchat_t;

	}


    template<> std::string XMLElement::GetAttribute<std::string>(const char* name, const std::string& defval) const
    {
        const char* pszRet = Attribute(name);
        return (pszRet) ? pszRet : defval;
    }

	template<> void XMLElement::SetAttribute<std::string>(const char* name, const std::string& value)
	{
		XMLAttribute* a = FindOrCreateAttribute(name);
		a->SetAttribute(value.c_str());
	}
}

namespace base
{
    namespace xml
    {
        bool SucceedLoad(tinyxml2::XMLError aError)
        {
#define CASE_ERROR_XML( xml_error ) case tinyxml2::##xml_error: lMsg = #xml_error; break;
            bool lOk = true;
            if (aError != tinyxml2::XML_SUCCESS)
            {
                lOk = false;
                std::string lMsg = "";
                switch (aError)
                {
                    CASE_ERROR_XML(XML_NO_ATTRIBUTE)
                    CASE_ERROR_XML(XML_WRONG_ATTRIBUTE_TYPE)
                    CASE_ERROR_XML(XML_ERROR_FILE_NOT_FOUND)
                    CASE_ERROR_XML(XML_ERROR_FILE_COULD_NOT_BE_OPENED)
                    CASE_ERROR_XML(XML_ERROR_FILE_READ_ERROR)
                    CASE_ERROR_XML(XML_ERROR_ELEMENT_MISMATCH)
                    CASE_ERROR_XML(XML_ERROR_PARSING_ELEMENT)
                    CASE_ERROR_XML(XML_ERROR_PARSING_ATTRIBUTE)
                    CASE_ERROR_XML(XML_ERROR_IDENTIFYING_TAG)
                    CASE_ERROR_XML(XML_ERROR_PARSING_TEXT)
                    CASE_ERROR_XML(XML_ERROR_PARSING_CDATA)
                    CASE_ERROR_XML(XML_ERROR_PARSING_COMMENT)
                    CASE_ERROR_XML(XML_ERROR_PARSING_DECLARATION)
                    CASE_ERROR_XML(XML_ERROR_PARSING_UNKNOWN)
                    CASE_ERROR_XML(XML_ERROR_EMPTY_DOCUMENT)
                    CASE_ERROR_XML(XML_ERROR_MISMATCHED_ELEMENT)
                    CASE_ERROR_XML(XML_ERROR_PARSING)
                    CASE_ERROR_XML(XML_CAN_NOT_CONVERT_TEXT)
                    CASE_ERROR_XML(XML_NO_TEXT_NODE)
                }
                //LOG_ERROR_APPLICATION("Error '%s' when loading xml", lMsg.c_str());
            }
#undef CASE_ERROR_XML
            return lOk;
        }
    }
}
