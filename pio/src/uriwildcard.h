#ifndef URI_H
#define URI_H

#include <Arduino.h>
#include <vector>

class Uri {

    protected:
        const String _uri;

    public:
        Uri(const char *uri) : _uri(uri) {}
        Uri(const String &uri) : _uri(uri) {}
        Uri(const __FlashStringHelper *uri) : _uri(String(uri)) {} 
        virtual ~Uri() {}

        virtual Uri* clone() const {
            return new Uri(_uri);
        };

        virtual bool canHandle(const String &requestUri, __attribute__((unused)) std::vector<String> &pathArgs) {
            //Look for Wildcard uri match.
            if (_uri.endsWith("*"))
                {
                    /*
                    Serial.print(F("URL Wildcard Match: "));
                    Serial.print(requestUri);
                    Serial.print(F(" : "));
                    Serial.println(_uri);

                    Serial.print(F("_requestUri Length: "));
                    Serial.print(requestUri.length());
                    Serial.print(F("_Uri.Length: "));
                    Serial.println(_uri.length());

                    Serial.print(F("_uri substring: "));
                    Serial.println(_uri.substring(0,_uri.length()-1));
                    Serial.print(F("The whole If/Then: "));
                    Serial.println(requestUri.startsWith(_uri.substring(0,_uri.length()-1)));
                    Serial.println();
                    */
                    if (requestUri.startsWith( _uri.substring(0,_uri.length()-1)))
                        return true;
                }
            // Else look for direct match.                    
            return _uri == requestUri;
        }
};
#endif