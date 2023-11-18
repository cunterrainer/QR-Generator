#pragma once
#include <unordered_map>

class Local
{
public:
    enum class Language
    {
        German, English, Hebrew
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
        QRButtonTooltip,

        ErrImageOutOfMemory,
        ErrFailedToGenerateQRCode,
        ErrImageWriterFailedToOpenNFD,
        ErrImageWriterFailedToSaveImage,
        ErrFailedToCopyImageToClipboard,
        ErrApplicationBadAllocException,
        ErrApplicationUnhandledException,
        ErrApplicationUnhandledStdException,
    };
private:
    static inline bool m_FromLeftToRight; // e.g. true for english, german; false for hebrew, arabic
    static inline Language m_SelectedLanguage;
    static inline std::unordered_map<Item, const char*> m_LangMap;
public:
    static inline void SetLanguage(Language lang) noexcept
    {
        switch (lang)
        {
        case Language::German:
            m_FromLeftToRight = true;
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

            m_LangMap[Item::ErrImageOutOfMemory]                 = "Es konnte kein Speicher für den QR-Code zugewiesen werden, angefordert: %u Bytes (%4.2f GB)\nVersuche es mit einer geringeren Skalierung, Rand breite, verringere die mindest Version oder verkürze den Inhalt";
            m_LangMap[Item::ErrFailedToGenerateQRCode]           = "%s\nQR-Code konnte nicht generiert werden da er zu groß ist.\nZum Beheben:\n- Verringere das Fehlerkorrekturlevel\n- Erhöhe das Maximale Level\n- Kürze die Texteingabe";
            m_LangMap[Item::ErrImageWriterFailedToOpenNFD]       = "Dateidialog konnte nicht geöffnet werden:\n%s";
            m_LangMap[Item::ErrImageWriterFailedToSaveImage]     = "Bild konnte nicht gespeichert werden.\nPfad: %s\nBreite: %d\nHöhe: %d";
            m_LangMap[Item::ErrFailedToCopyImageToClipboard]     = "Das Bild konnte nicht in die Zwischenablage kopiert werden (%s)";
            m_LangMap[Item::ErrApplicationBadAllocException]     = "Speicher konnte nicht zugewiesen werden. Möchten Sie die Anwendung neu starten?";
            m_LangMap[Item::ErrApplicationUnhandledException]    = "Es ist eine nicht behandelte Ausnahme aufgetreten. Möchten Sie die Anwendung neu starten?";
            m_LangMap[Item::ErrApplicationUnhandledStdException] = "Es ist eine nicht behandelte Ausnahme aufgetreten. Möchten Sie die Anwendung neu starten?\nWas: ";
            break;
        case Language::English:
            m_FromLeftToRight = true;
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

            m_LangMap[Item::ErrImageOutOfMemory]                 = "Failed to allocate memory for qr code image, requested: %u bytes (%4.2f GB)\nTry a smaller scale, border width, reduce the minimum version or shorten the content";
            m_LangMap[Item::ErrFailedToGenerateQRCode]           = "%s\nQR code could not be generated because it is too large.\nTo fix:\n- Reduce the error correction level\n- Increase the maximum level\n- Shorten the text input";
            m_LangMap[Item::ErrImageWriterFailedToOpenNFD]       = "Failed to open file dialog:\n%s";
            m_LangMap[Item::ErrImageWriterFailedToSaveImage]     = "Failed to save image.\nPath: %s\nWidth: %d\nHeight: %d";
            m_LangMap[Item::ErrFailedToCopyImageToClipboard]     = "Failed to copy image to clipboard (%s)";
            m_LangMap[Item::ErrApplicationBadAllocException]     = "Failed to allocate memory, do you want to restart the application?";
            m_LangMap[Item::ErrApplicationUnhandledException]    = "Unhandled exception occurred, do you want to restart the application?";
            m_LangMap[Item::ErrApplicationUnhandledStdException] = "Unhandled exception occurred, do you want to restart the application?\nWhat: ";
            break;
        case Language::Hebrew:
            m_FromLeftToRight = false;
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

    static inline bool IsFromLeftToRight() noexcept
    {
        return m_FromLeftToRight;
    }
};