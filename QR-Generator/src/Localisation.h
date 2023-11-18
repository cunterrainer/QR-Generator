#pragma once
#include <unordered_map>

class Local
{
public:
    enum class Language
    {
        German, English
    };

    enum class Item
    {
        LanguageSelectionMenu,

        QRInputField,
        QRErrorCorrection,
        QRErrorCorrectionCombo,
        QRIncreaseErrorCorrection,
        QRIncreaseErrorCorrectionTooltip,
        QRBorder,
        QRResolution,
        QRPrimaryColor,
        QRSecondaryColor,
        QRMinVersion,
        QRMaxVersion,
        QRMaskPattern,
        QRMaskPatternTooltip,
        QRSaveButton,
        QRButtonTooltip
    };
private:
    static inline Language m_SelectedLanguage;
    static inline std::unordered_map<Item, const char*> m_LangMap;
public:
    static inline void ChangeLanguage(Language lang) noexcept
    {
        switch (lang)
        {
        case Language::German:
            m_LangMap[Item::LanguageSelectionMenu]            = "Sprache";
            m_LangMap[Item::QRInputField]                     = u8"Text einfügen";
            m_LangMap[Item::QRErrorCorrection]                = "Fehlerkorrektur";
            m_LangMap[Item::QRErrorCorrectionCombo]           = "Niedrig\0Mittel\0Quartil\0Hoch\0";
            m_LangMap[Item::QRIncreaseErrorCorrection]        = u8"Erhöhe das Fehlerkorrekturlevel automatisch";
            m_LangMap[Item::QRIncreaseErrorCorrectionTooltip] = u8"Es wird automatisch die kleinstmögliche QR-Code-Version innerhalb des angegebenen Bereichs verwendet.\nWenn aktiviert wird das Fehlerkorrekturlevel erhöht sofern dies ohne Erhöhung der Version möglich ist";
            m_LangMap[Item::QRBorder]                         = "Rand";
            m_LangMap[Item::QRResolution]                     = u8"Auflösung";
            m_LangMap[Item::QRPrimaryColor]                   = u8"Primärfarbe";
            m_LangMap[Item::QRSecondaryColor]                 = u8"Sekundärfarbe";
            m_LangMap[Item::QRMinVersion]                     = "Mindest Version";
            m_LangMap[Item::QRMaxVersion]                     = "Maximal Version";
            m_LangMap[Item::QRMaskPattern]                    = "Maske";
            m_LangMap[Item::QRMaskPatternTooltip]             = "(-1 automatisch, 0 bis 7 manuell)";
            m_LangMap[Item::QRSaveButton]                     = "Speichern";
            m_LangMap[Item::QRButtonTooltip]                  = "Klicken um den QR-Code zu kopieren";
            break;
        case Language::English:
            m_LangMap[Item::LanguageSelectionMenu]            = "Language";
            m_LangMap[Item::QRInputField]                     = "Insert text";
            m_LangMap[Item::QRErrorCorrection]                = "Error correction";
            m_LangMap[Item::QRErrorCorrectionCombo]           = "Low\0Medium\0Quartile\0High\0";
            m_LangMap[Item::QRIncreaseErrorCorrection]        = "Automatically increase the error correction level";
            m_LangMap[Item::QRIncreaseErrorCorrectionTooltip] = "The smallest possible QR code version within the specified range is automatically selected for output.\nWhen this option is selected, the error correction level may be higher than specified if this is possible without increasing the version";
            m_LangMap[Item::QRBorder]                         = "Border";
            m_LangMap[Item::QRResolution]                     = "Resolution";
            m_LangMap[Item::QRPrimaryColor]                   = "Primary color";
            m_LangMap[Item::QRSecondaryColor]                 = "Secondary color";
            m_LangMap[Item::QRMinVersion]                     = "Minimum version";
            m_LangMap[Item::QRMaxVersion]                     = "Maximum version";
            m_LangMap[Item::QRMaskPattern]                    = "Mask pattern";
            m_LangMap[Item::QRMaskPatternTooltip]             = "(-1 automatic, 0 to 7 manual)";
            m_LangMap[Item::QRSaveButton]                     = "Save";
            m_LangMap[Item::QRButtonTooltip]                  = "Click to copy the QR code";
            break;
        }
        m_SelectedLanguage = lang;
    }

    static inline const char* Get(Item item) noexcept
    {
        return m_LangMap[item];
    }

    static inline Language GetLanguage() noexcept
    {
        return m_SelectedLanguage;
    }
};