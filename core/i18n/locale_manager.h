#pragma once
#include <string>
#include <map>

namespace NexusOS {
namespace I18n {

class LocaleManager {
private:
    std::string current_locale = "en_US";
    // Mock dictionary: map<locale, map<key, value>>
    std::map<std::string, std::map<std::string, std::string>> translations;

public:
    LocaleManager() {
        // Initialize mock translations
        translations["en_US"]["welcome_msg"] = "Welcome";
        translations["en_US"]["btn_cancel"] = "Cancel";
        
        translations["es_ES"]["welcome_msg"] = "Bienvenido";
        translations["es_ES"]["btn_cancel"] = "Cancelar";
        
        translations["ar_SA"]["welcome_msg"] = "مرحبا"; // Arabic Welcome
    }

    bool SetLocale(const std::string& locale) {
        if (locale.length() >= 5) {
            current_locale = locale;
            return true;
        }
        return false;
    }

    std::string GetLocale() const { return current_locale; }

    bool IsRTL() const {
        // Detect Right-to-Left languages (Arabic, Hebrew, Farsi/Persian)
        std::string lang = current_locale.substr(0, 2);
        return (lang == "ar" || lang == "he" || lang == "fa");
    }

    std::string Translate(const std::string& key) {
        // Try current locale
        if (translations[current_locale].count(key)) {
            return translations[current_locale][key];
        }
        // Fallback to en_US
        if (translations["en_US"].count(key)) {
            return translations["en_US"][key];
        }
        // Fallback to raw key
        return key;
    }
};

} // namespace I18n
} // namespace NexusOS
