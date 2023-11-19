#pragma once
#include <unordered_map>
#include <regex>

class Local
{
public:
    enum class Language
    {
        German, English, Hebrew, Arabic
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
    static inline std::unordered_map<Item, const char*> m_LTRMap;
    static inline std::unordered_map<Item, std::string> m_RTLMap;
private:
    static inline void Utf8Rev(char* str) noexcept
    {
        /* this assumes that str is valid UTF-8 */
        char* scanl, * scanr, * scanr2, c;

        /* first reverse the string */
        for (scanl = str, scanr = str + strlen(str); scanl < scanr;)
            c = *scanl, * scanl++ = *--scanr, *scanr = c;

        /* then scan all bytes and reverse each multibyte character */
        for (scanl = scanr = str; (c = *scanr++);)
        {
            if ((c & 0x80) == 0) // ASCII char
                scanl = scanr;
            else if ((c & 0xc0) == 0xc0) // start of multibyte
            {
                scanr2 = scanr;
                switch (scanr - scanl)
                {
                case 4: c = *scanl, *scanl++ = *--scanr, *scanr = c; // fallthrough
                case 3: // fallthrough
                case 2: c = *scanl, *scanl++ = *--scanr, *scanr = c;
                }
                scanr = scanl = scanr2;
            }
        }
    }
public:
    static inline void SetLanguage(Language lang) noexcept
    {
        switch (lang)
        {
        case Language::German:
            m_FromLeftToRight = true;
            m_LTRMap[Item::LanguageSelectionMenu]            = "Sprache";
            m_LTRMap[Item::QRInputField]                     = u8"Text einfügen";
            m_LTRMap[Item::QRErrorCorrection]                = "Fehlerkorrektur";
            m_LTRMap[Item::QRErrorCorrectionCombo]           = "Niedrig\0Mittel\0Quartil\0Hoch\0";
            m_LTRMap[Item::QRIncreaseErrorCorrection]        = u8"Erhöhe das Fehlerkorrekturlevel automatisch";
            m_LTRMap[Item::QRIncreaseErrorCorrectionTooltip] = u8"Es wird automatisch die kleinstmögliche QR-Code-Version innerhalb des angegebenen Bereichs verwendet.\nWenn aktiviert wird das Fehlerkorrekturlevel erhöht sofern dies ohne Erhöhung der Version möglich ist";
            m_LTRMap[Item::QRBorder]                         = "Rand";
            m_LTRMap[Item::QRResolution]                     = u8"Auflösung";
            m_LTRMap[Item::QRPrimaryColor]                   = u8"Primärfarbe";
            m_LTRMap[Item::QRSecondaryColor]                 = u8"Sekundärfarbe";
            m_LTRMap[Item::QRMinVersion]                     = "Mindest Version";
            m_LTRMap[Item::QRMaxVersion]                     = "Maximal Version";
            m_LTRMap[Item::QRMaskPattern]                    = "Maske";
            m_LTRMap[Item::QRMaskPatternTooltip]             = "(-1 automatisch, 0 bis 7 manuell)";
            m_LTRMap[Item::QRSaveButton]                     = "Speichern";
            m_LTRMap[Item::QRButtonTooltip]                  = "Klicken um den QR-Code zu kopieren";

            m_LTRMap[Item::ErrImageOutOfMemory]                 = "Es konnte kein Speicher für den QR-Code zugewiesen werden, angefordert: %u Bytes (%4.2f GB)\nVersuche es mit einer geringeren Skalierung, Rand breite, verringere die mindest Version oder verkürze den Inhalt";
            m_LTRMap[Item::ErrFailedToGenerateQRCode]           = "%s\nQR-Code konnte nicht generiert werden da er zu groß ist.\nZum Beheben:\n- Verringere das Fehlerkorrekturlevel\n- Erhöhe das Maximale Level\n- Kürze die Texteingabe";
            m_LTRMap[Item::ErrImageWriterFailedToOpenNFD]       = "Dateidialog konnte nicht geöffnet werden:\n%s";
            m_LTRMap[Item::ErrImageWriterFailedToSaveImage]     = "Bild konnte nicht gespeichert werden.\nPfad: %s\nBreite: %d\nHöhe: %d";
            m_LTRMap[Item::ErrFailedToCopyImageToClipboard]     = "Das Bild konnte nicht in die Zwischenablage kopiert werden (%s)";
            m_LTRMap[Item::ErrApplicationBadAllocException]     = "Speicher konnte nicht zugewiesen werden. Möchten Sie die Anwendung neu starten?";
            m_LTRMap[Item::ErrApplicationUnhandledException]    = "Es ist eine nicht behandelte Ausnahme aufgetreten. Möchten Sie die Anwendung neu starten?";
            m_LTRMap[Item::ErrApplicationUnhandledStdException] = "Es ist eine nicht behandelte Ausnahme aufgetreten. Möchten Sie die Anwendung neu starten?\nWas: ";
            break;
        case Language::English:
            m_FromLeftToRight = true;
            m_LTRMap[Item::LanguageSelectionMenu]            = "Language";
            m_LTRMap[Item::QRInputField]                     = "Insert text";
            m_LTRMap[Item::QRErrorCorrection]                = "Error correction";
            m_LTRMap[Item::QRErrorCorrectionCombo]           = "Low\0Medium\0Quartile\0High\0";
            m_LTRMap[Item::QRIncreaseErrorCorrection]        = "Automatically increase the error correction level";
            m_LTRMap[Item::QRIncreaseErrorCorrectionTooltip] = "The smallest possible QR code version within the specified range is automatically selected for output.\nWhen this option is selected, the error correction level may be higher than specified if this is possible without increasing the version";
            m_LTRMap[Item::QRBorder]                         = "Border";
            m_LTRMap[Item::QRResolution]                     = "Resolution";
            m_LTRMap[Item::QRPrimaryColor]                   = "Primary color";
            m_LTRMap[Item::QRSecondaryColor]                 = "Secondary color";
            m_LTRMap[Item::QRMinVersion]                     = "Minimum version";
            m_LTRMap[Item::QRMaxVersion]                     = "Maximum version";
            m_LTRMap[Item::QRMaskPattern]                    = "Mask pattern";
            m_LTRMap[Item::QRMaskPatternTooltip]             = "(-1 automatic, 0 to 7 manual)";
            m_LTRMap[Item::QRSaveButton]                     = "Save";
            m_LTRMap[Item::QRButtonTooltip]                  = "Click to copy the QR code";

            m_LTRMap[Item::ErrImageOutOfMemory]                 = "Failed to allocate memory for qr code image, requested: %u bytes (%4.2f GB)\nTry a smaller scale, border width, reduce the minimum version or shorten the content";
            m_LTRMap[Item::ErrFailedToGenerateQRCode]           = "%s\nQR code could not be generated because it is too large.\nTo fix:\n- Reduce the error correction level\n- Increase the maximum level\n- Shorten the text input";
            m_LTRMap[Item::ErrImageWriterFailedToOpenNFD]       = "Failed to open file dialog:\n%s";
            m_LTRMap[Item::ErrImageWriterFailedToSaveImage]     = "Failed to save image.\nPath: %s\nWidth: %d\nHeight: %d";
            m_LTRMap[Item::ErrFailedToCopyImageToClipboard]     = "Failed to copy image to clipboard (%s)";
            m_LTRMap[Item::ErrApplicationBadAllocException]     = "Failed to allocate memory, do you want to restart the application?";
            m_LTRMap[Item::ErrApplicationUnhandledException]    = "Unhandled exception occurred, do you want to restart the application?";
            m_LTRMap[Item::ErrApplicationUnhandledStdException] = "Unhandled exception occurred, do you want to restart the application?\nWhat: ";
            break;
        case Language::Hebrew:
            m_FromLeftToRight = false;
            m_RTLMap[Item::LanguageSelectionMenu]            = u8"שפה";
            m_RTLMap[Item::QRInputField]                     = u8"הכנס טקסט";
            m_RTLMap[Item::QRErrorCorrection]                = u8"תיקון שגיאות";
            m_RTLMap[Item::QRErrorCorrectionCombo]           = u8"_גבוה_רבעון_בינוני_נמוך";
            m_RTLMap[Item::QRIncreaseErrorCorrection]        = u8"הגדל באופן אוטומטי את רמת תיקון השגיאות";
            m_RTLMap[Item::QRIncreaseErrorCorrectionTooltip] = u8"כאשר אפשרות זו נבחרה, רמת תיקון השגיאה עשויה להיות גבוהה מהמצוין אם הדבר אפשרי מבלי להגדיל את הגרסה\nגרסת קוד ה-RQ הקטנה ביותר האפשרית בטווח שצוין נבחרת אוטומטית לפלט.";
            m_RTLMap[Item::QRBorder]                         = u8"גבול";
            m_RTLMap[Item::QRResolution]                     = u8"רזולוציית";
            m_RTLMap[Item::QRPrimaryColor]                   = u8"צבע יסוד";
            m_RTLMap[Item::QRSecondaryColor]                 = u8"צבע משני";
            m_RTLMap[Item::QRMinVersion]                     = u8"גרסה מינימלית";
            m_RTLMap[Item::QRMaxVersion]                     = u8"גרסה מקסימלית";
            m_RTLMap[Item::QRMaskPattern]                    = u8"דפוס מסכה";
            m_RTLMap[Item::QRMaskPatternTooltip]             = u8")-1 אוטומטי, 0 עד 7 ידני(";
            m_RTLMap[Item::QRSaveButton]                     = u8"שמור";
            m_RTLMap[Item::QRButtonTooltip]                  = u8"לחץ כדי להעתיק את קוד ה-RQ";

            m_RTLMap[Item::ErrImageOutOfMemory]                 = u8"qr נכשלה הקצאת זיכרון לתמונת קוד\n(%4.2f GB) בתים %u :מבוקש";
            m_RTLMap[Item::ErrFailedToGenerateQRCode]           = u8"%s\n.כי הוא גדול מדי QR לא ניתן היה ליצור קוד\nכדי לתקן:\n- הפחת את רמת תיקון השגיאות\n- הגדל את הרמה המקסימלית\n- קצר את קלט הטקסט";
            m_RTLMap[Item::ErrImageWriterFailedToOpenNFD]       = u8":פתיחת תיבת הדו-שיח של הקובץ נכשלה\n%s";
            m_RTLMap[Item::ErrImageWriterFailedToSaveImage]     = u8".שמירת התמונה נכשלה\nנָתִיב: %s\nרוֹחַב: %d\nגוֹבַה: %d";
            m_RTLMap[Item::ErrFailedToCopyImageToClipboard]     = u8"ההעתקה של התמונה ללוח נכשלה\n(%s)";
            m_RTLMap[Item::ErrApplicationBadAllocException]     = u8"?הקצאת הזיכרון נכשלה, האם ברצונך להפעיל מחדש את היישום";
            m_RTLMap[Item::ErrApplicationUnhandledException]    = u8"?אירעה חריגה שלא טופלה, האם ברצונך להפעיל מחדש את היישום";
            m_RTLMap[Item::ErrApplicationUnhandledStdException] = u8"?אירעה חריגה שלא טופלה, האם ברצונך להפעיל מחדש את היישום\nמה: ";
            break;
        case Language::Arabic:
            m_FromLeftToRight = false;
            m_RTLMap[Item::LanguageSelectionMenu]            = u8"لُغة";
            m_RTLMap[Item::QRInputField]                     = u8"إدراج نص";
            m_RTLMap[Item::QRErrorCorrection]                = u8"تصحيح الاخطاء";
            m_RTLMap[Item::QRErrorCorrectionCombo]           = u8"_عالٍ" u8"_الربع" u8"_مُتَوَسِّط_مُنْخَفِض";
            m_RTLMap[Item::QRIncreaseErrorCorrection]        = u8"زيادة مستوى تصحيح الخطأ تلقائيًا";
            m_RTLMap[Item::QRIncreaseErrorCorrectionTooltip] = u8"عند تحديد هذا الخيار، قد يكون مستوى تصحيح الخطأ أعلى من المحدد إذا كان ذلك ممكنًا دون زيادة الإصدار\nيتم تحديد أصغر إصدار ممكن لرمز الاستجابة السريعة ضمن النطاق المحدد تلقائيًا للإخراج.";
            m_RTLMap[Item::QRBorder]                         = u8"كَنار";
            m_RTLMap[Item::QRResolution]                     = u8"تَصْميم";
            m_RTLMap[Item::QRPrimaryColor]                   = u8"لون أصلي";
            m_RTLMap[Item::QRSecondaryColor]                 = u8"اللون الثانوي";
            m_RTLMap[Item::QRMinVersion]                     = u8"الحد الأدنى من الإصدار";
            m_RTLMap[Item::QRMaxVersion]                     = u8"الإصدار الأقصى";
            m_RTLMap[Item::QRMaskPattern]                    = u8"نمط القناع";
            m_RTLMap[Item::QRMaskPatternTooltip]             = u8")-1 تلقائي، 0 إلى 7 يدوي(";
            m_RTLMap[Item::QRSaveButton]                     = u8"يُوَفِّر";
            m_RTLMap[Item::QRButtonTooltip]                  = u8"انقر لنسخ رمز الاستجابة السريعة";

            m_RTLMap[Item::ErrImageOutOfMemory]                 = u8"فشل في تخصيص الذاكرة لصورة رمز الاستجابة السريعة\n(%4.2f GB) بايت %u :مطلوب\nحاول استخدام نطاق أصغر أو عرض الحدود أو تقليل الحد الأدنى للإصدار أو تقصير المحتوى";
            m_RTLMap[Item::ErrFailedToGenerateQRCode]           = u8"%s\nتعذر إنشاء رمز QR لأنه كبير جدًا.\nللإصلاح:\n- تقليل مستوى تصحيح الخطأ\n- زيادة المستوى الأقصى\n- تقصير إدخال النص";
            m_RTLMap[Item::ErrImageWriterFailedToOpenNFD]       = u8":فشل في فتح مربع حوار الملف\n%s";
            m_RTLMap[Item::ErrImageWriterFailedToSaveImage]     = u8".فشل حفظ الصورة\nطريق: %s\nعرض: %d\nارتفاع: %d";
            m_RTLMap[Item::ErrFailedToCopyImageToClipboard]     = u8"فشل نسخ الصورة إلى الحافظة\n(%s)";
            m_RTLMap[Item::ErrApplicationBadAllocException]     = u8"فشل تخصيص الذاكرة، هل تريد إعادة تشغيل التطبيق؟";
            m_RTLMap[Item::ErrApplicationUnhandledException]    = u8"حدث استثناء لم تتم معالجته، هل تريد إعادة تشغيل التطبيق؟";
            m_RTLMap[Item::ErrApplicationUnhandledStdException] = u8"حدث استثناء لم تتم معالجته، هل تريد إعادة تشغيل التطبيق؟\nماذا: ";
            break;
        }
        m_SelectedLanguage = lang;

        if (!m_FromLeftToRight)
        {
            for (auto& [key, val] : m_RTLMap) 
            {
                if (key >= Item::ErrImageOutOfMemory && key <= Item::ErrApplicationUnhandledStdException) continue;

                Utf8Rev(val.data());
                if (key == Item::QRErrorCorrectionCombo)
                {
                    val = std::regex_replace(val, std::regex("\\_"), std::string("\0", 1));
                }
            }
        }
    }

    static inline const char* NameAsString(Language lang) noexcept
    {
        switch (lang)
        {
        case Language::German:
            return "Deutsch";
        case Language::English:
            return "English";
        case Language::Hebrew:
            return u8"תירבע";
        case Language::Arabic:
            static const std::string arabic = []() { std::string a(u8"العَربِيّة"); Utf8Rev(a.data()); return a; }();
            return arabic.c_str();
        }
        return "";
    }

    static inline const char* Get(Item item) noexcept
    {
        return m_FromLeftToRight ? m_LTRMap[item] : m_RTLMap[item].c_str();
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