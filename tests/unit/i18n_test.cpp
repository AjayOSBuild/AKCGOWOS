#include "../test_framework.h"
#include "../../core/i18n/locale_manager.h"

using namespace NexusOS::I18n;

TEST(I18N_TranslationAndFallback) {
    LocaleManager i18n;
    
    // Default en_US
    ASSERT_EQ("Welcome", i18n.Translate("welcome_msg"));
    
    // Switch to Spanish
    ASSERT_TRUE(i18n.SetLocale("es_ES"));
    ASSERT_EQ("Bienvenido", i18n.Translate("welcome_msg"));
    
    // Fallback test: Missing key in es_ES falls back to en_US, then raw key
    ASSERT_EQ("missing_key", i18n.Translate("missing_key"));
}

TEST(I18N_TextDirectionRTL) {
    LocaleManager i18n;
    
    i18n.SetLocale("en_US");
    ASSERT_EQ(false, i18n.IsRTL());
    
    i18n.SetLocale("ar_SA"); // Arabic (Saudi Arabia)
    ASSERT_TRUE(i18n.IsRTL());
    
    i18n.SetLocale("he_IL"); // Hebrew (Israel)
    ASSERT_TRUE(i18n.IsRTL());
}

int main() {
    return TestRegistry::RunAll();
}
