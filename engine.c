#include <ibus.h>
#include <string.h>

typedef struct _MongolEngine MongolEngine;
typedef struct _MongolEngineClass MongolEngineClass;

struct _MongolEngine {
    IBusEngine parent;
    GString *latin_buffer; 
};

struct _MongolEngineClass {
    IBusEngineClass parent;
};

GType mongol_engine_get_type(void);

#define MONGOL_TYPE_ENGINE (mongol_engine_get_type())
#define MONGOL_ENGINE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MONGOL_TYPE_ENGINE, MongolEngine))
#define MONGOL_ENGINE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MONGOL_TYPE_ENGINE, MongolEngineClass))

G_DEFINE_TYPE(MongolEngine, mongol_engine, IBUS_TYPE_ENGINE)

// Эгшиг мөн эсэхийг шалгах (Том үсэг байсан ч жижиг болгож шалгана)
static gboolean is_latin_vowel(char c) {
    c = g_ascii_tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'q' || c == 'w' || c == 'y');
}

// Крилл үсгийг том/жижгээр нь ялгаж залгах функц
static void append_cyr(GString *cyr, const gchar *text, gboolean is_upper) {
    if (is_upper) {
        gchar *upper_text = g_utf8_strup(text, -1);
        g_string_append(cyr, upper_text);
        g_free(upper_text);
    } else {
        g_string_append(cyr, text);
    }
}

// Хөрвүүлэх үндсэн логик
static GString* convert_to_cyrillic(GString *latin) {
    GString *cyr = g_string_new("");
    for (int i = 0; i < latin->len; i++) {
        char c = latin->str[i];
        char next = (i + 1 < latin->len) ? latin->str[i + 1] : '\0';

        gboolean is_upper = g_ascii_isupper(c); // Тухайн үсэг том эсэх
        char lc = g_ascii_tolower(c);           // Дүрмээ шалгахын тулд жижиг болгох
        char lnext = g_ascii_tolower(next);

        // 1. Хосолсон үсгүүд
        if (lc == 's' && lnext == 'h') { append_cyr(cyr, "ш", is_upper); i++; continue; }
        if (lc == 'c' && lnext == 'h') { append_cyr(cyr, "ч", is_upper); i++; continue; }
        if (lc == 'y' && lnext == 'a') { append_cyr(cyr, "я", is_upper); i++; continue; }
        if (lc == 'y' && lnext == 'e') { append_cyr(cyr, "е", is_upper); i++; continue; }
        if (lc == 'y' && lnext == 'o') { append_cyr(cyr, "ё", is_upper); i++; continue; }
        if (lc == 'y' && lnext == 'u') { append_cyr(cyr, "ю", is_upper); i++; continue; }

        // 2. 'i' үсгийн тусгай дүрмүүд
        if (lc == 'i') {
            if (lnext == 'i') {
                append_cyr(cyr, "ий", is_upper);
                i++; 
            } else if (i > 0 && is_latin_vowel(latin->str[i - 1])) {
                append_cyr(cyr, "й", is_upper); 
            } else {
                append_cyr(cyr, "и", is_upper); 
            }
            continue;
        }

        // 3. Үндсэн үсгүүд (Бүрэн гүйцээсэн жагсаалт)
        switch (lc) {
            case 'a': append_cyr(cyr, "а", is_upper); break;
            case 'b': append_cyr(cyr, "б", is_upper); break;
            case 'c': append_cyr(cyr, "ц", is_upper); break;
            case 'd': append_cyr(cyr, "д", is_upper); break;
            case 'e': append_cyr(cyr, "э", is_upper); break;
            case 'f': append_cyr(cyr, "ф", is_upper); break;
            case 'g': append_cyr(cyr, "г", is_upper); break;
            case 'h': append_cyr(cyr, "х", is_upper); break;
            case 'x': append_cyr(cyr, "х", is_upper); break;
            case 'j': append_cyr(cyr, "ж", is_upper); break; // Нэмэв
            case 'k': append_cyr(cyr, "к", is_upper); break; // Нэмэв
            case 'l': append_cyr(cyr, "л", is_upper); break; // Нэмэв
            case 'm': append_cyr(cyr, "м", is_upper); break;
            case 'n': append_cyr(cyr, "н", is_upper); break;
            case 'o': append_cyr(cyr, "о", is_upper); break;
            case 'p': append_cyr(cyr, "п", is_upper); break; // Нэмэв
            case 'q': append_cyr(cyr, "ө", is_upper); break;
            case 'r': append_cyr(cyr, "р", is_upper); break; // Нэмэв
            case 's': append_cyr(cyr, "с", is_upper); break;
            case 't': append_cyr(cyr, "т", is_upper); break; // Нэмэв
            case 'u': append_cyr(cyr, "у", is_upper); break;
            case 'w': append_cyr(cyr, "ү", is_upper); break;
            case 'v': append_cyr(cyr, "в", is_upper); break;
            case 'y': append_cyr(cyr, "ы", is_upper); break;
            case 'z': append_cyr(cyr, "з", is_upper); break;
            case '\'': append_cyr(cyr, "ь", FALSE); break;
            case '"':  append_cyr(cyr, "ъ", FALSE); break;
            default: g_string_append_c(cyr, c); break; 
        }
    }
    return cyr;
}

static void update_preedit(MongolEngine *engine) {
    if (engine->latin_buffer->len > 0) {
        GString *cyr = convert_to_cyrillic(engine->latin_buffer);
        IBusText *text = ibus_text_new_from_string(cyr->str);
        ibus_engine_update_preedit_text(IBUS_ENGINE(engine), text, ibus_text_get_length(text), TRUE);
        g_string_free(cyr, TRUE);
    } else {
        ibus_engine_hide_preedit_text(IBUS_ENGINE(engine));
    }
}

static void commit_buffer(MongolEngine *engine) {
   if (engine->latin_buffer->len == 0) return;
    GString *cyr = convert_to_cyrillic(engine->latin_buffer);
    IBusText *text = ibus_text_new_from_string(cyr->str);
    
    // 1. Эхлээд буферээ хоосолж, дэлгэц дээрх зураастай түр текстийг устгах
    g_string_erase(engine->latin_buffer, 0, -1);
    update_preedit(engine);
    
    // 2. Дэлгэц түр текстээс цэвэрлэгдсэний дараа жинхэнэ үгээ буулгах
    ibus_engine_commit_text(IBUS_ENGINE(engine), text);
    
    g_string_free(cyr, TRUE);
}

static gboolean mongol_engine_process_key_event(IBusEngine *ibusengine, guint keyval, guint keycode, guint state) {
    MongolEngine *engine = MONGOL_ENGINE(ibusengine);

    // ЗАСВАР 1:Давхар бичигдэхийг зогсооно 
    if (state & IBUS_RELEASE_MASK) {
        return FALSE; 
    }

    if (state & (IBUS_CONTROL_MASK | IBUS_MOD1_MASK)) {
        commit_buffer(engine); 
        return FALSE; 
    }

    if (keyval == IBUS_KEY_BackSpace) {
        if (engine->latin_buffer->len > 0) {
            g_string_truncate(engine->latin_buffer, engine->latin_buffer->len - 1);
            update_preedit(engine);
            return TRUE;
        }
        return FALSE; 
    }

    if (keyval == IBUS_KEY_space || keyval == IBUS_KEY_Return) {
        if (engine->latin_buffer->len > 0) {
            commit_buffer(engine);
            IBusText *space_text = ibus_text_new_from_string(keyval == IBUS_KEY_space ? " " : "\n");
            ibus_engine_commit_text(ibusengine, space_text);
            return TRUE;
        }
        return FALSE;
    }

    // ЗАСВАР 2: Том (A-Z) болон Жижиг (a-z) үсгүүдийг хоёуланг нь буфферт авах
    if ((keyval >= IBUS_KEY_a && keyval <= IBUS_KEY_z) || 
        (keyval >= IBUS_KEY_A && keyval <= IBUS_KEY_Z)) {
        g_string_append_c(engine->latin_buffer, (char)keyval);
        update_preedit(engine);
        return TRUE;
    }
    
    // Тусгай тэмдэгтүүд (ь, ъ)
    if (keyval == IBUS_KEY_apostrophe) { 
        g_string_append_c(engine->latin_buffer, '\'');
        update_preedit(engine);
        return TRUE;
    }
    if (keyval == IBUS_KEY_quotedbl) { // Shift + ' буюу " орж ирвэл
        g_string_append_c(engine->latin_buffer, '"');
        update_preedit(engine);
        return TRUE;
    }

    // Хэрэв өөр товч (цэг, таслал, тоо) дарагдвал бичиж байсан үгээ батлаад цааш хэвийн шилжүүлэх
    if (engine->latin_buffer->len > 0) {
        commit_buffer(engine);
    }

    return FALSE; 
}

static void mongol_engine_class_init(MongolEngineClass *klass) {
    IBusEngineClass *engine_class = IBUS_ENGINE_CLASS(klass);
    engine_class->process_key_event = mongol_engine_process_key_event;
}

static void mongol_engine_init(MongolEngine *engine) {
    engine->latin_buffer = g_string_new("");
}