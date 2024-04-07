// dear imgui, v1.88 WIP
// (demo code)

// Помощь:
// - Прочтите FAQ на http://dearimgui.org/faq
// - Новички, прочтите «Руководство программиста» в imgui.cpp, чтобы узнать, как настроить Уважаемый ImGui в вашей кодовой базе.
// - Вызов и чтение ImGui::ShowDemoWindow() в imgui_demo.cpp. Все приложения в examples/ делают это.
// Прочтите imgui.cpp для получения более подробной информации, документации и комментариев.
// Получить последнюю версию на https://github.com/ocornut/imgui

// Сообщение тому, кто хочет удалить этот файл при интеграции Dear ImGui в свою кодовую базу:
// НЕ удаляйте этот файл из вашего проекта! Подумайте еще раз! Это самый полезный справочный код, который вы и другие
// кодеры захотят обратиться и вызвать. Включите функцию ImGui::ShowDemoWindow() в всегда доступный
// меню отладки вашей игры/приложения! Удаление этого файла из вашего проекта препятствует доступу к документации для всех
// в вашей команде, что, вероятно, приведет к более плохому использованию библиотеки.
// Все в этом файле будет удалено компоновщиком, если вы не вызовете ImGui::ShowDemoWindow().
// Если вы хотите связать ядро ​​Уважаемый ImGui в своих поставляемых сборках, но хотите получить полную гарантию того, что демо не будет
// связан, вы можете настроить свой imconfig.h с помощью #define IMGUI_DISABLE_DEMO_WINDOWS, и эти функции будут пустыми.
// В другой ситуации, когда у вас есть доступ к Dear ImGui, вы, вероятно, захотите, чтобы он был доступен для справки.
// Спасибо,
// -Ваш любимый друг, imgui_demo.cpp (которого удалять не будете)

// Сообщение для начинающих программистов на C/C++ о значении ключевого слова 'static':
// В этом демонстрационном коде мы часто используем «статические» переменные внутри функций. Статическая переменная сохраняется между вызовами,
// так что это по сути похоже на глобальную переменную, но объявленную внутри области действия функции. Мы делаем это как способ
// собрать код и данные в одном месте, чтобы исходный код демо-версии быстрее читался, быстрее писался и меньше
// по размеру. Это также удобный способ хранения простой информации, связанной с пользовательским интерфейсом, если ваша функция
// не требуется реентерабельность или использование в нескольких потоках. Это может быть шаблон, который вы захотите использовать в своем коде,
// но большая часть реальных данных, которые вы будете редактировать, скорее всего, будет храниться вне ваших функций.

// Демонстрационный код в этом файле предназначен для простого копирования и вставки в ваше приложение!
// Из-за этого:
// - Мы никогда не пропускаем префикс ImGui:: при вызове функций, хотя большая часть кода здесь находится в одном и том же пространстве имен.
// - Стараемся объявлять статические переменные в локальной области видимости, как можно ближе к коду, который их использует.
// - Мы никогда не используем какие-либо помощники/средства, используемые внутри компании Dear ImGui, если только они не доступны в общедоступном API.
// - Мы никогда не используем математические операторы для ImVec2/ImVec4. Для других наших исходных файлов мы используем их, и они предоставляются
// imgui_internal.h с использованием определения IMGUI_DEFINE_MATH_OPERATORS. Для вашего собственного исходного файла они необязательны.
// и требуют, чтобы вы либо включили их, либо предоставили свои собственные через IM_VEC2_CLASS_EXTRA в imconfig.h.
// Поскольку мы ничего не можем предположить о вашей поддержке математических операторов, мы не можем использовать их в imgui_demo.cpp.

// Навигация по этому файлу:
// - В Visual Studio IDE: CTRL+запятая ("Edit.GoToAll") может следовать за символами в комментариях, тогда как CTRL+F12 ("Edit.GoToImplementation") не может.
// - При установленном Visual Assist: ALT+G ("VAssistX.GoToImplementation") также может следовать за символами в комментариях.

/*

Индекс этого файла:

// [SECTION] Форвардные объявления, помощники
// [SECTION] Демонстрационное окно / ShowDemoWindow()
// - sub section: ShowDemoWindowWidgets()
// - sub section: ShowDemoWindowLayout()
// - sub section: ShowDemoWindowPopups()
// - sub section: ShowDemoWindowTables()
// - sub section: ShowDemoWindowMisc()
// [SECTION] Об окне / ShowAboutWindow()
// [SECTION] Редактор стилей / ShowStyleEditor()
// [SECTION] Пример приложения: панель главного меню / ShowExampleAppMainMenuBar()
// [SECTION] Пример приложения: консоль отладки / ShowExampleAppConsole()
// [SECTION] Пример приложения: журнал отладки / ShowExampleAppLog()
// [SECTION] Пример приложения: простой макет / ShowExampleAppLayout()
// [SECTION] Пример приложения: Редактор свойств / ShowExampleAppPropertyEditor()
// [SECTION] Пример приложения: длинный текст / ShowExampleAppLongText()
// [SECTION] Пример приложения: автоматическое изменение размера / ShowExampleAppAutoResize()
// [SECTION] Пример приложения: ограниченное изменение размера / ShowExampleAppConstrainedResize()
// [SECTION] Пример приложения: Простое наложение / ShowExampleAppSimpleOverlay()
// [SECTION] Пример приложения: Полноэкранное окно / ShowExampleAppFullscreen()
// [SECTION] Пример приложения: управление заголовками окон / ShowExampleAppWindowTitles()
// [SECTION] Пример приложения: пользовательский рендеринг с использованием API ImDrawList / ShowExampleAppCustomRendering()
// [SECTION] Пример приложения: стыковка, DockSpace / ShowExampleAppDockSpace()
// [SECTION] Пример приложения: обработка документов / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE

// System includes
#include <ctype.h>                          // toupper
#include <limits.h>                         // INT_MIN, INT_MAX
#include <math.h>                           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>                          // vsnprintf, sscanf, printf
#include <stdlib.h>                         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500   // MSVC 2008 or earlier
#include <stddef.h>                         // intptr_t
#else
#include <stdint.h>   // intptr_t
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning(disable:4127)    // выражение условия является константой
#pragma warning(disable:4996)    // 'Эта функция или переменная могут быть небезопасными': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning(disable:26451)   // [Static Analyzer] Арифметическое переполнение: использование оператора «xxx» для 4-байтового значения с последующим преобразованием результата в 8-байтовое значение. Приведите значение к более широкому типу перед вызовом оператора 'xxx', чтобы избежать переполнения (io.2).
#endif

// Clang/GCC предупреждения с -Weeverthing
#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"   // предупреждение: неизвестная группа предупреждений 'xxx' не все предупреждения известны во всех версиях Clang, и они, как правило, переименовываются. \
                                                              // поэтому игнорирование предупреждений вызывает появление новых предупреждений в некоторой конфигурации. Большой!
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                 // предупреждение: неизвестная группа предупреждений 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                  // предупреждение: использование приведения в старом стиле  // да, они более лаконичны.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"         // предупреждение: 'xx' устарело: имя POSIX для этого.. \
                                                                     // для strdup, используемого в демонстрационном коде (чтобы пользователь мог копировать и вставлять код)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"        // предупреждение: преобразование в 'void *' из меньшего целочисленного типа
#pragma clang diagnostic ignored "-Wformat-security"                 // предупреждение: строка формата не является строковым литералом
#pragma clang diagnostic ignored "-Wexit-time-destructors"           // предупреждение: для объявления требуется деструктор времени выхода                                    \
                                                                     // Порядок уничтожения при выходе не определен. если MemFree() приводит к коду пользователя, который был \
                                                                     // отключен перед выходом, это может вызвать проблемы. Стиль кодирования ImGui приветствует статические/глобальные объекты.
#pragma clang diagnostic ignored "-Wunused-macros"                   // предупреждение: макрос не используется \
                                                                     // мы определяем snprintf/vsnprintf в Windows, поэтому они доступны, но не всегда используются.
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"   // предупреждение: ноль как константа нулевого указателя \
                                                                     // некоторые стандартные варианты заголовков используют #define NULL 0
#pragma clang diagnostic ignored "-Wdouble-promotion"                // предупреждение: неявное преобразование из 'float' в 'double' при передаче аргумента в функцию \
                                                                     // использование printf() является страданием с этим, так как C++ va_arg заменяет многоточие float на double.
#pragma clang diagnostic ignored "-Wreserved-id-macro"               // предупреждение: имя макроса является зарезервированным идентификатором
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"   // предупреждение: неявное преобразование из 'xxx' в 'float' может привести к потере точности
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                  // предупреждение: неизвестная опция после '#pragma GCC Diagnostic'
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // предупреждение: приведение к указателю из целого числа другого размера
#pragma GCC diagnostic ignored "-Wformat-security"          // предупреждение: строка формата не является строковым литералом (потенциально небезопасно)
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // предупреждение: неявное преобразование из float в double при передаче аргумента в функцию
#pragma GCC diagnostic ignored "-Wconversion"               // предупреждение: преобразование в 'xxxx' из 'xxxx' может изменить его значение
#pragma GCC diagnostic ignored "-Wmisleading-indentation"   // [__GNUC__ >= 6] предупреждение: это предложение 'if' не защищает этот оператор // Только GCC 6.0+. См. #883 на GitHub.
#endif

// Подружитесь с пользователями Windows (обновление: май 2018 г., Блокнот теперь поддерживает возврат каретки в стиле Unix!)
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

// Helpers
#if defined(_MSC_VER) && !defined(snprintf)
#define snprintf _snprintf
#endif
#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf _vsnprintf
#endif

// Спецификаторы формата, 64-битная печать не была должным образом стандартизирована...
// В реальном приложении вы должны использовать PRId64 и PRIu64 из <inttypes.h> (не для Windows), а в Windows определить их самостоятельно.
#ifdef _MSC_VER
#define IM_PRId64 "I64d"
#define IM_PRIu64 "I64u"
#else
#define IM_PRId64 "lld"
#define IM_PRIu64 "llu"
#endif

// Helpers macros
// Обычно мы стараемся не использовать много помощников в imgui_demo.cpp, чтобы упростить копирование и вставку кода,
// но здесь делаем исключение, так как они значительно упрощают код...
// В других источниках imgui мы можем использовать более удобные внутренние функции из imgui_internal.h (ImMin/ImMax), но не в демо.
#define IM_MIN(A, B)        (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)        (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX) ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Применить соглашение о вызовах cdecl для функций, вызываемых стандартной библиотекой, в случае,
// если настройки компиляции изменили значение по умолчанию, например, на __vectorcall
#ifndef IMGUI_CDECL
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
static void ShowExampleAppDockSpace(bool* p_open);
static void ShowExampleAppDocuments(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowExampleAppFullscreen(bool* p_open);
static void ShowExampleAppWindowTitles(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleMenuFile();

// Помощник для отображения маленькой (?) метки, которая показывает всплывающую подсказку при наведении.
// В вашем собственном коде вы можете захотеть отобразить фактическую иконку, если вы используете шрифты с объединенными иконками (см. docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration."_RU >> u8"ERROR: стыковка не включена! См. Демонстрация > Конфигурация.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or "_RU >> u8"Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable в вашем коде или ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here"_RU >> u8"кликните сюда"))
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// Помощник для подключения демонстрационных маркеров, расположенных в коде, к интерактивному браузеру.
typedef void                   (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiDemoMarkerCallback GImGuiDemoMarkerCallback;
extern void*                   GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback        GImGuiDemoMarkerCallback         = NULL;
void*                          GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)                                                                   \
    do                                                                                               \
    {                                                                                                \
        if (GImGuiDemoMarkerCallback != NULL)                                                        \
            GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); \
    }                                                                                                \
    while (0)

// Помощник для отображения основных пользовательских элементов управления.
void ImGui::ShowUserGuide()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::BulletText("Double-click on title bar to collapse window."_RU >> u8"Дважды щелкните строку заголовка, чтобы свернуть окно.");
    ImGui::BulletText("Click and drag on lower corner to resize window\n"
                      "(double-click to auto fit window to its contents)."_RU >>
        u8"Щелкните и перетащите нижний угол, чтобы изменить размер окна.\n"
        "(дважды щелкните, чтобы автоматически подогнать окно под его содержимое).");
    ImGui::BulletText("CTRL+Click on a slider or drag box to input value as text."_RU >> u8"CTRL+Щелкните ползунок или перетащите поле, чтобы ввести значение в виде текста.");
    ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields."_RU >> u8"TAB/SHIFT+TAB для переключения между редактируемыми с клавиатуры полями.");
    ImGui::BulletText("CTRL+Tab to select a window."_RU >> u8"CTRL+Tab для выбора окна.");
    if (io.FontAllowUserScaling)
        ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents."_RU >> u8"CTRL+Колесико мыши для масштабирования содержимого окна.");
    ImGui::BulletText("While inputing text:\n"_RU >> u8"При вводе текста:\n");
    ImGui::Indent();
    ImGui::BulletText("CTRL+Left/Right to word jump."_RU >> u8"CTRL+Left/Right к слову прыжок..");
    ImGui::BulletText("CTRL+A or double-click to select all."_RU >> u8"CTRL+A или дважды щелкните, чтобы выбрать все.");
    ImGui::BulletText("CTRL+X/C/V to use clipboard cut/copy/paste."_RU >> u8"CTRL+X/C/V использовать буфер обмена вырезать/копировать/вставить.");
    ImGui::BulletText("CTRL+Z,CTRL+Y to undo/redo."_RU >> u8"CTRL+Z,CTRL+Y Чтобы отменить/повторить.");
    ImGui::BulletText("ESCAPE to revert."_RU >> u8"ESCAPE вернуться.");
    ImGui::Unindent();
    ImGui::BulletText("With keyboard navigation enabled:"_RU >> u8"С включенной навигацией с помощью клавиатуры:");
    ImGui::Indent();
    ImGui::BulletText("Arrow keys to navigate."_RU >> u8"Клавиши со стрелками для навигации.");
    ImGui::BulletText("Space to activate a widget."_RU >> u8"Пробел для активации виджета.");
    ImGui::BulletText("Return to input text into a widget."_RU >> u8"Вернуться к вводу текста в виджет.");
    ImGui::BulletText("Escape to deactivate a widget, close popup, exit child window."_RU >> u8"Escape, чтобы деактивировать виджет, закрыть всплывающее окно, выйти из дочернего окна.");
    ImGui::BulletText("Alt to jump to the menu layer of a window."_RU >> u8"Alt для перехода к слою меню окна.");
    ImGui::Unindent();
}

//-----------------------------------------------------------------------------
// [SECTION] Demo Window / ShowDemoWindow()
//-----------------------------------------------------------------------------
// - ShowDemoWindowWidgets()
// - ShowDemoWindowLayout()
// - ShowDemoWindowPopups()
// - ShowDemoWindowTables()
// - ShowDemoWindowColumns()
// - ShowDemoWindowMisc()
//-----------------------------------------------------------------------------

// Разбиваем содержимое большой функции ShowDemoWindow() на более мелкие функции
// (поскольку время компоновки очень больших функций растет нелинейно)
static void ShowDemoWindowWidgets();
static void ShowDemoWindowLayout();
static void ShowDemoWindowPopups();
static void ShowDemoWindowTables();
static void ShowDemoWindowColumns();
static void ShowDemoWindowMisc();

// Демонстрация большинства функций Dear ImGui (это большая функция!)
// Вы можете выполнить эту функцию, чтобы поэкспериментировать с пользовательским интерфейсом и понять, что она делает.
// Затем вы можете искать ключевые слова в коде, когда вас интересует конкретная функция.
void        ImGui::ShowDemoWindow(bool* p_open)
{
    // В исключительных случаях добавьте здесь дополнительное утверждение для тех, кто запутался в первоначальной настройке Dear ImGui
    // Большинство функций ImGui обычно просто аварийно завершают работу, если отсутствует контекст.
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"_RU >> u8"Отсутствует контекст Dear Imgui. Обратитесь к примерам приложения!");

    // Примеры приложений (доступны из меню «Примеры»)
    static bool show_app_main_menu_bar      = false;
    static bool show_app_dockspace          = false;
    static bool show_app_documents          = false;

    static bool show_app_console            = false;
    static bool show_app_log                = false;
    static bool show_app_layout             = false;
    static bool show_app_property_editor    = false;
    static bool show_app_long_text          = false;
    static bool show_app_auto_resize        = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_simple_overlay     = false;
    static bool show_app_fullscreen         = false;
    static bool show_app_window_titles      = false;
    static bool show_app_custom_rendering   = false;

    if (show_app_main_menu_bar)
        ShowExampleAppMainMenuBar();
    if (show_app_dockspace)
        ShowExampleAppDockSpace(&show_app_dockspace);   // Сначала обработайте приложение Docking, так как явные узлы DockSpace()
                                                        // должны быть отправлены заранее (прочитайте комментарии рядом с функцией DockSpace)
    if (show_app_documents)
        ShowExampleAppDocuments(&show_app_documents);   // Затем обработайте приложение Document, так как оно также может использовать DockSpace()

    if (show_app_console)
        ShowExampleAppConsole(&show_app_console);
    if (show_app_log)
        ShowExampleAppLog(&show_app_log);
    if (show_app_layout)
        ShowExampleAppLayout(&show_app_layout);
    if (show_app_property_editor)
        ShowExampleAppPropertyEditor(&show_app_property_editor);
    if (show_app_long_text)
        ShowExampleAppLongText(&show_app_long_text);
    if (show_app_auto_resize)
        ShowExampleAppAutoResize(&show_app_auto_resize);
    if (show_app_constrained_resize)
        ShowExampleAppConstrainedResize(&show_app_constrained_resize);
    if (show_app_simple_overlay)
        ShowExampleAppSimpleOverlay(&show_app_simple_overlay);
    if (show_app_fullscreen)
        ShowExampleAppFullscreen(&show_app_fullscreen);
    if (show_app_window_titles)
        ShowExampleAppWindowTitles(&show_app_window_titles);
    if (show_app_custom_rendering)
        ShowExampleAppCustomRendering(&show_app_custom_rendering);

    // Dear ImGui Apps (доступно из меню "Инструменты")
    static bool show_app_metrics      = false;
    static bool show_app_stack_tool   = false;
    static bool show_app_style_editor = false;
    static bool show_app_about        = false;

    if (show_app_metrics)
    {
        ImGui::ShowMetricsWindow(&show_app_metrics);
    }
    if (show_app_stack_tool)
    {
        ImGui::ShowStackToolWindow(&show_app_stack_tool);
    }
    if (show_app_about)
    {
        ImGui::ShowAboutWindow(&show_app_about);
    }
    if (show_app_style_editor)
    {
        ImGui::Begin("Dear ImGui Style Editor"_RU >> u8"Dear ImGui Редактор Стилей", &show_app_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    // Демонстрация различных флагов окна. Обычно вы просто используете значение по умолчанию!
    static bool      no_titlebar       = false;
    static bool      no_scrollbar      = false;
    static bool      no_menu           = false;
    static bool      no_move           = false;
    static bool      no_resize         = false;
    static bool      no_collapse       = false;
    static bool      no_close          = false;
    static bool      no_nav            = false;
    static bool      no_background     = false;
    static bool      no_bring_to_front = false;
    static bool      no_docking        = false;
    static bool      unsaved_document  = false;

    ImGuiWindowFlags window_flags      = 0;
    if (no_titlebar)
        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)
        window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)
        window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)
        window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)
        window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)
        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)
        window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)
        window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_docking)
        window_flags |= ImGuiWindowFlags_NoDocking;
    if (unsaved_document)
        window_flags |= ImGuiWindowFlags_UnsavedDocument;
    if (no_close)
        p_open = NULL;   // Не передавать наш bool* в Begin

    // Мы указываем положение/размер по умолчанию на случай, если в файле .ini нет данных.
    // Мы делаем это только для того, чтобы сделать демо-приложения более привлекательными, но обычно это не требуется.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Основная часть демонстрационного окна начинается здесь.
    if (!ImGui::Begin("Dear ImGui Demo", p_open, window_flags))
    {
        // Рано, если окно свернуто, в качестве оптимизации.
        ImGui::End();
        return;
    }

    // Большинство "больших" виджетов по умолчанию имеют общие настройки ширины. Подробнее см. в разделе «Демо->Макет->Ширина виджетов».

    // например Используйте 2/3 места для виджетов и 1/3 для меток (выравнивание по правому краю).
    // ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);

    // например Оставьте фиксированную ширину для меток (путем передачи отрицательного значения), остальная часть достается виджетам.
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"_RU >> u8"Меню"))
        {
            IMGUI_DEMO_MARKER("Menu/File"_RU >> u8"Меню/Файл");
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"_RU >> u8"Примеры"))
        {
            IMGUI_DEMO_MARKER("Menu/Examples"_RU >> u8"Меню/Примеры");
            ImGui::MenuItem("Main menu bar"_RU >> u8"Меню Бар", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Console"_RU >> u8"Консоль", NULL, &show_app_console);
            ImGui::MenuItem("Log"_RU >> u8"Лог", NULL, &show_app_log);
            ImGui::MenuItem("Simple layout"_RU >> u8"Простой макет", NULL, &show_app_layout);
            ImGui::MenuItem("Property editor"_RU >> u8"Редактор свойств", NULL, &show_app_property_editor);
            ImGui::MenuItem("Long text display"_RU >> u8"Отображение длинного текста", NULL, &show_app_long_text);
            ImGui::MenuItem("Auto-resizing window"_RU >> u8"Автоматическое изменение размера окна", NULL, &show_app_auto_resize);
            ImGui::MenuItem("Constrained-resizing window"_RU >> u8"Окно с ограниченным изменением размера", NULL, &show_app_constrained_resize);
            ImGui::MenuItem("Simple overlay"_RU >> u8"Простое наложение", NULL, &show_app_simple_overlay);
            ImGui::MenuItem("Fullscreen window"_RU >> u8"Полноэкранное окно", NULL, &show_app_fullscreen);
            ImGui::MenuItem("Manipulating window titles"_RU >> u8"Управление заголовками окон", NULL, &show_app_window_titles);
            ImGui::MenuItem("Custom rendering"_RU >> u8"Пользовательский рендеринг", NULL, &show_app_custom_rendering);
            ImGui::MenuItem("Dockspace"_RU >> u8"Докпространство", NULL, &show_app_dockspace);
            ImGui::MenuItem("Documents"_RU >> u8"Документы", NULL, &show_app_documents);
            ImGui::EndMenu();
        }
        //if (ImGui::MenuItem("MenuItem")) {} // Вы также можете использовать MenuItem() внутри строки меню!
        if (ImGui::BeginMenu("Tools"_RU >> u8"Инструменты"))
        {
            IMGUI_DEMO_MARKER("Menu/Tools"_RU >> u8"Меню/Инструменты");
#ifndef IMGUI_DISABLE_METRICS_WINDOW
            ImGui::MenuItem("Metrics/Debugger"_RU >> u8"Метрики/Отладчик", NULL, &show_app_metrics);
            ImGui::MenuItem("Stack Tool"_RU >> u8"Инструмент стека", NULL, &show_app_stack_tool);
#endif
            ImGui::MenuItem("Style Editor"_RU >> u8"Редактор Стилей", NULL, &show_app_style_editor);
            ImGui::MenuItem("About Dear ImGui"_RU >> u8"Об Dear ImGui", NULL, &show_app_about);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("dear imgui says hello! (%s) (%d)"_RU >> u8"dear imgui передает привет! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::Spacing();

    IMGUI_DEMO_MARKER("Help"_RU >> u8"Помощь");
    if (ImGui::CollapsingHeader("Help"_RU >> u8"Помощь"))
    {
        ImGui::Text("ABOUT THIS DEMO:"_RU >> u8"ОБ ЭТОЙ ДЕМО");
        ImGui::BulletText("Sections below are demonstrating many aspects of the library."_RU >> u8"Разделы ниже демонстрируют многие аспекты библиотеки..");
        ImGui::BulletText("The \"Examples\" menu above leads to more demo contents."_RU >> u8"Меню \"Примеры\" выше ведет к большему количеству демо-контента.");
        ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
                          "and Metrics\Debugger (general purpose Dear ImGui debugging tool)."_RU >>
            u8"Меню \"Инструменты\" дает доступ к: О блоке, Редактору стилей.,\n"
            "и Metrics\Debugger (инструмент отладки общего назначения Dear ImGui).");
        ImGui::Separator();

        ImGui::Text("PROGRAMMER GUIDE:"_RU >> u8"РУКОВОДСТВО ПО ПРОГРАММИРОВАНИЮ:");
        ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!"_RU >> u8"См. код ShowDemoWindow() в imgui_demo.cpp. <- ты здесь!");
        ImGui::BulletText("See comments in imgui.cpp."_RU >> u8"Смотрите комментарии в imgui.cpp.");
        ImGui::BulletText("See example applications in the examples/ folder."_RU >> u8"См. примеры приложений в папке examples/ folder.");
        ImGui::BulletText("Read the FAQ at http://www.dearimgui.org/faq/"_RU >> u8"Прочтите FAQ на http://www.dearimgui.org/faq/");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls."_RU >> u8"Set 'io.ConfigFlags |= NavEnableKeyboard' для управления клавиатурой.");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls."_RU >> u8"Set 'io.ConfigFlags |= NavEnableGamepad' для управления геймпадом.");
        ImGui::Separator();

        ImGui::Text("USER GUIDE:"_RU >> u8"ГИД ПОЛЬЗОВАТЕЛЯ:");
        ImGui::ShowUserGuide();
    }

    IMGUI_DEMO_MARKER("Configuration"_RU >> u8"Конфигурация");
    if (ImGui::CollapsingHeader("Configuration"_RU >> u8"Конфигурация"))
    {
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::TreeNode("Configuration##2"_RU >> u8"Конфигурация##2"))
        {
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", &io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
            ImGui::SameLine();
            HelpMarker("Enable keyboard controls."_RU >> u8"Включите управление с клавиатуры.");
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", &io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
            ImGui::SameLine();
            HelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details."_RU >> u8"Включите управление геймпадом. Требовать от бэкенда установки флагов io.Backend |= Флаги бэкенда Im Gui Имеет геймпад.\n\n Подробности читайте в инструкциях в imgui.cpp.");
            ImGui::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", &io.ConfigFlags, ImGuiConfigFlags_NavEnableSetMousePos);
            ImGui::SameLine();
            HelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlags_NavEnableSetMousePos."_RU >> u8"Укажите навигацию для перемещения курсора мыши. См. комментарий для Im Gui Config Flags Nav Enable Set Mouse Pos.");
            ImGui::CheckboxFlags("io.ConfigFlags: NoMouse", &io.ConfigFlags, ImGuiConfigFlags_NoMouse);
            if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)
            {
                // Параметр «NoMouse» может привести к тому, что мы застрянем с отключенной мышью! Давайте предложим альтернативный способ исправить это:
                if (fmodf((float)ImGui::GetTime(), 0.40f) < 0.20f)
                {
                    ImGui::SameLine();
                    ImGui::Text("<<PRESS SPACE TO DISABLE>>"_RU >> u8"<<НАЖМИТЕ ПРОБЕЛ, ЧТОБЫ ОТКЛЮЧИТЬ>>");
                }
                if (ImGui::IsKeyPressed(ImGuiKey_Space))
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            }
            ImGui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", &io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
            ImGui::SameLine();
            HelpMarker("Instruct backend to not alter mouse cursor shape and visibility."_RU >> u8"Проинструктируйте серверную часть не изменять форму и видимость курсора мыши.");

            ImGui::CheckboxFlags("io.ConfigFlags: DockingEnable", &io.ConfigFlags, ImGuiConfigFlags_DockingEnable);
            ImGui::SameLine();
            if (io.ConfigDockingWithShift)
                HelpMarker("Drag from window title bar or their tab to dock/undock. Hold SHIFT to enable docking.\n\nDrag from window menu button (upper-left button) to undock an entire node (all windows)."_RU >> u8"Перетащите из строки заголовка окна или их вкладки, чтобы закрепить/открепить. Удерживайте SHIFT, чтобы включить стыковку.\n\n Перетащите кнопку меню окна (верхняя левая кнопка), чтобы открепить весь узел (все окна).");
            else
                HelpMarker("Drag from window title bar or their tab to dock/undock. Hold SHIFT to disable docking.\n\nDrag from window menu button (upper-left button) to undock an entire node (all windows)."_RU >> u8"Перетащите из строки заголовка окна или их вкладки, чтобы закрепить/открепить. Удерживайте SHIFT, чтобы отключить стыковку.\n\n Перетащите кнопку меню окна (верхняя левая кнопка), чтобы открепить весь узел (все окна).");
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGui::Indent();
                ImGui::Checkbox("io.ConfigDockingNoSplit"_RU >> u8"io.ConfigDockingNoSplit", &io.ConfigDockingNoSplit);
                ImGui::SameLine();
                HelpMarker("Simplified docking mode: disable window splitting, so docking is limited to merging multiple windows together into tab-bars."_RU >> u8"Упрощенный режим стыковки: отключите разделение окон, чтобы стыковка ограничивалась объединением нескольких окон вместе в панели вкладок.");
                ImGui::Checkbox("io.ConfigDockingWithShift", &io.ConfigDockingWithShift);
                ImGui::SameLine();
                HelpMarker("Enable docking when holding Shift only (allow to drop in wider space, reduce visual noise)"_RU >> u8"Включить стыковку только при удерживании Shift (позволяет падать в более широком пространстве, уменьшает визуальный шум)");
                ImGui::Checkbox("io.ConfigDockingAlwaysTabBar", &io.ConfigDockingAlwaysTabBar);
                ImGui::SameLine();
                HelpMarker("Create a docking node and tab-bar on single floating windows."_RU >> u8"Создайте стыковочный узел и панель вкладок в отдельных плавающих окнах.");
                ImGui::Checkbox("io.ConfigDockingTransparentPayload", &io.ConfigDockingTransparentPayload);
                ImGui::SameLine();
                HelpMarker("Make window or viewport transparent when docking and only display docking boxes on the target viewport. Useful if rendering of multiple viewport cannot be synced. Best used with ConfigViewportsNoAutoMerge."_RU >>
                    u8"Сделайте окно или область просмотра прозрачными при стыковке и отображайте стыковочные блоки только в целевом окне просмотра. Полезно, если рендеринг нескольких видовых экранов не может быть синхронизирован. Лучше всего использовать с Config Viewports No Auto Merge.");
                ImGui::Unindent();
            }

            ImGui::CheckboxFlags("io.ConfigFlags: ViewportsEnable", &io.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);
            ImGui::SameLine();
            HelpMarker("[beta] Enable beta multi-viewports support. See ImGuiPlatformIO for details."_RU >> u8"[бета] Включить бета-версию поддержки нескольких окон просмотра. Подробнее см. в разделе ImGuiPlatformIO.");
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::Indent();
                ImGui::Checkbox("io.ConfigViewportsNoAutoMerge", &io.ConfigViewportsNoAutoMerge);
                ImGui::SameLine();
                HelpMarker("Set to make all floating imgui windows always create their own viewport. Otherwise, they are merged into the main host viewports when overlapping it."_RU >> u8"Установите, чтобы все плавающие окна imgui всегда создавали свои собственные области просмотра. В противном случае они сливаются с основным окном просмотра хоста при его наложении.");
                ImGui::Checkbox("io.ConfigViewportsNoTaskBarIcon", &io.ConfigViewportsNoTaskBarIcon);
                ImGui::SameLine();
                HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the task bar icon state right away)."_RU >> u8"Переключение этого во время выполнения обычно не поддерживается (большинство серверных программ платформы не обновляют состояние значка на панели задач сразу).");
                ImGui::Checkbox("io.ConfigViewportsNoDecoration", &io.ConfigViewportsNoDecoration);
                ImGui::SameLine();
                HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the decoration right away)."_RU >> u8"Переключение этого во время выполнения обычно не поддерживается (большинство серверных частей платформы не обновляют оформление сразу).");
                ImGui::Checkbox("io.ConfigViewportsNoDefaultParent", &io.ConfigViewportsNoDefaultParent);
                ImGui::SameLine();
                HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the parenting right away)."_RU >> u8"Переключение этого во время выполнения обычно не поддерживается (большинство серверных частей платформы не сразу обновляют родительский элемент).");
                ImGui::Unindent();
            }

            ImGui::Checkbox("io.ConfigInputTrickleEventQueue", &io.ConfigInputTrickleEventQueue);
            ImGui::SameLine();
            HelpMarker("Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates."_RU >> u8"Включите просачивание очереди ввода: некоторые типы событий, отправленные в течение одного кадра (например, нажатие кнопки + вверх), будут распределены по нескольким кадрам, улучшая взаимодействие с низкой частотой кадров.");
            ImGui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
            ImGui::SameLine();
            HelpMarker("Enable blinking cursor (optional as some users consider it to be distracting)."_RU >> u8"Включить мигающий курсор (необязательно, так как некоторые пользователи считают, что это отвлекает).");
            ImGui::Checkbox("io.ConfigDragClickToInputText", &io.ConfigDragClickToInputText);
            ImGui::SameLine();
            HelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving)."_RU >> u8"Включите преобразование виджетов Drag XXX в текстовый ввод простым щелчком мыши (без перемещения).");
            ImGui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
            ImGui::SameLine();
            HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback."_RU >> u8"Включить изменение размера окон от их краев и от нижнего левого угла.\n Для этого требуется (io.Backend Flags & Im Gui Backend Flags Has Mouse Cursors), поскольку для этого требуется обратная связь с курсором мыши.");
            ImGui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
            ImGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
            ImGui::SameLine();
            HelpMarker("Instruct Dear ImGui to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something)."_RU >>
                u8"Проинструктируйте Dear ImGui, чтобы он сам отображал курсор мыши. Обратите внимание, что курсор мыши, отображаемый с помощью пути рендеринга графического процессора вашего приложения, будет ощущаться более медленным, чем аппаратный курсор, но будет более синхронизирован с другими вашими визуальными эффектами.\n\n Некоторые настольные приложения могут использовать оба типа курсоров (например, включить только программный курсор). при изменении размера/перетаскивании чего-либо).");
            ImGui::Text("Also see Style->Rendering for rendering options."_RU >> u8"Также см. Style->Rendering для параметров рендеринга.");
            ImGui::TreePop();
            ImGui::Separator();
        }

        IMGUI_DEMO_MARKER("Configuration/Backend Flags"_RU >> u8"Флаги конфигурации/серверной части");
        if (ImGui::TreeNode("Backend Flags"_RU >> u8"Флаги серверной части"))
        {
            HelpMarker("Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\n"
                       "Here we expose them as read-only fields to avoid breaking interactions with your backend."_RU >>
                u8"Эти флаги устанавливаются бэкендами (файлы imgui impl xxx), чтобы указать их возможности..\n"
                "Здесь мы предоставляем их как поля только для чтения, чтобы избежать нарушения взаимодействия с вашим бэкэндом..");

            // Сделайте локальную копию, чтобы избежать изменения фактических флагов бэкэнда.
            // FIXME: мы не используем BeginDisabled(), чтобы метка оставалась яркой, возможно, нам нужен эквивалент BeginReadonly().
            ImGuiBackendFlags backend_flags = io.BackendFlags;
            ImGui::CheckboxFlags("io.BackendFlags: HasGamepad", &backend_flags, ImGuiBackendFlags_HasGamepad);
            ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", &backend_flags, ImGuiBackendFlags_HasMouseCursors);
            ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos", &backend_flags, ImGuiBackendFlags_HasSetMousePos);
            ImGui::CheckboxFlags("io.BackendFlags: PlatformHasViewports", &backend_flags, ImGuiBackendFlags_PlatformHasViewports);
            ImGui::CheckboxFlags("io.BackendFlags: HasMouseHoveredViewport", &backend_flags, ImGuiBackendFlags_HasMouseHoveredViewport);
            ImGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", &backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
            ImGui::CheckboxFlags("io.BackendFlags: RendererHasViewports", &backend_flags, ImGuiBackendFlags_RendererHasViewports);
            ImGui::TreePop();
            ImGui::Separator();
        }

        IMGUI_DEMO_MARKER("Configuration/Style"_RU >> u8"Конфигурация/Стиль");
        if (ImGui::TreeNode("Style"_RU >> u8"Стиль"))
        {
            HelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function."_RU >> u8"Доступ к тому же содержимому можно получить в «Инструменты->Редактор стилей» или вызовом функции «Показать редактор стилей().");
            ImGui::ShowStyleEditor();
            ImGui::TreePop();
            ImGui::Separator();
        }

        IMGUI_DEMO_MARKER("Configuration/Capture, Logging"_RU >> u8"Конфигурация/Захват, Регистрация");
        if (ImGui::TreeNode("Capture/Logging"_RU >> u8"Захват/Регистрация"))
        {
            HelpMarker("The logging API redirects all text output so you can easily capture the content of "
                       "a window or a block. Tree nodes can be automatically expanded.\n"
                       "Try opening any of the contents below in this window and then click one of the \"Log To\" button."_RU >>
                u8"Регистрация API перенаправляет весь вывод текста, чтобы вы могли легко фиксировать содержимое "
                "окно или блок. Узлы дерева могут быть автоматически развернуты.\n"
                "Попробуйте открыть любое содержимое ниже в этом окне, а затем нажмите одну из кнопок \"Log To\" button.");
            ImGui::LogButtons();

            HelpMarker("You can also call ImGui::LogText() to output directly to the log without a visual output."_RU >> u8"Вы также можете вызвать ImGui::LogText() для вывода непосредственно в журнал без визуального вывода.");
            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"_RU >> u8"Скопировать \"Hello, world!\" в буфер обмена"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!"_RU >> u8"Привет, мир!");
                ImGui::LogFinish();
            }
            ImGui::TreePop();
        }
    }

    IMGUI_DEMO_MARKER("Window options"_RU >> u8"Окно опций");
    if (ImGui::CollapsingHeader("Window options"_RU >> u8"Окно опций"))
    {
        if (ImGui::BeginTable("split", 3))
        {
            ImGui::TableNextColumn();
            ImGui::Checkbox("No titlebar"_RU >> u8"Без заголовка", &no_titlebar);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No scrollbar"_RU >> u8"Без полосы прокрутки", &no_scrollbar);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No menu"_RU >> u8"Без меню", &no_menu);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No move"_RU >> u8"Нельзя двигать", &no_move);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No resize"_RU >> u8"Нельзя измененить размер", &no_resize);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No collapse"_RU >> u8"Без коллапса", &no_collapse);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No close"_RU >> u8"Нельзя закрыть", &no_close);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No nav"_RU >> u8"Нет навигации", &no_nav);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No background"_RU >> u8"Без фона", &no_background);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No bring to front"_RU >> u8"Нельзя на передний план", &no_bring_to_front);
            ImGui::TableNextColumn();
            ImGui::Checkbox("No docking"_RU >> u8"Нет стыковки", &no_docking);
            ImGui::TableNextColumn();
            ImGui::Checkbox("Unsaved document"_RU >> u8"Без сохранения документа", &unsaved_document);
            ImGui::EndTable();
        }
    }

    // Все демонстрационное содержимое
    ShowDemoWindowWidgets();
    ShowDemoWindowLayout();
    ShowDemoWindowPopups();
    ShowDemoWindowTables();
    ShowDemoWindowMisc();

    // Конец ShowDemoWindow()
    ImGui::PopItemWidth();
    ImGui::End();
}

static void ShowDemoWindowWidgets()
{
    IMGUI_DEMO_MARKER("Widgets"_RU >> u8"Виджеты");
    if (!ImGui::CollapsingHeader("Widgets"_RU >> u8"Виджеты"))
        return;

    static bool disable_all = false;   // Флажок для этого находится в разделе «Отключено» внизу.
    if (disable_all)
        ImGui::BeginDisabled();

    IMGUI_DEMO_MARKER("Widgets/Basic"_RU >> u8"Виджеты/Basic");
    if (ImGui::TreeNode("Basic"_RU >> u8"Базовый"))
    {
        IMGUI_DEMO_MARKER("Widgets/Basic/Button"_RU >> u8"Виджеты/Базовый/Кнопка");
        static int clicked = 0;
        if (ImGui::Button("Button"_RU >> u8"Кнопка"))
            clicked++;
        if (clicked & 1)
        {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!"_RU >> u8"Спасибо, что нажали на меня!");
        }

        IMGUI_DEMO_MARKER("Widgets/Basic/Checkbox"_RU >> u8"Виджеты/Базовый/Флажок");
        static bool check = true;
        ImGui::Checkbox("checkbox"_RU >> u8"флажок", &check);

        IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton"_RU >> u8"Виджеты/Базовый/Переключатель");
        static int e = 0;
        ImGui::RadioButton("radio a"_RU >> u8"Переключатель a", &e, 0);
        ImGui::SameLine();
        ImGui::RadioButton("radio b"_RU >> u8"Переключатель b", &e, 1);
        ImGui::SameLine();
        ImGui::RadioButton("radio c"_RU >> u8"Переключатель c", &e, 2);

        // Цветные кнопки, демонстрация использования PushID() для добавления уникального идентификатора в стек идентификаторов и изменения стиля.
        IMGUI_DEMO_MARKER("Widgets/Basic/Buttons (Colored)"_RU >> u8"Виджеты/Базовый/Кнопки (Цветной)");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
            ImGui::Button("Click"_RU >> u8"Нажать");
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }

        // Используйте AlignTextToFramePadding(), чтобы выровнять базовую линию текста по базовой линии элементов виджета в рамке.
        // (иначе в последовательности Text+SameLine+Button текст по умолчанию будет немного выше!)
        // Подробности см. в разделе «Демо->Макет->Выравнивание базовой линии текста»('Demo->Layout->Text Baseline Alignment').
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Hold to repeat:"_RU >> u8"Удерживайте, чтобы повторить:");
        ImGui::SameLine();

        // Кнопки со стрелками с повторителем
        IMGUI_DEMO_MARKER("Widgets/Basic/Buttons (Repeating)"_RU >> u8"Виджеты/Базовый/Кнопки (Повторение)");
        static int counter = 0;
        float      spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left"_RU >> u8"##левый", ImGuiDir_Left))
        {
            counter--;
        }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right"_RU >> u8"##правый", ImGuiDir_Right))
        {
            counter++;
        }
        ImGui::PopButtonRepeat();
        ImGui::SameLine();
        ImGui::Text("%d", counter);

        IMGUI_DEMO_MARKER("Widgets/Basic/Tooltips"_RU >> u8"Виджеты/Базовый/Подсказки");
        ImGui::Text("Hover over me"_RU >> u8"Наведите курсор на меня");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("I am a tooltip"_RU >> u8"я подсказка");

        ImGui::SameLine();
        ImGui::Text("- or me"_RU >> u8"- или я");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("I am a fancy tooltip"_RU >> u8"Я причудливая подсказка");
            static float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
            ImGui::PlotLines("Curve"_RU >> u8"Изгиб", arr, IM_ARRAYSIZE(arr));
            ImGui::EndTooltip();
        }

        ImGui::Separator();
        ImGui::LabelText("label"_RU >> u8"Ярлык", "Value"_RU >> u8"Значение");

        {
            // Использование упрощенного однострочного API Combo() здесь
            // В разделе «Combo» приведены примеры использования более гибкого API BeginCombo()/EndCombo().
            IMGUI_DEMO_MARKER("Widgets/Basic/Combo"_RU >> u8"Виджеты/Базовый/Комбо");
            const char* items[]      = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK"};
            static int  item_current = 0;
            ImGui::Combo("combo"_RU >> u8"комбо", &item_current, items, IM_ARRAYSIZE(items));
            ImGui::SameLine();
            HelpMarker("Using the simplified one-liner Combo API here.\nRefer to the \"Combo\" section below for an explanation of how to use the more flexible and general BeginCombo/EndCombo API."_RU >> u8"Здесь используется упрощенный однострочный Combo API.\n Обратитесь к разделу \"Комбо\" ниже, чтобы узнать, как использовать более гибкий и общий API BeginCombo/EndCombo.");
        }

        {
            // Чтобы связать InputText() с std::string или любым другим пользовательским типом строки,
            // см. раздел «Ввод текста > Обратный вызов изменения размера» этой демонстрации и файл misc/cpp/imgui stdlib.h.
            IMGUI_DEMO_MARKER("Widgets/Basic/InputText"_RU >> u8"Виджеты/Базовый/InputText");
            static char str0[128] = "Hello, world!";
            ImGui::InputText("input text"_RU >> u8"ввод текста", str0, IM_ARRAYSIZE(str0));
            ImGui::SameLine();
            HelpMarker("USER:\n"
                       "Hold SHIFT or use mouse to select text.\n"
                       "CTRL+Left/Right to word jump.\n"
                       "CTRL+A or double-click to select all.\n"
                       "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                       "CTRL+Z,CTRL+Y undo/redo.\n"
                       "ESCAPE to revert.\n\n"
                       "PROGRAMMER:\n"
                       "You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputText() "
                       "to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated "
                       "in imgui_demo.cpp)."_RU >>
                u8"ПОЛЬЗОВАТЕЛЬ:\n"
                "Удерживайте SHIFT или используйте мышь, чтобы выделить текст.\n"
                "CTRL+Влево/Вправо для перехода к слову.\n"
                "CTRL+A или двойной щелчок, чтобы выбрать все.\n"
                "CTRL+X,CTRL+C,CTRL+V буфер обмена.\n"
                "CTRL+Z,CTRL+Y отменить/повторить.\n"
                "ESCAPE, чтобы вернуться.\n\n"
                "ПРОГРАММИСТ:\n"
                "Вы можете использовать средство ImGuiInputTextFlags_CallbackResize, если вам нужно подключить InputText() "
                "к типу динамической строки. Для примера см. misc/cpp/imgui stdlib.h (это не продемонстрировано "
                "в imgui_demo.cpp).");

            static char str1[128] = "";
            ImGui::InputTextWithHint("input text (w/ hint)"_RU >> u8"ввод текста (w/ hint)", "enter text here"_RU >> u8"введите текст здесь", str1, IM_ARRAYSIZE(str1));

            IMGUI_DEMO_MARKER("Widgets/Basic/InputInt, InputFloat"_RU >> u8"Виджеты/Базовый/InputInt, InputFloat");
            static int i0 = 123;
            ImGui::InputInt("input int"_RU >> u8"ввод int", &i0);

            static float f0 = 0.001f;
            ImGui::InputFloat("input float"_RU >> u8"ввод float", &f0, 0.01f, 1.0f, "%.3f");

            static double d0 = 999999.00000001;
            ImGui::InputDouble("input double"_RU >> u8"ввод double", &d0, 0.01f, 1.0f, "%.8f");

            static float f1 = 1.e10f;
            ImGui::InputFloat("input scientific"_RU >> u8"ввод scientific", &f1, 0.0f, 0.0f, "%e");
            ImGui::SameLine();
            HelpMarker("You can input value using the scientific notation,\n"
                       "  e.g. \"1e+8\" becomes \"100000000\"."_RU >>
                u8"Вы можете ввести значение, используя экспоненциальное представление,\n"
                "  e.g. \"1e+8\" becomes \"100000000\".");

            static float vec4a[4] = {0.10f, 0.20f, 0.30f, 0.44f};
            ImGui::InputFloat3("input float3"_RU >> u8"ввод float3", vec4a);
        }

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/DragInt, DragFloat"_RU >> u8"Виджеты/Базовый/DragInt, DragFloat");
            static int i1 = 50, i2 = 42;
            ImGui::DragInt("drag int"_RU >> u8"drag int", &i1, 1);
            ImGui::SameLine();
            HelpMarker("Click and drag to edit value.\n"
                       "Hold SHIFT/ALT for faster/slower edit.\n"
                       "Double-click or CTRL+click to input value."_RU >>
                u8"Нажмите и перетащите, чтобы изменить значение.\n"
                "Удерживайте SHIFT/ALT для более быстрого/медленного редактирования.\n"
                "Двойной щелчок или CTRL+щелчок для ввода значения.");

            ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

            static float f1 = 1.00f, f2 = 0.0067f;
            ImGui::DragFloat("drag float", &f1, 0.005f);
            ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
        }

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/SliderInt, SliderFloat"_RU >> u8"Виджеты/Базовый/SliderInt, SliderFloat");
            static int i1 = 0;
            ImGui::SliderInt("slider int", &i1, -1, 3);
            ImGui::SameLine();
            HelpMarker("CTRL+click to input value."_RU >> u8"CTRL+щелчок, чтобы ввести значение.");

            static float f1 = 0.123f, f2 = 0.0f;
            ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);

            IMGUI_DEMO_MARKER("Widgets/Basic/SliderAngle"_RU >> u8"Виджеты/Базовый/SliderAngle");
            static float angle = 0.0f;
            ImGui::SliderAngle("slider angle"_RU >> u8"ползунок угол", &angle);

            // Использование строки формата для отображения имени вместо целого числа.
            // Здесь мы полностью опускаем «%d» из строки формата, поэтому будет отображаться только имя.
            // Эту технику также можно использовать с функцией DragInt().
            IMGUI_DEMO_MARKER("Widgets/Basic/Slider (enum)"_RU >> u8"Виджеты/Базовый/Слайдер (enum)");
            enum Element
            {
                Element_Fire,
                Element_Earth,
                Element_Air,
                Element_Water,
                Element_COUNT
            };
            static int  elem                       = Element_Fire;
            const char* elems_names[Element_COUNT] = {"Fire"_RU >> u8"Огонь", "Earth"_RU >> u8"Земля", "Air"_RU >> u8"Воздух", "Water"_RU >> u8"Вода"};
            const char* elem_name                  = (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown"_RU >> u8"Неизвестный";
            ImGui::SliderInt("slider enum"_RU >> u8"слайдер перечисление", &elem, 0, Element_COUNT - 1, elem_name);
            ImGui::SameLine();
            HelpMarker("Using the format string parameter to display a name instead of the underlying integer."_RU >> u8"Использование параметра строки формата для отображения имени вместо базового целого числа.");
        }

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/ColorEdit3, ColorEdit4"_RU >> u8"Виджеты/Базовый/ColorEdit3, ColorEdit4");
            static float col1[3] = {1.0f, 0.0f, 0.2f};
            static float col2[4] = {0.4f, 0.7f, 0.0f, 0.5f};
            ImGui::ColorEdit3("цвет 1", col1);
            ImGui::SameLine();
            HelpMarker("Click on the color square to open a color picker.\n"
                       "Click and hold to use drag and drop.\n"
                       "Right-click on the color square to show options.\n"
                       "CTRL+click on individual component to input value.\n"_RU >>
                u8"Нажмите на цветной квадрат, чтобы открыть палитру цветов..\n"
                "Нажмите и удерживайте, чтобы использовать перетаскивание.\n"
                "Щелкните правой кнопкой мыши цветной квадрат, чтобы отобразить параметры..\n"
                "CTRL+щелчок по отдельному компоненту для ввода значения.\n");

            ImGui::ColorEdit4("color 2"_RU >> u8"цвет 2", col2);
        }

        {
            // Используя _simplified_ один лайнер ListBox() API здесь
            // См. раздел «Списки» для примеров того, как использовать более гибкие API BeginListBox()/EndListBox().
            IMGUI_DEMO_MARKER("Widgets/Basic/ListBox"_RU >> u8"Виджеты/Базовый/ListBox");
            const char* items[]      = {"Apple"_RU >> u8"Яблоко", "Banana"_RU >> u8"Банан", "Cherry"_RU >> u8"вишня", "Kiwi"_RU >> u8"киви", "Mango"_RU >> u8"манго", "Orange"_RU >> u8"Апельсин", "Pineapple"_RU >> u8"Ананас", "Strawberry"_RU >> u8"Клубника", "Watermelon"_RU >> u8"Арбуз"};
            static int  item_current = 1;
            ImGui::ListBox("listbox"_RU >> u8"списки", &item_current, items, IM_ARRAYSIZE(items), 4);
            ImGui::SameLine();
            HelpMarker("Using the simplified one-liner ListBox API here.\nRefer to the \"List boxes\" section below for an explanation of how to use the more flexible and general BeginListBox/EndListBox API."_RU >> u8"Здесь используется упрощенный однострочный API-интерфейс ListBox.\n Обратитесь к разделу \"ListBox\" ниже, чтобы узнать, как использовать более гибкий и общий API-интерфейс Begin ListBox/EndList Box.");
        }

        ImGui::TreePop();
    }

    // Тестирование ImGuiOnceUponAFrame помощник.
    //static ImGuiOnceUponAFrame once;
    //for (int i = 0; i < 5; i++)
    //    if (once)
    //        ImGui::Text("Это будет отображаться только один раз.");

    IMGUI_DEMO_MARKER("Widgets/Trees"_RU >> u8"Виджеты/Деревья");
    if (ImGui::TreeNode("Trees"_RU >> u8"Деревья"))
    {
        IMGUI_DEMO_MARKER("Widgets/Trees/Basic trees"_RU >> u8"Виджеты/Деревья/Базовые деревья");
        if (ImGui::TreeNode("Basic trees"_RU >> u8"Базовые деревья"))
        {
            for (int i = 0; i < 5; i++)
            {
                // Используйте SetNextItemOpen(), чтобы установить состояние узла по умолчанию как открытое. Мы могли бы
                // также используйте TreeNodeEx() с флагом ImGuiTreeNodeFlags_DefaultOpen для достижения того же результата!
                if (i == 0)
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d"_RU >> u8"Дочерний %d", i))
                {
                    ImGui::Text("blah blah"_RU >> u8"Бла Бла");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("button"_RU >> u8"кнопка"))
                    {}
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Trees/Advanced, with Selectable nodes"_RU >> u8"Виджеты/Деревья/Передовой, с выбираемыми узлами");
        if (ImGui::TreeNode("Advanced, with Selectable nodes"_RU >> u8"Расширенный, с выбираемыми узлами"))
        {
            HelpMarker("This is a more typical looking tree with selectable nodes.\n"
                       "Click to select, CTRL+Click to toggle, click on arrows or double-click to open."_RU >>
                u8"Это более типичное дерево с выбираемыми узлами..\n"
                "Нажмите, чтобы выбрать, CTRL+щелчок, чтобы переключить, нажмите на стрелки или дважды щелкните, чтобы открыть.");
            static ImGuiTreeNodeFlags base_flags                          = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            static bool               align_label_with_current_x_position = false;
            static bool               test_drag_and_drop                  = false;
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", &base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &base_flags, ImGuiTreeNodeFlags_SpanAvailWidth);
            ImGui::SameLine();
            HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node."_RU >> u8"Расширьте область попадания на всю доступную ширину вместо того, чтобы позволять размещать больше элементов после узла.");
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::Checkbox("Align label with current X position"_RU >> u8"Выровнять метку с текущей позицией X", &align_label_with_current_x_position);
            ImGui::Checkbox("Test tree node as drag source"_RU >> u8"Проверить узел дерева как источник перетаскивания", &test_drag_and_drop);
            ImGui::Text("Hello!"_RU >> u8"Привет!");
            if (align_label_with_current_x_position)
                ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

            // 'selection_mask' - это немое представление того, что может быть состоянием выбора на стороне пользователя.
            //  Вы можете сохранить состояние выделения внутри или снаружи ваших объектов в любом формате, который сочтете нужным.
            // 'node_clicked' - это временное хранилище того, на какой узел мы нажали, чтобы обработать выбор в конце
            /// из цикла. Может быть указателем на ваш собственный тип узла и т.д.
            static int selection_mask = (1 << 2);
            int        node_clicked   = -1;
            for (int i = 0; i < 6; i++)
            {
                // Отключите по умолчанию "открывать одним щелчком мыши" + установите выбранный флаг в соответствии с нашим выбором.
                // Чтобы изменить выделение, мы используем IsItemClicked() && !IsItemToggledOpen(), поэтому нажатие на стрелку не изменяет выделение.
                ImGuiTreeNodeFlags node_flags  = base_flags;
                const bool         is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                if (i < 3)
                {
                    // Элементы 0..2 являются узлом дерева
                    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags,
                        "Selectable Node %d"
                        "Selectable Node %d"_RU >>
                            u8"Выбираемый узел %d"
                            "Выбираемый узел %d",
                        i);
                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && ImGui::BeginDragDropSource())
                    {
                        ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                        ImGui::Text("This is a drag and drop source"_RU >> u8"Это источник перетаскивания");
                        ImGui::EndDragDropSource();
                    }
                    if (node_open)
                    {
                        ImGui::BulletText("Blah blah\nBlah Blah"_RU >> u8"Бла-бла\nБла-бла");
                        ImGui::TreePop();
                    }
                }
                else
                {
                    // Элементы 3..5 - это листья деревьев
                    // Единственная причина, по которой мы вообще используем TreeNode, - это разрешить выделение листа. В противном случае мы можем
                    // используйте BulletText() или переместите курсор с помощью GetTreeNodeToLabelSpacing() и вызовите Text().
                    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;   // ImGuiTreeNodeFlags_Bullet
                    ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d"_RU >> u8"Выбираемый лист %d", i);
                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && ImGui::BeginDragDropSource())
                    {
                        ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                        ImGui::Text("This is a drag and drop source"_RU >> u8"Это источник перетаскивания");
                        ImGui::EndDragDropSource();
                    }
                }
            }
            if (node_clicked != -1)
            {
                // Обновить состояние выбора
                // (обрабатывать вне цикла дерева, чтобы избежать визуальных несоответствий во время щелчка кадра)
                if (ImGui::GetIO().KeyCtrl)
                    selection_mask ^= (1 << node_clicked);   // CTRL+нажмите, чтобы переключить
                else                                         //if (!(selection_mask & (1 << node_clicked))) // В зависимости от желаемого поведения выбора может потребоваться сохранить выбор при нажатии на элемент, который является частью выбора.
                    selection_mask = (1 << node_clicked);    // Нажмите, чтобы выбрать один
            }
            if (align_label_with_current_x_position)
                ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Collapsing Headers"_RU >> u8"Виджеты/Сворачивание заголовков");
    if (ImGui::TreeNode("Collapsing Headers"_RU >> u8"Сворачивание заголовков"))
    {
        static bool closable_group = true;
        ImGui::Checkbox("Show 2nd header"_RU >> u8"Показать второй заголовок", &closable_group);
        if (ImGui::CollapsingHeader("Header"_RU >> u8"Заголовок", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("Some content %d"_RU >> u8"Некоторый контент %d", i);
        }
        if (ImGui::CollapsingHeader("Header with a close button"_RU >> u8"Заголовок с кнопкой закрытия", &closable_group))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("More content %d"_RU >> u8"Больше контента %d", i);
        }

        if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Bullets"_RU >> u8"Виджеты/Bullets");
    if (ImGui::TreeNode("Bullets"))
    {
        ImGui::BulletText("Bullet point 1"_RU >> u8"Bullet точка 1");
        ImGui::BulletText("Bullet point 2\nOn multiple lines"_RU >> u8"Bullet точка 2\nНа нескольких линиях");
        if (ImGui::TreeNode("Tree node"_RU >> u8"Узел дерева"))
        {
            ImGui::BulletText("Another bullet point"_RU >> u8"Еще один пункт");
            ImGui::TreePop();
        }
        ImGui::Bullet();
        ImGui::Text("Bullet point 3 (two calls)"_RU >> u8"Пункт 3 (два звонка)");
        ImGui::Bullet();
        ImGui::SmallButton("Button"_RU >> u8"Кнопка");
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Text"_RU >> u8"Виджеты/Текст");
    if (ImGui::TreeNode("Text"_RU >> u8"Текст"))
    {
        IMGUI_DEMO_MARKER("Widgets/Text/Colored Text"_RU >> u8"Виджеты/Текст/Цветной Текст");
        if (ImGui::TreeNode("Colorful Text"_RU >> u8"Красочный Текст"))
        {
            // Использование ярлыка. Вы можете использовать PushStyleColor()/PopStyleColor() для большей гибкости.
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Pink"_RU >> u8"Розовый");
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow"_RU >> u8"Желтый");
            ImGui::TextDisabled("Disabled"_RU >> u8"Выключенный");
            ImGui::SameLine();
            HelpMarker("The TextDisabled color is stored in ImGuiStyle."_RU >> u8"Цвет отключенного текста(TextDisabled) сохраняется в ImGuiStyle.");
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text/Word Wrapping"_RU >> u8"Виджеты/Текст/Перенос слов");
        if (ImGui::TreeNode("Word Wrapping"_RU >> u8"Перенос слов"))
        {
            // Использование ярлыка. Вы можете использовать PushTextWrapPos()/PopTextWrapPos() для большей гибкости.
            ImGui::TextWrapped("This text should automatically wrap on the edge of the window. The current implementation "
                               "for text wrapping follows simple rules suitable for English and possibly other languages."_RU >>
                u8"Этот текст должен автоматически переноситься на край окна. Текущая реализация "
                "для переноса текста следует простым правилам, подходящим для английского и, возможно, других языков.");
            ImGui::Spacing();

            static float wrap_width = 200.0f;
            ImGui::SliderFloat("Wrap width"_RU >> u8"Ширина обертывания", &wrap_width, -20, 600, "%.0f");

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            for (int n = 0; n < 2; n++)
            {
                ImGui::Text("Test paragraph %d:"_RU >> u8"Тестовый абзац %d:", n);
                ImVec2 pos        = ImGui::GetCursorScreenPos();
                ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
                ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                if (n == 0)
                    ImGui::Text("The lazy dog is a good dog. This paragraph should fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog."_RU >> u8"Ленивая собака — хорошая собака. Этот абзац должен умещаться в пределах %.0f пикселей. Проверка слова из 1 символа. Быстрая коричневая лиса прыгает через ленивую собаку.", wrap_width);
                else
                    ImGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");

                // Нарисуйте фактическую ограничивающую рамку текста, следуя маркером нашего ожидаемого предела (не должны перекрываться!)
                draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
                draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
                ImGui::PopTextWrapPos();
            }

            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text/UTF-8 Text"_RU >> u8"Виджеты/Текст/UTF-8 Текст");
        if (ImGui::TreeNode("UTF-8 Text"_RU >> u8"UTF-8 Текст"))
        {
            // UTF-8 тест с японскими иероглифами
            // (Нужен подходящий шрифт? Попробуйте "Google Noto" или "Arial Unicode". Видеть docs/FONTS.md для подробностей.)
            // - Из С++ 11 вы можете использовать синтаксис u8 "мой текст" для кодирования литеральных строк как UTF-8
            // - Для более раннего компилятора вы можете закодировать свои исходные коды как UTF-8 (например, в Visual Studio вы можете
            //   можете сохранить ваши исходные файлы как «UTF-8 без подписи»).
            // - ТОЛЬКО ДЛЯ ЭТОГО ДЕМО-ФАЙЛА, ПОТОМУ ЧТО МЫ ХОТИМ ПОДДЕРЖАТЬ СТАРЫЕ КОМПИЛЯТОРЫ, МЫ *НЕ* ВКЛЮЧАЕМ RAW UTF-8
            //   ПЕРСОНАЖИ В ЭТОМ ИСХОДНОМ ФАЙЛЕ. Вместо этого мы кодируем несколько строк шестнадцатеричными константами.
            //   Не делайте этого в своем приложении! Пожалуйста, используйте u8"текст на любом языке" в своем приложении!
            //   Обратите внимание, что значения символов сохраняются даже с помощью InputText(), если шрифт не может быть отображен,
            //   так что вы можете безопасно копировать и вставлять искаженные символы в другое приложение.
            ImGui::TextWrapped("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. "
                               "Call io.Fonts->AddFontFromFileTTF() manually to load extra character ranges. "
                               "Read docs/FONTS.md for details."_RU >>
                u8"Текст CJK будет отображаться только в том случае, если шрифт был загружен с соответствующими диапазонами символов CJK.. "
                "Вызов io.Fonts->AddFontFromFileTTF() вручную, чтобы загрузить дополнительные диапазоны символов. "
                "Читайте docs/FONTS.md для подробностей.");
            ImGui::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)");   // Обычно мы использовали бы u8 "бла-бла" с соответствующими символами непосредственно в строке.
            ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
            //static char buf[32] = u8"NIHONGO"; // <- вот как бы вы написали это на С++ 11, используя настоящие кандзи
            ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Images"_RU >> u8"Виджеты/Изображения");
    if (ImGui::TreeNode("Images"_RU >> u8"Изображения"))
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). "
                           "Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. "
                           "Hover the texture for a zoomed view!"_RU >>
            u8"Ниже мы показываем текстуру шрифта (это единственная текстура, к которой у нас есть доступ в этой демонстрации). "
            "Используйте тип «ImTextureID» в качестве хранилища для передачи указателей или идентификаторов на ваши собственные данные текстуры. "
            "Наведите курсор на текстуру для увеличения!");

        // Ниже мы показываем текстуру шрифта, потому что это единственная текстура, к которой у нас есть доступ внутри демо!
        // Помните, что ImTextureID — это просто хранилище для всего, что вы хотите. По сути это значение, которое
        // будет передано серверной части рендеринга через структуру ImDrawCmd.
        // Если вы используете один из бэкэнда рендеринга imgui_impl_XXXX.cpp по умолчанию, все они имеют комментарии вверху
        // их соответствующего исходного файла, чтобы указать, что они ожидают сохранить в ImTextureID, например:
        // - Средство визуализации imgui_impl_dx11.cpp ожидает указатель 'ID3D11ShaderResourceView*'
        // - Рендерер imgui_impl_opengl3.cpp ожидает идентификатор текстуры GLuint OpenGL и т. д.
        // Более:
        // - Если вы решили, что ImTextureID = MyEngineTexture*, то вы можете передать указатели MyEngineTexture*
        // в ImGui::Image() и собирать ширину/высоту с помощью собственных функций и т. д.
        // - Вы можете использовать ShowMetricsWindow() для проверки данных отрисовки, которые передаются вашему рендереру,
        // это поможет вам отладить проблемы, если вы в этом запутались.
        // - Рассмотрите возможность использования низкоуровневого API ImDrawList::AddImage() через ImGui::GetWindowDrawList()->AddImage().
        // - Читать https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
        // - Читать https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
        ImTextureID my_tex_id = io.Fonts->TexID;
        float       my_tex_w  = (float)io.Fonts->TexWidth;
        float       my_tex_h  = (float)io.Fonts->TexHeight;
        {
            ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos        = ImGui::GetCursorScreenPos();
            ImVec2 uv_min     = ImVec2(0.0f, 0.0f);               // Верхний левый
            ImVec2 uv_max     = ImVec2(1.0f, 1.0f);               // Нижний правый
            ImVec4 tint_col   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // Нет оттенка
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);   // 50% непрозрачный белый
            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                float region_sz = 32.0f;
                float region_x  = io.MousePos.x - pos.x - region_sz * 0.5f;
                float region_y  = io.MousePos.y - pos.y - region_sz * 0.5f;
                float zoom      = 4.0f;
                if (region_x < 0.0f)
                {
                    region_x = 0.0f;
                }
                else if (region_x > my_tex_w - region_sz)
                {
                    region_x = my_tex_w - region_sz;
                }
                if (region_y < 0.0f)
                {
                    region_y = 0.0f;
                }
                else if (region_y > my_tex_h - region_sz)
                {
                    region_y = my_tex_h - region_sz;
                }
                ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                ImGui::EndTooltip();
            }
        }

        IMGUI_DEMO_MARKER("Widgets/Images/Textured buttons"_RU >> u8"Виджеты/Изображения/Текстурированные кнопки");
        ImGui::TextWrapped("And now some textured buttons.."_RU >> u8"А теперь немного текстурированных кнопок..");
        static int pressed_count = 0;
        for (int i = 0; i < 8; i++)
        {
            ImGui::PushID(i);
            int    frame_padding = -1 + i;                                       // -1 == использует заполнение по умолчанию (style.FramePadding)
            ImVec2 size          = ImVec2(32.0f, 32.0f);                         // Размер изображения, которое мы хотим сделать видимым
            ImVec2 uv0           = ImVec2(0.0f, 0.0f);                           // UV координаты нижнего левого
            ImVec2 uv1           = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);   // UV координаты (32,32) в нашей текстуре
            ImVec4 bg_col        = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);               // Черный фон
            ImVec4 tint_col      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);               // Нет оттенка
            if (ImGui::ImageButton(my_tex_id, size, uv0, uv1, frame_padding, bg_col, tint_col))
                pressed_count += 1;
            ImGui::PopID();
            ImGui::SameLine();
        }
        ImGui::NewLine();
        ImGui::Text("Pressed %d times."_RU >> u8"Нажато %d раз.", pressed_count);
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Combo"_RU >> u8"Виджеты/Комбо");
    if (ImGui::TreeNode("Combo"_RU >> u8"Комбо"))
    {
        // Выставлять флаги как чекбоксы для демо
        static ImGuiComboFlags flags = 0;
        ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);
        ImGui::SameLine();
        HelpMarker("Only makes a difference if the popup is larger than the combo"_RU >> u8"Имеет значение только в том случае, если всплывающее окно больше, чем комбо");
        if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
            flags &= ~ImGuiComboFlags_NoPreview;   // Очистите другой флаг, так как мы не можем объединить оба
        if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
            flags &= ~ImGuiComboFlags_NoArrowButton;   // Очистите другой флаг, так как мы не можем объединить оба

        // Используя универсальный API BeginCombo(), вы имеете полный контроль над тем, как отображать содержимое комбо.
        // (ваши данные выбора могут быть индексом, указателем на объект, идентификатором объекта, флагом навязчиво
        // хранится в самом объекте и т. д.)
        const char* items[]             = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO"};
        static int  item_current_idx    = 0;                         // Здесь мы сохраняем наши данные выбора в виде индекса.
        const char* combo_preview_value = items[item_current_idx];   // Передайте значение предварительного просмотра, видимое перед открытием комбо (это может быть что угодно)
        if (ImGui::BeginCombo("combo 1"_RU >> u8"комбо 1", combo_preview_value, flags))
        {
            static bool test_check = false;
            ImGui::Checkbox("test"_RU >> u8"тест", &test_check);
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;
                // Установите начальный фокус при открытии комбо (прокрутка + фокус навигации по клавиатуре)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Упрощенный однострочный API Combo(), использующий значения, упакованные в одну константную строку.
        // Это удобно, когда выборка мала и известна во время компиляции.
        static int item_current_2 = 0;
        ImGui::Combo("combo 2 (one-liner)"_RU >> u8"комбо 2 (однострочный)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        // Упрощенный однострочный Combo() с использованием массива const char*
        // Это не очень полезно (может быть устаревшим): лучше использовать BeginCombo()/EndCombo() для полного контроля.
        static int item_current_3 = -1;   // Если выбор не находится в пределах 0..count, Combo не будет отображать предварительный просмотр
        ImGui::Combo("combo 3 (array)"_RU >> u8"комбо 3 (масив)", &item_current_3, items, IM_ARRAYSIZE(items));

        // Упрощенный однострочный Combo() с использованием функции доступа
        struct Funcs
        {
            static bool ItemGetter(void* data, int n, const char** out_str)
            {
                *out_str = ((const char**)data)[n];
                return true;
            }
        };
        static int item_current_4 = 0;
        ImGui::Combo("combo 4 (function)"_RU >> u8"комбо 4 (функция)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/List Boxes"_RU >> u8"Виджеты/Список Боксов");
    if (ImGui::TreeNode("List boxes"_RU >> u8"Список Боксов"))
    {
        // Используя универсальный API BeginListBox(), вы имеете полный контроль над тем, как отображать содержимое списка.
        // (ваши данные выбора могут быть индексом, указателем на объект, идентификатором объекта, флагом навязчиво
        // хранится в самом объекте и т.д.)
        const char* items[]          = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO"};
        static int  item_current_idx = 0;   // Здесь мы сохраняем наши данные выбора в виде индекса.
        if (ImGui::BeginListBox("listbox 1"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Установите начальный фокус при открытии комбо (прокрутка + фокус навигации по клавиатуре)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        // Нестандартный размер: используйте всю ширину, 5 элементов в высоту
        ImGui::Text("Full-width:"_RU >> u8"Полная ширина:");
        if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Установите начальный фокус при открытии комбо (прокрутка + фокус навигации по клавиатуре)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Selectables"_RU >> u8"Виджеты/Выбираемые объекты");
    if (ImGui::TreeNode("Selectables"_RU >> u8"Выбираемые объекты"))
    {
        // Selectable() имеет 2 перегрузки:
        // - Тот, который принимает "bool selected" в качестве информации о выборе только для чтения.
        // Когда нажимается Selectable(), он возвращает true, и вы можете соответствующим образом изменить состояние выбора.
        // - тот, который принимает "bool* p selected" в качестве информации о выборе для чтения и записи (удобно в некоторых случаях)
        // Чем раньше, тем гибче, так как в реальном приложении ваш выбор может быть сохранен разными способами
        // и не обязательно внутри логического значения (например, во флагах внутри объектов, в качестве внешнего списка и т. д.).
        IMGUI_DEMO_MARKER("Widgets/Selectables/Basic"_RU >> u8"Виджеты/Выбираемые объекты/Базовый");
        if (ImGui::TreeNode("Basic"_RU >> u8"Базовый"))
        {
            static bool selection[5] = {false, true, false, false, false};
            ImGui::Selectable("1. I am selectable"_RU >> u8"1. меня можно выбрать", &selection[0]);
            ImGui::Selectable("2. I am selectable"_RU >> u8"2. меня можно выбрать", &selection[1]);
            ImGui::Text("(I am not selectable)"_RU >> u8"(меня нельзя выбрать)");
            ImGui::Selectable("4. I am selectable"_RU >> u8"4. меня можно выбрать", &selection[3]);
            if (ImGui::Selectable("5. I am double clickable"_RU >> u8"5. я кликабельный дважды", selection[4], ImGuiSelectableFlags_AllowDoubleClick))
                if (ImGui::IsMouseDoubleClicked(0))
                    selection[4] = !selection[4];
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection"_RU >> u8"Виджеты/Выбираемые объекты/Одиночный выбор");
        if (ImGui::TreeNode("Selection State: Single Selection"_RU >> u8"Состояние выбора: Одиночный выбор"))
        {
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d"_RU >> u8"Объект %d", n);
                if (ImGui::Selectable(buf, selected == n))
                    selected = n;
            }
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/Multiple Selection"_RU >> u8"Виджеты/Выбираемые объекты/Множественный выбор");
        if (ImGui::TreeNode("Selection State: Multiple Selection"_RU >> u8"Состояние выбора: Множественный выбор"))
        {
            HelpMarker("Hold CTRL and click to select multiple items."_RU >> u8"Удерживайте CTRL и щелкните, чтобы выбрать несколько элементов.");
            static bool selection[5] = {false, false, false, false, false};
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d"_RU >> u8"Объект %d", n);
                if (ImGui::Selectable(buf, selection[n]))
                {
                    if (!ImGui::GetIO().KeyCtrl)   // Очистить выделение, когда CTRL не удерживается
                        memset(selection, 0, sizeof(selection));
                    selection[n] ^= 1;
                }
            }
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/Rendering more text into the same line"_RU >> u8"Виджеты/Выбираемые объекты/Отображение большего количества текста в той же строке");
        if (ImGui::TreeNode("Rendering more text into the same line"_RU >> u8"Отображение большего количества текста в той же строке"))
        {
            // Используя переопределение Selectable(), которое принимает параметр "bool* p selected",
            // эта функция автоматически переключает ваше логическое значение.
            static bool selected[3] = {false, false, false};
            ImGui::Selectable("main.c", &selected[0]);
            ImGui::SameLine(300);
            ImGui::Text(" 2,345 bytes");
            ImGui::Selectable("Hello.cpp", &selected[1]);
            ImGui::SameLine(300);
            ImGui::Text("12,345 bytes");
            ImGui::Selectable("Hello.h", &selected[2]);
            ImGui::SameLine(300);
            ImGui::Text(" 2,345 bytes");
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/In columns"_RU >> u8"Виджеты/Выбираемые объекты/В столбцах");
        if (ImGui::TreeNode("In columns"_RU >> u8"В столбцах"))
        {
            static bool selected[10] = {};

            if (ImGui::BeginTable("split1", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d"_RU >> u8"Элемент %d", i);
                    ImGui::TableNextColumn();
                    ImGui::Selectable(label, &selected[i]);   // FIXME-TABLE: Перекрытие выбора
                }
                ImGui::EndTable();
            }
            ImGui::Spacing();
            if (ImGui::BeginTable("split2", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d"_RU >> u8"Элемент %d", i);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Selectable(label, &selected[i], ImGuiSelectableFlags_SpanAllColumns);
                    ImGui::TableNextColumn();
                    ImGui::Text("Some other contents"_RU >> u8"Некоторое другое содержимое");
                    ImGui::TableNextColumn();
                    ImGui::Text("123456");
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/Grid"_RU >> u8"Виджеты/Выбираемые объекты/Сетка");
        if (ImGui::TreeNode("Grid"_RU >> u8"Сетка"))
        {
            static char selected[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

            // Добавьте немного глупого веселья...
            const float time           = (float)ImGui::GetTime();
            const bool  winning_state  = memchr(selected, 0, sizeof(selected)) == NULL;   // Если все ячейки выделены...
            if (winning_state)
                ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f + 0.5f * cosf(time * 2.0f), 0.5f + 0.5f * sinf(time * 3.0f)));

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                {
                    if (x > 0)
                        ImGui::SameLine();
                    ImGui::PushID(y * 4 + x);
                    if (ImGui::Selectable("Sailor", selected[y][x] != 0, 0, ImVec2(50, 50)))
                    {
                        // Переключить щелкнутую ячейку + переключить соседей
                        selected[y][x] ^= 1;
                        if (x > 0)
                        {
                            selected[y][x - 1] ^= 1;
                        }
                        if (x < 3)
                        {
                            selected[y][x + 1] ^= 1;
                        }
                        if (y > 0)
                        {
                            selected[y - 1][x] ^= 1;
                        }
                        if (y < 3)
                        {
                            selected[y + 1][x] ^= 1;
                        }
                    }
                    ImGui::PopID();
                }

            if (winning_state)
                ImGui::PopStyleVar();
            ImGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Выбираемые объекты/Alignment"_RU >> u8"Виджеты/Выбираемые объекты/Выравнивание");
        if (ImGui::TreeNode("Alignment"_RU >> u8"Выравнивание"))
        {
            HelpMarker("By default, Selectables uses style.SelectableTextAlign but it can be overridden on a per-item "
                       "basis using PushStyleVar(). You'll probably want to always keep your default situation to "
                       "left-align otherwise it becomes difficult to layout multiple items on a same line"_RU >>
                u8"По умолчанию Selectables использует style.SelectableTextAlign, но его можно переопределить для каждого элемента."
                "основе использования PushStyleVar(). Вы, вероятно, захотите всегда сохранять ситуацию по умолчанию "
                "выровнять по левому краю, иначе будет сложно расположить несколько элементов в одной строке.");
            static bool selected[3 * 3] = {true, false, true, false, true, false, true, false, true};
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    ImVec2 alignment = ImVec2((float)x / 2.0f, (float)y / 2.0f);
                    char   name[32];
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    if (x > 0)
                        ImGui::SameLine();
                    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);
                    ImGui::Selectable(name, &selected[3 * y + x], ImGuiSelectableFlags_None, ImVec2(80, 80));
                    ImGui::PopStyleVar();
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    // Чтобы связать InputText() с std::string или любым другим пользовательским типом строки,
    // см. раздел «Ввод текста > Обратный вызов изменения размера» этой демонстрации и файл misc/cpp/imgui_stdlib.h.
    IMGUI_DEMO_MARKER("Widgets/Text Input"_RU >> u8"Виджеты/Ввод текста");
    if (ImGui::TreeNode("Text Input"_RU >> u8"Ввод текста"))
    {
        IMGUI_DEMO_MARKER("Widgets/Text Input/Multi-line Text Input"_RU >> u8"Widgets/Ввод текста/Многострочный ввод текста");
        if (ImGui::TreeNode("Multi-line Text Input"_RU >> u8"Многострочный ввод текста"))
        {
            // Примечание: здесь для простоты мы используем буфер фиксированного размера. Смотрите ImGuiInputTextFlags_CallbackResize
            // и код в misc/cpp/imgui_stdlib.h о том, как настроить InputText() для динамического изменения размера строк.
            static char                text[1024 * 16] = "/*\n"
                                                         " Ошибка Pentium F00F, сокращение от F0 0F C7 C8,\n"
                                                         " шестнадцатеричное кодирование одной ошибочной инструкции,\n"
                                                         " более формально, недопустимый операнд с заблокированным CMPXCHG8B\n"
                                                         " ошибка инструкции, является конструктивным недостатком в большинстве\n"
                                                         " Intel Pentium, Pentium MMX и Pentium OverDrive\n"
                                                         " процессоры (все в микроархитектуре P5).\n"
                                                         "*/\n\n"
                                                         "ярлык:\n"
                                                         "\tблокировка cmpxchg8b eax\n";

            static ImGuiInputTextFlags flags           = ImGuiInputTextFlags_AllowTabInput;
            HelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp because we don't want to include <string> in here)"_RU >>
                u8"Вы можете использовать ImGuiInputTextFlags_CallbackResize средство, если вам нужно подключить InputTextMultiline() к типу динамической строки. Для примера см. misc/cpp/imgui stdlib.h.. (Это не продемонстрировано в imgui_demo.cpp, потому что мы не хотим включать здесь <string>)");
            ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
            ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
            ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Filtered Text Input"_RU >> u8"Виджеты/Ввод Текста/Фильтрованный Ввод Текста");
        if (ImGui::TreeNode("Filtered Text Input"_RU >> u8"Фильтрованный Ввод Текста"))
        {
            struct TextFilters
            {
                // Возвращает 0 (проходит), если символ 'i' или 'm' или 'g' или 'u' или 'i'
                static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar))
                        return 0;
                    return 1;
                }
            };

            static char buf1[64] = "";
            ImGui::InputText("default"_RU >> u8"по умолчанию", buf1, 64);
            static char buf2[64] = "";
            ImGui::InputText("decimal"_RU >> u8"десятичная дробь", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
            static char buf3[64] = "";
            ImGui::InputText("hexadecimal"_RU >> u8"шестнадцатеричный", buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
            static char buf4[64] = "";
            ImGui::InputText("uppercase"_RU >> u8"верхний регистр", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
            static char buf5[64] = "";
            ImGui::InputText("no blank"_RU >> u8"нет пробела", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
            static char buf6[64] = "";
            ImGui::InputText("\"imgui\" letters"_RU >> u8"\"imgui\" буквы", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Password input");
        if (ImGui::TreeNode("Password Input"_RU >> u8"Ввод пароля"))
        {
            static char password[64] = "password123";
            ImGui::InputText("password"_RU >> u8"пароль", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            ImGui::SameLine();
            HelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n"_RU >> u8"Отображать все символы как'*'.\nОтключить вырезание и копирование буфера обмена.\nОтключить ведение журнала.\n");
            ImGui::InputTextWithHint("password (w/ hint)"_RU >> u8"пароль (w/ совет)", "<password>"_RU >> u8"<пароль>", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            ImGui::InputText("password (clear)"_RU >> u8"пароль (очистить)", password, IM_ARRAYSIZE(password));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Completion, History, Edit Callbacks"_RU >> u8"Завершение, история, редактирование обратных вызовов"))
        {
            struct Funcs
            {
                static int MyCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
                    {
                        data->InsertChars(data->CursorPos, "..");
                    }
                    else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
                    {
                        if (data->EventKey == ImGuiKey_UpArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Up!"_RU >> u8"Нажатый вверх!");
                            data->SelectAll();
                        }
                        else if (data->EventKey == ImGuiKey_DownArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Down!"_RU >> u8"Нажатый вниз!");
                            data->SelectAll();
                        }
                    }
                    else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
                    {
                        // Переключить регистр первого символа
                        char c = data->Buf[0];
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                            data->Buf[0] ^= 32;
                        data->BufDirty = true;

                        // Увеличение счетчика
                        int* p_int     = (int*)data->UserData;
                        *p_int         = *p_int + 1;
                    }
                    return 0;
                }
            };
            static char buf1[64];
            ImGui::InputText("Completion"_RU >> u8"Завершение", buf1, 64, ImGuiInputTextFlags_CallbackCompletion, Funcs::MyCallback);
            ImGui::SameLine();
            HelpMarker("Here we append \"..\" each time Tab is pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback."_RU >> u8"Здесь мы добавляем \"..\" при каждом нажатии Tab. См. «Примеры> Консоль» для более наглядной демонстрации использования этого обратного вызова.");

            static char buf2[64];
            ImGui::InputText("History"_RU >> u8"История", buf2, 64, ImGuiInputTextFlags_CallbackHistory, Funcs::MyCallback);
            ImGui::SameLine();
            HelpMarker("Here we replace and select text each time Up/Down are pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback."_RU >> u8"Здесь мы заменяем и выделяем текст каждый раз, когда нажимаем Вверх/Вниз. См. «Примеры> Консоль» для более наглядной демонстрации использования этого обратного вызова.");

            static char buf3[64];
            static int  edit_count = 0;
            ImGui::InputText("Edit"_RU >> u8"Правка", buf3, 64, ImGuiInputTextFlags_CallbackEdit, Funcs::MyCallback, (void*)&edit_count);
            ImGui::SameLine();
            HelpMarker("Here we toggle the casing of the first character on every edits + count edits."_RU >> u8"Здесь мы переключаем регистр первого символа при каждой правке + подсчитываем правки.");
            ImGui::SameLine();
            ImGui::Text("(%d)", edit_count);

            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Resize Callback"_RU >> u8"Виджеты/Ввод текста/Изменить размер обратного вызова");
        if (ImGui::TreeNode("Resize Callback"_RU >> u8"Изменить размер обратного вызова"))
        {
            // Чтобы связать InputText() с std::string или любым другим пользовательским типом строки,
            // вы можете использовать флажок ImGuiInputTextFlags_CallbackResize + создать пользовательскую оболочку ImGui::InputText()
            // используя ваш предпочтительный тип. См. misc/cpp/imgui_stdlib.h для реализации этого с использованием std::string.
            HelpMarker("Using ImGuiInputTextFlags_CallbackResize to wire your custom string type to InputText().\n\n"
                       "See misc/cpp/imgui_stdlib.h for an implementation of this for std::string."_RU >>
                u8"Использование ImGuiInputTextFlags_CallbackResize для подключения вашего пользовательского типа строки к InputText().\n\n"
                "См. misc/cpp/imgui_stdlib.h для реализации этого для std::string.");
            struct Funcs
            {
                static int MyResizeCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                    {
                        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                        IM_ASSERT(my_str->begin() == data->Buf);
                        my_str->resize(data->BufSize);   // NB: при изменении размера вызовов обычно data->BufSize == data->BufTextLen + 1
                        data->Buf = my_str->begin();
                    }
                    return 0;
                }

                // Примечание. Поскольку ImGui:: является пространством имен, вы обычно добавляете свою собственную функцию в пространство имен.
                // Например, ваш код может объявить функцию 'ImGui::InputText(const char* label, MyString* my str)'
                static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
                {
                    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                    return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
                }
            };

            // Для этой демонстрации мы используем ImVector в качестве строкового контейнера.
            // Обратите внимание, что поскольку нам нужно сохранить завершающий нулевой символ, наш размер/емкость на 1 больше
            // чем обычно сообщает типичный строковый класс.
            static ImVector<char> my_str;
            if (my_str.empty())
                my_str.push_back(0);
            Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
            ImGui::Text("Data: %p\nSize: %d\nCapacity: %d"_RU >> u8"Данные: %p\nРазмер: %d\nЕмкость: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    // Вкладки
    IMGUI_DEMO_MARKER("Widgets/Tabs"_RU >> u8"Виджеты/Вкладки");
    if (ImGui::TreeNode("Tabs"_RU >> u8"Вкладки"))
    {
        IMGUI_DEMO_MARKER("Widgets/Tabs/Basic"_RU >> u8"Виджеты/Вкладки/Базовый");
        if (ImGui::TreeNode("Basic"_RU >> u8"Базовый"))
        {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("MyTabBar"_RU >> u8"Моя панель вкладок", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Avocado"_RU >> u8"Авокадо"))
                {
                    ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah"_RU >> u8"Это вкладка авокадо!\nбла-бла-бла-бла-бла");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Broccoli"_RU >> u8"Брокколи"))
                {
                    ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah"_RU >> u8"Это вкладка Брокколи!\nбла-бла-бла-бла-бла");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Cucumber"_RU >> u8"Огурец"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah"_RU >> u8"Это вкладка огурцы!\nбла-бла-бла-бла-бла");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tabs/Advanced & Close Button"_RU >> u8"Виджеты/Вкладки/Кнопка «Дополнительно и закрыть»");
        if (ImGui::TreeNode("Advanced & Close Button"_RU >> u8"Кнопка «Дополнительно и закрыть»"))
        {
            // Выставьте пару доступных флагов. В большинстве случаев вы можете просто вызвать BeginTabBar() без флагов (0).
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
            ImGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", &tab_bar_flags, ImGuiTabBarFlags_Reorderable);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", &tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
            ImGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", &tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
            if ((tab_bar_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
                tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

            // Панель вкладок
            const char* names[4]  = {"Artichoke"_RU >> u8"Артишок", "Beetroot"_RU >> u8"Свекла", "Celery"_RU >> u8"Сельдерей", "Daikon"_RU >> u8"Дайкон"};
            static bool opened[4] = {true, true, true, true};   // Постоянное состояние пользователя
            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                if (n > 0)
                {
                    ImGui::SameLine();
                }
                ImGui::Checkbox(names[n], &opened[n]);
            }

            // Передача bool* в BeginTabItem() аналогична передаче в Begin():
            // для базового логического значения будет установлено значение false при закрытии вкладки.
            if (ImGui::BeginTabBar("MyTabBar"_RU >> u8"Моя панель вкладок", tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
                    {
                        ImGui::Text("This is the %s tab!"_RU >> u8"Это вкладка %s!", names[n]);
                        if (n & 1)
                            ImGui::Text("I am an odd tab."_RU >> u8"Я странная вкладка.");
                        ImGui::EndTabItem();
                    }
                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tabs/TabItemButton & Leading-Trailing flags"_RU >> u8"Виджеты/Вкладки/TabItemButton и начальные-конечные флаги");
        if (ImGui::TreeNode("TabItemButton & Leading/Trailing flags"_RU >> u8"TabItemButton и начальные/конечные флаги"))
        {
            static ImVector<int> active_tabs;
            static int           next_tab_id = 0;
            if (next_tab_id == 0)   // Инициализировать с некоторыми вкладками по умолчанию
                for (int i = 0; i < 3; i++)
                    active_tabs.push_back(next_tab_id++);

            // TabItemButton() и Leading/Trailing флаги — это отдельные функции, которые мы продемонстрируем вместе.
            // (Можно отправлять обычные вкладки с ведущими/конечными флагами или TabItemButton вкладки без начальных/конечных флагов...
            // но они, как правило, имеют больше смысла вместе)
            static bool show_leading_button  = true;
            static bool show_trailing_button = true;
            ImGui::Checkbox("Show Leading TabItemButton()"_RU >> u8"Показать ведущий TabItemButton()", &show_leading_button);
            ImGui::Checkbox("Show Trailing TabItemButton()"_RU >> u8"Показать завершающий TabItemButton()", &show_trailing_button);

            // Предоставьте некоторые другие флаги, которые полезны для демонстрации того, как они взаимодействуют с вкладками Leading/Trailing.
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
            ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
            if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

            if (ImGui::BeginTabBar("MyTabBar"_RU >> u8"Моя панель вкладок", tab_bar_flags))
            {
                // Демонстрация TabItemButton(): нажмите "?" кнопка для открытия меню
                if (show_leading_button)
                    if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
                        ImGui::OpenPopup("MyHelpMenu"_RU >> u8"Мое меню помощи");
                if (ImGui::BeginPopup("MyHelpMenu"_RU >> u8"Мое меню помощи"))
                {
                    ImGui::Selectable("Hello!"_RU >> u8"Привет!");
                    ImGui::EndPopup();
                }

                // Демонстрационные конечные вкладки: нажмите кнопку «+», чтобы добавить новую вкладку (в вашем приложении вы можете использовать значок шрифта вместо «+»)
                // Обратите внимание, что мы отправляем его перед обычными вкладками, но из-за ImGuiTabItemFlags_Trailing флага всегда будет отображаться в конце.
                if (show_trailing_button)
                    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                        active_tabs.push_back(next_tab_id++);   // Добавить новую вкладку

                // Отправьте наши обычные вкладки
                for (int n = 0; n < active_tabs.Size;)
                {
                    bool open = true;
                    char name[16];
                    snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
                    if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
                    {
                        ImGui::Text("This is the %s tab!"_RU >> u8"Это вкладка %s!", name);
                        ImGui::EndTabItem();
                    }

                    if (!open)
                        active_tabs.erase(active_tabs.Data + n);
                    else
                        n++;
                }

                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    // Виджеты Plot/Graph не очень хороши.
    // Рассмотрите возможность использования сторонней библиотеки, такой как ImPlot: https://github.com/epezent/implot
    // (увидеть других https://github.com/ocornut/imgui/wiki/Useful-Extensions)
    IMGUI_DEMO_MARKER("Widgets/Plotting"_RU >> u8"Виджеты/Графики");
    if (ImGui::TreeNode("Plotting"_RU >> u8"Графики"))
    {
        static bool animate = true;
        ImGui::Checkbox("Animate"_RU >> u8"Анимировать", &animate);

        // График в виде линий и график в виде гистограммы
        IMGUI_DEMO_MARKER("Widgets/Plotting/PlotLines, PlotHistogram"_RU >> u8"Виджеты/Графики/График линий, График гистограммы");
        static float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
        ImGui::PlotLines("Frame Times"_RU >> u8"Время кадра", arr, IM_ARRAYSIZE(arr));
        ImGui::PlotHistogram("Histogram"_RU >> u8"Гистограмма", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));

        // Заполните массив смежных значений с плавающей запятой для построения графика
        // Совет: если ваши объекты с плавающей запятой не являются смежными, а являются частью структуры, вы можете передать указатель на ваш первый объект с плавающей запятой
        // и sizeof() вашей структуры в параметре "шаг".
        static float  values[90]    = {};
        static int    values_offset = 0;
        static double refresh_time  = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime())   // Создайте данные с фиксированной частотой 60 Гц для демонстрации
        {
            static float phase    = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset         = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }

        // Графики могут отображать наложенные тексты
        // (в этом примере мы будем отображать среднее значение)
        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                average += values[n];
            average /= (float)IM_ARRAYSIZE(values);
            char overlay[32];
            sprintf(overlay, "avg %f"_RU >> u8"в среднем %f", average);
            ImGui::PlotLines("Lines"_RU >> u8"Линии", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
        }

        // Используйте функции для генерации вывода
        // ИСПРАВЛЕНИЕ: Это довольно неудобно, потому что текущий API графиков передает только индексы.
        // Мы, вероятно, хотим, чтобы API передавал числа с плавающей запятой, а пользователь предоставлял частоту дискретизации/количество.
        struct Funcs
        {
            static float Sin(void*, int i)
            {
                return sinf(i * 0.1f);
            }
            static float Saw(void*, int i)
            {
                return (i & 1) ? 1.0f : -1.0f;
            }
        };
        static int func_type = 0, display_count = 70;
        ImGui::Separator();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::Combo("func"_RU >> u8"функция", &func_type, "Sin\0Saw\0");
        ImGui::SameLine();
        ImGui::SliderInt("Sample count"_RU >> u8"Количество образцов", &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        ImGui::PlotLines("Lines"_RU >> u8"Линии", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
        ImGui::PlotHistogram("Histogram"_RU >> u8"Гистограмма", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
        ImGui::Separator();

        // Анимируйте простой индикатор выполнения
        IMGUI_DEMO_MARKER("Widgets/Plotting/ProgressBar"_RU >> u8"Виджеты/Графики/Прогресс Бар");
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f)
            {
                progress = +1.1f;
                progress_dir *= -1.0f;
            }
            if (progress <= -0.1f)
            {
                progress = -0.1f;
                progress_dir *= -1.0f;
            }
        }

        // Обычно мы используем ImVec2(-1.0f,0.0f) или ImVec2(-FLT MIN,0.0f), чтобы использовать всю доступную ширину,
        // или ImVec2(width,0.0f) на заданную ширину. ImVec2(0.0f,0.0f) использует ItemWidth.
        ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Progress Bar"_RU >> u8"Прогресс Бар");

        float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
        char  buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
        ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Color"_RU >> u8"Виджеты/Цвет");
    if (ImGui::TreeNode("Color/Picker Widgets"_RU >> u8"Цвет/Виджеты выбора"))
    {
        static ImVec4 color              = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        static bool   alpha_preview      = true;
        static bool   alpha_half_preview = false;
        static bool   drag_and_drop      = true;
        static bool   options_menu       = true;
        static bool   hdr                = false;
        ImGui::Checkbox("With Alpha Preview"_RU >> u8"С предпросмотром альфа-канала", &alpha_preview);
        ImGui::Checkbox("With Half Alpha Preview"_RU >> u8"С предпросмотром полуальфа-версии", &alpha_half_preview);
        ImGui::Checkbox("With Drag and Drop"_RU >> u8"С перетаскиванием", &drag_and_drop);
        ImGui::Checkbox("With Options Menu"_RU >> u8"С меню опций", &options_menu);
        ImGui::SameLine();
        HelpMarker("Right-click on the individual color widget to show options."_RU >> u8"Щелкните правой кнопкой мыши виджет отдельного цвета, чтобы отобразить параметры..");
        ImGui::Checkbox("With HDR"_RU >> u8"С HDR", &hdr);
        ImGui::SameLine();
        HelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets."_RU >> u8"В настоящее время все, что это делает, это снимает ограничения 0..1 на перетаскивание виджетов..");
        ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit"_RU >> u8"Виджеты/Цвет/Изменить Цвет");
        ImGui::Text("Color widget:"_RU >> u8"Цветной виджет:");
        ImGui::SameLine();
        HelpMarker("Click on the color square to open a color picker.\n"
                   "CTRL+click on individual component to input value.\n"_RU >>
            u8"Нажмите на цветной квадрат, чтобы открыть палитру цветов..\n"
            "CTRL+щелчок по отдельному компоненту для ввода значения.\n");
        ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit (HSV, with Alpha)"_RU >> u8"Виджеты/Цвет/Изменить Цвет (HSV, с Альфой)");
        ImGui::Text("Color widget HSV with Alpha:"_RU >> u8"Цветной виджет HSV с альфа-каналом:");
        ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_DisplayHSV | misc_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit (float display)"_RU >> u8"Виджеты/Цвет/Изменить Цвет (плавающий дисплей)");
        ImGui::Text("Color widget with Float Display:"_RU >> u8"Цветной виджет с Плавающим дисплеем:");
        ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | misc_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (with Picker)"_RU >> u8"Виджеты/Цвет/Цветная кнопка (с выбором)");
        ImGui::Text("Color button with Picker:"_RU >> u8"Цветная кнопка с выбором:");
        ImGui::SameLine();
        HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\n"
                   "With the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only "
                   "be used for the tooltip and picker popup."_RU >>
            u8"С флагом ImGuiColorEditFlags_NoInputs вы можете скрыть все вводы слайдера/текстовых вводов.\n"
            "С флагом ImGuiColorEditFlags_NoLabel вы можете передать непустую метку, которая будет только "
            "использоваться для всплывающей подсказки и всплывающего окна выбора.");
        ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (with custom Picker popup)"_RU >> u8"Виджеты/Цвет/Цветная кнопка (с всплывающим окном пользовательского выбора)");
        ImGui::Text("Color button with Custom Picker Popup:"_RU >> u8"Цветная кнопка с всплывающим окном пользовательского выбора:");

        // Создайте палитру по умолчанию. Палитра сохранится и может быть отредактирована.
        static bool   saved_palette_init = true;
        static ImVec4 saved_palette[32]  = {};
        if (saved_palette_init)
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f;   // Alpha
            }
            saved_palette_init = false;
        }

        static ImVec4 backup_color;
        bool          open_popup = ImGui::ColorButton("MyColor##3b", color, misc_flags);
        ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
        open_popup |= ImGui::Button("Palette"_RU >> u8"Палитра");
        if (open_popup)
        {
            ImGui::OpenPopup("mypicker"_RU >> u8"мой выбор");
            backup_color = color;
        }
        if (ImGui::BeginPopup("mypicker"_RU >> u8"мой выбор"))
        {
            ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!"_RU >> u8"МОЙ СОБСТВЕННЫЙ НАБОР ЦВЕТОВ С ПОТРЯСАЮЩЕЙ ПАЛИТРОЙ!");
            ImGui::Separator();
            ImGui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
            ImGui::SameLine();

            ImGui::BeginGroup();   // Заблокировать положение X
            ImGui::Text("Current"_RU >> u8"Текущий");
            ImGui::ColorButton("##current"_RU >> u8"##текущий", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
            ImGui::Text("Previous"_RU >> u8"Предыдущий");
            if (ImGui::ColorButton("##previous"_RU >> u8"##предыдущий", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
                color = backup_color;
            ImGui::Separator();
            ImGui::Text("Palette"_RU >> u8"Палитра");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::PushID(n);
                if ((n % 8) != 0)
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

                ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
                if (ImGui::ColorButton("##palette"_RU >> u8"##палитра", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w);   // Preserve alpha!

                // Разрешить пользователю добавлять цвета в каждый элемент палитры. Обратите внимание, что ColorButton() уже является
                // перетащите источник по умолчанию, если не указан флаг the ImGuiColorEditFlags_NoDragDrop.
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    ImGui::EndDragDropTarget();
                }

                ImGui::PopID();
            }
            ImGui::EndGroup();
            ImGui::EndPopup();
        }

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (simple)"_RU >> u8"Виджеты/Цвет/Цветная кнопка (простой)");
        ImGui::Text("Color button only:"_RU >> u8"Только цветная кнопка:");
        static bool no_border = false;
        ImGui::Checkbox("ImGuiColorEditFlags_NoBorder", &no_border);
        ImGui::ColorButton("MyColor##3c", *(ImVec4*)&color, misc_flags | (no_border ? ImGuiColorEditFlags_NoBorder : 0), ImVec2(80, 80));

        IMGUI_DEMO_MARKER("Widgets/Color/ColorPicker"_RU >> u8"Виджеты/Цвет/Выбор цвета");
        ImGui::Text("Color picker:"_RU >> u8"Выбор цвета:");
        static bool   alpha        = true;
        static bool   alpha_bar    = true;
        static bool   side_preview = true;
        static bool   ref_color    = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int    display_mode = 0;
        static int    picker_mode  = 0;
        ImGui::Checkbox("With Alpha"_RU >> u8"С Альфой", &alpha);
        ImGui::Checkbox("With Alpha Bar"_RU >> u8"С альфа-баром", &alpha_bar);
        ImGui::Checkbox("With Side Preview"_RU >> u8"С предварительным просмотром сбоку", &side_preview);
        if (side_preview)
        {
            ImGui::SameLine();
            ImGui::Checkbox("With Ref Color"_RU >> u8"С исходным цветом", &ref_color);
            if (ref_color)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | misc_flags);
            }
        }
        ImGui::Combo("Display Mode"_RU >> u8"Режим отображения", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0"_RU >> u8"Авто/Текущий\0Никто\0Только RGB\0Только HSV\0Только Hex\0");
        ImGui::SameLine();
        HelpMarker("ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, "
                   "but the user can change it with a right-click on those inputs.\n\nColorPicker defaults to displaying RGB+HSV+Hex "
                   "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions()."_RU >>
            u8"ColorEdit по умолчанию отображает входные сигналы RGB, если вы не указали режим отображения, "
            "но пользователь может изменить его, щелкнув правой кнопкой мыши эти входные данные.\n\nПо умолчанию ColorPicker отображает RGB+HSV+Hex. "
            "если вы не укажете режим отображения.\n\nВы можете изменить значения по умолчанию, используя SetColorEditOptions().");
        ImGui::SameLine();
        HelpMarker("When not specified explicitly (Auto/Current mode), user can right-click the picker to change mode."_RU >> u8"Если это не указано явно (авто/текущий режим), пользователь может щелкнуть правой кнопкой мыши средство выбора, чтобы изменить режим.");
        ImGuiColorEditFlags flags = misc_flags;
        if (!alpha)
            flags |= ImGuiColorEditFlags_NoAlpha;   // Это по умолчанию, если вы вызываете ColorPicker3() вместо ColorPicker4()
        if (alpha_bar)
            flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview)
            flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1)
            flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)
            flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1)
            flags |= ImGuiColorEditFlags_NoInputs;   // Отключить все дисплеи RGB/HSV/Hex
        if (display_mode == 2)
            flags |= ImGuiColorEditFlags_DisplayRGB;   // Переопределить режим отображения
        if (display_mode == 3)
            flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4)
            flags |= ImGuiColorEditFlags_DisplayHex;
        ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

        ImGui::Text("Set defaults in code:"_RU >> u8"Установите значения по умолчанию в коде:");
        ImGui::SameLine();
        HelpMarker("SetColorEditOptions() is designed to allow you to set boot-time default.\n"
                   "We don't have Push/Pop functions because you can force options on a per-widget basis if needed,"
                   "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid"
                   "encouraging you to persistently save values that aren't forward-compatible."_RU >>
            u8"SetColorEditOptions() позволяет вам установить значение по умолчанию во время загрузки..\n"
            "У нас нет функций Push/Pop, потому что при необходимости вы можете принудительно настроить параметры для каждого виджета.,"
            "и пользователь может изменить непринудительные с помощью меню параметров.\nУ нас нет геттера, чтобы избежать"
            "побуждая вас постоянно сохранять значения, которые не совместимы с предыдущими версиями.");
        if (ImGui::Button("Default: Uint8 + HSV + Hue Bar"_RU >> u8"По умолчанию: Uint8 + HSV + панель оттенка"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_PickerHueBar);
        if (ImGui::Button("Default: Float + HDR + Hue Wheel"_RU >> u8"По умолчанию: Плавающая + HDR + Колесо оттенков"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

        // Всегда обе уменьшенные версии обоих типов средств выбора (чтобы сделать их более заметными в демо для людей, которые быстро просматривают их)
        ImGui::Text("Both types:"_RU >> u8"Оба типа:");
        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker3("##MyColor##5", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker3("##MyColor##6", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

        // Поддержка кодирования HSV (чтобы избежать циклических обходов RGB<>HSV и сингулярностей, когда S==0 или V==0)
        static ImVec4 color_hsv(0.23f, 1.0f, 1.0f, 1.0f);   // Хранится как ВПГ!
        ImGui::Spacing();
        ImGui::Text("HSV encoded colors"_RU >> u8"Кодированные цвета HSV");
        ImGui::SameLine();
        HelpMarker("By default, colors are given to ColorEdit and ColorPicker in RGB, but ImGuiColorEditFlags_InputHSV"
                   "allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the"
                   "added benefit that you can manipulate hue values with the picker even when saturation or value are zero."_RU >>
            u8"По умолчанию цвета задаются ColorEdit и ColorPicker в RGB, но ImGuiColorEditFlags_InputHSV"
            "позволяет сохранять цвета как HSV и передавать их в ColorEdit и ColorPicker как HSV. Это связано с"
            "дополнительное преимущество, заключающееся в том, что вы можете манипулировать значениями оттенка с помощью средства выбора, даже если насыщенность или значение равны нулю.");
        ImGui::Text("Color widget with InputHSV:"_RU >> u8"Цветной виджет с InputHSV:");
        ImGui::ColorEdit4("HSV shown as RGB##1", (float*)&color_hsv, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("HSV shown as HSV##1", (float*)&color_hsv, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::DragFloat4("Raw HSV values", (float*)&color_hsv, 0.01f, 0.0f, 1.0f);

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Drag and Slider Flags"_RU >> u8"Виджеты/Флаги перетаскивания и ползунка");
    if (ImGui::TreeNode("Drag/Slider Flags"_RU >> u8"Флаги перетаскивания/ползунка"))
    {
        // Продемонстрируйте использование расширенных флагов для функций Drag XXX и Slider XXX. Обратите внимание, что флаги одинаковы!
        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
        ImGui::CheckboxFlags("ImGuiSliderFlags_AlwaysClamp", &flags, ImGuiSliderFlags_AlwaysClamp);
        ImGui::SameLine();
        HelpMarker("Always clamp value to min/max bounds (if any) when input manually with CTRL+Click."_RU >> u8"Всегда фиксируйте значение до минимальных/максимальных границ (если есть) при вводе вручную с помощью CTRL+Click.");
        ImGui::CheckboxFlags("ImGuiSliderFlags_Logarithmic", &flags, ImGuiSliderFlags_Logarithmic);
        ImGui::SameLine();
        HelpMarker("Enable logarithmic editing (more precision for small values)."_RU >> u8"Включить логарифмическое редактирование (больше точности для небольших значений).");
        ImGui::CheckboxFlags("ImGuiSliderFlags_NoRoundToFormat", &flags, ImGuiSliderFlags_NoRoundToFormat);
        ImGui::SameLine();
        HelpMarker("Disable rounding underlying value to match precision of the format string (e.g. %.3f values are rounded to those 3 digits)."_RU >> u8"Отключите округление базового значения, чтобы оно соответствовало точности строки формата (например, значения %.3f округляются до этих трех цифр).");
        ImGui::CheckboxFlags("ImGuiSliderFlags_NoInput", &flags, ImGuiSliderFlags_NoInput);
        ImGui::SameLine();
        HelpMarker("Disable CTRL+Click or Enter key allowing to input text directly into the widget."_RU >> u8"Отключите CTRL+щелчок или клавишу Enter, чтобы вводить текст непосредственно в виджет.");

        // Drags
        static float drag_f = 0.5f;
        static int   drag_i = 50;
        ImGui::Text("Underlying float value: %f"_RU >> u8"Базовое плавающее значение: %f", drag_f);
        ImGui::DragFloat("DragFloat (0 -> 1)", &drag_f, 0.005f, 0.0f, 1.0f, "%.3f", flags);
        ImGui::DragFloat("DragFloat (0 -> +inf)", &drag_f, 0.005f, 0.0f, FLT_MAX, "%.3f", flags);
        ImGui::DragFloat("DragFloat (-inf -> 1)", &drag_f, 0.005f, -FLT_MAX, 1.0f, "%.3f", flags);
        ImGui::DragFloat("DragFloat (-inf -> +inf)", &drag_f, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
        ImGui::DragInt("DragInt (0 -> 100)", &drag_i, 0.5f, 0, 100, "%d", flags);

        // Sliders
        static float slider_f = 0.5f;
        static int   slider_i = 50;
        ImGui::Text("Underlying float value: %f"_RU >> u8"Базовое плавающее значение: %f", slider_f);
        ImGui::SliderFloat("SliderFloat (0 -> 1)", &slider_f, 0.0f, 1.0f, "%.3f", flags);
        ImGui::SliderInt("SliderInt (0 -> 100)", &slider_i, 0, 100, "%d", flags);

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Range Widgets"_RU >> u8"Виджеты/Виджеты Диапазона");
    if (ImGui::TreeNode("Range Widgets"_RU >> u8"Виджеты Диапазона"))
    {
        static float begin = 10, end = 90;
        static int   begin_i = 100, end_i = 1000;
        ImGui::DragFloatRange2("range float"_RU >> u8"плавающий диапазон", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragIntRange2("range int"_RU >> u8"диапазон int", &begin_i, &end_i, 5, 0, 1000, "Min: %d units", "Max: %d units");
        ImGui::DragIntRange2("range int (no bounds)"_RU >> u8"диапазон int (без границ)", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Data Types"_RU >> u8"Виджеты/Типы данных");
    if (ImGui::TreeNode("Data Types"_RU >> u8"Типы данных"))
    {
// Функции DragScalar/InputScalar/SliderScalar позволяют использовать различные типы данных.
// - подписанный/неподписанный
// - 8/16/32/64-bits
// - целое/плавающее/двойное
// Чтобы не засорять публичный API всеми возможными комбинациями, мы используем перечисление ImGuiDataType.
// для передачи типа и передачи всех аргументов по указателю.
// По этой причине приведенный ниже тестовый код создает локальные переменные для хранения «ноля», «единицы» и т. д. для каждого типа.
// На практике, если вы часто используете данный тип, который не покрывается обычными точками входа API,
// вы можете сами обернуть его внутри однострочной функции, которая может принимать типизированный аргумент в качестве значения вместо void*,
// и затем передать их адрес универсальной функции. Например:
//   bool MySliderU64(const char *label, u64* value, u64 min = 0, u64 max = 0, const char* format = "%lld")
//   {
//      return SliderScalar(label, ImGuiDataType_U64, value, &min, &max, format);
//   }

// Установите ограничения (в качестве вспомогательных переменных, чтобы мы могли взять их адрес, как описано выше)
// Примечание. Функции SliderScalar() имеют максимальный используемый диапазон, равный половине максимального значения естественного типа, следовательно, /2.
#ifndef LLONG_MIN
        ImS64 LLONG_MIN  = -9223372036854775807LL - 1;
        ImS64 LLONG_MAX  = 9223372036854775807LL;
        ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
#endif
        const char    s8_zero = 0, s8_one = 1, s8_fifty = 50, s8_min = -128, s8_max = 127;
        const ImU8    u8_zero = 0, u8_one = 1, u8_fifty = 50, u8_min = 0, u8_max = 255;
        const short   s16_zero = 0, s16_one = 1, s16_fifty = 50, s16_min = -32768, s16_max = 32767;
        const ImU16   u16_zero = 0, u16_one = 1, u16_fifty = 50, u16_min = 0, u16_max = 65535;
        const ImS32   s32_zero = 0, s32_one = 1, s32_fifty = 50, s32_min = INT_MIN / 2, s32_max = INT_MAX / 2, s32_hi_a = INT_MAX / 2 - 100, s32_hi_b = INT_MAX / 2;
        const ImU32   u32_zero = 0, u32_one = 1, u32_fifty = 50, u32_min = 0, u32_max = UINT_MAX / 2, u32_hi_a = UINT_MAX / 2 - 100, u32_hi_b = UINT_MAX / 2;
        const ImS64   s64_zero = 0, s64_one = 1, s64_fifty = 50, s64_min = LLONG_MIN / 2, s64_max = LLONG_MAX / 2, s64_hi_a = LLONG_MAX / 2 - 100, s64_hi_b = LLONG_MAX / 2;
        const ImU64   u64_zero = 0, u64_one = 1, u64_fifty = 50, u64_min = 0, u64_max = ULLONG_MAX / 2, u64_hi_a = ULLONG_MAX / 2 - 100, u64_hi_b = ULLONG_MAX / 2;
        const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
        const double  f64_zero = 0., f64_one = 1., f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

        // State
        static char   s8_v       = 127;
        static ImU8   u8_v       = 255;
        static short  s16_v      = 32767;
        static ImU16  u16_v      = 65535;
        static ImS32  s32_v      = -1;
        static ImU32  u32_v      = (ImU32)-1;
        static ImS64  s64_v      = -1;
        static ImU64  u64_v      = (ImU64)-1;
        static float  f32_v      = 0.123f;
        static double f64_v      = 90000.01234567890123456789;

        const float   drag_speed = 0.2f;
        static bool   drag_clamp = false;
        IMGUI_DEMO_MARKER("Widgets/Data Types/Drags"_RU >> u8"Виджеты/Типы данных/Drags");
        ImGui::Text("Drags:");
        ImGui::Checkbox("Clamp integers to 0..50"_RU >> u8"Ограничьте целые числа до 0..50", &drag_clamp);
        ImGui::SameLine();
        HelpMarker("As with every widgets in dear imgui, we never modify values unless there is a user interaction.\n"
                   "You can override the clamping limits by using CTRL+Click to input a value."_RU >>
            u8"Как и в случае с любыми виджетами в dear imgui, мы никогда не изменяем значения, если нет взаимодействия с пользователем.\n"
            "Вы можете переопределить пределы зажима, используя сочетание клавиш CTRL+Click для ввода значения.");
        ImGui::DragScalar("drag s8", ImGuiDataType_S8, &s8_v, drag_speed, drag_clamp ? &s8_zero : NULL, drag_clamp ? &s8_fifty : NULL);
        ImGui::DragScalar("drag u8", ImGuiDataType_U8, &u8_v, drag_speed, drag_clamp ? &u8_zero : NULL, drag_clamp ? &u8_fifty : NULL, "%u ms");
        ImGui::DragScalar("drag s16", ImGuiDataType_S16, &s16_v, drag_speed, drag_clamp ? &s16_zero : NULL, drag_clamp ? &s16_fifty : NULL);
        ImGui::DragScalar("drag u16", ImGuiDataType_U16, &u16_v, drag_speed, drag_clamp ? &u16_zero : NULL, drag_clamp ? &u16_fifty : NULL, "%u ms");
        ImGui::DragScalar("drag s32", ImGuiDataType_S32, &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL);
        ImGui::DragScalar("drag s32 hex", ImGuiDataType_S32, &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL, "0x%08X");
        ImGui::DragScalar("drag u32", ImGuiDataType_U32, &u32_v, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        ImGui::DragScalar("drag s64", ImGuiDataType_S64, &s64_v, drag_speed, drag_clamp ? &s64_zero : NULL, drag_clamp ? &s64_fifty : NULL);
        ImGui::DragScalar("drag u64", ImGuiDataType_U64, &u64_v, drag_speed, drag_clamp ? &u64_zero : NULL, drag_clamp ? &u64_fifty : NULL);
        ImGui::DragScalar("drag float", ImGuiDataType_Float, &f32_v, 0.005f, &f32_zero, &f32_one, "%f");
        ImGui::DragScalar("drag float log", ImGuiDataType_Float, &f32_v, 0.005f, &f32_zero, &f32_one, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::DragScalar("drag double", ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, NULL, "%.10f grams");
        ImGui::DragScalar("drag double log", ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, &f64_one, "0 < %.10f < 1", ImGuiSliderFlags_Logarithmic);

        IMGUI_DEMO_MARKER("Widgets/Data Types/Sliders"_RU >> u8"Виджеты/Типы данных/Ползунки");
        ImGui::Text("Sliders"_RU >> u8"Ползунки");
        ImGui::SliderScalar("slider s8 full"_RU >> u8"ползунок s8 полный", ImGuiDataType_S8, &s8_v, &s8_min, &s8_max, "%d");
        ImGui::SliderScalar("slider u8 full"_RU >> u8"ползунок u8 полный", ImGuiDataType_U8, &u8_v, &u8_min, &u8_max, "%u");
        ImGui::SliderScalar("slider s16 full"_RU >> u8"ползунок s16 полный", ImGuiDataType_S16, &s16_v, &s16_min, &s16_max, "%d");
        ImGui::SliderScalar("slider u16 full"_RU >> u8"ползунок u16 полный", ImGuiDataType_U16, &u16_v, &u16_min, &u16_max, "%u");
        ImGui::SliderScalar("slider s32 low"_RU >> u8"ползунок s32 низкий", ImGuiDataType_S32, &s32_v, &s32_zero, &s32_fifty, "%d");
        ImGui::SliderScalar("slider s32 high"_RU >> u8"ползунок s32 высокий", ImGuiDataType_S32, &s32_v, &s32_hi_a, &s32_hi_b, "%d");
        ImGui::SliderScalar("slider s32 full"_RU >> u8"ползунок s32 полный", ImGuiDataType_S32, &s32_v, &s32_min, &s32_max, "%d");
        ImGui::SliderScalar("slider s32 hex"_RU >> u8"ползунок s32 hex", ImGuiDataType_S32, &s32_v, &s32_zero, &s32_fifty, "0x%04X");
        ImGui::SliderScalar("slider u32 low"_RU >> u8"ползунок u32 низкий", ImGuiDataType_U32, &u32_v, &u32_zero, &u32_fifty, "%u");
        ImGui::SliderScalar("slider u32 high"_RU >> u8"ползунок u32 высокий", ImGuiDataType_U32, &u32_v, &u32_hi_a, &u32_hi_b, "%u");
        ImGui::SliderScalar("slider u32 full"_RU >> u8"ползунок u32 полный", ImGuiDataType_U32, &u32_v, &u32_min, &u32_max, "%u");
        ImGui::SliderScalar("slider s64 low"_RU >> u8"ползунок s64 низкий", ImGuiDataType_S64, &s64_v, &s64_zero, &s64_fifty, "%" IM_PRId64);
        ImGui::SliderScalar("slider s64 high"_RU >> u8"ползунок s64 высокий", ImGuiDataType_S64, &s64_v, &s64_hi_a, &s64_hi_b, "%" IM_PRId64);
        ImGui::SliderScalar("slider s64 full"_RU >> u8"ползунок s64 полный", ImGuiDataType_S64, &s64_v, &s64_min, &s64_max, "%" IM_PRId64);
        ImGui::SliderScalar("slider u64 low"_RU >> u8"ползунок u64 низкий", ImGuiDataType_U64, &u64_v, &u64_zero, &u64_fifty, "%" IM_PRIu64 " ms");
        ImGui::SliderScalar("slider u64 high"_RU >> u8"ползунок u64 высокий", ImGuiDataType_U64, &u64_v, &u64_hi_a, &u64_hi_b, "%" IM_PRIu64 " ms");
        ImGui::SliderScalar("slider u64 full"_RU >> u8"ползунок u64 полный", ImGuiDataType_U64, &u64_v, &u64_min, &u64_max, "%" IM_PRIu64 " ms");
        ImGui::SliderScalar("slider float low"_RU >> u8"ползунок float низкий", ImGuiDataType_Float, &f32_v, &f32_zero, &f32_one);
        ImGui::SliderScalar("slider float low log"_RU >> u8"ползунок float низкий лог", ImGuiDataType_Float, &f32_v, &f32_zero, &f32_one, "%.10f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderScalar("slider float high"_RU >> u8"ползунок float высокий", ImGuiDataType_Float, &f32_v, &f32_lo_a, &f32_hi_a, "%e");
        ImGui::SliderScalar("slider double low"_RU >> u8"ползунок double низкий", ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one, "%.10f grams");
        ImGui::SliderScalar("slider double low log"_RU >> u8"ползунок double низкий лог", ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one, "%.10f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderScalar("slider double high"_RU >> u8"ползунок double высокий", ImGuiDataType_Double, &f64_v, &f64_lo_a, &f64_hi_a, "%e grams");

        ImGui::Text("Sliders (reverse)"_RU >> u8"Ползунки (обратный)");
        ImGui::SliderScalar("slider s8 reverse"_RU >> u8"ползунок s8 обратный", ImGuiDataType_S8, &s8_v, &s8_max, &s8_min, "%d");
        ImGui::SliderScalar("slider u8 reverse"_RU >> u8"ползунок u8 обратный", ImGuiDataType_U8, &u8_v, &u8_max, &u8_min, "%u");
        ImGui::SliderScalar("slider s32 reverse"_RU >> u8"ползунок s32 обратный", ImGuiDataType_S32, &s32_v, &s32_fifty, &s32_zero, "%d");
        ImGui::SliderScalar("slider u32 reverse"_RU >> u8"ползунок u32 обратный", ImGuiDataType_U32, &u32_v, &u32_fifty, &u32_zero, "%u");
        ImGui::SliderScalar("slider s64 reverse"_RU >> u8"ползунок s64 обратный", ImGuiDataType_S64, &s64_v, &s64_fifty, &s64_zero, "%" IM_PRId64);
        ImGui::SliderScalar("slider u64 reverse"_RU >> u8"ползунок u64 обратный", ImGuiDataType_U64, &u64_v, &u64_fifty, &u64_zero, "%" IM_PRIu64 " ms");

        IMGUI_DEMO_MARKER("Widgets/Data Types/Inputs"_RU >> u8"Виджеты/Типы данных/Inputs");
        static bool inputs_step = true;
        ImGui::Text("Inputs");
        ImGui::Checkbox("Show step buttons", &inputs_step);
        ImGui::InputScalar("input s8", ImGuiDataType_S8, &s8_v, inputs_step ? &s8_one : NULL, NULL, "%d");
        ImGui::InputScalar("input u8", ImGuiDataType_U8, &u8_v, inputs_step ? &u8_one : NULL, NULL, "%u");
        ImGui::InputScalar("input s16", ImGuiDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
        ImGui::InputScalar("input u16", ImGuiDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
        ImGui::InputScalar("input s32", ImGuiDataType_S32, &s32_v, inputs_step ? &s32_one : NULL, NULL, "%d");
        ImGui::InputScalar("input s32 hex", ImGuiDataType_S32, &s32_v, inputs_step ? &s32_one : NULL, NULL, "%04X");
        ImGui::InputScalar("input u32", ImGuiDataType_U32, &u32_v, inputs_step ? &u32_one : NULL, NULL, "%u");
        ImGui::InputScalar("input u32 hex", ImGuiDataType_U32, &u32_v, inputs_step ? &u32_one : NULL, NULL, "%08X");
        ImGui::InputScalar("input s64", ImGuiDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
        ImGui::InputScalar("input u64", ImGuiDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
        ImGui::InputScalar("input float", ImGuiDataType_Float, &f32_v, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("input double", ImGuiDataType_Double, &f64_v, inputs_step ? &f64_one : NULL);

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Multi-component Widgets"_RU >> u8"Виджеты/Многокомпонентные виджеты");
    if (ImGui::TreeNode("Multi-component Widgets"_RU >> u8"Многокомпонентные виджеты"))
    {
        static float vec4f[4] = {0.10f, 0.20f, 0.30f, 0.44f};
        static int   vec4i[4] = {1, 5, 100, 255};

        ImGui::InputFloat2("input float2", vec4f);
        ImGui::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
        ImGui::InputInt2("input int2", vec4i);
        ImGui::DragInt2("drag int2", vec4i, 1, 0, 255);
        ImGui::SliderInt2("slider int2", vec4i, 0, 255);
        ImGui::Spacing();

        ImGui::InputFloat3("input float3", vec4f);
        ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
        ImGui::InputInt3("input int3", vec4i);
        ImGui::DragInt3("drag int3", vec4i, 1, 0, 255);
        ImGui::SliderInt3("slider int3", vec4i, 0, 255);
        ImGui::Spacing();

        ImGui::InputFloat4("input float4", vec4f);
        ImGui::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
        ImGui::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
        ImGui::InputInt4("input int4", vec4i);
        ImGui::DragInt4("drag int4", vec4i, 1, 0, 255);
        ImGui::SliderInt4("slider int4", vec4i, 0, 255);

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Vertical Sliders"_RU >> u8"Виджеты/Вертикальные ползунки");
    if (ImGui::TreeNode("Vertical Sliders"_RU >> u8"Вертикальные ползунки"))
    {
        const float spacing = 4;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        ImGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 0, 5);
        ImGui::SameLine();

        static float values[7] = {0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f};
        ImGui::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
            ImGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], 0.0f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.3f", values[i]);
            ImGui::PopStyleColor(4);
            ImGui::PopID();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set2");
        static float values2[4] = {0.20f, 0.80f, 0.40f, 0.25f};
        const int    rows       = 3;
        const ImVec2 small_slider_size(18, (float)(int)((160.0f - (rows - 1) * spacing) / rows));
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0)
                ImGui::SameLine();
            ImGui::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                ImGui::PushID(nx * rows + ny);
                ImGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                    ImGui::SetTooltip("%.3f", values2[nx]);
                ImGui::PopID();
            }
            ImGui::EndGroup();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
            ImGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            ImGui::PopStyleVar();
            ImGui::PopID();
        }
        ImGui::PopID();
        ImGui::PopStyleVar();
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Drag and drop"_RU >> u8"Виджеты/Drag and drop");
    if (ImGui::TreeNode("Drag and Drop"))
    {
        IMGUI_DEMO_MARKER("Widgets/Drag and drop/Standard widgets"_RU >> u8"Виджеты/Drag and drop/Стандартные виджеты");
        if (ImGui::TreeNode("Drag and drop in standard widgets"_RU >> u8"Drag and drop в стандартных виджетах"))
        {
            // Виджеты ColorEdit автоматически действуют как источник и цель перетаскивания.
            // Они используют стандартизированные строки полезной нагрузки IMGUI_PAYLOAD_TYPE_COLOR_3F и IMGUI_PAYLOAD_TYPE_COLOR_4F.
            // чтобы ваши собственные виджеты могли использовать цвета при перетаскивании.
            // Также см. демонстрацию 'Demo->Widgets->Color/Picker Widgets->Palette'.
            HelpMarker("You can drag from the color squares."_RU >> u8"Вы можете перетащить из цветных квадратов.");
            static float col1[3] = {1.0f, 0.0f, 0.2f};
            static float col2[4] = {0.4f, 0.7f, 0.0f, 0.5f};
            ImGui::ColorEdit3("color 1"_RU >> u8"цвет 1", col1);
            ImGui::ColorEdit4("color 2"_RU >> u8"цвет 2", col2);
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Drag and drop/Copy-swap items"_RU >> u8"Виджеты/Drag and drop/Копировать-заменять элементы");
        if (ImGui::TreeNode("Drag and drop to copy/swap items"_RU >> u8"Drag and drop копировать/обмен предметами"))
        {
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (ImGui::RadioButton("Copy"_RU >> u8"Копировать", mode == Mode_Copy))
            {
                mode = Mode_Copy;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Move"_RU >> u8"Перемещать", mode == Mode_Move))
            {
                mode = Mode_Move;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Swap"_RU >> u8"Менять", mode == Mode_Swap))
            {
                mode = Mode_Swap;
            }
            static const char* names[9] = {"Bobby"_RU >> u8"Бобби", "Beatrice"_RU >> u8"Беатрис", "Betty"_RU >> u8"Бетти", "Brianna"_RU >> u8"Брианна", "Barry"_RU >> u8"Барри", "Bernard"_RU >> u8"Бернар", "Bibi"_RU >> u8"Биби", "Blaine"_RU >> u8"Блейн", "Bryn"_RU >> u8"Брин"};
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                ImGui::PushID(n);
                if ((n % 3) != 0)
                    ImGui::SameLine();
                ImGui::Button(names[n], ImVec2(60, 60));

                // Наши кнопки являются источниками перетаскивания и целями перетаскивания!
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    // Установить полезную нагрузку для переноса индекса нашего элемента (может быть что угодно)
                    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

                    // Отобразить предварительный просмотр (может быть что угодно, например, при перетаскивании изображения мы можем решить отобразить
                    // имя файла и небольшой превью изображения и т. д.)
                    if (mode == Mode_Copy)
                    {
                        ImGui::Text("Copy %s"_RU >> u8"Копировать %s", names[n]);
                    }
                    if (mode == Mode_Move)
                    {
                        ImGui::Text("Move %s"_RU >> u8"Перемещать %s", names[n]);
                    }
                    if (mode == Mode_Swap)
                    {
                        ImGui::Text("Swap %s"_RU >> u8"Менять %s", names[n]);
                    }
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n]         = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp  = names[n];
                            names[n]         = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Drag and Drop/Drag to reorder items (simple)"_RU >> u8"Виджеты/Drag and Drop/Перетащить, чтобы изменить порядок элементов(простой)");
        if (ImGui::TreeNode("Drag to reorder items (simple)"_RU >> u8"Перетащить, чтобы изменить порядок элементов(простой)"))
        {
            // Простое изменение порядка
            HelpMarker("We don't use the drag and drop api at all here! "
                       "Instead we query when the item is held but not hovered, and order items accordingly."_RU >>
                u8"Мы вообще не используем API перетаскивания.! "
                "Вместо этого мы запрашиваем, когда элемент удерживается, но не зависает, и соответствующим образом упорядочиваем элементы..");
            static const char* item_names[] = {"Item One"_RU >> u8"Элемент Один", "Item Two"_RU >> u8"Элемент Два", "Item Three"_RU >> u8"Элемент Три", "Item Four"_RU >> u8"Элемент Четыре", "Item Five"_RU >> u8"Элемент Пять"};
            for (int n = 0; n < IM_ARRAYSIZE(item_names); n++)
            {
                const char* item = item_names[n];
                ImGui::Selectable(item);

                if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                {
                    int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names))
                    {
                        item_names[n]      = item_names[n_next];
                        item_names[n_next] = item;
                        ImGui::ResetMouseDragDelta();
                    }
                }
            }
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Querying Item Status (Edited,Active,Hovered etc.)"_RU >> u8"Виджеты/Запрос статуса элемента (Отредактированный, активный, Наведено и т.д.)");
    if (ImGui::TreeNode("Querying Item Status (Edited/Active/Hovered etc.)"_RU >> u8"Запрос статуса элемента (Отредактированный/активный/наведено и т. д.)"))
    {
        // Выберите тип элемента
        const char* item_names[]  = {"Text", "Button", "Button (w/ repeat)", "Checkbox", "SliderFloat", "InputText", "InputTextMultiline", "InputFloat", "InputFloat3", "ColorEdit4", "Selectable", "MenuItem", "TreeNode", "TreeNode (w/ double-click)", "Combo", "ListBox"};
        static int  item_type     = 4;
        static bool item_disabled = false;
        ImGui::Combo("Item Type"_RU >> u8"Тип элемента", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));
        ImGui::SameLine();
        HelpMarker("Testing how various types of items are interacting with the IsItemXXX functions. Note that the bool return value of most ImGui function is generally equivalent to calling ImGui::IsItemHovered()."_RU >> u8"Тестирование того, как различные типы элементов взаимодействуют с функциями IsItemXXX. Обратите внимание, что логическое значение, возвращаемое большинством функций ImGui, обычно эквивалентно вызову ImGui:: IsItemHovered().");
        ImGui::Checkbox("Item Disabled"_RU >> u8"Элемент отключен", &item_disabled);

        // Отправьте выбранный элемент элемента, чтобы мы могли запросить его статус в коде, следующем за ним.
        bool         ret      = false;
        static bool  b        = false;
        static float col4f[4] = {1.0f, 0.5, 0.0f, 1.0f};
        static char  str[16]  = {};
        if (item_disabled)
            ImGui::BeginDisabled(true);
        if (item_type == 0)
        {
            ImGui::Text("ITEM: Text");
        }   // Тестирование текстовых элементов без идентификатора/взаимодействия
        if (item_type == 1)
        {
            ret = ImGui::Button("ITEM: Button");
        }   // Кнопка тестирования
        if (item_type == 2)
        {
            ImGui::PushButtonRepeat(true);
            ret = ImGui::Button("ITEM: Button");
            ImGui::PopButtonRepeat();
        }   // Кнопка тестирования (с повторителем)
        if (item_type == 3)
        {
            ret = ImGui::Checkbox("ITEM: Checkbox", &b);
        }   // Флажок Тестирование
        if (item_type == 4)
        {
            ret = ImGui::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f);
        }   // Тестирование базового элемента
        if (item_type == 5)
        {
            ret = ImGui::InputText("ITEM: InputText", &str[0], IM_ARRAYSIZE(str));
        }   // Тестирование вводимого текста (который обрабатывает табуляцию)
        if (item_type == 6)
        {
            ret = ImGui::InputTextMultiline("ITEM: InputTextMultiline", &str[0], IM_ARRAYSIZE(str));
        }   // Тестирование входного текста (в котором используется дочернее окно)
        if (item_type == 7)
        {
            ret = ImGui::InputFloat("ITEM: InputFloat", col4f, 1.0f);
        }   // Тестирование кнопок +/- на скалярном вводе
        if (item_type == 8)
        {
            ret = ImGui::InputFloat3("ITEM: InputFloat3", col4f);
        }   // Тестирование многокомпонентных элементов (сообщается, что флаги IsItemXXX объединены)
        if (item_type == 9)
        {
            ret = ImGui::ColorEdit4("ITEM: ColorEdit4", col4f);
        }   // Тестирование многокомпонентных элементов (сообщается, что флаги IsItemXXX объединены)
        if (item_type == 10)
        {
            ret = ImGui::Selectable("ITEM: Selectable");
        }   // Тестирование выбираемого элемента
        if (item_type == 11)
        {
            ret = ImGui::MenuItem("ITEM: MenuItem");
        }   // Пункт меню «Тестирование» (используют политику кнопок ImGuiButtonFlags_PressedOnRelease)
        if (item_type == 12)
        {
            ret = ImGui::TreeNode("ITEM: TreeNode");
            if (ret)
                ImGui::TreePop();
        }   // Тестирование узла дерева
        if (item_type == 13)
        {
            ret = ImGui::TreeNodeEx("ITEM: TreeNode w/ ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_NoTreePushOnOpen);
        }   // Тестирование узла дерева с политикой кнопок ImGuiButtonFlags_PressedOnDoubleClick.
        if (item_type == 14)
        {
            const char* items[] = {"Apple", "Banana", "Cherry", "Kiwi"};
            static int  current = 1;
            ret                 = ImGui::Combo("ITEM: Combo", &current, items, IM_ARRAYSIZE(items));
        }
        if (item_type == 15)
        {
            const char* items[] = {"Apple", "Banana", "Cherry", "Kiwi"};
            static int  current = 1;
            ret                 = ImGui::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));
        }

        // Отображение значений IsItemHovered() и других общих функций состояния элемента.
        // Обратите внимание, что флаги ImGuiHoveredFlags_XXX можно комбинировать.
        // Поскольку BulletText сам по себе является элементом, и это повлияет на вывод функций IsItemXXX,
        // мы запрашиваем каждое состояние в одном вызове, чтобы избежать их сохранения и упростить код.
        ImGui::BulletText("Return value = %d\n"
                          "IsItemFocused() = %d\n"
                          "IsItemHovered() = %d\n"
                          "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
                          "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
                          "IsItemHovered(_AllowWhenOverlapped) = %d\n"
                          "IsItemHovered(_AllowWhenDisabled) = %d\n"
                          "IsItemHovered(_RectOnly) = %d\n"
                          "IsItemActive() = %d\n"
                          "IsItemEdited() = %d\n"
                          "IsItemActivated() = %d\n"
                          "IsItemDeactivated() = %d\n"
                          "IsItemDeactivatedAfterEdit() = %d\n"
                          "IsItemVisible() = %d\n"
                          "IsItemClicked() = %d\n"
                          "IsItemToggledOpen() = %d\n"
                          "GetItemRectMin() = (%.1f, %.1f)\n"
                          "GetItemRectMax() = (%.1f, %.1f)\n"
                          "GetItemRectSize() = (%.1f, %.1f)",
            ret, ImGui::IsItemFocused(), ImGui::IsItemHovered(), ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup), ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem), ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped), ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled), ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly), ImGui::IsItemActive(), ImGui::IsItemEdited(), ImGui::IsItemActivated(), ImGui::IsItemDeactivated(),
            ImGui::IsItemDeactivatedAfterEdit(), ImGui::IsItemVisible(), ImGui::IsItemClicked(), ImGui::IsItemToggledOpen(), ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y, ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);

        if (item_disabled)
            ImGui::EndDisabled();

        char buf[1] = "";
        ImGui::InputText("unused"_RU >> u8"неиспользованный", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        HelpMarker("This widget is only here to be able to tab-out of the widgets above and see e.g. Deactivated() status."_RU >> u8"Этот виджет здесь только для того, чтобы иметь возможность переключаться между виджетами выше и видеть e. г. Статус Deactivated().");

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Querying Window Status (Focused,Hovered etc.)"_RU >> u8"Виджеты/Запрос состояния окна (Сфокусированный, Наведённый и т.д.)");
    if (ImGui::TreeNode("Querying Window Status (Focused/Hovered etc.)"_RU >> u8"Запрос состояния окна (Сфокусированный/Наведённый и т.д.)"))
    {
        static bool embed_all_inside_a_child_window = false;
        ImGui::Checkbox("Embed everything inside a child window for testing _RootWindow flag."_RU >> u8"Вставьте все в дочернее окно для тестирования флага _RootWindow.", &embed_all_inside_a_child_window);
        if (embed_all_inside_a_child_window)
            ImGui::BeginChild("outer_child", ImVec2(0, ImGui::GetFontSize() * 20.0f), true);

        // Тестирование функции IsWindowFocused() с ее различными флагами.
        ImGui::BulletText("IsWindowFocused() = %d\n"
                          "IsWindowFocused(_ChildWindows) = %d\n"
                          "IsWindowFocused(_ChildWindows|_NoPopupHierarchy) = %d\n"
                          "IsWindowFocused(_ChildWindows|_DockHierarchy) = %d\n"
                          "IsWindowFocused(_ChildWindows|_RootWindow) = %d\n"
                          "IsWindowFocused(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
                          "IsWindowFocused(_ChildWindows|_RootWindow|_DockHierarchy) = %d\n"
                          "IsWindowFocused(_RootWindow) = %d\n"
                          "IsWindowFocused(_RootWindow|_NoPopupHierarchy) = %d\n"
                          "IsWindowFocused(_RootWindow|_DockHierarchy) = %d\n"
                          "IsWindowFocused(_AnyWindow) = %d\n",
            ImGui::IsWindowFocused(), ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows), ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy), ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_DockHierarchy), ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow), ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_NoPopupHierarchy),
            ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_DockHierarchy), ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow), ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_NoPopupHierarchy), ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_DockHierarchy), ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow));

        // Тестирование функции IsWindowHovered() с ее различными флагами.
        ImGui::BulletText("IsWindowHovered() = %d\n"
                          "IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
                          "IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n"
                          "IsWindowHovered(_ChildWindows) = %d\n"
                          "IsWindowHovered(_ChildWindows|_NoPopupHierarchy) = %d\n"
                          "IsWindowHovered(_ChildWindows|_DockHierarchy) = %d\n"
                          "IsWindowHovered(_ChildWindows|_RootWindow) = %d\n"
                          "IsWindowHovered(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
                          "IsWindowHovered(_ChildWindows|_RootWindow|_DockHierarchy) = %d\n"
                          "IsWindowHovered(_RootWindow) = %d\n"
                          "IsWindowHovered(_RootWindow|_NoPopupHierarchy) = %d\n"
                          "IsWindowHovered(_RootWindow|_DockHierarchy) = %d\n"
                          "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = %d\n"
                          "IsWindowHovered(_AnyWindow) = %d\n",
            ImGui::IsWindowHovered(), ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup), ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_DockHierarchy),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_NoPopupHierarchy), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_DockHierarchy), ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow), ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_NoPopupHierarchy),
            ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_DockHierarchy), ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup), ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow));

        ImGui::BeginChild("child"_RU >> u8"дочерний", ImVec2(0, 50), true);
        ImGui::Text("This is another child window for testing the _ChildWindows flag."_RU >> u8"Это еще одно дочернее окно для проверки флага _ChildWindows.");
        ImGui::EndChild();
        if (embed_all_inside_a_child_window)
            ImGui::EndChild();

        // Вызов IsItemHovered() после начала возвращает статус зависания строки заголовка.
        // Это особенно полезно, если вы хотите создать контекстное меню, связанное с заголовком окна.
        // Это также будет работать при прикреплении к вкладке (вкладка заменяет строку заголовка и гарантирует те же свойства).
        static bool test_window = false;
        ImGui::Checkbox("Hovered/Active tests after Begin() for title bar testing"_RU >> u8"Hovered/Active тесты после Begin() для тестирования Заголовка", &test_window);
        if (test_window)
        {
            // FIXME-DOCK: это окно нельзя закрепить в демонстрационном окне ImGui, это вызовет петлю обратной связи и зависнет.
            // Можем ли мы исправить это с помощью функции ImGuiWindowClass? Или вызов API, чтобы пометить нашего родителя как «не пропускать элементы»?
            ImGui::Begin("Title bar Hovered/Active tests"_RU >> u8"Заголовок Наведенные/активные тесты", &test_window);
            if (ImGui::BeginPopupContextItem())   // <-- Это использует IsItemHovered()
            {
                if (ImGui::MenuItem("Close"_RU >> u8"Закрыть"))
                {
                    test_window = false;
                }
                ImGui::EndPopup();
            }
            ImGui::Text("IsItemHovered() after begin = %d (== is title bar hovered)\n"
                        "IsItemActive() after begin = %d (== is window being clicked/moved)\n"_RU >>
                    u8"IsItemHovered() после begin = %d (== наведено ли на Заголовок)\n"
                    "IsItemActive() после begin = %d (== щелкают/перемещают ли окно)\n",
                ImGui::IsItemHovered(), ImGui::IsItemActive());
            ImGui::End();
        }

        ImGui::TreePop();
    }

    // Продемонстрируйте BeginDisabled/EndDisabled с помощью флажка, расположенного внизу раздела (что немного странно:
    // по логике мы должны поставить этот флажок в верхней части раздела, но мы не хотим, чтобы эта функция занимала это место)
    if (disable_all)
        ImGui::EndDisabled();

    IMGUI_DEMO_MARKER("Widgets/Disable Block"_RU >> u8"Виджеты/Отключить блокировку");
    if (ImGui::TreeNode("Disable block"_RU >> u8"Отключить блокировку"))
    {
        ImGui::Checkbox("Disable entire section above"_RU >> u8"Отключить весь раздел выше", &disable_all);
        ImGui::SameLine();
        HelpMarker("Demonstrate using BeginDisabled()/EndDisabled() across this section."_RU >> u8"Продемонстрируйте использование BeginDisabled()/EndDisabled() в этом разделе.");
        ImGui::TreePop();
    }
}

static void ShowDemoWindowLayout()
{
    IMGUI_DEMO_MARKER("Layout"_RU >> u8"Макет");
    if (!ImGui::CollapsingHeader("Layout & Scrolling"_RU >> u8"Макет и прокрутка"))
        return;

    IMGUI_DEMO_MARKER("Layout/Child windows"_RU >> u8"Макет/Дочерние окна");
    if (ImGui::TreeNode("Child windows"_RU >> u8"Дочерние окна"))
    {
        HelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window."_RU >> u8"Используйте дочерние окна, чтобы начать с автономных независимых областей прокрутки/отсечения в основном окне.");
        static bool disable_mouse_wheel = false;
        static bool disable_menu        = false;
        ImGui::Checkbox("Disable Mouse Wheel"_RU >> u8"Отключить колесико мыши", &disable_mouse_wheel);
        ImGui::Checkbox("Disable Menu"_RU >> u8"Отключить меню", &disable_menu);

        // Дочерний элемент 1: нет границы, включить горизонтальную полосу прокрутки
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
            if (disable_mouse_wheel)
                window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
            ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
            for (int i = 0; i < 100; i++)
                ImGui::Text("%04d: scrollable region"_RU >> u8"%04d: прокручиваемая область", i);
            ImGui::EndChild();
        }

        ImGui::SameLine();

        // Дочерний элемент 2: закругленная граница
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
            if (disable_mouse_wheel)
                window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
            if (!disable_menu)
                window_flags |= ImGuiWindowFlags_MenuBar;
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("ChildR", ImVec2(0, 260), true, window_flags);
            if (!disable_menu && ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"_RU >> u8"Меню"))
                {
                    ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            if (ImGui::BeginTable("split"_RU >> u8"расколоть", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
            {
                for (int i = 0; i < 100; i++)
                {
                    char buf[32];
                    sprintf(buf, "%03d", i);
                    ImGui::TableNextColumn();
                    ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
                ImGui::EndTable();
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();
        }

        ImGui::Separator();

        // Продемонстрируйте несколько дополнительных вещей
        // - Изменение ImGuiCol_ChildBg (прозрачный черный цвет в стилях по умолчанию)
        // - Использование SetCursorPos() для позиционирования дочернего окна (дочернее окно является элементом из POV родительского окна)
        // Вы также можете вызвать SetNextWindowPos() для позиционирования дочернего окна. Родительское окно будет эффективно
        // макет с этой позиции.
        // - Использование ImGui:: GetItemRectMin/Max() для запроса состояния "элемент" (поскольку дочернее окно является элементом из
        // POV родительского окна). Подробности см. в разделе «Демонстрация->Статус запроса (отредактировано/активно/зависит и т. д.)».
        {
            static int offset_x = 0;
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            ImGui::DragInt("Offset X", &offset_x, 1.0f, -1000, 1000);

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)offset_x);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
            ImGui::BeginChild("Red"_RU >> u8"Красный", ImVec2(200, 100), true, ImGuiWindowFlags_None);
            for (int n = 0; n < 50; n++)
                ImGui::Text("Some test %d"_RU >> u8"Какой-то тест %d", n);
            ImGui::EndChild();
            bool   child_is_hovered = ImGui::IsItemHovered();
            ImVec2 child_rect_min   = ImGui::GetItemRectMin();
            ImVec2 child_rect_max   = ImGui::GetItemRectMax();
            ImGui::PopStyleColor();
            ImGui::Text("Hovered: %d"_RU >> u8"Наведён: %d", child_is_hovered);
            ImGui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)"_RU >> u8"Прямоугольник дочернего окна: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Widgets Width"_RU >> u8"Макет/Ширина виджетов");
    if (ImGui::TreeNode("Widgets Width"_RU >> u8"Ширина виджетов"))
    {
        static float f                   = 0.0f;
        static bool  show_indented_items = true;
        ImGui::Checkbox("Show indented items"_RU >> u8"Показать элементы с отступом", &show_indented_items);

        // Используйте SetNextItemWidth(), чтобы установить ширину одного предстоящего элемента.
        // Используйте PushItemWidth()/PopItemWidth(), чтобы установить ширину группы элементов.
        // В реальном коде вы, вероятно, захотите выбрать значения ширины, пропорциональные размеру вашего шрифта
        // т. е. г. Использование «20.0f * GetFontSize()» в качестве ширины вместо «200.0f» и т. д.

        ImGui::Text("SetNextItemWidth/PushItemWidth(100)");
        ImGui::SameLine();
        HelpMarker("Fixed width."_RU >> u8"Фиксированная ширина.");
        ImGui::PushItemWidth(100);
        ImGui::DragFloat("float##1b", &f);
        if (show_indented_items)
        {
            ImGui::Indent();
            ImGui::DragFloat("float (indented)##1b", &f);
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();

        ImGui::Text("SetNextItemWidth/PushItemWidth(-100)");
        ImGui::SameLine();
        HelpMarker("Align to right edge minus 100"_RU >> u8"Выровнять по правому краю минус 100");
        ImGui::PushItemWidth(-100);
        ImGui::DragFloat("float##2a", &f);
        if (show_indented_items)
        {
            ImGui::Indent();
            ImGui::DragFloat("float (indented)##2b", &f);
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();

        ImGui::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
        ImGui::SameLine();
        HelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)"_RU >> u8"Половина доступной ширины.\n(~ правый-cursor_pos)\n(работает в наборе столбцов)");
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
        ImGui::DragFloat("float##3a", &f);
        if (show_indented_items)
        {
            ImGui::Indent();
            ImGui::DragFloat("float (indented)##3b", &f);
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();

        ImGui::Text("SetNextItemWidth/PushItemWidth(-GetContentRegionAvail().x * 0.5f)");
        ImGui::SameLine();
        HelpMarker("Align to right edge minus half"_RU >> u8"Выровнять по правому краю минус половина");
        ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
        ImGui::DragFloat("float##4a", &f);
        if (show_indented_items)
        {
            ImGui::Indent();
            ImGui::DragFloat("float (indented)##4b", &f);
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();

        // Продемонстрируйте использование PushItemWidth для окружения трех элементов.
        // Вызов SetNextItemWidth() перед каждым из них будет иметь одинаковый эффект.
        ImGui::Text("SetNextItemWidth/PushItemWidth(-FLT_MIN)");
        ImGui::SameLine();
        HelpMarker("Align to right edge"_RU >> u8"Выровнять по правому краю");
        ImGui::PushItemWidth(-FLT_MIN);
        ImGui::DragFloat("##float5a", &f);
        if (show_indented_items)
        {
            ImGui::Indent();
            ImGui::DragFloat("float (indented)##5b", &f);
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout"_RU >> u8"Макет/Базовый горизонтальный Макет");
    if (ImGui::TreeNode("Basic Horizontal Layout"_RU >> u8"Базовый горизонтальный Макет"))
    {
        ImGui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)"_RU >> u8"(Используйте ImGui:: sameLine(), чтобы продолжать добавлять элементы справа от предыдущего элемента)");

        // Text
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine"_RU >> u8"Макет/Базовый горизонтальный Макет/SameLine");
        ImGui::Text("Two items: Hello"_RU >> u8"Два предмета: Привет");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor"_RU >> u8"Моряк");

        // Adjust spacing
        ImGui::Text("More spacing: Hello"_RU >> u8"Больше интервалов: Привет");
        ImGui::SameLine(0, 20);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor"_RU >> u8"Моряк");

        // Button
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Normal buttons"_RU >> u8"Обычные кнопки");
        ImGui::SameLine();
        ImGui::Button("Banana"_RU >> u8"Банан");
        ImGui::SameLine();
        ImGui::Button("Apple"_RU >> u8"Яблоко");
        ImGui::SameLine();
        ImGui::Button("Corniflower"_RU >> u8"Василек");

        // Button
        ImGui::Text("Small buttons"_RU >> u8"Маленькие кнопки");
        ImGui::SameLine();
        ImGui::SmallButton("Like this one"_RU >> u8"Как этот");
        ImGui::SameLine();
        ImGui::Text("can fit within a text block."_RU >> u8"может поместиться в текстовом блоке.");

        // Выровнен по произвольному положению. Простая/ дешевая колонка.
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (with offset)"_RU >> u8"Макет/Базовый горизонтальный Макет/SameLine (со смещением)");
        ImGui::Text("Aligned"_RU >> u8"Выровнено");
        ImGui::SameLine(150);
        ImGui::Text("x=150");
        ImGui::SameLine(300);
        ImGui::Text("x=300");
        ImGui::Text("Aligned"_RU >> u8"Выровнено");
        ImGui::SameLine(150);
        ImGui::SmallButton("x=150");
        ImGui::SameLine(300);
        ImGui::SmallButton("x=300");

        // Checkbox
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (more)"_RU >> u8"Макет/Базовый горизонтальный Макет/SameLine (подробнее)");
        static bool c1 = false, c2 = false, c3 = false, c4 = false;
        ImGui::Checkbox("My"_RU >> u8"Мой", &c1);
        ImGui::SameLine();
        ImGui::Checkbox("Tailor"_RU >> u8"Портной", &c2);
        ImGui::SameLine();
        ImGui::Checkbox("Is"_RU >> u8"Является", &c3);
        ImGui::SameLine();
        ImGui::Checkbox("Rich"_RU >> u8"Богатым", &c4);

        // Various
        static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
        ImGui::PushItemWidth(80);
        const char* items[] = {"AAAA", "BBBB", "CCCC", "DDDD"};
        static int  item    = -1;
        ImGui::Combo("Combo"_RU >> u8"Комбо", &item, items, IM_ARRAYSIZE(items));
        ImGui::SameLine();
        ImGui::SliderFloat("X", &f0, 0.0f, 5.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("Y", &f1, 0.0f, 5.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("Z", &f2, 0.0f, 5.0f);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(80);
        ImGui::Text("Lists:"_RU >> u8"Списки:");
        static int selection[4] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
            ImGui::PopID();
            //if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
        }
        ImGui::PopItemWidth();

        // Dummy
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Dummy"_RU >> u8"Макет/Базовый горизонтальный Макет/Пустышка");
        ImVec2 button_sz(40, 40);
        ImGui::Button("A", button_sz);
        ImGui::SameLine();
        ImGui::Dummy(button_sz);
        ImGui::SameLine();
        ImGui::Button("B", button_sz);

        // Ручная упаковка
        // (в конечном итоге мы должны предоставить это как функцию автоматического макета, но сейчас вы можете сделать это вручную)
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Manual wrapping"_RU >> u8"Макет/Базовый горизонтальный Макет/Ручная упаковка");
        ImGui::Text("Manual wrapping:"_RU >> u8"Ручная упаковка:");
        ImGuiStyle& style             = ImGui::GetStyle();
        int         buttons_count     = 20;
        float       window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (int n = 0; n < buttons_count; n++)
        {
            ImGui::PushID(n);
            ImGui::Button("Box", button_sz);
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;   // Ожидаемая позиция, если следующая кнопка находится на той же строке
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Groups"_RU >> u8"Макет/Группы");
    if (ImGui::TreeNode("Groups"_RU >> u8"Группы"))
    {
        HelpMarker("BeginGroup() basically locks the horizontal position for new line. "
                   "EndGroup() bundles the whole group so that you can use \"item\" functions such as "
                   "IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group."_RU >>
            u8"BeginGroup() в основном блокирует горизонтальное положение для новой строки. "
            "EndGroup() связывает всю группу, так что вы можете использовать функции \"item\", такие как "
            "IsItemHovered()/IsItemActive() или SameLine() и т. д. на всю группу.");
        ImGui::BeginGroup();
        {
            ImGui::BeginGroup();
            ImGui::Button("AAA");
            ImGui::SameLine();
            ImGui::Button("BBB");
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Button("CCC");
            ImGui::Button("DDD");
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::Button("EEE");
            ImGui::EndGroup();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("First group hovered"_RU >> u8"Наведено Первая группа");
        }
        // Захватите размер группы и создайте виджеты, используя тот же размер
        ImVec2      size      = ImGui::GetItemRectSize();
        const float values[5] = {0.5f, 0.20f, 0.80f, 0.60f, 0.25f};
        ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        ImGui::Button("ACTION"_RU >> u8"ДЕЙСТВИЕ", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        ImGui::SameLine();
        ImGui::Button("REACTION"_RU >> u8"РЕАКЦИЯ", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::Button("LEVERAGE\nBUZZWORD"_RU >> u8"ИСПОЛЬЗОВАТЬ\nМОДНОЕ СЛОВО", size);
        ImGui::SameLine();

        if (ImGui::BeginListBox("List"_RU >> u8"Список", size))
        {
            ImGui::Selectable("Selected"_RU >> u8"Выбрано", true);
            ImGui::Selectable("Not Selected"_RU >> u8"Не выбрано", false);
            ImGui::EndListBox();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Text Baseline Alignment"_RU >> u8"Макет/Выравнивание текста по базовой линии");
    if (ImGui::TreeNode("Text Baseline Alignment"_RU >> u8"Выравнивание текста по базовой линии"))
    {
        {
            ImGui::BulletText("Text baseline:"_RU >> u8"Базовый текст:");
            ImGui::SameLine();
            HelpMarker("This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. "
                       "Lines only composed of text or \"small\" widgets use less vertical space than lines with framed widgets."_RU >>
                u8"Это проверка вертикального выравнивания, которое применяется к тексту, чтобы оно соответствовало виджетам.. "
                "Строки, состоящие только из текста или \"small\" виджетов, занимают меньше места по вертикали, чем строки с виджетами в рамке..");
            ImGui::Indent();

            ImGui::Text("KO Blahblah");
            ImGui::SameLine();
            ImGui::Button("Some framed item"_RU >> u8"Какой-то предмет в рамке");
            ImGui::SameLine();
            HelpMarker("Baseline of button will look misaligned with text.."_RU >> u8"Базовая линия кнопки будет выглядеть не совмещенной с текстом..");

            // Если ваша строка начинается с текста, вызовите AlignTextToFramePadding(), чтобы выровнять текст по предстоящим виджетам.
            // (поскольку мы не знаем, что будет после оператора Text(), нам нужно переместить базовую линию текста
            // вниз на FramePadding.y раньше времени)
            ImGui::AlignTextToFramePadding();
            ImGui::Text("OK Blahblah");
            ImGui::SameLine();
            ImGui::Button("Some framed item"_RU >> u8"Какой-то предмет в рамке");
            ImGui::SameLine();
            HelpMarker("We call AlignTextToFramePadding() to vertically align the text baseline by +FramePadding.y"_RU >> u8"Мы вызываем AlignTextToFramePadding(), чтобы выровнять базовую линию текста по вертикали от +FramePadding.y");

            // SmallButton() использует тот же вертикальный отступ, что и текст.
            ImGui::Button("TEST##1");
            ImGui::SameLine();
            ImGui::Text("TEST");
            ImGui::SameLine();
            ImGui::SmallButton("TEST##2");

            // Если ваша строка начинается с текста, вызовите AlignTextToFramePadding(), чтобы выровнять текст по предстоящим виджетам.
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Text aligned to framed item"_RU >> u8"Текст выровнен по элементу в рамке");
            ImGui::SameLine();
            ImGui::Button("Item##1");
            ImGui::SameLine();
            ImGui::Text("Item");
            ImGui::SameLine();
            ImGui::SmallButton("Item##2");
            ImGui::SameLine();
            ImGui::Button("Item##3");

            ImGui::Unindent();
        }

        ImGui::Spacing();

        {
            ImGui::BulletText("Multi-line text:"_RU >> u8"Mмногострочный текст:");
            ImGui::Indent();
            ImGui::Text("One\nTwo\nThree");
            ImGui::SameLine();
            ImGui::Text("Hello\nWorld");
            ImGui::SameLine();
            ImGui::Text("Banana");

            ImGui::Text("Banana");
            ImGui::SameLine();
            ImGui::Text("Hello\nWorld");
            ImGui::SameLine();
            ImGui::Text("One\nTwo\nThree");

            ImGui::Button("HOP##1");
            ImGui::SameLine();
            ImGui::Text("Banana");
            ImGui::SameLine();
            ImGui::Text("Hello\nWorld");
            ImGui::SameLine();
            ImGui::Text("Banana");

            ImGui::Button("HOP##2");
            ImGui::SameLine();
            ImGui::Text("Hello\nWorld");
            ImGui::SameLine();
            ImGui::Text("Banana");
            ImGui::Unindent();
        }

        ImGui::Spacing();

        {
            ImGui::BulletText("Misc items:"_RU >> u8"Разные Элементы:");
            ImGui::Indent();

            // SmallButton() устанавливает FramePadding в ноль. Базовая линия текста выравнивается по базовой линии предыдущей кнопки.
            ImGui::Button("80x80", ImVec2(80, 80));
            ImGui::SameLine();
            ImGui::Button("50x50", ImVec2(50, 50));
            ImGui::SameLine();
            ImGui::Button("Button()");
            ImGui::SameLine();
            ImGui::SmallButton("SmallButton()");

            // Дерево
            const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::Button("Button##1");
            ImGui::SameLine(0.0f, spacing);
            if (ImGui::TreeNode("Node##1"))
            {
                // Данные дерева заполнителей
                for (int i = 0; i < 6; i++)
                    ImGui::BulletText("Item %d.."_RU >> u8"Элемент %d..", i);
                ImGui::TreePop();
            }

            // Выровняйте текстовый узел по вертикали немного ниже, чтобы он был вертикально центрирован с предстоящим виджетом.
            // В противном случае вы можете использовать SmallButton() (меньше подходит).
            ImGui::AlignTextToFramePadding();

            // Распространенная ошибка, которой следует избегать: если мы хотим использовать SameLine после TreeNode,
            // нам нужно сделать это до того, как мы добавим другое содержимое ниже узла.
            bool node_open = ImGui::TreeNode("Node##2");
            ImGui::SameLine(0.0f, spacing);
            ImGui::Button("Button##2");
            if (node_open)
            {
                // Данные дерева заполнителей
                for (int i = 0; i < 6; i++)
                    ImGui::BulletText("Item %d.."_RU >> u8"Элемент %d..", i);
                ImGui::TreePop();
            }

            // Bullet
            ImGui::Button("Button##3");
            ImGui::SameLine(0.0f, spacing);
            ImGui::BulletText("Bullet text"_RU >> u8"Текст маркера");

            ImGui::AlignTextToFramePadding();
            ImGui::BulletText("Node"_RU >> u8"Узел");
            ImGui::SameLine(0.0f, spacing);
            ImGui::Button("Button##4");
            ImGui::Unindent();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Scrolling"_RU >> u8"Макет/Прокрутка");
    if (ImGui::TreeNode("Scrolling"_RU >> u8"Прокрутка"))
    {
        // Функции вертикальной прокрутки
        IMGUI_DEMO_MARKER("Layout/Scrolling/Vertical"_RU >> u8"Макет/Прокрутка/Вертикальный");
        HelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position."_RU >> u8"Используйте SetScrollHereY() или SetScrollFromPosY() для прокрутки до заданной вертикальной позиции..");

        static int   track_item               = 50;
        static bool  enable_track             = true;
        static bool  enable_extra_decorations = false;
        static float scroll_to_off_px         = 0.0f;
        static float scroll_to_pos_px         = 200.0f;

        ImGui::Checkbox("Decoration"_RU >> u8"Декорация", &enable_extra_decorations);

        ImGui::Checkbox("Track"_RU >> u8"Трэк", &enable_track);
        ImGui::PushItemWidth(100);
        ImGui::SameLine(140);
        enable_track |= ImGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

        bool scroll_to_off = ImGui::Button("Scroll Offset"_RU >> u8"Смещение прокрутки");
        ImGui::SameLine(140);
        scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");

        bool scroll_to_pos = ImGui::Button("Scroll To Pos"_RU >> u8"Прокрутка до позиции");
        ImGui::SameLine(140);
        scroll_to_pos |= ImGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, FLT_MAX, "X/Y = %.0f px");
        ImGui::PopItemWidth();

        if (scroll_to_off || scroll_to_pos)
            enable_track = false;

        ImGuiStyle& style   = ImGui::GetStyle();
        float       child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
        if (child_w < 1.0f)
            child_w = 1.0f;
        ImGui::PushID("##VerticalScrolling");
        for (int i = 0; i < 5; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::BeginGroup();
            const char* names[] = {"Top", "25%", "Center", "75%", "Bottom"};
            ImGui::TextUnformatted(names[i]);

            const ImGuiWindowFlags child_flags      = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
            const ImGuiID          child_id         = ImGui::GetID((void*)(intptr_t)i);
            const bool             child_is_visible = ImGui::BeginChild(child_id, ImVec2(child_w, 200.0f), true, child_flags);
            if (ImGui::BeginMenuBar())
            {
                ImGui::TextUnformatted("abc");
                ImGui::EndMenuBar();
            }
            if (scroll_to_off)
                ImGui::SetScrollY(scroll_to_off_px);
            if (scroll_to_pos)
                ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible)   // Избегайте вызова SetScrollHereY при работе с отобранными элементами.
            {
                for (int item = 0; item < 100; item++)
                {
                    if (enable_track && item == track_item)
                    {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d"_RU >> u8"Элемент %d", item);
                        ImGui::SetScrollHereY(i * 0.25f);   // 0.0f:top, 0.5f:center, 1.0f:bottom
                    }
                    else
                    {
                        ImGui::Text("Item %d"_RU >> u8"Элемент %d", item);
                    }
                }
            }
            float scroll_y     = ImGui::GetScrollY();
            float scroll_max_y = ImGui::GetScrollMaxY();
            ImGui::EndChild();
            ImGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
            ImGui::EndGroup();
        }
        ImGui::PopID();

        // Функции горизонтальной прокрутки
        IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal"_RU >> u8"Макет/Прокрутка/Горизонтальный");
        ImGui::Spacing();
        HelpMarker("Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\n"
                   "Because the clipping rectangle of most window hides half worth of WindowPadding on the "
                   "left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the "
                   "equivalent SetScrollFromPosY(+1) wouldn't."_RU >>
            u8"Используйте SetScrollHereX() или SetScrollFromPosX() для прокрутки до заданной горизонтальной позиции..\n\n"
            "Поскольку прямоугольник обрезки большинства окон скрывает половину значения WindowPadding на "
            "влево/вправо, использование SetScrollFromPosX(+1) обычно приводит к обрезанию текста, тогда как "
            "эквивалентный SetScrollFromPosY(+1) не будет.");
        ImGui::PushID("##HorizontalScrolling");
        for (int i = 0; i < 5; i++)
        {
            float            child_height     = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
            ImGuiWindowFlags child_flags      = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
            ImGuiID          child_id         = ImGui::GetID((void*)(intptr_t)i);
            bool             child_is_visible = ImGui::BeginChild(child_id, ImVec2(-100, child_height), true, child_flags);
            if (scroll_to_off)
                ImGui::SetScrollX(scroll_to_off_px);
            if (scroll_to_pos)
                ImGui::SetScrollFromPosX(ImGui::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible)   // Избегайте вызова SetScrollHereY при работе с отобранными элементами.
            {
                for (int item = 0; item < 100; item++)
                {
                    if (item > 0)
                        ImGui::SameLine();
                    if (enable_track && item == track_item)
                    {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d"_RU >> u8"Элемент %d", item);
                        ImGui::SetScrollHereX(i * 0.25f);   // 0.0f:left, 0.5f:center, 1.0f:right
                    }
                    else
                    {
                        ImGui::Text("Item %d"_RU >> u8"Элемент %d", item);
                    }
                }
            }
            float scroll_x     = ImGui::GetScrollX();
            float scroll_max_x = ImGui::GetScrollMaxX();
            ImGui::EndChild();
            ImGui::SameLine();
            const char* names[] = {"Left"_RU >> u8"Левый", "25%", "Center"_RU >> u8"Центр", "75%", "Right"_RU >> u8"Правый"};
            ImGui::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
            ImGui::Spacing();
        }
        ImGui::PopID();

        // Различные Демонстрации горизонтальной прокрутки
        IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal (more)"_RU >> u8"Макет/Прокрутка/Горизонтальный (подробнее)");
        HelpMarker("Horizontal scrolling for a window is enabled via the ImGuiWindowFlags_HorizontalScrollbar flag.\n\n"
                   "You may want to also explicitly specify content width by using SetNextWindowContentWidth() before Begin()."_RU >>
            u8"Горизонтальная прокрутка для окна включается с помощью флага ImGuiWindowFlags_HorizontalScrollbar.\n\n"
            "Вы также можете явно указать ширину содержимого, используя SetNextWindowContentWidth() перед Begin().");
        static int lines = 7;
        ImGui::SliderInt("Lines"_RU >> u8"Линии", &lines, 1, 15);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
        ImGui::BeginChild("scrolling"_RU >> u8"прокрутка", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Показать случайные вещи. Ради этой тривиальной демонстрации мы используем базовую функцию Button() + SameLine().
            // Если вы хотите создать собственную временную шкалу для реального приложения, возможно, вам лучше манипулировать
            // положение курсора самостоятельно, также как использование SetCursorPos/SetCursorScreenPos для позиционирования виджетов
            // сам. Вы также можете использовать API ImDrawList более низкого уровня.
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0)
                    ImGui::SameLine();
                ImGui::PushID(n + line * 1000);
                char num_buf[16];
                sprintf(num_buf, "%d", n);
                const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
                float       hue   = n * 0.05f;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }
        float scroll_x     = ImGui::GetScrollX();
        float scroll_max_x = ImGui::GetScrollMaxX();
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        float scroll_x_delta = 0.0f;
        ImGui::SmallButton("<<");
        if (ImGui::IsItemActive())
            scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f;
        ImGui::SameLine();
        ImGui::Text("Scroll from code"_RU >> u8"Прокрутить код");
        ImGui::SameLine();
        ImGui::SmallButton(">>");
        if (ImGui::IsItemActive())
            scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f;
        ImGui::SameLine();
        ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
        if (scroll_x_delta != 0.0f)
        {
            // Продемонстрируйте трюк: вы можете использовать Begin, чтобы установить себя в контексте другого окна
            // (здесь мы уже вышли из вашего дочернего окна)
            ImGui::BeginChild("scrolling"_RU >> u8"прокрутка");
            ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
            ImGui::EndChild();
        }
        ImGui::Spacing();

        static bool show_horizontal_contents_size_demo_window = false;
        ImGui::Checkbox("Show Horizontal contents size demo window"_RU >> u8"Показать Горизонтальный размер содержимого демонстрационного окна", &show_horizontal_contents_size_demo_window);

        if (show_horizontal_contents_size_demo_window)
        {
            static bool  show_h_scrollbar      = true;
            static bool  show_button           = true;
            static bool  show_tree_nodes       = true;
            static bool  show_text_wrapped     = false;
            static bool  show_columns          = true;
            static bool  show_tab_bar          = true;
            static bool  show_child            = false;
            static bool  explicit_content_size = false;
            static float contents_size_x       = 300.0f;
            if (explicit_content_size)
                ImGui::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
            ImGui::Begin("Horizontal contents size demo window"_RU >> u8"Горизонтальный размер содержимого демонстрационного окна", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? ImGuiWindowFlags_HorizontalScrollbar : 0);

            IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal contents size demo window"_RU >> u8"Макет/Прокрутка/Горизонтальный размер содержимого демонстрационного окна");
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
            HelpMarker("Test of different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\nUse 'Metrics->Tools->Show windows rectangles' to visualize rectangles."_RU >> u8"Проверка различных виджетов, которые реагируют и влияют на рост рабочего прямоугольника при включении горизонтальной прокрутки..\n\nИспользуйте «Метрики->Инструменты->Показать прямоугольники окон», чтобы визуализировать прямоугольники.");
            ImGui::Checkbox("H-scrollbar"_RU >> u8"H-полоса прокрутки", &show_h_scrollbar);
            ImGui::Checkbox("Button"_RU >> u8"Кнопка", &show_button);                       // Будет увеличиваться размер содержимого (если явно не перезаписано)
            ImGui::Checkbox("Tree nodes"_RU >> u8"Узлы дерева", &show_tree_nodes);          // Будет увеличиваться размер содержимого и отображаться подсветка по всей ширине
            ImGui::Checkbox("Text wrapped"_RU >> u8"Перенос текста", &show_text_wrapped);   // Будет расти и использовать размер содержимого
            ImGui::Checkbox("Columns"_RU >> u8"Столбцы", &show_columns);                    // Будет использоваться размер содержимого
            ImGui::Checkbox("Tab bar"_RU >> u8"Панель вкладок", &show_tab_bar);             // Будет использоваться размер содержимого
            ImGui::Checkbox("Child"_RU >> u8"Дочерний", &show_child);                       // Будет расти и использовать размер содержимого
            ImGui::Checkbox("Explicit content size"_RU >> u8"Явный размер содержимого", &explicit_content_size);
            ImGui::Text("Scroll %.1f/%.1f %.1f/%.1f"_RU >> u8"Прокрутить %.1f/%.1f %.1f/%.1f", ImGui::GetScrollX(), ImGui::GetScrollMaxX(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
            if (explicit_content_size)
            {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(100);
                ImGui::DragFloat("##csx", &contents_size_x);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
                ImGui::Dummy(ImVec2(0, 10));
            }
            ImGui::PopStyleVar(2);
            ImGui::Separator();
            if (show_button)
            {
                ImGui::Button("this is a 300-wide button"_RU >> u8"это кнопка шириной 300", ImVec2(300, 0));
            }
            if (show_tree_nodes)
            {
                bool open = true;
                if (ImGui::TreeNode("this is a tree node"_RU >> u8"это узел дерева"))
                {
                    if (ImGui::TreeNode("another one of those tree node..."_RU >> u8"еще один из этих узлов дерева..."))
                    {
                        ImGui::Text("Some tree contents"_RU >> u8"Некоторое содержание дерева");
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::CollapsingHeader("CollapsingHeader", &open);
            }
            if (show_text_wrapped)
            {
                ImGui::TextWrapped("This text should automatically wrap on the edge of the work rectangle."_RU >> u8"Этот текст должен автоматически переноситься на край рабочего прямоугольника.");
            }
            if (show_columns)
            {
                ImGui::Text("Tables:"_RU >> u8"Таблицы:");
                if (ImGui::BeginTable("table"_RU >> u8"Таблица", 4, ImGuiTableFlags_Borders))
                {
                    for (int n = 0; n < 4; n++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Width %.2f"_RU >> u8"Ширина %.2f", ImGui::GetContentRegionAvail().x);
                    }
                    ImGui::EndTable();
                }
                ImGui::Text("Columns:"_RU >> u8"Столбцы:");
                ImGui::Columns(4);
                for (int n = 0; n < 4; n++)
                {
                    ImGui::Text("Width %.2f"_RU >> u8"Ширина %.2f", ImGui::GetColumnWidth());
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
            }
            if (show_tab_bar && ImGui::BeginTabBar("Hello"_RU >> u8"Привет"))
            {
                if (ImGui::BeginTabItem("OneOneOne"_RU >> u8"ОдинОдинОдин"))
                {
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("TwoTwoTwo"_RU >> u8"ДваДваДва"))
                {
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("ThreeThreeThree"_RU >> u8"ТриТриТри"))
                {
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("FourFourFour"_RU >> u8"ЧетыреЧетыреЧетыре"))
                {
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            if (show_child)
            {
                ImGui::BeginChild("child"_RU >> u8"дочерний", ImVec2(0, 0), true);
                ImGui::EndChild();
            }
            ImGui::End();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Clipping"_RU >> u8"Макет/Обрезка");
    if (ImGui::TreeNode("Clipping"_RU >> u8"Обрезка"))
    {
        static ImVec2 size(100.0f, 100.0f);
        static ImVec2 offset(30.0f, 30.0f);
        ImGui::DragFloat2("size"_RU >> u8"размер", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
        ImGui::TextWrapped("(Click and drag to scroll)"_RU >> u8"(Нажмите и перетащите для прокрутки)");

        for (int n = 0; n < 3; n++)
        {
            if (n > 0)
                ImGui::SameLine();
            ImGui::PushID(n);
            ImGui::BeginGroup();   // Заблокировать положение X

            ImGui::InvisibleButton("##empty", size);
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                offset.x += ImGui::GetIO().MouseDelta.x;
                offset.y += ImGui::GetIO().MouseDelta.y;
            }
            const ImVec2 p0        = ImGui::GetItemRectMin();
            const ImVec2 p1        = ImGui::GetItemRectMax();
            const char*  text_str  = "Line 1 hello\nLine 2 clip me!"_RU >> u8"1 строка привет\n2 строка обрежьте меня!";
            const ImVec2 text_pos  = ImVec2(p0.x + offset.x, p0.y + offset.y);
            ImDrawList*  draw_list = ImGui::GetWindowDrawList();

            switch (n)
            {
                case 0:
                    HelpMarker("Using ImGui::PushClipRect():\n"
                               "Will alter ImGui hit-testing logic + ImDrawList rendering.\n"
                               "(use this if you want your clipping rectangle to affect interactions)"_RU >>
                        u8"Использование ImGui:: PushClipRect():\n"
                        "Будет изменена логика проверки попаданий ImGui + рендеринг ImDrawList.\n"
                        "(используйте это, если вы хотите, чтобы ваш прямоугольник обрезки влиял на взаимодействие)");
                    ImGui::PushClipRect(p0, p1, true);
                    draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                    draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                    ImGui::PopClipRect();
                    break;
                case 1:
                    HelpMarker("Using ImDrawList::PushClipRect():\n"
                               "Will alter ImDrawList rendering only.\n"
                               "(use this as a shortcut if you are only using ImDrawList calls)"_RU >>
                        u8"Использование ImDrawList::PushClipRect():\n"
                        "Изменит только рендеринг ImDrawList.\n"
                        "(используйте это как ярлык, если вы используете только вызовы ImDrawList)");
                    draw_list->PushClipRect(p0, p1, true);
                    draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                    draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                    draw_list->PopClipRect();
                    break;
                case 2:
                    HelpMarker("Using ImDrawList::AddText() with a fine ClipRect:\n"
                               "Will alter only this specific ImDrawList::AddText() rendering.\n"
                               "(this is often used internally to avoid altering the clipping rectangle and minimize draw calls)"_RU >>
                        u8"Использование ImDrawList::AddText() с прекрасным ClipRect:\n"
                        "Изменит только этот конкретный рендеринг ImDrawList::AddText().\n"
                        "(это часто используется внутри, чтобы избежать изменения прямоугольника отсечения и минимизировать вызовы отрисовки.)");
                    ImVec4 clip_rect(p0.x, p0.y, p1.x, p1.y);   // AddText() принимает здесь ImVec4*, так что давайте конвертируем.
                    draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                    draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), text_pos, IM_COL32_WHITE, text_str, NULL, 0.0f, &clip_rect);
                    break;
            }
            ImGui::EndGroup();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

static void ShowDemoWindowPopups()
{
    IMGUI_DEMO_MARKER("Popups"_RU >> u8"Всплывающие окна");
    if (!ImGui::CollapsingHeader("Popups & Modal windows"_RU >> u8"Всплывающие и модальные окна"))
        return;

    // Свойства всплывающих окон:
    // - Они блокируют нормальное обнаружение зависания мыши за их пределами. (*)
    // - Если они не модальные, их можно закрыть, щелкнув в любом месте за их пределами или нажав ESCAPE.
    // - Их состояние видимости (~bool) поддерживается внутренне Дорогой ImGui, а не программистом как
    //   мы привыкли к обычным вызовам Begin(). Пользователь может управлять состоянием видимости, вызывая OpenPopup().
    // (*) Можно использовать IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup), чтобы обойти его и обнаружить зависание даже
    //     когда обычно блокируется всплывающим окном.
    // Эти три свойства связаны. Библиотека должна сохранять свое состояние видимости, ПОТОМУ ЧТО она может закрыться.
    // всплывающие окна в любое время.

    // Типичное использование для обычных окон:
    //   bool my_tool_is_active = false; if (ImGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Типичное использование всплывающих окон:
    //   if (ImGui::Button("Open")) ImGui::OpenPopup("MyPopup"); if (ImGui::BeginPopup("MyPopup") { [...] EndPopup(); }

    // С всплывающими окнами мы должны пройти вызов библиотеки (здесь OpenPopup), чтобы управлять состоянием видимости.
    // Сначала это может немного сбивать с толку, но вскоре все становится понятным. Следуйте приведенным ниже примерам.

    IMGUI_DEMO_MARKER("Popups/Popups"_RU >> u8"Всплывающие окна/Всплывающие окна");
    if (ImGui::TreeNode("Popups"_RU >> u8"Всплывающие окна"))
    {
        ImGui::TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. "
                           "Clicking outside the popup closes it."_RU >>
            u8"Когда всплывающее окно активно, оно запрещает взаимодействие с окнами, которые находятся за всплывающим окном.. "
            "Щелчок за пределами всплывающего окна закрывает его.");

        static int  selected_fish = -1;
        const char* names[]       = {"Bream"_RU >> u8"Лещ", "Haddock"_RU >> u8"Пикша", "Mackerel"_RU >> u8"Скумбрия", "Pollock"_RU >> u8"Минтай", "Tilefish"_RU >> u8"Черепичная рыба"};
        static bool toggles[]     = {true, false, false, false, false};

        // Простое всплывающее окно выбора (если вы хотите показать текущий выбор внутри самой кнопки,
        // вы можете создать строку с помощью оператора "###", чтобы сохранить идентификатор константы с меткой переменной)
        if (ImGui::Button("Select.."_RU >> u8"Выбрать.."))
            ImGui::OpenPopup("my_select_popup");
        ImGui::SameLine();
        ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (ImGui::BeginPopup("my_select_popup"))
        {
            ImGui::Text("Aquarium"_RU >> u8"Аквариум");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_fish = i;
            ImGui::EndPopup();
        }

        // Отображение меню с переключателями
        if (ImGui::Button("Toggle.."_RU >> u8"Тумблер.."))
            ImGui::OpenPopup("my_toggle_popup");
        if (ImGui::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ImGui::MenuItem(names[i], "", &toggles[i]);
            if (ImGui::BeginMenu("Sub-menu"_RU >> u8"Подменю"))
            {
                ImGui::MenuItem("Click me"_RU >> u8"Нажми на меня");
                ImGui::EndMenu();
            }

            ImGui::Separator();
            ImGui::Text("Tooltip here"_RU >> u8"Подсказка здесь");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("I am a tooltip over a popup"_RU >> u8"Я всплывающая подсказка над всплывающим окном");

            if (ImGui::Button("Stacked Popup"_RU >> u8"Сложенное всплывающее окно"))
                ImGui::OpenPopup("another popup"_RU >> u8"другое всплывающее окно");
            if (ImGui::BeginPopup("another popup"_RU >> u8"другое всплывающее окно"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);
                if (ImGui::BeginMenu("Sub-menu"_RU >> u8"Подменю"))
                {
                    ImGui::MenuItem("Click me"_RU >> u8"Нажми на меня");
                    if (ImGui::Button("Stacked Popup"_RU >> u8"Сложенное всплывающее окно"))
                        ImGui::OpenPopup("another popup"_RU >> u8"другое всплывающее окно");
                    if (ImGui::BeginPopup("another popup"_RU >> u8"другое всплывающее окно"))
                    {
                        ImGui::Text("I am the last one here."_RU >> u8"Я здесь последний.");
                        ImGui::EndPopup();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
            ImGui::EndPopup();
        }

        // Вызовите более полный ShowExampleMenuFile, который мы используем в разных местах этой демонстрации.
        if (ImGui::Button("With a menu.."_RU >> u8"С меню.."))
            ImGui::OpenPopup("my_file_popup");
        if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
                {
                    ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"_RU >> u8"Правка"))
                {
                    ImGui::MenuItem("Dummy"_RU >> u8"Пустышка");
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello from popup!"_RU >> u8"Привет из всплывающего окна!");
            ImGui::Button("This is a dummy button.."_RU >> u8"это фиктивная кнопка..");
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Context menus"_RU >> u8"Всплывающие окна/Контекстные меню");
    if (ImGui::TreeNode("Context menus"_RU >> u8"Контекстные меню"))
    {
        HelpMarker("\"Context\" functions are simple helpers to associate a Popup to a given Item or Window identifier."_RU >> u8"\"Контекст\" функции — это простые помощники для связывания всплывающего окна с данным идентификатором элемента или окна.");

        // BeginPopupContextItem() является помощником для обеспечения общего/простого поведения всплывающего окна по существу:
        //     if (id == 0)
        //         id = GetItemID(); // Use last item id
        //     if (IsItemHovered() && IsMouseReleased(ImGuiMouseButton_Right))
        //         OpenPopup(id);
        //     return BeginPopup(id);
        // Для расширенного расширенного использования вы можете реплицировать и настроить этот код.
        // Подробнее см. в BeginPopupContextItem().

        // Пример 1
        // При использовании после элемента с идентификатором (например, кнопки) мы можем пропустить предоставление идентификатора для BeginPopupContextItem(),
        // и BeginPopupContextItem() будет использовать идентификатор последнего элемента в качестве идентификатора всплывающего окна.
        {
            const char* names[5] = {"Label1", "Label2", "Label3", "Label4", "Label5"};
            for (int n = 0; n < 5; n++)
            {
                ImGui::Selectable(names[n]);
                if (ImGui::BeginPopupContextItem())   // <-- использовать последний идентификатор элемента в качестве идентификатора всплывающего окна
                {
                    ImGui::Text("This a popup for \"%s\"!"_RU >> u8"Это всплывающее окно для \"%s\"!", names[n]);
                    if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Right-click to open popup"_RU >> u8"Щелкните правой кнопкой мыши, чтобы открыть всплывающее окно");
            }
        }

        // Пример 2
        // Всплывающее окно для элемента Text(), у которого нет идентификатора: нам нужно предоставить идентификатор для BeginPopupContextItem().
        // Использование явного идентификатора также удобно, если вы хотите активировать всплывающие окна из разных мест.
        {
            HelpMarker("Text() elements don't have stable identifiers so we need to provide one."_RU >> u8"Text() элементы не имеют стабильных идентификаторов, поэтому нам нужно их предоставить.");
            static float value = 0.5f;
            ImGui::Text("Value = %.3f <-- (1) right-click this text"_RU >> u8"Значение = %.3f <-- (1) щелкните правой кнопкой мыши этот текст", value);
            if (ImGui::BeginPopupContextItem("my popup"_RU >> u8"мое всплывающее окно"))
            {
                if (ImGui::Selectable("Set to zero"_RU >> u8"Установить на ноль"))
                    value = 0.0f;
                if (ImGui::Selectable("Set to PI"_RU >> u8"Установлен на PI"))
                    value = 3.1415f;
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
                ImGui::EndPopup();
            }

            // Мы также можем использовать OpenPopupOnItemClick() для переключения видимости данного всплывающего окна.
            // Здесь мы делаем так, чтобы щелчок правой кнопкой мыши по этому другому текстовому элементу открывал то же всплывающее окно, что и выше.
            // Само всплывающее окно будет представлено кодом выше.
            ImGui::Text("(2) Or right-click this text"_RU >> u8"(2) Или щелкните правой кнопкой мыши этот текст");
            ImGui::OpenPopupOnItemClick("my popup"_RU >> u8"мое всплывающее окно", ImGuiPopupFlags_MouseButtonRight);

            // Вернемся к исходной точке: вручную откройте то же самое всплывающее окно.
            if (ImGui::Button("(3) Or click this button"_RU >> u8"(3) Или нажмите эту кнопку"))
                ImGui::OpenPopup("my popup"_RU >> u8"мое всплывающее окно");
        }

        // Пример 3
        // При использовании BeginPopupContextItem() с неявным идентификатором (NULL == использовать последний идентификатор элемента),
        // нам нужно убедиться, что ваш идентификатор предмета стабилен.
        // В этом примере мы демонстрируем изменение метки элемента с сохранением его идентификатора с помощью оператора ### (см. FAQ).
        {
            HelpMarker("Showcase using a popup ID linked to item ID, with the item having a changing label + stable ID using the ### operator."_RU >> u8"Демонстрация с использованием идентификатора всплывающего окна, связанного с идентификатором элемента, с элементом, имеющим изменяющуюся метку + стабильный идентификатор с помощью оператора ###.");
            static char name[32] = "Label1";
            char        buf[64];
            sprintf(buf, "Button: %s###Button"_RU >> u8"Кнопка: %s###Button", name);   // ### идентификатор переопределения оператора, игнорирующий предыдущую метку
            ImGui::Button(buf);
            if (ImGui::BeginPopupContextItem())
            {
                ImGui::Text("Edit name:"_RU >> u8"Изменить имя:");
                ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
                if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::SameLine();
            ImGui::Text("(<-- right-click here)"_RU >> u8"(<-- щелкните правой кнопкой мыши здесь)");
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Modals"_RU >> u8"Всплывающие окна/Модальные");
    if (ImGui::TreeNode("Modals"_RU >> u8"Модальные"))
    {
        ImGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside."_RU >> u8"Модальные окна похожи на всплывающие окна, но пользователь не может закрыть их, щелкнув снаружи.");

        if (ImGui::Button("Delete.."_RU >> u8"Удалить.."))
            ImGui::OpenPopup("Delete?"_RU >> u8"Удалить?");

        // Всегда центрировать это окно при появлении
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Delete?"_RU >> u8"Удалить?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n"_RU >> u8"Все эти красивые файлы будут удалены.\nЭта операция не может быть отменена!\n\n");
            ImGui::Separator();

            //static int unused_i = 0;
            //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time"_RU >> u8"Не спрашивай меня в следующий раз", &dont_ask_me_next_time);
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel"_RU >> u8"Отмена", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::Button("Stacked modals.."_RU >> u8"Сложенные модальные окна.."))
            ImGui::OpenPopup("Stacked 1"_RU >> u8"Сложены 1");
        if (ImGui::BeginPopupModal("Stacked 1"_RU >> u8"Сложены 1", NULL, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
                {
                    if (ImGui::MenuItem("Some menu item"_RU >> u8"Какой-то пункт меню"))
                    {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it."_RU >> u8"Привет от Stacked The First\nИспользование стиля. Цвета [ImGuiCol_ModalWindowDimBg] позади него.");

            // Tпищевое поведение виджетов, складывающих свои обычные всплывающие окна поверх модального.
            static int   item     = 1;
            static float color[4] = {0.4f, 0.7f, 0.0f, 0.5f};
            ImGui::Combo("Combo"_RU >> u8"Комбо", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
            ImGui::ColorEdit4("color"_RU >> u8"цвет", color);

            if (ImGui::Button("Add another modal.."_RU >> u8"Добавить еще один модальный.."))
                ImGui::OpenPopup("Stacked 2"_RU >> u8"Сложены 2");

            // Также продемонстрируйте передачу bool* в BeginPopupModal(), это создаст обычную кнопку закрытия, которая
            // закроет всплывающее окно. Обратите внимание, что состояние видимости всплывающих окон принадлежит imgui, поэтому входное значение
            // bool на самом деле здесь не имеет значения.
            bool unused_open = true;
            if (ImGui::BeginPopupModal("Stacked 2"_RU >> u8"Сложены 2", &unused_open))
            {
                ImGui::Text("Hello from Stacked The Second!"_RU >> u8"Привет от Stacked The Second!");
                if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Menus inside a regular window"_RU >> u8"Всплывающие окна/Меню внутри обычного окна");
    if (ImGui::TreeNode("Menus inside a regular window"_RU >> u8"Меню внутри обычного окна"))
    {
        ImGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!"_RU >> u8"Ниже мы тестируем добавление пунктов меню в обычное окно. Это довольно необычно, но должно работать!");
        ImGui::Separator();

        // Примечание. В качестве причуды в этом очень конкретном примере мы хотим отличить родительский элемент этого меню от
        // родитель различных всплывающих меню выше. Для этого мы заключаем элементы в блок PushID()/PopID().
        // чтобы сделать их двумя разными наборами меню. Если мы этого не сделаем, открытие любого всплывающего окна выше и наведение курсора на наше меню здесь приведет к
        // Открой это. Это связано с тем, что когда меню активно, мы позволяем переключиться на родственное меню, просто наведя на него курсор.
        // что является желаемым поведением для обычных меню.
        ImGui::PushID("foo");
        ImGui::MenuItem("Menu item", "CTRL+M"_RU >> u8"Пункт меню", "CTRL+M");
        if (ImGui::BeginMenu("Menu inside a regular window"_RU >> u8"Меню внутри обычного окна"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        ImGui::PopID();
        ImGui::Separator();
        ImGui::TreePop();
    }
}

// Фиктивная структура данных, которую мы используем для демонстрации таблицы.
// (pre-C++11 не позволяет нам создавать экземпляр шаблона ImVector<MyItem>, если эта структура определена внутри демо-функции)
namespace
{
    // Мы передаем наш собственный идентификатор в TableSetupColumn(), чтобы упростить идентификацию столбцов в коде сортировки.
    // Этот идентификатор будет передан в ImGuiTableSortSpec:: ColumnUserID.
    // Но можно опустить параметр идентификатора пользователя в TableSetupColumn() и просто использовать вместо него индекс столбца! (ImGuiTableSortSpec:: ColumnIndex)
    // Если вы не используете сортировку, вы, как правило, никогда не будете заботиться о присвоении столбцу идентификатора!
    enum MyItemColumnID
    {
        MyItemColumnID_ID,
        MyItemColumnID_Name,
        MyItemColumnID_Action,
        MyItemColumnID_Quantity,
        MyItemColumnID_Description
    };

    struct MyItem
    {
        int                               ID;
        const char*                       Name;
        int                               Quantity;

        // У нас есть проблема, которая затрагивает _только эту демонстрацию_ и не должна влиять на ваш код:
        // Поскольку мы не полагаемся на std:: или другую стороннюю библиотеку для компиляции дорогого imgui, у нас есть надежный доступ только к qsort(),
        // однако qsort не позволяет передавать пользовательские данные в функцию сравнения.
        // В качестве обходного пути мы сохраняем спецификации сортировки в static/global для доступа к функции сравнения.
        // В вашем собственном случае использования вы, вероятно, передадите спецификации сортировки своим функциям сортировки/сравнения напрямую, а не будете использовать глобальную переменную.
        // Технически мы могли бы вызвать ImGui::TableGetSortSpecs() в CompareWithSortSpecs(), но учитывая, что эта функция вызывается
        // очень часто по алгоритму сортировки это было бы немного расточительно.
        static const ImGuiTableSortSpecs* s_current_sort_specs;

        // Функция сравнения, которую будет использовать qsort()
        static int IMGUI_CDECL            CompareWithSortSpecs(const void* lhs, const void* rhs)
        {
            const MyItem* a = (const MyItem*)lhs;
            const MyItem* b = (const MyItem*)rhs;
            for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
            {
                // Здесь мы идентифицируем столбцы, используя значение ColumnUserID, которое мы сами передали в TableSetupColumn().
                // Мы также можем выбрать идентификацию столбцов на основе их индекса (sort_spec->ColumnIndex), что проще!
                const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
                int                              delta     = 0;
                switch (sort_spec->ColumnUserID)
                {
                    case MyItemColumnID_ID:
                        delta = (a->ID - b->ID);
                        break;
                    case MyItemColumnID_Name:
                        delta = (strcmp(a->Name, b->Name));
                        break;
                    case MyItemColumnID_Quantity:
                        delta = (a->Quantity - b->Quantity);
                        break;
                    case MyItemColumnID_Description:
                        delta = (strcmp(a->Name, b->Name));
                        break;
                    default:
                        IM_ASSERT(0);
                        break;
                }
                if (delta > 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
                if (delta < 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
            }

            // qsort() нестабилен, поэтому всегда возвращайте способ различать элементы.
            // Ваша собственная функция сравнения может захотеть избежать возврата к неявным спецификациям сортировки e. г. сравнение имени, если оно еще не было частью спецификаций сортировки.
            return (a->ID - b->ID);
        }
    };
    const ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;
}   // namespace

// Сделайте пользовательский интерфейс компактным, потому что в нем так много полей.
static void PushStyleCompact()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

static void PopStyleCompact()
{
    ImGui::PopStyleVar(2);
}

// Показать поле со списком с выбором политики размеров
static void EditTableSizingFlags(ImGuiTableFlags* p_flags)
{
    struct EnumDesc
    {
        ImGuiTableFlags Value;
        const char*     Name;
        const char*     Tooltip;
    };
    static const EnumDesc policies[] = {{ImGuiTableFlags_None, "Default"_RU >> u8"По умолчанию",
                                            "Use default sizing policy:\n- ImGuiTableFlags_SizingFixedFit if ScrollX is on or if host window has ImGuiWindowFlags_AlwaysAutoResize.\n- ImGuiTableFlags_SizingStretchSame otherwise."_RU >> u8"Использовать политику размеров по умолчанию:\n- ImGuiTableFlags_SizingFixedFit если ScrollX включен или если окно хоста ImGuiWindowFlags_AlwaysAutoResize.\n- ImGuiTableFlags_SizingStretchSame в противном случае."},
        {ImGuiTableFlags_SizingFixedFit, "ImGuiTableFlags_SizingFixedFit", "Columns default to _WidthFixed (if resizable) or _WidthAuto (if not resizable), matching contents width."_RU >> u8"Столбцы по умолчанию имеют значение _WidthFixed (если размер может изменяться) или _WidthAuto (если размер не изменяется), что соответствует ширине содержимого."},
        {ImGuiTableFlags_SizingFixedSame, "ImGuiTableFlags_SizingFixedSame", "Columns are all the same width, matching the maximum contents width.\nImplicitly disable ImGuiTableFlags_Resizable and enable ImGuiTableFlags_NoKeepColumnsVisible."_RU >> u8"Все столбцы имеют одинаковую ширину, соответствующую максимальной ширине содержимого.\nНеявно отключите ImGuiTableFlags_Resizable и включите ImGuiTableFlags_NoKeepColumnsVisible."},
        {ImGuiTableFlags_SizingStretchProp, "ImGuiTableFlags_SizingStretchProp", "Columns default to _WidthStretch with weights proportional to their widths."_RU >> u8"Столбцы по умолчанию имеют значение _WidthStretch с весами, пропорциональными их ширине."}, {ImGuiTableFlags_SizingStretchSame, "ImGuiTableFlags_SizingStretchSame", "Columns default to _WidthStretch with same weights."_RU >> u8"Столбцы по умолчанию имеют значение _WidthStretch с одинаковыми весами."}};
    int                   idx;
    for (idx = 0; idx < IM_ARRAYSIZE(policies); idx++)
        if (policies[idx].Value == (*p_flags & ImGuiTableFlags_SizingMask_))
            break;
    const char* preview_text = (idx < IM_ARRAYSIZE(policies)) ? policies[idx].Name + (idx > 0 ? strlen("ImGuiTableFlags") : 0) : "";
    if (ImGui::BeginCombo("Sizing Policy", preview_text))
    {
        for (int n = 0; n < IM_ARRAYSIZE(policies); n++)
            if (ImGui::Selectable(policies[n].Name, idx == n))
                *p_flags = (*p_flags & ~ImGuiTableFlags_SizingMask_) | policies[n].Value;
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 50.0f);
        for (int m = 0; m < IM_ARRAYSIZE(policies); m++)
        {
            ImGui::Separator();
            ImGui::Text("%s:", policies[m].Name);
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().IndentSpacing * 0.5f);
            ImGui::TextUnformatted(policies[m].Tooltip);
        }
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void EditTableColumnsFlags(ImGuiTableColumnFlags* p_flags)
{
    ImGui::CheckboxFlags("_Disabled", p_flags, ImGuiTableColumnFlags_Disabled);
    ImGui::SameLine();
    HelpMarker("Master disable flag (also hide from context menu)");
    ImGui::CheckboxFlags("_DefaultHide", p_flags, ImGuiTableColumnFlags_DefaultHide);
    ImGui::CheckboxFlags("_DefaultSort", p_flags, ImGuiTableColumnFlags_DefaultSort);
    if (ImGui::CheckboxFlags("_WidthStretch", p_flags, ImGuiTableColumnFlags_WidthStretch))
        *p_flags &= ~(ImGuiTableColumnFlags_WidthMask_ ^ ImGuiTableColumnFlags_WidthStretch);
    if (ImGui::CheckboxFlags("_WidthFixed", p_flags, ImGuiTableColumnFlags_WidthFixed))
        *p_flags &= ~(ImGuiTableColumnFlags_WidthMask_ ^ ImGuiTableColumnFlags_WidthFixed);
    ImGui::CheckboxFlags("_NoResize", p_flags, ImGuiTableColumnFlags_NoResize);
    ImGui::CheckboxFlags("_NoReorder", p_flags, ImGuiTableColumnFlags_NoReorder);
    ImGui::CheckboxFlags("_NoHide", p_flags, ImGuiTableColumnFlags_NoHide);
    ImGui::CheckboxFlags("_NoClip", p_flags, ImGuiTableColumnFlags_NoClip);
    ImGui::CheckboxFlags("_NoSort", p_flags, ImGuiTableColumnFlags_NoSort);
    ImGui::CheckboxFlags("_NoSortAscending", p_flags, ImGuiTableColumnFlags_NoSortAscending);
    ImGui::CheckboxFlags("_NoSortDescending", p_flags, ImGuiTableColumnFlags_NoSortDescending);
    ImGui::CheckboxFlags("_NoHeaderLabel", p_flags, ImGuiTableColumnFlags_NoHeaderLabel);
    ImGui::CheckboxFlags("_NoHeaderWidth", p_flags, ImGuiTableColumnFlags_NoHeaderWidth);
    ImGui::CheckboxFlags("_PreferSortAscending", p_flags, ImGuiTableColumnFlags_PreferSortAscending);
    ImGui::CheckboxFlags("_PreferSortDescending", p_flags, ImGuiTableColumnFlags_PreferSortDescending);
    ImGui::CheckboxFlags("_IndentEnable", p_flags, ImGuiTableColumnFlags_IndentEnable);
    ImGui::SameLine();
    HelpMarker("Default for column 0");
    ImGui::CheckboxFlags("_IndentDisable", p_flags, ImGuiTableColumnFlags_IndentDisable);
    ImGui::SameLine();
    HelpMarker("Default for column >0");
}

static void ShowTableColumnsStatusFlags(ImGuiTableColumnFlags flags)
{
    ImGui::CheckboxFlags("_IsEnabled", &flags, ImGuiTableColumnFlags_IsEnabled);
    ImGui::CheckboxFlags("_IsVisible", &flags, ImGuiTableColumnFlags_IsVisible);
    ImGui::CheckboxFlags("_IsSorted", &flags, ImGuiTableColumnFlags_IsSorted);
    ImGui::CheckboxFlags("_IsHovered", &flags, ImGuiTableColumnFlags_IsHovered);
}

static void ShowDemoWindowTables()
{
    //ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    IMGUI_DEMO_MARKER("Tables"_RU >> u8"Таблицы");
    if (!ImGui::CollapsingHeader("Tables & Columns"_RU >> u8"Таблицы и столбцы"))
        return;

    // Использование их в качестве базового значения для создания ширины/высоты, которые являются фактором размера нашего шрифта.
    const float TEXT_BASE_WIDTH  = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    ImGui::PushID("Tables"_RU >> u8"Таблицы");

    int open_action = -1;
    if (ImGui::Button("Open all"_RU >> u8"Открыть все"))
        open_action = 1;
    ImGui::SameLine();
    if (ImGui::Button("Close all"_RU >> u8"Закрыть все"))
        open_action = 0;
    ImGui::SameLine();

    // Опции
    static bool disable_indent = false;
    ImGui::Checkbox("Disable tree indentation"_RU >> u8"Отключить отступ дерева", &disable_indent);
    ImGui::SameLine();
    HelpMarker("Disable the indenting of tree nodes so demo tables can use the full window width."_RU >> u8"Отключите отступы узлов дерева, чтобы демонстрационные таблицы могли использовать всю ширину окна.");
    ImGui::Separator();
    if (disable_indent)
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

    // О стилях таблиц
    // Большинство параметров настраиваются отдельно для каждой таблицы с помощью флагов, передаваемых API BeginTable() и TableSetupColumns.
    // Однако есть несколько общих настроек, которые являются частью структуры ImGuiStyle:
    //   style.CellPadding                          // Заполнение внутри каждой ячейки
    //   style.Colors[ImGuiCol_TableHeaderBg]       // Фон заголовка таблицы
    //   style.Colors[ImGuiCol_TableBorderStrong]   // Внешние границы и границы заголовка таблицы
    //   style.Colors[ImGuiCol_TableBorderLight]    // Внутренние границы таблицы
    //   style.Colors[ImGuiCol_TableRowBg]          // Фон строки таблицы, когда ImGuiTableFlags_RowBg включен (четные строки)
    //   style.Colors[ImGuiCol_TableRowBgAlt]       // Фон строки таблицы, когда ImGuiTableFlags_RowBg включен (строки коэффициентов)

    // Demos
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Basic"_RU >> u8"Таблицы/Базовый");
    if (ImGui::TreeNode("Basic"_RU >> u8"Базовый"))
    {
        // Здесь мы продемонстрируем три различных способа вывода таблицы.
        // Это очень простые вариации одного и того же!

        // [Метод 1] Использование TableNextRow() для создания новой строки и TableSetColumnIndex() для выбора столбца.
        // Во многих ситуациях это самый гибкий и простой в использовании шаблон.
        HelpMarker("Using TableNextRow() + calling TableSetColumnIndex() _before_ each cell, in a loop."_RU >> u8"Использование TableNextRow() + вызов TableSetColumnIndex() _before_ каждой ячейкой в ​​цикле.");
        if (ImGui::BeginTable("table1", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Row %d Column %d"_RU >> u8"Строка %d Столбец %d", row, column);
                }
            }
            ImGui::EndTable();
        }

        // [Метод 2] Использование функции TableNextColumn(), вызываемой несколько раз, вместо использования цикла for + TableSetColumnIndex().
        // Как правило, это более удобно, когда у вас есть код, вручную отправляющий содержимое каждого столбца.
        HelpMarker("Using TableNextRow() + calling TableNextColumn() _before_ each cell, manually."_RU >> u8"Использование TableNextRow() + вызов TableNextColumn() _before_ каждой ячейкой вручную.");
        if (ImGui::BeginTable("table2", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Row %d"_RU >> u8"Ряд %d", row);
                ImGui::TableNextColumn();
                ImGui::Text("Some contents"_RU >> u8"Некоторое содержание");
                ImGui::TableNextColumn();
                ImGui::Text("123.456");
            }
            ImGui::EndTable();
        }

        // [Метод 3] Мы вызываем TableNextColumn() _перед_ каждой ячейкой. Мы никогда не вызываем TableNextRow(),
        // так как TableNextColumn() будет автоматически перебирать и создавать новые roes по мере необходимости.
        // Как правило, это удобнее, когда все ваши ячейки содержат данные одного типа.
        HelpMarker("Only using TableNextColumn(), which tends to be convenient for tables where every cells contains the same type of contents.\n"
                   "This is also more similar to the old NextColumn() function of the Columns API, and provided to facilitate the Columns->Tables API transition."_RU >>
            u8"Только использование TableNextColumn(), что удобно для таблиц, где каждая ячейка содержит содержимое одного типа..\n"
            "Это также больше похоже на старую функцию NextColumn() API Columns и предоставляется для облегчения перехода API Columns->Tables..");
        if (ImGui::BeginTable("table3", 3))
        {
            for (int item = 0; item < 14; item++)
            {
                ImGui::TableNextColumn();
                ImGui::Text("Item %d"_RU >> u8"Элемент %d", item);
            }
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Borders, background"_RU >> u8"Таблицы/Границы, фон");
    if (ImGui::TreeNode("Borders, background"_RU >> u8"Границы, фон"))
    {
        // Выставляйте несколько флагов, связанных с границами, в интерактивном режиме.
        enum ContentsType
        {
            CT_Text,
            CT_FillButton
        };
        static ImGuiTableFlags flags           = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        static bool            display_headers = false;
        static int             contents_type   = CT_Text;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
        ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
        ImGui::SameLine();
        HelpMarker("ImGuiTableFlags_Borders\n = ImGuiTableFlags_BordersInnerV\n | ImGuiTableFlags_BordersOuterV\n | ImGuiTableFlags_BordersInnerV\n | ImGuiTableFlags_BordersOuterH");
        ImGui::Indent();

        ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
        ImGui::Indent();
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
        ImGui::Unindent();

        ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
        ImGui::Indent();
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
        ImGui::Unindent();

        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags, ImGuiTableFlags_BordersInner);
        ImGui::Unindent();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Cell contents:"_RU >> u8"Содержимое ячейки:");
        ImGui::SameLine();
        ImGui::RadioButton("Text"_RU >> u8"Текст", &contents_type, CT_Text);
        ImGui::SameLine();
        ImGui::RadioButton("FillButton"_RU >> u8"Кнопка Заполнения", &contents_type, CT_FillButton);
        ImGui::Checkbox("Display headers"_RU >> u8"Показать заголовки", &display_headers);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody);
        ImGui::SameLine();
        HelpMarker("Disable vertical borders in columns Body (borders will always appears in Headers"_RU >> u8"Отключить вертикальные границы в основной части столбцов (границы всегда будут отображаться в заголовках");
        PopStyleCompact();

        if (ImGui::BeginTable("table1", 3, flags))
        {
            // Отобразите заголовки, чтобы мы могли проверить их взаимодействие с границами.
            // (Заголовки не являются основной целью этого раздела демонстрации, поэтому мы не будем слишком подробно о них рассказывать. Подробности см. в других разделах)
            if (display_headers)
            {
                ImGui::TableSetupColumn("One"_RU >> u8"Один");
                ImGui::TableSetupColumn("Two"_RU >> u8"Два");
                ImGui::TableSetupColumn("Three"_RU >> u8"Три");
                ImGui::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    char buf[32];
                    sprintf(buf, "Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                    if (contents_type == CT_Text)
                        ImGui::TextUnformatted(buf);
                    else if (contents_type == CT_FillButton)
                        ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, stretch"_RU >> u8"Таблицы/Изменяемый размер, растягивание");
    if (ImGui::TreeNode("Resizable, stretch"_RU >> u8"Изменяемый размер, растягивание"))
    {
        // По умолчанию, если мы не включаем ScrollX, политика изменения размера для каждого столбца — «Растянуть».
        // Каждая колонка поддерживает вес размера, и они будут занимать всю доступную ширину.
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
        ImGui::SameLine();
        HelpMarker("Using the _Resizable flag automatically enables the _BordersInnerV flag as well, this is why the resize borders are still showing when unchecking this."_RU >> u8"Использование флага _Resizable также автоматически включает флаг _BordersInnerV, поэтому границы изменения размера все еще отображаются при снятии этого флажка.");
        PopStyleCompact();

        if (ImGui::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, fixed"_RU >> u8"Таблицы/Изменяемый размер, фиксированный");
    if (ImGui::TreeNode("Resizable, fixed"_RU >> u8"Изменяемый размер, фиксированный"))
    {
        // Здесь мы используем ImGuiTableFlags_SizingFixedFit (хотя _ScrollX не установлен)
        // Таким образом, столбцы примут политику «Фиксированная» и будут поддерживать фиксированную ширину независимо от всей доступной ширины (если только таблица не маленькая)
        // Если доступной ширины недостаточно для размещения всех столбцов, их размер, тем не менее, будет уменьшен.
        // FIXME-TABLE: Предоставление растягивания при инициализации имеет смысл, особенно для таблиц, у которых нет сохраненных настроек
        HelpMarker("Using _Resizable + _SizingFixedFit flags.\n"
                   "Fixed-width columns generally makes more sense if you want to use horizontal scrolling.\n\n"
                   "Double-click a column border to auto-fit the column to its contents."_RU >>
            u8"Использование флагов _Resizable + _SizingFixedFit.\n"
            "Столбцы с фиксированной шириной обычно имеют больше смысла, если вы хотите использовать горизонтальную прокрутку.\n\n"
            "Дважды щелкните границу столбца, чтобы автоматически подогнать столбец под его содержимое.");
        PushStyleCompact();
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
        ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
        PopStyleCompact();

        if (ImGui::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, mixed"_RU >> u8"Таблицы/Изменяемый размер, смешанный");
    if (ImGui::TreeNode("Resizable, mixed"_RU >> u8"Изменяемый размер, смешанный"))
    {
        HelpMarker("Using TableSetupColumn() to alter resizing policy on a per-column basis.\n\n"
                   "When combining Fixed and Stretch columns, generally you only want one, maybe two trailing columns to use _WidthStretch."_RU >>
            u8"Использование TableSetupColumn() для изменения политики изменения размера для каждого столбца.\n\n"
            "При объединении столбцов Fixed и Stretch обычно требуется, чтобы только один, а то и два конечных столбца использовали _WidthStretch.");
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("table1", 3, flags))
        {
            ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%s %d,%d", (column == 2) ? "Stretch"_RU >> u8"Растянутый" : "Fixed"_RU >> u8"Зафиксированный", column, row);
                }
            }
            ImGui::EndTable();
        }
        if (ImGui::BeginTable("table2", 6, flags))
        {
            ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide);
            ImGui::TableSetupColumn("DDD", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("EEE", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("FFF", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultHide);
            ImGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 6; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%s %d,%d", (column >= 3) ? "Stretch"_RU >> u8"Растянутый" : "Fixed"_RU >> u8"Зафиксированный", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Reorderable, hideable, with headers"_RU >> u8"Таблицы/Переупорядочиваемый, скрываемый, с заголовками");
    if (ImGui::TreeNode("Reorderable, hideable, with headers"_RU >> u8"Переупорядочиваемый, скрываемый, с заголовками"))
    {
        HelpMarker("Click and drag column headers to reorder columns.\n\n"
                   "Right-click on a header to open a context menu."_RU >>
            u8"Нажмите и перетащите заголовки столбцов, чтобы изменить порядок столбцов.\n\n"
            "Щелкните правой кнопкой мыши заголовок, чтобы открыть контекстное меню.");
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable);
        ImGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize);
        ImGui::SameLine();
        HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appears in Headers)"_RU >> u8"Отключить вертикальные границы в теле столбцов, пока не будет наведен курсор для изменения размера (границы всегда будут отображаться в заголовках)");
        PopStyleCompact();

        if (ImGui::BeginTable("table1", 3, flags))
        {
            // Отправьте имя столбца с помощью TableSetupColumn() и вызовите TableHeadersRow(), чтобы создать строку с заголовком в каждом столбце.
            // (Позже мы покажем, как TableSetupColumn() имеет другие применения, необязательные флаги, вес размеров и т. д.)
            ImGui::TableSetupColumn("One"_RU >> u8"Один");
            ImGui::TableSetupColumn("Two"_RU >> u8"Два");
            ImGui::TableSetupColumn("Three"_RU >> u8"Три");
            ImGui::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }

        // Используйте outer_size.x == 0.0f вместо значения по умолчанию, чтобы сделать таблицу как можно более узкой (действительно только при отсутствии прокрутки и растянутого столбца)
        if (ImGui::BeginTable("table2", 3, flags | ImGuiTableFlags_SizingFixedFit, ImVec2(0.0f, 0.0f)))
        {
            ImGui::TableSetupColumn("One"_RU >> u8"Один");
            ImGui::TableSetupColumn("Two"_RU >> u8"Два");
            ImGui::TableSetupColumn("Three"_RU >> u8"Три");
            ImGui::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Fixed %d,%d"_RU >> u8"Зафиксированный %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Padding"_RU >> u8"Таблицы/Отступы");
    if (ImGui::TreeNode("Padding"_RU >> u8"Отступы"))
    {
        // Первый пример: продемонстрируйте использование флагов заполнения и влияние BorderOuterV/BorderInnerV на заполнение X.
        // Мы не раскрываем здесь BorderOuterH/BorderInnerH, потому что они не влияют на заполнение X.
        HelpMarker("We often want outer padding activated when any using features which makes the edges of a column visible:\n"
                   "e.g.:\n"
                   "- BorderOuterV\n"
                   "- any form of row selection\n"
                   "Because of this, activating BorderOuterV sets the default to PadOuterX. Using PadOuterX or NoPadOuterX you can override the default.\n\n"
                   "Actual padding values are using style.CellPadding.\n\n"
                   "In this demo we don't show horizontal borders to emphasis how they don't affect default horizontal padding."_RU >>
            u8"Мы часто хотим, чтобы внешнее заполнение активировалось, когда используются какие-либо функции, которые делают края столбца видимыми.:\n"
            "e.g.:\n"
            "- BorderOuterV\n"
            "- любая форма выбора строки\n"
            "По этой причине при активации BorderOuterV по умолчанию устанавливается значение PadOuterX. Используя PadOuterX или NoPadOuterX, вы можете переопределить значение по умолчанию.\n\n"
            "Фактические значения заполнения используют style.CellPadding.\n\n"
            "В этой демонстрации мы не показываем горизонтальные границы, чтобы подчеркнуть, что они не влияют на горизонтальное заполнение по умолчанию.");

        static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersV;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_PadOuterX", &flags1, ImGuiTableFlags_PadOuterX);
        ImGui::SameLine();
        HelpMarker("Enable outer-most padding (default if ImGuiTableFlags_BordersOuterV is set)"_RU >> u8"Включить самое внешнее заполнение (по умолчанию, если установлен ImGuiTableFlags_BordersOuterV)");
        ImGui::CheckboxFlags("ImGuiTableFlags_NoPadOuterX", &flags1, ImGuiTableFlags_NoPadOuterX);
        ImGui::SameLine();
        HelpMarker("Disable outer-most padding (default if ImGuiTableFlags_BordersOuterV is not set)"_RU >> u8"Отключить самое внешнее заполнение (по умолчанию, если ImGuiTableFlags_BordersOuterV не установлен)");
        ImGui::CheckboxFlags("ImGuiTableFlags_NoPadInnerX", &flags1, ImGuiTableFlags_NoPadInnerX);
        ImGui::SameLine();
        HelpMarker("Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)"_RU >> u8"Отключить внутреннее заполнение между столбцами (двойное внутреннее заполнение, если BordersOuterV включен, одинарное внутреннее заполнение, если BordersOuterV выключен)");
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags1, ImGuiTableFlags_BordersOuterV);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags1, ImGuiTableFlags_BordersInnerV);
        static bool show_headers = false;
        ImGui::Checkbox("show_headers", &show_headers);
        PopStyleCompact();

        if (ImGui::BeginTable("table_padding", 3, flags1))
        {
            if (show_headers)
            {
                ImGui::TableSetupColumn("One"_RU >> u8"Один");
                ImGui::TableSetupColumn("Two"_RU >> u8"Два");
                ImGui::TableSetupColumn("Three"_RU >> u8"Три");
                ImGui::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (row == 0)
                    {
                        ImGui::Text("Avail %.2f"_RU >> u8"Доступно %.2f", ImGui::GetContentRegionAvail().x);
                    }
                    else
                    {
                        char buf[32];
                        sprintf(buf, "Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                        ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                    }
                    //if (ImGui::TableGetColumnFlags() & ImGuiTableColumnFlags_IsHovered)
                    //    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 100, 0, 255));
                }
            }
            ImGui::EndTable();
        }

        // Второй пример: установить style.CellPadding до (0.0) или пользовательское значение.
        // FIXME-TABLE: Вертикальная граница фактически не отображается так же, как горизонтальная...
        HelpMarker("Setting style.CellPadding to (0,0) or a custom value."_RU >> u8"Параметр style.CellPadding к (0,0) или пользовательское значение.");
        static ImGuiTableFlags flags2 = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        static ImVec2          cell_padding(0.0f, 0.0f);
        static bool            show_widget_frame_bg = true;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags2, ImGuiTableFlags_Borders);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags2, ImGuiTableFlags_BordersH);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags2, ImGuiTableFlags_BordersV);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags2, ImGuiTableFlags_BordersInner);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags2, ImGuiTableFlags_BordersOuter);
        ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags2, ImGuiTableFlags_RowBg);
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags2, ImGuiTableFlags_Resizable);
        ImGui::Checkbox("show_widget_frame_bg", &show_widget_frame_bg);
        ImGui::SliderFloat2("CellPadding"_RU >> u8"Заполнение ячейки", &cell_padding.x, 0.0f, 10.0f, "%.0f");
        PopStyleCompact();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        if (ImGui::BeginTable("table_padding_2", 3, flags2))
        {
            static char text_bufs[3 * 5][16];   // Мини текстовое хранилище для ячеек 3х5
            static bool init = true;
            if (!show_widget_frame_bg)
                ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
            for (int cell = 0; cell < 3 * 5; cell++)
            {
                ImGui::TableNextColumn();
                if (init)
                    strcpy(text_bufs[cell], "edit me"_RU >> u8"отредактируй меня");
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::PushID(cell);
                ImGui::InputText("##cell", text_bufs[cell], IM_ARRAYSIZE(text_bufs[cell]));
                ImGui::PopID();
            }
            if (!show_widget_frame_bg)
                ImGui::PopStyleColor();
            init = false;
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();

        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Explicit widths"_RU >> u8"Таблицы/Явная ширина");
    if (ImGui::TreeNode("Sizing policies"_RU >> u8"Политика определения размеров"))
    {
        static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_ContextMenuInBody;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags1, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags1, ImGuiTableFlags_NoHostExtendX);
        PopStyleCompact();

        static ImGuiTableFlags sizing_policy_flags[4] = {ImGuiTableFlags_SizingFixedFit, ImGuiTableFlags_SizingFixedSame, ImGuiTableFlags_SizingStretchProp, ImGuiTableFlags_SizingStretchSame};
        for (int table_n = 0; table_n < 4; table_n++)
        {
            ImGui::PushID(table_n);
            ImGui::SetNextItemWidth(TEXT_BASE_WIDTH * 30);
            EditTableSizingFlags(&sizing_policy_flags[table_n]);

            // Чтобы было легче понять различную политику размеров,
            // Для каждой политики: мы отображаем одну таблицу, в которой столбцы имеют одинаковую ширину содержимого, и одну, в которой столбцы имеют разную ширину содержимого.
            if (ImGui::BeginTable("table1", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Oh dear"_RU >> u8"О, Боже.");
                    ImGui::TableNextColumn();
                    ImGui::Text("Oh dear"_RU >> u8"О, Боже.");
                    ImGui::TableNextColumn();
                    ImGui::Text("Oh dear"_RU >> u8"О, Боже.");
                }
                ImGui::EndTable();
            }
            if (ImGui::BeginTable("table2", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("AAAA");
                    ImGui::TableNextColumn();
                    ImGui::Text("BBBBBBBB");
                    ImGui::TableNextColumn();
                    ImGui::Text("CCCCCCCCCCCC");
                }
                ImGui::EndTable();
            }
            ImGui::PopID();
        }

        ImGui::Spacing();
        ImGui::TextUnformatted("Advanced"_RU >> u8"Расширенный");
        ImGui::SameLine();
        HelpMarker("This section allows you to interact and see the effect of various sizing policies depending on whether Scroll is enabled and the contents of your columns."_RU >> u8"Этот раздел позволяет вам взаимодействовать и видеть эффект различных политик изменения размеров в зависимости от того, включена ли прокрутка и каково содержимое ваших колонок.");

        enum ContentsType
        {
            CT_ShowWidth,
            CT_ShortText,
            CT_LongText,
            CT_Button,
            CT_FillButton,
            CT_InputText
        };
        static ImGuiTableFlags flags         = ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
        static int             contents_type = CT_ShowWidth;
        static int             column_count  = 3;

        PushStyleCompact();
        ImGui::PushID("Advanced"_RU >> u8"Расширенный");
        ImGui::PushItemWidth(TEXT_BASE_WIDTH * 30);
        EditTableSizingFlags(&flags);
        ImGui::Combo("Contents"_RU >> u8"Содержимое", &contents_type, "Show width\0Short Text\0Long Text\0Button\0Fill Button\0InputText\0"_RU >> u8"Показать ширину\0Короткий текст\0Длинный текст\0Кнопка\0Кнопка заполнения\0Ввод текста\0");
        if (contents_type == CT_FillButton)
        {
            ImGui::SameLine();
            HelpMarker("Be mindful that using right-alignment (e.g. size.x = -FLT_MIN) creates a feedback loop where contents width can feed into auto-column width can feed into contents width."_RU >> u8"Помните, что использование выравнивания по правому краю (например, size. x = -FLT_MIN) создает петлю обратной связи, в которой ширина содержимого может передаваться в ширину автоматического столбца, которая может входить в ширину содержимого.");
        }
        ImGui::DragInt("Columns"_RU >> u8"Столбцы", &column_count, 0.1f, 1, 64, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths);
        ImGui::SameLine();
        HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth."_RU >>
            u8"Отключить распределение остаточной ширины на растянутые столбцы (выделение ширины для таблицы шириной 100 с 3 столбцами: Без этого флага: 33,33,34. С этим флагом: 33,33,33). При большем количестве столбцов изменение размера будет менее плавным.");
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoClip", &flags, ImGuiTableFlags_NoClip);
        ImGui::PopItemWidth();
        ImGui::PopID();
        PopStyleCompact();

        if (ImGui::BeginTable("table2", column_count, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 7)))
        {
            for (int cell = 0; cell < 10 * column_count; cell++)
            {
                ImGui::TableNextColumn();
                int column = ImGui::TableGetColumnIndex();
                int row    = ImGui::TableGetRowIndex();

                ImGui::PushID(cell);
                char        label[32];
                static char text_buf[32] = "";
                sprintf(label, "Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                switch (contents_type)
                {
                    case CT_ShortText:
                        ImGui::TextUnformatted(label);
                        break;
                    case CT_LongText:
                        ImGui::Text("Some %s text %d,%d\nOver two lines.."_RU >> u8"Некоторые %s текст %d,%d\nБолее двух строк..", column == 0 ? "long"_RU >> u8"длинный" : "longeeer"_RU >> u8"длиннее", column, row);
                        break;
                    case CT_ShowWidth:
                        ImGui::Text("W: %.1f", ImGui::GetContentRegionAvail().x);
                        break;
                    case CT_Button:
                        ImGui::Button(label);
                        break;
                    case CT_FillButton:
                        ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
                        break;
                    case CT_InputText:
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::InputText("##", text_buf, IM_ARRAYSIZE(text_buf));
                        break;
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Vertical scrolling, with clipping"_RU >> u8"Таблицы/Вертикальная прокрутка, с обрезкой");
    if (ImGui::TreeNode("Vertical scrolling, with clipping"_RU >> u8"Вертикальная прокрутка с обрезкой"))
    {
        HelpMarker("Here we activate ScrollY, which will create a child window container to allow hosting scrollable contents.\n\nWe also demonstrate using ImGuiListClipper to virtualize the submission of many items."_RU >> u8"Здесь мы активируем scrollY, который создаст дочерний контейнер окна, позволяющий размещать прокручивать содержимое.\n\n Мы также демонстрируем использование ImGuiListClipper для виртуализации отправки многих элементов.");
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        PopStyleCompact();

        // При использовании ScrollX или ScrollY нам нужно указать размер контейнера нашей таблицы!
        // В противном случае по умолчанию таблица будет занимать все доступное место, как вызов BeginChild().
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("table_scrolly", 3, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1);   // Сделать верхнюю строку всегда видимой
            ImGui::TableSetupColumn("One"_RU >> u8"Один", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Two"_RU >> u8"Два", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Three"_RU >> u8"Три", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            // Продемонстрируйте использование клипера для больших вертикальных списков
            ImGuiListClipper clipper;
            clipper.Begin(1000);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    ImGui::TableNextRow();
                    for (int column = 0; column < 3; column++)
                    {
                        ImGui::TableSetColumnIndex(column);
                        ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Horizontal scrolling"_RU >> u8"Таблицы/Горизонтальная прокрутка");
    if (ImGui::TreeNode("Horizontal scrolling"_RU >> u8"Горизонтальная прокрутка"))
    {
        HelpMarker("When ScrollX is enabled, the default sizing policy becomes ImGuiTableFlags_SizingFixedFit, "
                   "as automatically stretching columns doesn't make much sense with horizontal scrolling.\n\n"
                   "Also note that as of the current version, you will almost always want to enable ScrollY along with ScrollX,"
                   "because the container window won't automatically extend vertically to fix contents (this may be improved in future versions)."_RU >>
            u8"Когда ScrollX включен, политика изменения размера по умолчанию становится ImGuiTableFlags_SizingFixedFit, "
            "поскольку автоматическое растягивание столбцов не имеет особого смысла при горизонтальной прокрутке.\n\n"
            "Также обратите внимание, что в текущей версии вы почти всегда захотите включить ScrollY вместе со ScrollX,"
            "потому что окно контейнера не будет автоматически расширяться по вертикали для исправления содержимого (это может быть улучшено в будущих версиях).");
        static ImGuiTableFlags flags       = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        static int             freeze_cols = 1;
        static int             freeze_rows = 1;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
        ImGui::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
        ImGui::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        PopStyleCompact();

        // При использовании ScrollX или ScrollY нам нужно указать размер контейнера нашей таблицы!
        // В противном случае по умолчанию таблица будет занимать все доступное место, как вызов BeginChild().
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("table_scrollx", 7, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableSetupColumn("Line #"_RU >> u8"Линия #", ImGuiTableColumnFlags_NoHide);   // Сделайте первый столбец недоступным для скрытия, чтобы он соответствовал нашему использованию TableSetupScrollFreeze().
            ImGui::TableSetupColumn("One"_RU >> u8"Один");
            ImGui::TableSetupColumn("Two"_RU >> u8"Два");
            ImGui::TableSetupColumn("Three"_RU >> u8"Три");
            ImGui::TableSetupColumn("Four"_RU >> u8"Четыре");
            ImGui::TableSetupColumn("Five"_RU >> u8"Пять");
            ImGui::TableSetupColumn("Six"_RU >> u8"Шесть");
            ImGui::TableHeadersRow();
            for (int row = 0; row < 20; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 7; column++)
                {
                    // И TableNextColumn(), и TableSetColumnIndex() возвращают true, когда столбец виден или выполняется измерение ширины.
                    // Потому что здесь мы знаем, что:
                    // - A) все наши столбцы вносят одинаковый вклад в высоту строки
                    // - B) столбец 0 всегда виден,
                    // Мы всегда отправляем только этот один столбец и можем пропустить другие.
                    // Более продвинутое поведение отсечения для каждого столбца может выиграть от опроса флагов состояния с помощью TableGetColumnFlags().
                    if (!ImGui::TableSetColumnIndex(column) && column > 0)
                        continue;
                    if (column == 0)
                        ImGui::Text("Line %d"_RU >> u8"Линия %d", row);
                    else
                        ImGui::Text("Hello world %d,%d"_RU >> u8"Привет, мир %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::TextUnformatted("Stretch + ScrollX");
        ImGui::SameLine();
        HelpMarker("Showcase using Stretch columns + ScrollX together: "
                   "this is rather unusual and only makes sense when specifying an 'inner_width' for the table!\n"
                   "Without an explicit value, inner_width is == outer_size.x and therefore using Stretch columns + ScrollX together doesn't make sense."_RU >>
            u8"Демонстрация с использованием столбцов Stretch + ScrollX вместе: "
            "это довольно необычно и имеет смысл только при указании 'inner_width' для таблицы!\n"
            "Без явного значения inner_width равна == outer_size.x, и поэтому совместное использование столбцов Stretch + ScrollX не имеет смысла.");
        static ImGuiTableFlags flags2      = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg | ImGuiTableFlags_ContextMenuInBody;
        static float           inner_width = 1000.0f;
        PushStyleCompact();
        ImGui::PushID("flags3");
        ImGui::PushItemWidth(TEXT_BASE_WIDTH * 30);
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags2, ImGuiTableFlags_ScrollX);
        ImGui::DragFloat("inner_width", &inner_width, 1.0f, 0.0f, FLT_MAX, "%.1f");
        ImGui::PopItemWidth();
        ImGui::PopID();
        PopStyleCompact();
        if (ImGui::BeginTable("table2", 7, flags2, outer_size, inner_width))
        {
            for (int cell = 0; cell < 20 * 7; cell++)
            {
                ImGui::TableNextColumn();
                ImGui::Text("Hello world %d,%d"_RU >> u8"Привет, мир %d,%d", ImGui::TableGetColumnIndex(), ImGui::TableGetRowIndex());
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Columns flags"_RU >> u8"Таблицы/Столбцы флагов");
    if (ImGui::TreeNode("Columns flags"_RU >> u8"Столбцы флагов"))
    {
        // Создайте первую таблицу, чтобы показать все параметры/флаги, которые мы хотим сделать видимыми в нашем примере!
        const int                    column_count                   = 3;
        const char*                  column_names[column_count]     = {"One"_RU >> u8"Один", "Two"_RU >> u8"Два", "Three"_RU >> u8"Три"};
        static ImGuiTableColumnFlags column_flags[column_count]     = {ImGuiTableColumnFlags_DefaultSort, ImGuiTableColumnFlags_None, ImGuiTableColumnFlags_DefaultHide};
        static ImGuiTableColumnFlags column_flags_out[column_count] = {0, 0, 0};   // Вывод из TableGetColumnFlags()

        if (ImGui::BeginTable("table_columns_flags_checkboxes", column_count, ImGuiTableFlags_None))
        {
            PushStyleCompact();
            for (int column = 0; column < column_count; column++)
            {
                ImGui::TableNextColumn();
                ImGui::PushID(column);
                ImGui::AlignTextToFramePadding();   // FIXME-TABLE: обходной путь для неправильного распространения базовой линии текста по столбцам
                ImGui::Text("'%s'", column_names[column]);
                ImGui::Spacing();
                ImGui::Text("Input flags:"_RU >> u8"Входные флаги:");
                EditTableColumnsFlags(&column_flags[column]);
                ImGui::Spacing();
                ImGui::Text("Output flags:"_RU >> u8"Выходные флаги:");
                ImGui::BeginDisabled();
                ShowTableColumnsStatusFlags(column_flags_out[column]);
                ImGui::EndDisabled();
                ImGui::PopID();
            }
            PopStyleCompact();
            ImGui::EndTable();
        }

        // Создайте для примера настоящую таблицу, которая нам нужна!
        // Мы используем таблицу прокрутки, чтобы иметь возможность продемонстрировать разницу между флагами _IsEnabled и _IsVisible выше, иначе в
        // столбцы таблицы без прокрутки всегда видны (если только не использовать ImGuiTableFlags_NoKeepColumnsVisible + уменьшение размера родительского окна)
        const ImGuiTableFlags flags      = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;
        ImVec2                outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 9);
        if (ImGui::BeginTable("table_columns_flags", column_count, flags, outer_size))
        {
            for (int column = 0; column < column_count; column++)
                ImGui::TableSetupColumn(column_names[column], column_flags[column]);
            ImGui::TableHeadersRow();
            for (int column = 0; column < column_count; column++)
                column_flags_out[column] = ImGui::TableGetColumnFlags(column);
            float indent_step = (float)((int)TEXT_BASE_WIDTH / 2);
            for (int row = 0; row < 8; row++)
            {
                ImGui::Indent(indent_step);   // Добавьте отступ, чтобы продемонстрировать использование флагов IndentEnable/IndentDisable для каждого столбца.
                ImGui::TableNextRow();
                for (int column = 0; column < column_count; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%s %s", (column == 0) ? "Indented"_RU >> u8"С отступом" : "Hello"_RU >> u8"Привет", ImGui::TableGetColumnName(column));
                }
            }
            ImGui::Unindent(indent_step * 8.0f);

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Columns widths"_RU >> u8"Таблицы/Ширина столбцов");
    if (ImGui::TreeNode("Columns widths"_RU >> u8"Ширина столбцов"))
    {
        HelpMarker("Using TableSetupColumn() to setup default width."_RU >> u8"Использование TableSetupColumn() для установки ширины по умолчанию.");

        static ImGuiTableFlags flags1 = ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBodyUntilResize;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags1, ImGuiTableFlags_Resizable);
        ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags1, ImGuiTableFlags_NoBordersInBodyUntilResize);
        PopStyleCompact();
        if (ImGui::BeginTable("table1", 3, flags1))
        {
            // Мы также можем установить ImGuiTableFlags_SizingFixedFit для таблицы, и все столбцы по умолчанию будут иметь значение ImGuiTableColumnFlags_WidthFixed.
            ImGui::TableSetupColumn("one"_RU >> u8"один", ImGuiTableColumnFlags_WidthFixed, 100.0f);   // По умолчанию 100.0f
            ImGui::TableSetupColumn("two"_RU >> u8"два", ImGuiTableColumnFlags_WidthFixed, 200.0f);    // По умолчанию 200.0f
            ImGui::TableSetupColumn("three"_RU >> u8"три", ImGuiTableColumnFlags_WidthFixed);          // По умолчанию авто
            ImGui::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (row == 0)
                        ImGui::Text("(w: %5.1f)", ImGui::GetContentRegionAvail().x);
                    else
                        ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }

        HelpMarker("Using TableSetupColumn() to setup explicit width.\n\nUnless _NoKeepColumnsVisible is set, fixed columns with set width may still be shrunk down if there's not enough space in the host."_RU >> u8"Использование TableSetupColumn() для установки явной ширины.\n\nЕсли не задано значение _NoKeepColumnsVisible, фиксированные столбцы с установленной шириной могут быть сжаты, если на хосте недостаточно места.");

        static ImGuiTableFlags flags2 = ImGuiTableFlags_None;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags2, ImGuiTableFlags_NoKeepColumnsVisible);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags2, ImGuiTableFlags_BordersInnerV);
        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags2, ImGuiTableFlags_BordersOuterV);
        PopStyleCompact();
        if (ImGui::BeginTable("table2", 4, flags2))
        {
            // Мы также можем установить ImGuiTableFlags_SizingFixedFit для таблицы, и все столбцы по умолчанию будут иметь значение ImGuiTableColumnFlags_WidthFixed.
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 30.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (row == 0)
                        ImGui::Text("(w: %5.1f)", ImGui::GetContentRegionAvail().x);
                    else
                        ImGui::Text("Hello %d,%d"_RU >> u8"Привет %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Nested tables"_RU >> u8"Таблицы/Вложенные таблицы");
    if (ImGui::TreeNode("Nested tables"_RU >> u8"Вложенные таблицы"))
    {
        HelpMarker("This demonstrate embedding a table into another table cell."_RU >> u8"Это демонстрирует встраивание таблицы в другую ячейку таблицы.");

        if (ImGui::BeginTable("table_nested1", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
        {
            ImGui::TableSetupColumn("A0");
            ImGui::TableSetupColumn("A1");
            ImGui::TableHeadersRow();

            ImGui::TableNextColumn();
            ImGui::Text("A0 Row 0");
            {
                float rows_height = TEXT_BASE_HEIGHT * 2;
                if (ImGui::BeginTable("table_nested2", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
                {
                    ImGui::TableSetupColumn("B0");
                    ImGui::TableSetupColumn("B1");
                    ImGui::TableHeadersRow();

                    ImGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
                    ImGui::TableNextColumn();
                    ImGui::Text("B0 Row 0");
                    ImGui::TableNextColumn();
                    ImGui::Text("B1 Row 0");
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
                    ImGui::TableNextColumn();
                    ImGui::Text("B0 Row 1");
                    ImGui::TableNextColumn();
                    ImGui::Text("B1 Row 1");

                    ImGui::EndTable();
                }
            }
            ImGui::TableNextColumn();
            ImGui::Text("A1 Row 0");
            ImGui::TableNextColumn();
            ImGui::Text("A0 Row 1");
            ImGui::TableNextColumn();
            ImGui::Text("A1 Row 1");
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Row height"_RU >> u8"Таблицы/Высота строки");
    if (ImGui::TreeNode("Row height"_RU >> u8"Высота строки"))
    {
        HelpMarker("You can pass a 'min_row_height' to TableNextRow().\n\nRows are padded with 'style.CellPadding.y' on top and bottom, so effectively the minimum row height will always be >= 'style.CellPadding.y * 2.0f'.\n\nWe cannot honor a _maximum_ row height as that would requires a unique clipping rectangle per row."_RU >>
            u8"Вы можете передать 'min_row_height' в TableNextRow().\n\nСтроки дополняются 'style.CellPadding.y' сверху и снизу, поэтому минимальная высота строки всегда будет >= 'style.CellPadding.у * 2,0f'.\n\nМы не можем соблюдать _maximum_ высоту строки, поскольку для этого потребуется уникальный прямоугольник отсечения для каждой строки.");
        if (ImGui::BeginTable("table_row_height", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerV))
        {
            for (int row = 0; row < 10; row++)
            {
                float min_row_height = (float)(int)(TEXT_BASE_HEIGHT * 0.30f * row);
                ImGui::TableNextRow(ImGuiTableRowFlags_None, min_row_height);
                ImGui::TableNextColumn();
                ImGui::Text("min_row_height = %.2f", min_row_height);
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Outer size"_RU >> u8"Таблицы/Внешний размер");
    if (ImGui::TreeNode("Outer size"_RU >> u8"Внешний размер"))
    {
        // Демонстрация использования ImGuiTableFlags_NoHostExtendX и ImGuiTableFlags_NoHostExtendY
        // Важно отметить, что два флага ведут себя немного по-разному!
        ImGui::Text("Using NoHostExtendX and NoHostExtendY:");
        PushStyleCompact();
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
        ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
        ImGui::SameLine();
        HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used."_RU >> u8"Сделайте так, чтобы внешняя ширина автоматически подгонялась под столбцы, переопределяя external_size.х значение.\n\nДоступно, только если ScrollX/ScrollY отключены и столбцы Stretch не используются.");
        ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY);
        ImGui::SameLine();
        HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible."_RU >> u8"Сделайте так, чтобы внешняя высота останавливалась точно на external_size.y (предотвратить автоматическое расширение таблицы за предел).\n\nДоступно, только если ScrollX/ScrollY отключены. Данные ниже предела будут обрезаны и не видны.");
        PopStyleCompact();

        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 5.5f);
        if (ImGui::BeginTable("table1", 3, flags, outer_size))
        {
            for (int row = 0; row < 10; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::SameLine();
        ImGui::Text("Hello!"_RU >> u8"Привет!");

        ImGui::Spacing();

        ImGui::Text("Using explicit size:"_RU >> u8"Использование явного размера:");
        if (ImGui::BeginTable("table2", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }
        ImGui::SameLine();
        if (ImGui::BeginTable("table3", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            for (int row = 0; row < 3; row++)
            {
                ImGui::TableNextRow(0, TEXT_BASE_HEIGHT * 1.5f);
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Background color"_RU >> u8"Таблицы/Цвет фона");
    if (ImGui::TreeNode("Background color"_RU >> u8"Цвет фона"))
    {
        static ImGuiTableFlags flags         = ImGuiTableFlags_RowBg;
        static int             row_bg_type   = 1;
        static int             row_bg_target = 1;
        static int             cell_bg_type  = 1;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
        ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
        ImGui::SameLine();
        HelpMarker("ImGuiTableFlags_RowBg automatically sets RowBg0 to alternative colors pulled from the Style."_RU >> u8"ImGuiTableFlags_RowBg автоматически устанавливает для RowBg0 альтернативные цвета, извлеченные из стиля.");
        ImGui::Combo("row bg type"_RU >> u8"тип фона строки", (int*)&row_bg_type, "None\0Red\0Gradient\0");
        ImGui::Combo("row bg target"_RU >> u8"цель фона строки", (int*)&row_bg_target, "RowBg0\0RowBg1\0");
        ImGui::SameLine();
        HelpMarker("Target RowBg0 to override the alternating odd/even colors,\nTarget RowBg1 to blend with them."_RU >> u8"Настройте RowBg0, чтобы переопределить чередующиеся нечетные/четные цвета,\nНацельте RowBg1, чтобы смешаться с ними.");
        ImGui::Combo("cell bg type"_RU >> u8"тип фона ячейки", (int*)&cell_bg_type, "None\0Blue\0");
        ImGui::SameLine();
        HelpMarker("We are colorizing cells to B1->C2 here."_RU >> u8"Здесь мы раскрашиваем ячейки до B1->C2.");
        IM_ASSERT(row_bg_type >= 0 && row_bg_type <= 2);
        IM_ASSERT(row_bg_target >= 0 && row_bg_target <= 1);
        IM_ASSERT(cell_bg_type >= 0 && cell_bg_type <= 1);
        PopStyleCompact();

        if (ImGui::BeginTable("table1", 5, flags))
        {
            for (int row = 0; row < 6; row++)
            {
                ImGui::TableNextRow();

                // Продемонстрируйте настройку цвета фона строки с помощью «ImGui:: TableSetBgColor (ImGuiTableBgTarget_RowBgX, ...)»
                // Мы используем прозрачный цвет, чтобы мы могли видеть тот, что позади, если нашей целью является RowBg1, а RowBg0 уже был нацелен флагом ImGuiTableFlags_RowBg.
                if (row_bg_type != 0)
                {
                    ImU32 row_bg_color = ImGui::GetColorU32(row_bg_type == 1 ? ImVec4(0.7f, 0.3f, 0.3f, 0.65f) : ImVec4(0.2f + row * 0.1f, 0.2f, 0.2f, 0.65f));   // Плоский или градиент?
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0 + row_bg_target, row_bg_color);
                }

                // Заполнить ячейки
                for (int column = 0; column < 5; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%c%c", 'A' + row, '0' + column);

                    // Изменить фон ячеек B1->C2
                    // Демонстрация установки цвета фона ячейки с помощью 'ImGui:: TableSetBgColor(ImGuiTableBgTarget_CellBg, ...)'
                    // (цвет CellBg будет наложен на цвета RowBg и ColumnBg)
                    // Мы также можем передать номер столбца в качестве третьего параметра в TableSetBgColor() и сделать это вне цикла столбца.
                    if (row >= 1 && row <= 2 && column >= 1 && column <= 2 && cell_bg_type == 1)
                    {
                        ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Tree view"_RU >> u8"Таблицы/Древовидный вид");
    if (ImGui::TreeNode("Tree view"_RU >> u8"Древовидный вид"))
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("3ways", 3, flags))
        {
            // Первый столбец будет использовать _WidthStretch по умолчанию, когда ScrollX выключен, и _WidthFixed, когда ScrollX включен.
            ImGui::TableSetupColumn("Name"_RU >> u8"Имя", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Size"_RU >> u8"Размер", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            ImGui::TableSetupColumn("Type"_RU >> u8"Тип", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();

            // Простое хранилище для вывода фиктивной файловой системы.
            struct MyTreeNode
            {
                const char* Name;
                const char* Type;
                int         Size;
                int         ChildIdx;
                int         ChildCount;
                static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    const bool is_folder = (node->ChildCount > 0);
                    if (is_folder)
                    {
                        bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
                        ImGui::TableNextColumn();
                        ImGui::TextDisabled("--");
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(node->Type);
                        if (open)
                        {
                            for (int child_n = 0; child_n < node->ChildCount; child_n++)
                                DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", node->Size);
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(node->Type);
                    }
                }
            };
            static const MyTreeNode nodes[] = {
                {"Root"_RU >> u8"Корень", "Folder"_RU >> u8"Папка", -1, 1, 3},                                                                      // 0
                {"Music"_RU >> u8"Музыка", "Folder"_RU >> u8"Папка", -1, 4, 2},                                                                     // 1
                {"Textures"_RU >> u8"Текстуры", "Folder"_RU >> u8"Папка", -1, 6, 3},                                                                // 2
                {"desktop.ini", "System file"_RU >> u8"Системный файл", 1024, -1, -1},                                                              // 3
                {"File1_a.wav", "Audio file"_RU >> u8"Аудиофайл", 123000, -1, -1},                                                                  // 4
                {"File1_b.wav", "Audio file"_RU >> u8"Аудиофайл", 456000, -1, -1},                                                                  // 5
                {"Image001.png", "Image file"_RU >> u8"Файл изображения", 203128, -1, -1},                                                          // 6
                {"Copy of Image001.png"_RU >> u8"Копия Image001. png", "Image file"_RU >> u8"Файл изображения", 203256, -1, -1},                    // 7
                {"Copy of Image001 (Final2).png"_RU >> u8"Копия Image001 (Final2).png", "Image file"_RU >> u8"Файл изображения", 203512, -1, -1},   // 8
            };

            MyTreeNode::DisplayNode(&nodes[0], nodes);

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Item width"_RU >> u8"Таблицы/Ширина элемента");
    if (ImGui::TreeNode("Item width"_RU >> u8"Ширина элемента"))
    {
        HelpMarker("Showcase using PushItemWidth() and how it is preserved on a per-column basis.\n\n"
                   "Note that on auto-resizing non-resizable fixed columns, querying the content width for e.g. right-alignment doesn't make sense."_RU >>
            u8"Продемонстрируйте использование PushItemWidth() и то, как оно сохраняется для каждого столбца.\n\n"
            "Обратите внимание, что при автоматическом изменении размера неизменяемых фиксированных столбцов запрос ширины содержимого для e. г. выравнивание по правому краю не имеет смысла.");
        if (ImGui::BeginTable("table_item_width", 3, ImGuiTableFlags_Borders))
        {
            ImGui::TableSetupColumn("small"_RU >> u8"маленький");
            ImGui::TableSetupColumn("half"_RU >> u8"половина");
            ImGui::TableSetupColumn("right-align"_RU >> u8"выравнивание по правому краю");
            ImGui::TableHeadersRow();

            for (int row = 0; row < 3; row++)
            {
                ImGui::TableNextRow();
                if (row == 0)
                {
                    // Настройте ItemWidth один раз (вместо того, чтобы настраивать каждый раз, что также возможно, но менее эффективно)
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushItemWidth(TEXT_BASE_WIDTH * 3.0f);   // Маленький
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushItemWidth(-FLT_MIN);   // Выровнено по правому краю
                }

                // Нарисуйте наше содержимое
                static float dummy_f = 0.0f;
                ImGui::PushID(row);
                ImGui::TableSetColumnIndex(0);
                ImGui::SliderFloat("float0", &dummy_f, 0.0f, 1.0f);
                ImGui::TableSetColumnIndex(1);
                ImGui::SliderFloat("float1", &dummy_f, 0.0f, 1.0f);
                ImGui::TableSetColumnIndex(2);
                ImGui::SliderFloat("float2", &dummy_f, 0.0f, 1.0f);
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    // Продемонстрируйте использование вызовов TableHeader() вместо TableHeadersRow()
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Custom headers"_RU >> u8"Таблицы/Пользовательские заголовки");
    if (ImGui::TreeNode("Custom headers"_RU >> u8"Пользовательские заголовки"))
    {
        const int COLUMNS_COUNT = 3;
        if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
        {
            ImGui::TableSetupColumn("Apricot"_RU >> u8"Абрикос");
            ImGui::TableSetupColumn("Banana"_RU >> u8"Банан");
            ImGui::TableSetupColumn("Cherry"_RU >> u8"Вишня");

            // Фиктивное хранилище выбора всего столбца
            // FIXME: Было бы неплохо продемонстрировать полнофункциональный выбор с помощью этих флажков.
            static bool column_selected[3] = {};

            // Вместо того, чтобы вызывать TableHeadersRow(), мы сами отправим пользовательские заголовки.
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            for (int column = 0; column < COLUMNS_COUNT; column++)
            {
                ImGui::TableSetColumnIndex(column);
                const char* column_name = ImGui::TableGetColumnName(column);   // Получить имя, переданное в TableSetupColumn()
                ImGui::PushID(column);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("##checkall", &column_selected[column]);
                ImGui::PopStyleVar();
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::TableHeader(column_name);
                ImGui::PopID();
            }

            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    char buf[32];
                    sprintf(buf, "Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Selectable(buf, column_selected[column]);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    // Продемонстрируйте создание настраиваемых контекстных меню внутри столбцов, поэкспериментировав с контекстными меню, предоставляемыми TableHeadersRow()/TableHeader().
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Context menus"_RU >> u8"Таблицы/Контекстные меню");
    if (ImGui::TreeNode("Context menus"_RU >> u8"Контекстные меню"))
    {
        HelpMarker("By default, right-clicking over a TableHeadersRow()/TableHeader() line will open the default context-menu.\nUsing ImGuiTableFlags_ContextMenuInBody we also allow right-clicking over columns body."_RU >> u8"По умолчанию щелчок правой кнопкой мыши по строке TableHeadersRow()/TableHeader() открывает контекстное меню по умолчанию.\nИспользуя ImGuiTableFlags_ContextMenuInBody, мы также разрешаем щелкать правой кнопкой мыши по телу столбца.");
        static ImGuiTableFlags flags1 = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody;

        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags1, ImGuiTableFlags_ContextMenuInBody);
        PopStyleCompact();

        // Контекстные меню: первый пример
        // [1.1] Щелкните правой кнопкой мыши строку TableHeadersRow(), чтобы открыть контекстное меню таблицы по умолчанию.
        // [1.2] Щелчок правой кнопкой мыши в столбцах также открывает контекстное меню таблицы по умолчанию (если установлено ImGuiTableFlags_ContextMenuInBody)
        const int COLUMNS_COUNT = 3;
        if (ImGui::BeginTable("table_context_menu", COLUMNS_COUNT, flags1))
        {
            ImGui::TableSetupColumn("One"_RU >> u8"Один");
            ImGui::TableSetupColumn("Two"_RU >> u8"Два");
            ImGui::TableSetupColumn("Three"_RU >> u8"Три");

            // [1.1]] Щелкните правой кнопкой мыши строку TableHeadersRow(), чтобы открыть контекстное меню таблицы по умолчанию.
            ImGui::TableHeadersRow();

            // Отправить фиктивное содержимое
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                }
            }
            ImGui::EndTable();
        }

        // Контекстные меню: второй пример
        // [2.1] Щелкните правой кнопкой мыши строку TableHeadersRow(), чтобы открыть контекстное меню таблицы по умолчанию.
        // [2.2] Щелкните правой кнопкой мыши на «..», чтобы открыть пользовательское всплывающее окно
        // [2.3] Щелкните правой кнопкой мыши в столбцах, чтобы открыть другое пользовательское всплывающее окно
        HelpMarker("Demonstrate mixing table context menu (over header), item context button (over button) and custom per-colum context menu (over column body)."_RU >> u8"Демонстрация контекстного меню таблицы микширования (над заголовком), кнопки контекста элемента (кнопка над) и пользовательского контекстного меню для каждого столбца (над телом столбца).");
        ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
        if (ImGui::BeginTable("table_context_menu_2", COLUMNS_COUNT, flags2))
        {
            ImGui::TableSetupColumn("One"_RU >> u8"Один");
            ImGui::TableSetupColumn("Two"_RU >> u8"Два");
            ImGui::TableSetupColumn("Three"_RU >> u8"Три");

            // [2.1] Щелкните правой кнопкой мыши строку TableHeadersRow(), чтобы открыть контекстное меню таблицы по умолчанию.
            ImGui::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    // Отправить фиктивное содержимое
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Cell %d,%d"_RU >> u8"Ячейка %d,%d", column, row);
                    ImGui::SameLine();

                    // [2.2] Щелкните правой кнопкой мыши «..», чтобы открыть пользовательское всплывающее окно.
                    ImGui::PushID(row * COLUMNS_COUNT + column);
                    ImGui::SmallButton("..");
                    if (ImGui::BeginPopupContextItem())
                    {
                        ImGui::Text("This is the popup for Button(\"..\") in Cell %d,%d"_RU >> u8"Это всплывающее окно для кнопки(\"..\") в Ячейке %d,%d", column, row);
                        if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                }
            }

            // [2.3] Щелкните правой кнопкой мыши в любом месте столбца, чтобы открыть другое пользовательское всплывающее окно.
            // (вместо проверки на ! IsAnyItemHovered() мы могли бы также вызвать OpenPopup() с ImGuiPopupFlags_NoOpenOverExistingPopup
            // для управления приоритетом всплывающих окон при срабатывании всплывающих окон, здесь «мы наводим курсор на столбец» перекрываются)
            int hovered_column = -1;
            for (int column = 0; column < COLUMNS_COUNT + 1; column++)
            {
                ImGui::PushID(column);
                if (ImGui::TableGetColumnFlags(column) & ImGuiTableColumnFlags_IsHovered)
                    hovered_column = column;
                if (hovered_column == column && !ImGui::IsAnyItemHovered() && ImGui::IsMouseReleased(1))
                    ImGui::OpenPopup("MyPopup"_RU >> u8"Мое всплывающее окно");
                if (ImGui::BeginPopup("MyPopup"_RU >> u8"Мое всплывающее окно"))
                {
                    if (column == COLUMNS_COUNT)
                        ImGui::Text("This is a custom popup for unused space after the last column."_RU >> u8"Это пользовательское всплывающее окно для неиспользуемого пространства после последнего столбца.");
                    else
                        ImGui::Text("This is a custom popup for Column %d"_RU >> u8"Это настраиваемое всплывающее окно для столбца %d", column);
                    if (ImGui::Button("Close"_RU >> u8"Закрыть"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }

            ImGui::EndTable();
            ImGui::Text("Hovered column: %d"_RU >> u8"Наведенный столбец: %d", hovered_column);
        }
        ImGui::TreePop();
    }

    // Продемонстрируйте создание нескольких таблиц с одинаковым идентификатором
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Synced instances"_RU >> u8"Таблицы/Синхронизированные экземпляры");
    if (ImGui::TreeNode("Synced instances"_RU >> u8"Синхронизированные экземпляры"))
    {
        HelpMarker("Multiple tables with the same identifier will share their settings, width, visibility, order etc."_RU >> u8"Несколько таблиц с одним и тем же идентификатором будут иметь общие настройки, ширину, видимость, порядок и т. д.");
        for (int n = 0; n < 3; n++)
        {
            char buf[32];
            sprintf(buf, "Synced Table %d"_RU >> u8"Синхронизированная таблица %d", n);
            bool open = ImGui::CollapsingHeader(buf, ImGuiTreeNodeFlags_DefaultOpen);
            if (open && ImGui::BeginTable("Table"_RU >> u8"Таблица", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings))
            {
                ImGui::TableSetupColumn("One"_RU >> u8"Один");
                ImGui::TableSetupColumn("Two"_RU >> u8"Два");
                ImGui::TableSetupColumn("Three"_RU >> u8"Три");
                ImGui::TableHeadersRow();
                for (int cell = 0; cell < 9; cell++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("this cell %d"_RU >> u8"эта ячейка %d", cell);
                }
                ImGui::EndTable();
            }
        }
        ImGui::TreePop();
    }

    // Демонстрация использования средств сортировки
    // Это упрощенная версия «Расширенного» примера, где мы в основном фокусируемся на коде, необходимом для обработки сортировки.
    // Обратите внимание, что в примере «Расширенный» также демонстрируется запуск сортировки вручную (например, если количество товаров было изменено)
    static const char* template_items_names[] = {"Banana"_RU >> u8"Банан", "Apple"_RU >> u8"Яблоко", "Cherry"_RU >> u8"Вишня", "Watermelon"_RU >> u8"Арбуз", "Grapefruit"_RU >> u8"Грейпфрут", "Strawberry"_RU >> u8"Клубника", "Mango"_RU >> u8"Манго", "Kiwi"_RU >> u8"Киви", "Orange"_RU >> u8"Апельсин", "Pineapple"_RU >> u8"Ананас", "Blueberry"_RU >> u8"Черника", "Plum"_RU >> u8"Слива", "Coconut"_RU >> u8"Кокос", "Pear"_RU >> u8"Груша", "Apricot"_RU >> u8"Абрикос"};
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Sorting"_RU >> u8"Таблицы/Сортировка");
    if (ImGui::TreeNode("Sorting"_RU >> u8"Сортировка"))
    {
        // Создать список элементов
        static ImVector<MyItem> items;
        if (items.Size == 0)
        {
            items.resize(50, MyItem());
            for (int n = 0; n < items.Size; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem&   item       = items[n];
                item.ID              = n;
                item.Name            = template_items_names[template_n];
                item.Quantity        = (n * n - n) % 20;   // Назначение количества по умолчанию
            }
        }

        // Опции
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
        PushStyleCompact();
        ImGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
        ImGui::SameLine();
        HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1)."_RU >> u8"Когда сортировка включена: удерживайте клавишу Shift при нажатии на заголовки для сортировки по нескольким столбцам. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount > 1).");
        ImGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
        ImGui::SameLine();
        HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0)."_RU >> u8"Когда сортировка включена: не разрешать сортировку, отключать сортировку по умолчанию. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount == 0).");
        PopStyleCompact();

        if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
        {
            // Объявлять столбцы
            // Мы используем параметр "user_id" TableSetupColumn(), чтобы указать идентификатор пользователя, который будет сохранен в спецификациях сортировки.
            // Это делается для того, чтобы наша функция сортировки могла идентифицировать столбец, учитывая наш собственный идентификатор. Мы также могли бы идентифицировать их по их индексу!
            // Продемонстрировать использование комбинации флагов среди доступных флагов, связанных с сортировкой:
            // - ImGuiTableColumnFlags_DefaultSort
            // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
            // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_ID);
            ImGui::TableSetupColumn("Name"_RU >> u8"Имя", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
            ImGui::TableSetupColumn("Action"_RU >> u8"Действие", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
            ImGui::TableSetupColumn("Quantity"_RU >> u8"Количество", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Quantity);
            ImGui::TableSetupScrollFreeze(0, 1);   // Сделать строку всегда видимой
            ImGui::TableHeadersRow();

            // Отсортируйте наши данные, если спецификации сортировки были изменены!
            if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
                if (sorts_specs->SpecsDirty)
                {
                    MyItem::s_current_sort_specs = sorts_specs;   // Хранить в переменной, доступной для функции сортировки.
                    if (items.Size > 1)
                        qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
                    MyItem::s_current_sort_specs = NULL;
                    sorts_specs->SpecsDirty      = false;
                }

            // Продемонстрируйте использование клипера для больших вертикальных списков
            ImGuiListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Показать элемент данных
                    MyItem* item = &items[row_n];
                    ImGui::PushID(item->ID);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%04d", item->ID);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(item->Name);
                    ImGui::TableNextColumn();
                    ImGui::SmallButton("None");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", item->Quantity);
                    ImGui::PopID();
                }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    // В этом примере мы представим большинство флагов и настроек таблицы.
    // Конкретные флаги и настройки см. в соответствующем разделе для получения более подробной информации.
    // Этот раздел в основном полезен для экспериментов с комбинацией определенных флагов или настроек друг с другом.
    //ImGui::SetNextItemOpen(true, ImGuiCond_Once); // [DEBUG]
    if (open_action != -1)
        ImGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Advanced"_RU >> u8"Таблицы/Расширенный");
    if (ImGui::TreeNode("Advanced"_RU >> u8"Расширенный"))
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;

        enum ContentsType
        {
            CT_Text,
            CT_Button,
            CT_SmallButton,
            CT_FillButton,
            CT_Selectable,
            CT_SelectableSpanRow
        };
        static int    contents_type           = CT_SelectableSpanRow;
        const char*   contents_type_names[]   = {"Text"_RU >> u8"Текст", "Button"_RU >> u8"Кнопка", "SmallButton"_RU >> u8"Маленькая Кнопка", "FillButton"_RU >> u8"Кнопка Заполнения", "Selectable"_RU >> u8"Выбираемый", "Selectable (span row)"_RU >> u8"Выбираемый (пролетная строка)"};
        static int    freeze_cols             = 1;
        static int    freeze_rows             = 1;
        static int    items_count             = IM_ARRAYSIZE(template_items_names) * 2;
        static ImVec2 outer_size_value        = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);
        static float  row_min_height          = 0.0f;   // Авто
        static float  inner_width_with_scroll = 0.0f;   // Автопродление
        static bool   outer_size_enabled      = true;
        static bool   show_headers            = true;
        static bool   show_wrapped_text       = false;
        //static ImGuiTextFilter filter;
        //ImGui::SetNextItemOpen(true, ImGuiCond_Once); // FIXME-TABLE: Включение этого приводит к начальному обрезанию первого прохода по таблице, что, как правило, влияет на размер столбца.
        if (ImGui::TreeNode("Options"_RU >> u8"Опции"))
        {
            // Сделайте пользовательский интерфейс компактным, потому что в нем так много полей.
            PushStyleCompact();
            ImGui::PushItemWidth(TEXT_BASE_WIDTH * 28.0f);

            if (ImGui::TreeNodeEx("Features:"_RU >> u8"Функции:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
                ImGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable);
                ImGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable);
                ImGui::CheckboxFlags("ImGuiTableFlags_Sortable", &flags, ImGuiTableFlags_Sortable);
                ImGui::CheckboxFlags("ImGuiTableFlags_NoSavedSettings", &flags, ImGuiTableFlags_NoSavedSettings);
                ImGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags, ImGuiTableFlags_ContextMenuInBody);
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Decorations:"_RU >> u8"Украшения:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
                ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody);
                ImGui::SameLine();
                HelpMarker("Disable vertical borders in columns Body (borders will always appears in Headers)"_RU >> u8"Отключите вертикальные границы в колонках Body (границы всегда будут отображаться в Headers)");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize);
                ImGui::SameLine();
                HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appears in Headers)"_RU >> u8"Отключить вертикальные границы в колонках 'Тело' до тех пор, пока на них не наведут курсор для изменения размера (границы всегда будут отображаться в заголовках)");
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Sizing:"_RU >> u8"Размеры:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                EditTableSizingFlags(&flags);
                ImGui::SameLine();
                HelpMarker("In the Advanced demo we override the policy of each column so those table-wide settings have less effect that typical."_RU >> u8"В расширенной демонстрации мы переопределяем политику каждого столбца, чтобы эти настройки для всей таблицы имели меньший эффект, чем обычные.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
                ImGui::SameLine();
                HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used."_RU >> u8"Сделайте так, чтобы внешняя ширина автоматически подгонялась под столбцы, переопределяя external_size.х значение.\n\nДоступно, только если ScrollX/ScrollY отключены и столбцы Stretch не используются.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY);
                ImGui::SameLine();
                HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible."_RU >> u8"Сделайте так, чтобы внешняя высота останавливалась точно на external_size.y (предотвратить автоматическое расширение таблицы за предел).\n\nДоступно, только если ScrollX/ScrollY отключены. Данные ниже предела будут обрезаны и не видны.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags, ImGuiTableFlags_NoKeepColumnsVisible);
                ImGui::SameLine();
                HelpMarker("Only available if ScrollX is disabled."_RU >> u8"Доступно, только если ScrollX отключен.");
                ImGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths);
                ImGui::SameLine();
                HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth."_RU >>
                    u8"Отключить распределение остаточной ширины на растянутые столбцы (выделение ширины для таблицы шириной 100 с 3 столбцами: Без этого флага: 33,33,34. С этим флагом: 33,33,33). При большем количестве столбцов изменение размера будет менее плавным.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoClip", &flags, ImGuiTableFlags_NoClip);
                ImGui::SameLine();
                HelpMarker("Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with ScrollFreeze options."_RU >> u8"Отключите прямоугольник обрезки для каждого отдельного столбца (уменьшите количество команд рисования, элементы смогут перетекать в другие столбцы). Обычно несовместим с параметрами ScrollFreeze.");
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Padding:"_RU >> u8"Отступы:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::CheckboxFlags("ImGuiTableFlags_PadOuterX", &flags, ImGuiTableFlags_PadOuterX);
                ImGui::CheckboxFlags("ImGuiTableFlags_NoPadOuterX", &flags, ImGuiTableFlags_NoPadOuterX);
                ImGui::CheckboxFlags("ImGuiTableFlags_NoPadInnerX", &flags, ImGuiTableFlags_NoPadInnerX);
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Scrolling:"_RU >> u8"Прокрутка:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
                ImGui::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
                ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
                ImGui::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Sorting:"_RU >> u8"Сортировка:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
                ImGui::SameLine();
                HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1)."_RU >> u8"Когда сортировка включена: удерживайте клавишу Shift при нажатии на заголовки для сортировки по нескольким столбцам. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount > 1).");
                ImGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
                ImGui::SameLine();
                HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0)."_RU >> u8"Когда сортировка включена: не разрешать сортировку, отключать сортировку по умолчанию. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount == 0).");
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Other:"_RU >> u8"Другой:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("show_headers", &show_headers);
                ImGui::Checkbox("show_wrapped_text", &show_wrapped_text);

                ImGui::DragFloat2("##OuterSize", &outer_size_value.x);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Checkbox("outer_size", &outer_size_enabled);
                ImGui::SameLine();
                HelpMarker("If scrolling is disabled (ScrollX and ScrollY not set):\n"
                           "- The table is output directly in the parent window.\n"
                           "- OuterSize.x < 0.0f will right-align the table.\n"
                           "- OuterSize.x = 0.0f will narrow fit the table unless there are any Stretch column.\n"
                           "- OuterSize.y then becomes the minimum size for the table, which will extend vertically if there are more rows (unless NoHostExtendY is set)."_RU >>
                    u8"Если прокрутка отключена (ScrollX и ScrollY не установлены):\n"
                    "- Таблица выводится прямо в родительское окно.\n"
                    "- OuterSize.x < 0.0f выровняет таблицу по правому краю.\n"
                    "- OuterSize.x = 0.0f сузит таблицу, если нет столбца Stretch.\n"
                    "- OuterSize.y затем становится минимальным размером для таблицы, которая будет расширяться по вертикали, если есть больше строк (если не установлен NoHostExtendY).");

                // С точки зрения пользователя мы склонны использовать 'inner_width' по-разному в зависимости от того, встроена ли в нашу таблицу прокрутка.
                // Чтобы облегчить игру с этой демонстрацией, мы фактически передадим 0.0f в BeginTable(), когда ScrollX отключен.
                ImGui::DragFloat("inner_width (when ScrollX active)"_RU >> u8"inner_width (когда ScrollX активен)", &inner_width_with_scroll, 1.0f, 0.0f, FLT_MAX);

                ImGui::DragFloat("row_min_height", &row_min_height, 1.0f, 0.0f, FLT_MAX);
                ImGui::SameLine();
                HelpMarker("Specify height of the Selectable item."_RU >> u8"Укажите высоту выбираемого элемента.");

                ImGui::DragInt("items_count", &items_count, 0.1f, 0, 9999);
                ImGui::Combo("items_type (first column)"_RU >> u8"items_type (первый столбец)", &contents_type, contents_type_names, IM_ARRAYSIZE(contents_type_names));
                //filter.Draw("filter");
                ImGui::TreePop();
            }

            ImGui::PopItemWidth();
            PopStyleCompact();
            ImGui::Spacing();
            ImGui::TreePop();
        }

        // Обновить список предметов, если мы изменили количество предметов
        static ImVector<MyItem> items;
        static ImVector<int>    selection;
        static bool             items_need_sort = false;
        if (items.Size != items_count)
        {
            items.resize(items_count, MyItem());
            for (int n = 0; n < items_count; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem&   item       = items[n];
                item.ID              = n;
                item.Name            = template_items_names[template_n];
                item.Quantity        = (template_n == 3) ? 10 : (template_n == 4) ? 20 : 0;   // Назначение количества по умолчанию
            }
        }

        const ImDrawList* parent_draw_list                = ImGui::GetWindowDrawList();
        const int         parent_draw_list_draw_cmd_count = parent_draw_list->CmdBuffer.Size;
        ImVec2            table_scroll_cur, table_scroll_max;   // Для отображения отладки
        const ImDrawList* table_draw_list    = NULL;            // "

        // Отправить таблицу
        const float       inner_width_to_use = (flags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
        if (ImGui::BeginTable("table_advanced", 6, flags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
        {
            // Объявить столбцы
            // Мы используем параметр user_id функции TableSetupColumn(), чтобы указать идентификатор пользователя, который будет храниться в спецификациях сортировки.
            // Это сделано для того, чтобы наша функция сортировки могла идентифицировать столбец по нашему собственному идентификатору. Мы также могли бы идентифицировать их на основе их индекса!
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 0.0f, MyItemColumnID_ID);
            ImGui::TableSetupColumn("Name"_RU >> u8"Имя", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
            ImGui::TableSetupColumn("Action"_RU >> u8"Действие", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
            ImGui::TableSetupColumn("Quantity"_RU >> u8"Количество", ImGuiTableColumnFlags_PreferSortDescending, 0.0f, MyItemColumnID_Quantity);
            ImGui::TableSetupColumn("Description"_RU >> u8"Описание", (flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Description);
            ImGui::TableSetupColumn("Hidden"_RU >> u8"Скрытый", ImGuiTableColumnFlags_DefaultHide | ImGuiTableColumnFlags_NoSort);
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);

            // Отсортируйте наши данные, если спецификации сортировки были изменены!
            ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs();
            if (sorts_specs && sorts_specs->SpecsDirty)
                items_need_sort = true;
            if (sorts_specs && items_need_sort && items.Size > 1)
            {
                MyItem::s_current_sort_specs = sorts_specs;   // Хранить в переменной, доступной для функции сортировки.
                qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
                MyItem::s_current_sort_specs = NULL;
                sorts_specs->SpecsDirty      = false;
            }
            items_need_sort                       = false;

            // Обратите внимание, сортируем ли мы в настоящее время по полю «Количество»,
            // мы будем использовать это для запуска сортировки, когда узнаем, что данные в этом столбце были изменены.
            const bool sorts_specs_using_quantity = (ImGui::TableGetColumnFlags(3) & ImGuiTableColumnFlags_IsSorted) != 0;

            // Показать заголовки
            if (show_headers)
                ImGui::TableHeadersRow();

            // Показать данные
            // FIXME-TABLE FIXME-NAV: Как мы можем добиться приличного подъема/опускания, даже если у нас есть кнопки?
            ImGui::PushButtonRepeat(true);
#if 1
            // Продемонстрируйте использование клипера для больших вертикальных списков
            ImGuiListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
            {
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
#else
            // Without clipper
            {
                for (int row_n = 0; row_n < items.Size; row_n++)
#endif
                {
                    MyItem*    item             = &items[row_n];
                    //if (!filter.PassFilter(item->Name))
                    //    continue;

                    const bool item_is_selected = selection.contains(item->ID);
                    ImGui::PushID(item->ID);
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

                    // Для демонстрационных целей мы можем выбирать среди различных типов элементов, представленных в первом столбце.
                    ImGui::TableSetColumnIndex(0);
                    char label[32];
                    sprintf(label, "%04d", item->ID);
                    if (contents_type == CT_Text)
                        ImGui::TextUnformatted(label);
                    else if (contents_type == CT_Button)
                        ImGui::Button(label);
                    else if (contents_type == CT_SmallButton)
                        ImGui::SmallButton(label);
                    else if (contents_type == CT_FillButton)
                        ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
                    else if (contents_type == CT_Selectable || contents_type == CT_SelectableSpanRow)
                    {
                        ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;
                        if (ImGui::Selectable(label, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
                        {
                            if (ImGui::GetIO().KeyCtrl)
                            {
                                if (item_is_selected)
                                    selection.find_erase_unsorted(item->ID);
                                else
                                    selection.push_back(item->ID);
                            }
                            else
                            {
                                selection.clear();
                                selection.push_back(item->ID);
                            }
                        }
                    }

                    if (ImGui::TableSetColumnIndex(1))
                        ImGui::TextUnformatted(item->Name);

                    // Здесь мы демонстрируем маркировку нашего набора данных как требующего повторной сортировки, если мы изменили количество,
                    // и мы в настоящее время сортируем по столбцу, показывающему количество.
                    // Чтобы избежать запуска сортировки при удерживании кнопки, мы запускаем ее только тогда, когда кнопка отпущена.
                    // Вам, вероятно, понадобится более продвинутая система в вашем коде, если вы хотите автоматически сортировать при изменении определенной записи.
                    if (ImGui::TableSetColumnIndex(2))
                    {
                        if (ImGui::SmallButton("Chop"_RU >> u8"Отбивная"))
                        {
                            item->Quantity += 1;
                        }
                        if (sorts_specs_using_quantity && ImGui::IsItemDeactivated())
                        {
                            items_need_sort = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton("Eat"_RU >> u8"Есть"))
                        {
                            item->Quantity -= 1;
                        }
                        if (sorts_specs_using_quantity && ImGui::IsItemDeactivated())
                        {
                            items_need_sort = true;
                        }
                    }

                    if (ImGui::TableSetColumnIndex(3))
                        ImGui::Text("%d", item->Quantity);

                    ImGui::TableSetColumnIndex(4);
                    if (show_wrapped_text)
                        ImGui::TextWrapped("Lorem ipsum dolor sit amet"_RU >> u8"клиент очень доволен");
                    else
                        ImGui::Text("Lorem ipsum dolor sit amet"_RU >> u8"клиент очень доволен");

                    if (ImGui::TableSetColumnIndex(5))
                        ImGui::Text("1234");

                    ImGui::PopID();
                }
            }
            ImGui::PopButtonRepeat();

            // Сохраните некоторую информацию, чтобы отобразить детали отладки ниже
            table_scroll_cur = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
            table_scroll_max = ImVec2(ImGui::GetScrollMaxX(), ImGui::GetScrollMaxY());
            table_draw_list  = ImGui::GetWindowDrawList();
            ImGui::EndTable();
        }
        static bool show_debug_details = false;
        ImGui::Checkbox("Debug details"_RU >> u8"Детали отладки", &show_debug_details);
        if (show_debug_details && table_draw_list)
        {
            ImGui::SameLine(0.0f, 0.0f);
            const int table_draw_list_draw_cmd_count = table_draw_list->CmdBuffer.Size;
            if (table_draw_list == parent_draw_list)
                ImGui::Text(": DrawCmd: +%d (in same window)"_RU >> u8": DrawCmd: +%d (в том же окне)", table_draw_list_draw_cmd_count - parent_draw_list_draw_cmd_count);
            else
                ImGui::Text(": DrawCmd: +%d (in child window), Scroll: (%.f/%.f) (%.f/%.f)"_RU >> u8": DrawCmd: +%d (в дочернем окне), Прокрутить: (%.f/%.f) (%.f/%.f)", table_draw_list_draw_cmd_count - 1, table_scroll_cur.x, table_scroll_max.x, table_scroll_cur.y, table_scroll_max.y);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();

    ShowDemoWindowColumns();

    if (disable_indent)
        ImGui::PopStyleVar();
}

// Продемонстрируйте старый/устаревший API Columns!
// [2020: Столбцы недостаточно представлены и не поддерживаются. Лучше использовать более гибкий и мощный API BeginTable()!]
static void ShowDemoWindowColumns()
{
    IMGUI_DEMO_MARKER("Columns (legacy API)"_RU >> u8"Столбцы (устаревший API)");
    bool open = ImGui::TreeNode("Legacy Columns API"_RU >> u8"Устаревший API столбцов");
    ImGui::SameLine();
    HelpMarker("Columns() is an old API! Prefer using the more flexible and powerful BeginTable() API!"_RU >> u8"Columns() — это старый API! Лучше использовать более гибкий и мощный API BeginTable()!");
    if (!open)
        return;

    // Основные столбцы
    IMGUI_DEMO_MARKER("Columns (legacy API)/Basic"_RU >> u8"Столбцы (устаревший API)/Базовый");
    if (ImGui::TreeNode("Basic"_RU >> u8"Базовый"))
    {
        ImGui::Text("Without border:"_RU >> u8"Без границы:");
        ImGui::Columns(3, "mycolumns3", false);   // 3-сторонняя, без границы
        ImGui::Separator();
        for (int n = 0; n < 14; n++)
        {
            char label[32];
            sprintf(label, "Item %d"_RU >> u8"Элемент %d", n);
            if (ImGui::Selectable(label))
            {}
            //if (ImGui::Button(label, ImVec2(-FLT_MIN,0.0f))) {}
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::Text("With border:"_RU >> u8"С границами:");
        ImGui::Columns(4, "mycolumns");   // 4-полосная, с границами
        ImGui::Separator();
        ImGui::Text("ID");
        ImGui::NextColumn();
        ImGui::Text("Name"_RU >> u8"Имя");
        ImGui::NextColumn();
        ImGui::Text("Path"_RU >> u8"Путь");
        ImGui::NextColumn();
        ImGui::Text("Hovered"_RU >> u8"Наведение");
        ImGui::NextColumn();
        ImGui::Separator();
        const char* names[3] = {"One"_RU >> u8"Один", "Two"_RU >> u8"Два", "Three"_RU >> u8"Три"};
        const char* paths[3] = {"/path/one"_RU >> u8"/путь/один", "/path/two"_RU >> u8"/путь/два", "/path/three"_RU >> u8"/путь/три"};
        static int  selected = -1;
        for (int i = 0; i < 3; i++)
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                selected = i;
            bool hovered = ImGui::IsItemHovered();
            ImGui::NextColumn();
            ImGui::Text(names[i]);
            ImGui::NextColumn();
            ImGui::Text(paths[i]);
            ImGui::NextColumn();
            ImGui::Text("%d", hovered);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Borders"_RU >> u8"Столбцы (старый API)/Границы");
    if (ImGui::TreeNode("Borders"_RU >> u8"Границы"))
    {
        // NB: Будущие столбцы API должны разрешать автоматические горизонтальные границы.
        static bool h_borders     = true;
        static bool v_borders     = true;
        static int  columns_count = 4;
        const int   lines_count   = 3;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns"_RU >> u8"%d столбцы");
        if (columns_count < 2)
            columns_count = 2;
        ImGui::SameLine();
        ImGui::Checkbox("horizontal"_RU >> u8"горизонтальный", &h_borders);
        ImGui::SameLine();
        ImGui::Checkbox("vertical"_RU >> u8"вертикальный", &v_borders);
        ImGui::Columns(columns_count, NULL, v_borders);
        for (int i = 0; i < columns_count * lines_count; i++)
        {
            if (h_borders && ImGui::GetColumnIndex() == 0)
                ImGui::Separator();
            ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
            ImGui::Text("Width %.2f"_RU >> u8"Ширина %.2f", ImGui::GetColumnWidth());
            ImGui::Text("Avail %.2f"_RU >> u8"Доступно %.2f", ImGui::GetContentRegionAvail().x);
            ImGui::Text("Offset %.2f"_RU >> u8"Смещение %.2f", ImGui::GetColumnOffset());
            ImGui::Text("Long text that is likely to clip"_RU >> u8"Длинный текст, который может быть обрезан");
            ImGui::Button("Button"_RU >> u8"Кнопка", ImVec2(-FLT_MIN, 0.0f));
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        if (h_borders)
            ImGui::Separator();
        ImGui::TreePop();
    }

    // Создайте несколько элементов в одной ячейке перед переключением на следующий столбец
    IMGUI_DEMO_MARKER("Columns (legacy API)/Mixed items"_RU >> u8"Столбцы (старый API)/Смешанные элементы");
    if (ImGui::TreeNode("Mixed items"_RU >> u8"Смешанные элементы"))
    {
        ImGui::Columns(3, "mixed"_RU >> u8"смешанный");
        ImGui::Separator();

        ImGui::Text("Hello"_RU >> u8"Привет");
        ImGui::Button("Banana"_RU >> u8"Банан");
        ImGui::NextColumn();

        ImGui::Text("ImGui");
        ImGui::Button("Apple"_RU >> u8"Яблоко");
        static float foo = 1.0f;
        ImGui::InputFloat("red"_RU >> u8"красный", &foo, 0.05f, 0, "%.3f");
        ImGui::Text("An extra line here."_RU >> u8"Здесь лишняя строка.");
        ImGui::NextColumn();

        ImGui::Text("Sailor"_RU >> u8"Моряк");
        ImGui::Button("Corniflower"_RU >> u8"Василек");
        static float bar = 1.0f;
        ImGui::InputFloat("blue"_RU >> u8"синий", &bar, 0.05f, 0, "%.3f");
        ImGui::NextColumn();

        if (ImGui::CollapsingHeader("Category A"_RU >> u8"Категория A"))
        {
            ImGui::Text("Бла бла бла");
        }
        ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category B"_RU >> u8"Категория B"))
        {
            ImGui::Text("Бла бла бла");
        }
        ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category C"_RU >> u8"Категория C"))
        {
            ImGui::Text("Бла бла бла");
        }
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    // перенос слов
    IMGUI_DEMO_MARKER("Columns (legacy API)/Word-wrapping"_RU >> u8"Столбцы (старый API)/перенос слов");
    if (ImGui::TreeNode("Word-wrapping"_RU >> u8"перенос слов"))
    {
        ImGui::Columns(2, "word-wrapping"_RU >> u8"перенос слов");
        ImGui::Separator();
        ImGui::TextWrapped("The quick brown fox jumps over the lazy dog."_RU >> u8"Быстрая коричневая лиса прыгает через ленивую собаку.");
        ImGui::TextWrapped("Hello Left"_RU >> u8"Привет, Слева");
        ImGui::NextColumn();
        ImGui::TextWrapped("The quick brown fox jumps over the lazy dog."_RU >> u8"Быстрая коричневая лиса прыгает через ленивую собаку.");
        ImGui::TextWrapped("Hello Right"_RU >> u8"Привет, Справа");
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Horizontal Scrolling"_RU >> u8"Столбцы (устаревший API)/Горизонтальная прокрутка");
    if (ImGui::TreeNode("Horizontal Scrolling"_RU >> u8"Горизонтальная прокрутка"))
    {
        ImGui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ImVec2 child_size = ImVec2(0, ImGui::GetFontSize() * 20.0f);
        ImGui::BeginChild("##ScrollingRegion", child_size, false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Columns(10);

        // Также продемонстрируйте использование clipper для больших вертикальных списков.
        int              ITEMS_COUNT = 2000;
        ImGuiListClipper clipper;
        clipper.Begin(ITEMS_COUNT);
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                for (int j = 0; j < 10; j++)
                {
                    ImGui::Text("Line %d Column %d..."_RU >> u8"Строка %d Столбец %d...", i, j);
                    ImGui::NextColumn();
                }
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Tree"_RU >> u8"Столбцы (старый API)/Дерево");
    if (ImGui::TreeNode("Tree"_RU >> u8"Дерево"))
    {
        ImGui::Columns(2, "tree"_RU >> u8"дерево", true);
        for (int x = 0; x < 3; x++)
        {
            bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Node%d"_RU >> u8"Узел%d", x);
            ImGui::NextColumn();
            ImGui::Text("Node contents"_RU >> u8"Содержимое узла");
            ImGui::NextColumn();
            if (open1)
            {
                for (int y = 0; y < 3; y++)
                {
                    bool open2 = ImGui::TreeNode((void*)(intptr_t)y, "Node%d.%d"_RU >> u8"Узел%d.%d", x, y);
                    ImGui::NextColumn();
                    ImGui::Text("Node contents"_RU >> u8"Содержимое узла");
                    if (open2)
                    {
                        ImGui::Text("Even more contents"_RU >> u8"Еще больше контента");
                        if (ImGui::TreeNode("Tree in column"_RU >> u8"Дерево в столбце"))
                        {
                            ImGui::Text("The quick brown fox jumps over the lazy dog"_RU >> u8"Быстрая коричневая лиса прыгает через ленивую собаку");
                            ImGui::TreePop();
                        }
                    }
                    ImGui::NextColumn();
                    if (open2)
                        ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::Columns(1);
        ImGui::TreePop();
    }

    ImGui::TreePop();
}

namespace ImGui
{
    extern ImGuiKeyData* GetKeyData(ImGuiKey key);
}

static void ShowDemoWindowMisc()
{
    IMGUI_DEMO_MARKER("Filtering"_RU >> u8"Фильтрация");
    if (ImGui::CollapsingHeader("Filtering"_RU >> u8"Фильтрация"))
    {
        // Вспомогательный класс для простой настройки текстового фильтра.
        // Возможно, вы захотите реализовать более полнофункциональную схему фильтрации в своем собственном приложении.
        static ImGuiTextFilter filter;
        ImGui::Text("Filter usage:\n"
                    "  \"\"         display all lines\n"
                    "  \"xxx\"      display lines containing \"xxx\"\n"
                    "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                    "  \"-xxx\"     hide lines containing \"xxx\""_RU >>
            u8"Использование фильтра:\n"
            "  \"\"         отображать все строки\n"
            "  \"xxx\"      отображать строки, содержащие \"xxx\"\n"
            "  \"xxx,yyy\"  отображать строки, содержащие \"xxx\" или \"yyy\"\n"
            "  \"-xxx\"     скрыть строки, содержащие \"xxx\"");
        filter.Draw();
        const char* lines[] = {"aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world"_RU >> u8"Привет, мир"};
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                ImGui::BulletText("%s", lines[i]);
    }

    IMGUI_DEMO_MARKER("Inputs, Navigation & Focus"_RU >> u8"Вводы, Навигация и Фокус");
    if (ImGui::CollapsingHeader("Inputs, Navigation & Focus"_RU >> u8"Вводы, Навигация и Фокус"))
    {
        ImGuiIO& io = ImGui::GetIO();

        // Отображать выходные флаги ImGuiIO
        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Output"_RU >> u8"Вводы, Навигация и Фокус/Вывод");
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Output"_RU >> u8"Выход"))
        {
            ImGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
            ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
            ImGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
            ImGui::Text("io.WantTextInput: %d", io.WantTextInput);
            ImGui::Text("io.WantSetMousePos: %d", io.WantSetMousePos);
            ImGui::Text("io.NavActive: %d, io.NavVisible: %d", io.NavActive, io.NavVisible);
            ImGui::TreePop();
        }

        // Показать состояние мыши
        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse State"_RU >> u8"Вводы, Навигация и Фокус/Состояние Мыши");
        if (ImGui::TreeNode("Mouse State"_RU >> u8"Состояние Мыши"))
        {
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)"_RU >> u8"Позиция Мыши: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse pos: <INVALID>"_RU >> u8"Позиция Мыши: <INVALID>");
            ImGui::Text("Mouse delta: (%g, %g)"_RU >> u8"Дельта Мыши: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);

            int count = IM_ARRAYSIZE(io.MouseDown);
            ImGui::Text("Mouse down:"_RU >> u8"Мышь вниз:");
            for (int i = 0; i < count; i++)
                if (ImGui::IsMouseDown(i))
                {
                    ImGui::SameLine();
                    ImGui::Text("b%d (%.02f secs)"_RU >> u8"b%d (%.02f сек)", i, io.MouseDownDuration[i]);
                }
            ImGui::Text("Mouse clicked:"_RU >> u8"Щелчок мышью:");
            for (int i = 0; i < count; i++)
                if (ImGui::IsMouseClicked(i))
                {
                    ImGui::SameLine();
                    ImGui::Text("b%d (%d)", i, ImGui::GetMouseClickedCount(i));
                }
            ImGui::Text("Mouse released:"_RU >> u8"Мышь выпущена:");
            for (int i = 0; i < count; i++)
                if (ImGui::IsMouseReleased(i))
                {
                    ImGui::SameLine();
                    ImGui::Text("b%d", i);
                }
            ImGui::Text("Mouse wheel: %.1f"_RU >> u8"Колесико мыши: %.1f", io.MouseWheel);
            ImGui::Text("Pen Pressure: %.1f"_RU >> u8"Давление пера: %.1f", io.PenPressure);   // Примечание: в настоящее время не используется
            ImGui::TreePop();
        }

        // Отображение состояния клавиатуры/мыши
        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Keyboard, Gamepad & Navigation State"_RU >> u8"Вводы, Навигация и Фокус/Клавиатура, Геймпад и Состояние Навигации");
        if (ImGui::TreeNode("Keyboard, Gamepad & Navigation State"_RU >> u8"Клавиатура, Геймпад и Состояние Навигации"))
        {
            // Мы повторяем как устаревший собственный диапазон, так и именованные диапазоны ImGuiKey, что немного странно, но позволяет отображать данные для старых/новых бэкэндов.
            // Код пользователя никогда не должен проходить через такие обручи: старый код может использовать собственные коды клавиш, новый код может использовать коды ImGuiKey.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
            struct funcs
            {
                static bool IsLegacyNativeDupe(ImGuiKey)
                {
                    return false;
                }
            };
            const ImGuiKey key_first = ImGuiKey_NamedKey_BEGIN;
#else
            struct funcs
            {
                static bool IsLegacyNativeDupe(ImGuiKey key)
                {
                    return key < 512 && ImGui::GetIO().KeyMap[key] != -1;
                }
            };   // Скрыть дубликаты Native<>ImGuiKey, если оба существуют в массиве
            const ImGuiKey key_first = 0;
            //ImGui::Text("Legacy raw:");       for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (io.KeysDown[key]) { ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); } }
#endif
            ImGui::Text("Keys down:"_RU >> u8"Клавиши вниз:");
            for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++)
            {
                if (funcs::IsLegacyNativeDupe(key))
                    continue;
                if (ImGui::IsKeyDown(key))
                {
                    ImGui::SameLine();
                    ImGui::Text("\"%s\" %d (%.02f secs)", ImGui::GetKeyName(key), key, ImGui::GetKeyData(key)->DownDuration);
                }
            }
            ImGui::Text("Keys pressed:"_RU >> u8"Клавиши нажаты:");
            for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++)
            {
                if (funcs::IsLegacyNativeDupe(key))
                    continue;
                if (ImGui::IsKeyPressed(key))
                {
                    ImGui::SameLine();
                    ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key);
                }
            }
            ImGui::Text("Keys released:"_RU >> u8"Ключи отпущены:");
            for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++)
            {
                if (funcs::IsLegacyNativeDupe(key))
                    continue;
                if (ImGui::IsKeyReleased(key))
                {
                    ImGui::SameLine();
                    ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key);
                }
            }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:"_RU >> u8"Очередь символов:");
            for (int i = 0; i < io.InputQueueCharacters.Size; i++)
            {
                ImWchar c = io.InputQueueCharacters[i];
                ImGui::SameLine();
                ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c);
            }   // FIXME: Мы должны преобразовать 'c' в UTF-8 здесь, но функции не являются общедоступными.
            ImGui::Text("NavInputs down:"_RU >> u8"NavInputs вниз:");
            for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++)
                if (io.NavInputs[i] > 0.0f)
                {
                    ImGui::SameLine();
                    ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]);
                }
            ImGui::Text("NavInputs pressed:"_RU >> u8"NavInputs нажаты:");
            for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++)
                if (io.NavInputsDownDuration[i] == 0.0f)
                {
                    ImGui::SameLine();
                    ImGui::Text("[%d]", i);
                }

            // Нарисуйте произвольную раскладку клавиатуры США, чтобы визуализировать переведенные клавиши
            {
                const ImVec2 key_size          = ImVec2(35.0f, 35.0f);
                const float  key_rounding      = 3.0f;
                const ImVec2 key_face_size     = ImVec2(25.0f, 25.0f);
                const ImVec2 key_face_pos      = ImVec2(5.0f, 3.0f);
                const float  key_face_rounding = 2.0f;
                const ImVec2 key_label_pos     = ImVec2(7.0f, 4.0f);
                const ImVec2 key_step          = ImVec2(key_size.x - 1.0f, key_size.y - 1.0f);
                const float  key_row_offset    = 9.0f;

                ImVec2       board_min         = ImGui::GetCursorScreenPos();
                ImVec2       board_max         = ImVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset + 10.0f, board_min.y + 3 * key_step.y + 10.0f);
                ImVec2       start_pos         = ImVec2(board_min.x + 5.0f - key_step.x, board_min.y);

                struct KeyLayoutData
                {
                    int         Row, Col;
                    const char* Label;
                    ImGuiKey    Key;
                };
                const KeyLayoutData keys_to_display[] = {{0, 0, "", ImGuiKey_Tab}, {0, 1, "Q", ImGuiKey_Q}, {0, 2, "W", ImGuiKey_W}, {0, 3, "E", ImGuiKey_E}, {0, 4, "R", ImGuiKey_R}, {1, 0, "", ImGuiKey_CapsLock}, {1, 1, "A", ImGuiKey_A}, {1, 2, "S", ImGuiKey_S}, {1, 3, "D", ImGuiKey_D}, {1, 4, "F", ImGuiKey_F}, {2, 0, "", ImGuiKey_LeftShift}, {2, 1, "Z", ImGuiKey_Z}, {2, 2, "X", ImGuiKey_X}, {2, 3, "C", ImGuiKey_C}, {2, 4, "V", ImGuiKey_V}};

                ImDrawList*         draw_list         = ImGui::GetWindowDrawList();
                draw_list->PushClipRect(board_min, board_max, true);
                for (int n = 0; n < IM_ARRAYSIZE(keys_to_display); n++)
                {
                    const KeyLayoutData* key_data = &keys_to_display[n];
                    ImVec2               key_min  = ImVec2(start_pos.x + key_data->Col * key_step.x + key_data->Row * key_row_offset, start_pos.y + key_data->Row * key_step.y);
                    ImVec2               key_max  = ImVec2(key_min.x + key_size.x, key_min.y + key_size.y);
                    draw_list->AddRectFilled(key_min, key_max, IM_COL32(204, 204, 204, 255), key_rounding);
                    draw_list->AddRect(key_min, key_max, IM_COL32(24, 24, 24, 255), key_rounding);
                    ImVec2 face_min = ImVec2(key_min.x + key_face_pos.x, key_min.y + key_face_pos.y);
                    ImVec2 face_max = ImVec2(face_min.x + key_face_size.x, face_min.y + key_face_size.y);
                    draw_list->AddRect(face_min, face_max, IM_COL32(193, 193, 193, 255), key_face_rounding, ImDrawFlags_None, 2.0f);
                    draw_list->AddRectFilled(face_min, face_max, IM_COL32(252, 252, 252, 255), key_face_rounding);
                    ImVec2 label_min = ImVec2(key_min.x + key_label_pos.x, key_min.y + key_label_pos.y);
                    draw_list->AddText(label_min, IM_COL32(64, 64, 64, 255), key_data->Label);
                    if (ImGui::IsKeyDown(key_data->Key))
                        draw_list->AddRectFilled(key_min, key_max, IM_COL32(255, 0, 0, 128), key_rounding);
                }
                draw_list->PopClipRect();
                ImGui::Dummy(ImVec2(board_max.x - board_min.x, board_max.y - board_min.y));
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Capture override"_RU >> u8"Переопределение захвата"))
        {
            HelpMarker("The value of io.WantCaptureMouse and io.WantCaptureKeyboard are normally set by Dear ImGui "
                       "to instruct your application of how to route inputs. Typically, when a value is true, it means "
                       "Dear ImGui wants the corresponding inputs and we expect the underlying application to ignore them.\n\n"
                       "The most typical case is: when hovering a window, Dear ImGui set io.WantCaptureMouse to true, "
                       "and underlying application should ignore mouse inputs (in practice there are many and more subtle "
                       "rules leading to how those flags are set)."_RU >>
                u8"Значения io.WantCaptureMouse и io.WantCaptureKeyboard обычно устанавливаются Dear ImGui "
                "чтобы проинструктировать ваше приложение о том, как маршрутизировать входные данные. Как правило, когда значение истинно, это означает "
                "Dear ImGui нужны соответствующие входные данные, и мы ожидаем, что базовое приложение их проигнорирует..\n\n"
                "Самый типичный случай: при наведении на окно Dear ImGui установил io.WantCaptureMouse в true, "
                "и базовое приложение должно игнорировать ввод мыши (на практике есть много и более тонких "
                "правил, ведущих к тому, как устанавливаются эти флаги).");

            ImGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
            ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
            ImGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);

            HelpMarker("Hovering the colored canvas will override io.WantCaptureXXX fields.\n"
                       "Notice how normally (when set to none), the value of io.WantCaptureKeyboard would be false when hovering and true when clicking."_RU >>
                u8"Наведение курсора на цветной холст переопределит Поля io.WantCaptureXXX.\n"
                "Обратите внимание, как обычно (если установлено значение none) значение io.WantCaptureKeyboard будет иметь значение false при наведении курсора мыши и значение true при нажатии.");
            static int  capture_override_mouse    = -1;
            static int  capture_override_keyboard = -1;
            const char* capture_override_desc[]   = {"None", "Set to false"_RU >> u8"Установлен в false", "Set to true"_RU >> u8"Установлен в true"};
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);
            ImGui::SliderInt("SetNextFrameWantCaptureMouse()", &capture_override_mouse, -1, +1, capture_override_desc[capture_override_mouse + 1], ImGuiSliderFlags_AlwaysClamp);
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);
            ImGui::SliderInt("SetNextFrameWantCaptureKeyboard()", &capture_override_keyboard, -1, +1, capture_override_desc[capture_override_keyboard + 1], ImGuiSliderFlags_AlwaysClamp);

            ImGui::ColorButton("##panel", ImVec4(0.7f, 0.1f, 0.7f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(256.0f, 192.0f));   // Dummy item
            if (ImGui::IsItemHovered() && capture_override_mouse != -1)
                ImGui::SetNextFrameWantCaptureMouse(capture_override_mouse == 1);
            if (ImGui::IsItemHovered() && capture_override_keyboard != -1)
                ImGui::SetNextFrameWantCaptureKeyboard(capture_override_keyboard == 1);

            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Tabbing"_RU >> u8"Ввод, Навигация и Фокус/Табуляция");
        if (ImGui::TreeNode("Tabbing"_RU >> u8"Табуляция"))
        {
            ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields."_RU >> u8"Используйте TAB/SHIFT+TAB для переключения между редактируемыми с клавиатуры полями.");
            static char buf[32] = "привет";
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
            ImGui::PushAllowKeyboardFocus(false);
            ImGui::InputText("4 (tab skip)"_RU >> u8"4 (tab пропустить)", buf, IM_ARRAYSIZE(buf));
            ImGui::SameLine();
            HelpMarker("Item won't be cycled through when using TAB or Shift+Tab."_RU >> u8"Элемент не будет циклически перемещаться при использовании TAB или Shift+Tab.");
            ImGui::PopAllowKeyboardFocus();
            ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Focus from code"_RU >> u8"Ввод, Навигация и Фокус/Фокус из Кода");
        if (ImGui::TreeNode("Focus from code"_RU >> u8"Фокус из кода"))
        {
            bool focus_1 = ImGui::Button("Focus on 1"_RU >> u8"Фокус на 1");
            ImGui::SameLine();
            bool focus_2 = ImGui::Button("Focus on 2"_RU >> u8"Фокус на 2");
            ImGui::SameLine();
            bool        focus_3   = ImGui::Button("Focus on 3"_RU >> u8"Фокус на 3");
            int         has_focus = 0;
            static char buf[128]  = "нажмите на кнопку, чтобы установить фокус";

            if (focus_1)
                ImGui::SetKeyboardFocusHere();
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive())
                has_focus = 1;

            if (focus_2)
                ImGui::SetKeyboardFocusHere();
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive())
                has_focus = 2;

            ImGui::PushAllowKeyboardFocus(false);
            if (focus_3)
                ImGui::SetKeyboardFocusHere();
            ImGui::InputText("3 (tab skip)"_RU >> u8"3 (tab пропустить)", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive())
                has_focus = 3;
            ImGui::SameLine();
            HelpMarker("Item won't be cycled through when using TAB or Shift+Tab."_RU >> u8"Элемент не будет циклически перемещаться при использовании TAB или Shift+Tab.");
            ImGui::PopAllowKeyboardFocus();

            if (has_focus)
                ImGui::Text("Item with focus: %d"_RU >> u8"Объект с фокусом: %d", has_focus);
            else
                ImGui::Text("Item with focus: <none>"_RU >> u8"Объект с фокусом: <none>");

            // Используйте параметр >= 0 для SetKeyboardFocusHere(), чтобы сфокусировать предстоящий элемент
            static float f3[3]       = {0.0f, 0.0f, 0.0f};
            int          focus_ahead = -1;
            if (ImGui::Button("Focus on X"_RU >> u8"Фокус на X"))
            {
                focus_ahead = 0;
            }
            ImGui::SameLine();
            if (ImGui::Button("Focus on Y"_RU >> u8"Фокус на Y"))
            {
                focus_ahead = 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Focus on Z"_RU >> u8"Фокус на Z"))
            {
                focus_ahead = 2;
            }
            if (focus_ahead != -1)
                ImGui::SetKeyboardFocusHere(focus_ahead);
            ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code."_RU >> u8"NB: Курсор и выбор сохраняются при фокусировании последнего использованного элемента в коде.");
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Dragging"_RU >> u8"Ввод, Навигация и Фокус/Перетаскивание");
        if (ImGui::TreeNode("Dragging"_RU >> u8"Перетаскивание"))
        {
            ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget."_RU >> u8"Вы можете использовать ImGui:: GetMouseDragDelta(0) для запроса количества перетаскивания любого виджета.");
            for (int button = 0; button < 3; button++)
            {
                ImGui::Text("IsMouseDragging(%d):", button);
                ImGui::Text("  w/ default threshold: %d,"_RU >> u8"  w/ порог по умолчанию: %d,", ImGui::IsMouseDragging(button));
                ImGui::Text("  w/ zero threshold: %d,"_RU >> u8"  w/ нулевой порог: %d,", ImGui::IsMouseDragging(button, 0.0f));
                ImGui::Text("  w/ large threshold: %d,"_RU >> u8"  w/ большой порог: %d,", ImGui::IsMouseDragging(button, 20.0f));
            }

            ImGui::Button("Drag Me"_RU >> u8"Перетащи меня");
            if (ImGui::IsItemActive())
                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);   // Нарисуйте линию между кнопкой и курсором мыши

            // Операции перетаскивания «разблокируются», когда мышь переместилась за определенный порог.
            // (порог по умолчанию хранится в io.MouseDragThreshold). Вы можете запросить более низкую или более высокую
            // порог с использованием второго параметра IsMouseDragGing() и GetMouseDragDelta().
            ImVec2 value_raw                 = ImGui::GetMouseDragDelta(0, 0.0f);
            ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
            ImVec2 mouse_delta               = io.MouseDelta;
            ImGui::Text("GetMouseDragDelta(0):");
            ImGui::Text("  w/ default threshold: (%.1f, %.1f)"_RU >> u8"  w/ порог по умолчанию: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
            ImGui::Text("  w/ zero threshold: (%.1f, %.1f)"_RU >> u8"  w/ нулевой порог: (%.1f, %.1f)", value_raw.x, value_raw.y);
            ImGui::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
            ImGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse cursors"_RU >> u8"Вводы, Навигация и Фокус/Курсоры мыши");
        if (ImGui::TreeNode("Mouse cursors"_RU >> u8"Курсоры мыши"))
        {
            const char* mouse_cursors_names[] = {"Arrow"_RU >> u8"Стрелка", "TextInput"_RU >> u8"Ввод текста", "ResizeAll"_RU >> u8"Изменить размер Всему", "ResizeNS"_RU >> u8"Изменить размер NS", "ResizeEW"_RU >> u8"Изменить размер EW", "ResizeNESW"_RU >> u8"Изменить размер NESW", "ResizeNWSE"_RU >> u8"Изменить размер NWSE", "Hand"_RU >> u8"Рука", "NotAllowed"_RU >> u8"Не допускается"};
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ImGuiMouseCursor current = ImGui::GetMouseCursor();
            ImGui::Text("Current mouse cursor = %d: %s"_RU >> u8"Текущий курсор мыши = %d: %s", current, mouse_cursors_names[current]);
            ImGui::Text("Hover to see mouse cursors:"_RU >> u8"Наведите курсор, чтобы увидеть курсоры мыши:");
            ImGui::SameLine();
            HelpMarker("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. "
                       "If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, "
                       "otherwise your backend needs to handle it."_RU >>
                u8"Ваше приложение может отображать другой курсор мыши в зависимости от того, что возвращает ImGui::GetMouseCursor(). "
                "Если установлен программный рендеринг курсора (io.MouseDrawCursor), ImGui будет рисовать правильный курсор для вас, "
                "иначе ваш бэкенд должен справиться с этим.");
            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s"_RU >> u8"Курсор мыши %d: %s", i, mouse_cursors_names[i]);
                ImGui::Bullet();
                ImGui::Selectable(label, false);
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(i);
            }
            ImGui::TreePop();
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Об окне/ShowAboutWindow()
// // Доступ из демо-версии Dear ImGui -> Инструменты -> О программе
//-----------------------------------------------------------------------------

void ImGui::ShowAboutWindow(bool* p_open)
{
    if (!ImGui::Begin("About Dear ImGui"_RU >> u8"Об Dear ImGui", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Tools/About Dear ImGui"_RU >> u8"Инструменты/Об Dear ImGui");
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Omar Cornut and all Dear ImGui contributors."_RU >> u8"Омар Корнут и все участники Dear ImGui.");
    ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information."_RU >> u8"Dear ImGui находится под лицензией MIT, см. LICENSE для получения дополнительной информации.");

    static bool show_config_info = false;
    ImGui::Checkbox("Config/Build Information"_RU >> u8"Конфигурация/Информация о сборке", &show_config_info);
    if (show_config_info)
    {
        ImGuiIO&    io                = ImGui::GetIO();
        ImGuiStyle& style             = ImGui::GetStyle();

        bool        copy_to_clipboard = ImGui::Button("Copy to clipboard"_RU >> u8"Скопировать в буфер обмена");
        ImVec2      child_size        = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18);
        ImGui::BeginChildFrame(ImGui::GetID("cfg_infos"), child_size, ImGuiWindowFlags_NoMove);
        if (copy_to_clipboard)
        {
            ImGui::LogToClipboard();
            ImGui::LogText("```\n");   // Обратные кавычки заставят текст отображаться без форматирования при вставке на GitHub.
        }

        ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ImGui::Separator();
        ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
        ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
        ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_KEYIO");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_FILE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
        ImGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
        ImGui::Text("define: _WIN32");
#endif
#ifdef _WIN64
        ImGui::Text("define: _WIN64");
#endif
#ifdef __linux__
        ImGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
        ImGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
        ImGui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef _MSVC_LANG
        ImGui::Text("define: _MSVC_LANG=%d", (int)_MSVC_LANG);
#endif
#ifdef __MINGW32__
        ImGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
        ImGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
        ImGui::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
        ImGui::Text("define: __clang_version__=%s", __clang_version__);
#endif
#ifdef IMGUI_HAS_VIEWPORT
        ImGui::Text("define: IMGUI_HAS_VIEWPORT");
#endif
#ifdef IMGUI_HAS_DOCK
        ImGui::Text("define: IMGUI_HAS_DOCK");
#endif
        ImGui::Separator();
        ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
        ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
        ImGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)
            ImGui::Text(" NavEnableKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
            ImGui::Text(" NavEnableGamepad");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)
            ImGui::Text(" NavEnableSetMousePos");
        if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)
            ImGui::Text(" NavNoCaptureKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)
            ImGui::Text(" NoMouse");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
            ImGui::Text(" NoMouseCursorChange");
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            ImGui::Text(" DockingEnable");
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            ImGui::Text(" ViewportsEnable");
        if (io.ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            ImGui::Text(" DpiEnableScaleViewports");
        if (io.ConfigFlags & ImGuiConfigFlags_DpiEnableScaleFonts)
            ImGui::Text(" DpiEnableScaleFonts");
        if (io.MouseDrawCursor)
            ImGui::Text("io.MouseDrawCursor");
        if (io.ConfigViewportsNoAutoMerge)
            ImGui::Text("io.ConfigViewportsNoAutoMerge");
        if (io.ConfigViewportsNoTaskBarIcon)
            ImGui::Text("io.ConfigViewportsNoTaskBarIcon");
        if (io.ConfigViewportsNoDecoration)
            ImGui::Text("io.ConfigViewportsNoDecoration");
        if (io.ConfigViewportsNoDefaultParent)
            ImGui::Text("io.ConfigViewportsNoDefaultParent");
        if (io.ConfigDockingNoSplit)
            ImGui::Text("io.ConfigDockingNoSplit");
        if (io.ConfigDockingWithShift)
            ImGui::Text("io.ConfigDockingWithShift");
        if (io.ConfigDockingAlwaysTabBar)
            ImGui::Text("io.ConfigDockingAlwaysTabBar");
        if (io.ConfigDockingTransparentPayload)
            ImGui::Text("io.ConfigDockingTransparentPayload");
        if (io.ConfigMacOSXBehaviors)
            ImGui::Text("io.ConfigMacOSXBehaviors");
        if (io.ConfigInputTextCursorBlink)
            ImGui::Text("io.ConfigInputTextCursorBlink");
        if (io.ConfigWindowsResizeFromEdges)
            ImGui::Text("io.ConfigWindowsResizeFromEdges");
        if (io.ConfigWindowsMoveFromTitleBarOnly)
            ImGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
        if (io.ConfigMemoryCompactTimer >= 0.0f)
            ImGui::Text("io.ConfigMemoryCompactTimer = %.1f", io.ConfigMemoryCompactTimer);
        ImGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
        if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)
            ImGui::Text(" HasGamepad");
        if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)
            ImGui::Text(" HasMouseCursors");
        if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)
            ImGui::Text(" HasSetMousePos");
        if (io.BackendFlags & ImGuiBackendFlags_PlatformHasViewports)
            ImGui::Text(" PlatformHasViewports");
        if (io.BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport)
            ImGui::Text(" HasMouseHoveredViewport");
        if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)
            ImGui::Text(" RendererHasVtxOffset");
        if (io.BackendFlags & ImGuiBackendFlags_RendererHasViewports)
            ImGui::Text(" RendererHasViewports");
        ImGui::Separator();
        ImGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
        ImGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
        ImGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui::Separator();
        ImGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
        ImGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
        ImGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
        ImGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
        ImGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
        ImGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
        ImGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

        if (copy_to_clipboard)
        {
            ImGui::LogText("\n```\n");
            ImGui::LogFinish();
        }
        ImGui::EndChildFrame();
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Style Editor / ShowStyleEditor()
//-----------------------------------------------------------------------------
// - ShowFontSelector()
// - ShowStyleSelector()
// - ShowStyleEditor()
//-----------------------------------------------------------------------------

// Вперед объявить ShowFontAtlas(), который пока не стоит размещать в публичном API
namespace ImGui
{
    IMGUI_API void ShowFontAtlas(ImFontAtlas* atlas);
}

// Вспомогательная демонстрационная функция для выбора среди загруженных шрифтов.
// Здесь мы используем обычный API BeginCombo()/EndCombo(), который является более гибким.
void ImGui::ShowFontSelector(const char* label)
{
    ImGuiIO& io           = ImGui::GetIO();
    ImFont*  font_current = ImGui::GetFont();
    if (ImGui::BeginCombo(label, font_current->GetDebugName()))
    {
        for (int n = 0; n < io.Fonts->Fonts.Size; n++)
        {
            ImFont* font = io.Fonts->Fonts[n];
            ImGui::PushID((void*)font);
            if (ImGui::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("- Load additional fonts with io.Fonts->AddFontFromFileTTF().\n"
               "- The font atlas is built when calling io.Fonts->GetTexDataAsXXXX() or io.Fonts->Build().\n"
               "- Read FAQ and docs/FONTS.md for more details.\n"
               "- If you need to add/remove fonts at runtime (e.g. for DPI change), do it before calling NewFrame()."_RU >>
        u8"- Загрузите дополнительные шрифты с помощью io.Fonts->AddFontFromFileTTF().\n"
        "- Атлас шрифтов строится при вызове io.Fonts->GetTexDataAsXXXX() или io.Fonts->Build().\n"
        "- Прочтите FAQ и документы/FONTS. мд для более подробной информации.\n"
        "- Если вам нужно добавить/удалить шрифты во время выполнения (например, для изменения DPI), сделайте это перед вызовом NewFrame().");
}

// Вспомогательная демонстрационная функция для выбора цвета по умолчанию. См. ShowStyleEditor() для более продвинутых опций.
// Здесь мы используем упрощенный API Combo(), который упаковывает элементы в одну литеральную строку.
// Полезно для быстрых полей со списком, где выбор известен локально.
bool ImGui::ShowStyleSelector(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Dark\0Light\0Classic\0"_RU >> u8"Темный\0Светлый\0Классический\0"))
    {
        switch (style_idx)
        {
            case 0:
                ImGui::StyleColorsDark();
                break;
            case 1:
                ImGui::StyleColorsLight();
                break;
            case 2:
                ImGui::StyleColorsClassic();
                break;
        }
        return true;
    }
    return false;
}

void ImGui::ShowStyleEditor(ImGuiStyle* ref)
{
    IMGUI_DEMO_MARKER("Tools/Style Editor"_RU >> u8"Инструменты/Редактор стилей");
    // Вы можете передать эталонную структуру ImGuiStyle для сравнения, возврата и сохранения.
    // (без указателя ссылочного стиля мы будем использовать сравниваемый локально в качестве ссылки)
    ImGuiStyle&       style = ImGui::GetStyle();
    static ImGuiStyle ref_saved_style;

    // По умолчанию используется внутренняя память в качестве эталона
    static bool       init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

    if (ImGui::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    ImGui::ShowFontSelector("Fonts##Selector");

    // Упрощенные настройки (размеры границ с плавающим указателем отображаются как логические, представляющие 0.0f or 1.0f)
    if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        style.GrabRounding = style.FrameRounding;   // Сделать GrabRounding всегда таким же значением, как FrameRounding
    {
        bool border = (style.WindowBorderSize > 0.0f);
        if (ImGui::Checkbox("WindowBorder", &border))
        {
            style.WindowBorderSize = border ? 1.0f : 0.0f;
        }
    }
    ImGui::SameLine();
    {
        bool border = (style.FrameBorderSize > 0.0f);
        if (ImGui::Checkbox("FrameBorder", &border))
        {
            style.FrameBorderSize = border ? 1.0f : 0.0f;
        }
    }
    ImGui::SameLine();
    {
        bool border = (style.PopupBorderSize > 0.0f);
        if (ImGui::Checkbox("PopupBorder", &border))
        {
            style.PopupBorderSize = border ? 1.0f : 0.0f;
        }
    }

    // Кнопка Сохранить/Вернуть
    if (ImGui::Button("Save Ref"_RU >> u8"Сохранить Ref"))
        *ref = ref_saved_style = style;
    ImGui::SameLine();
    if (ImGui::Button("Revert Ref"_RU >> u8"Вернуть Ref"))
        style = *ref;
    ImGui::SameLine();
    HelpMarker("Save/Revert in local non-persistent storage. Default Colors definition are not affected. "
               "Use \"Export\" below to save them somewhere."_RU >>
        u8"Сохранить/вернуть в локальном непостоянном хранилище. На определение цветов по умолчанию это не влияет. "
        "Использовать \"Экспорт\" ниже, чтобы сохранить их где-нибудь.");

    ImGui::Separator();

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Sizes"_RU >> u8"Размеры"))
        {
            ImGui::Text("Main"_RU >> u8"Основной");
            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
            ImGui::Text("Borders"_RU >> u8"Границы");
            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::Text("Rounding"_RU >> u8"Округление");
            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::Text("Alignment"_RU >> u8"Выравнивание");
            ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"_RU >> u8"None\0Слева\0Справа\0"))
                style.WindowMenuButtonPosition = window_menu_button_position - 1;
            ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0"_RU >> u8"Слева\0Справа\0");
            ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SameLine();
            HelpMarker("Alignment applies when a button is larger than its text content."_RU >> u8"Выравнивание применяется, когда кнопка больше, чем ее текстовое содержимое.");
            ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SameLine();
            HelpMarker("Alignment applies when a selectable is larger than its text content."_RU >> u8"Выравнивание применяется, когда выбираемый элемент больше, чем его текстовое содержимое.");
            ImGui::Text("Safe Area Padding"_RU >> u8"Отступы безопасной зоны");
            ImGui::SameLine();
            HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured)."_RU >> u8"Отрегулируйте, если вы не видите края экрана (e. g. на TV, где масштабирование не настроено).");
            ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Colors"_RU >> u8"Цвета"))
        {
            static int  output_dest          = 0;
            static bool output_only_modified = true;
            if (ImGui::Button("Export"_RU >> u8"Экспорт"))
            {
                if (output_dest == 0)
                    ImGui::LogToClipboard();
                else
                    ImGui::LogToTTY();
                ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const ImVec4& col  = style.Colors[i];
                    const char*   name = ImGui::GetStyleColorName(i);
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                        ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                }
                ImGui::LogFinish();
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"_RU >> u8"В буфер обмена\0В TTY\0");
            ImGui::SameLine();
            ImGui::Checkbox("Only Modified Colors"_RU >> u8"Только измененные цвета", &output_only_modified);

            static ImGuiTextFilter filter;
            filter.Draw("Filter colors"_RU >> u8"Фильтровать цвета", ImGui::GetFontSize() * 16);

            static ImGuiColorEditFlags alpha_flags = 0;
            if (ImGui::RadioButton("Opaque"_RU >> u8"Непрозрачный", alpha_flags == ImGuiColorEditFlags_None))
            {
                alpha_flags = ImGuiColorEditFlags_None;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Alpha"_RU >> u8"Альфа", alpha_flags == ImGuiColorEditFlags_AlphaPreview))
            {
                alpha_flags = ImGuiColorEditFlags_AlphaPreview;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Both"_RU >> u8"Оба", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf))
            {
                alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf;
            }
            ImGui::SameLine();
            HelpMarker("In the color list:\n"
                       "Left-click on color square to open color picker,\n"
                       "Right-click to open edit options menu."_RU >>
                u8"В списке цветов:\n"
                "Щелкните левой кнопкой мыши на цветном квадрате, чтобы открыть палитру цветов,\n"
                "Щелкните правой кнопкой мыши, чтобы открыть меню параметров редактирования.");

            ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
            ImGui::PushItemWidth(-160);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;
                ImGui::PushID(i);
                ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                {
                    // Советы: в реальном пользовательском приложении вы можете захотеть объединить и использовать иконочный шрифт с основным шрифтом,
                    // так что вместо "Сохранить"/"Вернуть" вы бы использовали значки!
                    // Прочтите FAQ и документацию/ШРИФТЫ. md об использовании иконочных шрифтов. Это действительно просто и супер удобно!
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    if (ImGui::Button("Save"_RU >> u8"Сохранить"))
                    {
                        ref->Colors[i] = style.Colors[i];
                    }
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    if (ImGui::Button("Revert"_RU >> u8"Вернуть"))
                    {
                        style.Colors[i] = ref->Colors[i];
                    }
                }
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                ImGui::TextUnformatted(name);
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Fonts"_RU >> u8"Шрифты"))
        {
            ImGuiIO&     io    = ImGui::GetIO();
            ImFontAtlas* atlas = io.Fonts;
            HelpMarker("Read FAQ and docs/FONTS.md for details on font loading."_RU >> u8"Прочтите FAQ и документы/FONTS.md для получения подробной информации о загрузке шрифтов.");
            ImGui::ShowFontAtlas(atlas);

            // Масштабирование шрифта после запекания. Обратите внимание, что это НЕ лучший способ масштабирования шрифтов, читайте ниже.
            // (мы применяем жесткое ограничение вручную, так как по умолчанию DragFloat/SliderFloat позволяет CTRL+Click тексту выходить за границы).
            const float MIN_SCALE = 0.3f;
            const float MAX_SCALE = 2.0f;
            HelpMarker("Those are old settings provided for convenience.\n"
                       "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                       "rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
                       "Using those settings here will give you poor quality results."_RU >>
                u8"Это старые настройки, предоставленные для удобства.\n"
                "Однако _правильный_ способ масштабирования вашего пользовательского интерфейса в настоящее время заключается в перезагрузке шрифта с расчетным размером, "
                "перестройте атлас шрифтов и вызовите style.ScaleAllSizes() в ссылочной структуре ImGuiStyle.\n"
                "Использование этих настроек здесь даст вам результаты низкого качества.");
            static float window_scale = 1.0f;
            ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
            if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp))   // Масштабировать только это окно
                ImGui::SetWindowFontScale(window_scale);
            ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp);   // Масштабировать все
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Rendering"_RU >> u8"Рендеринг"))
        {
            ImGui::Checkbox("Anti-aliased lines"_RU >> u8"Сглаженные линии", &style.AntiAliasedLines);
            ImGui::SameLine();
            HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well."_RU >> u8"При отключении сглаживания линий вы, вероятно, также захотите отключить границы в своем стиле.");

            ImGui::Checkbox("Anti-aliased lines use texture"_RU >> u8"Сглаженные линии используют текстуру", &style.AntiAliasedLinesUseTex);
            ImGui::SameLine();
            HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering)."_RU >> u8"Более быстрые линии с использованием текстурных данных. Требовать от серверной части рендеринга с билинейной фильтрацией (не с точечной/ближайшей фильтрацией).");

            ImGui::Checkbox("Anti-aliased fill"_RU >> u8"Сглаженная заливка", &style.AntiAliasedFill);
            ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
            ImGui::DragFloat("Curve Tessellation Tolerance"_RU >> u8"Допуск тесселяции кривой", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
            if (style.CurveTessellationTol < 0.10f)
                style.CurveTessellationTol = 0.10f;

            // При редактировании значения «Максимальная ошибка кругового сегмента» нарисуйте предварительный просмотр его эффекта на кругах, созданных автоматически.
            ImGui::DragFloat("Circle Tessellation Max Error"_RU >> u8"Максимальная ошибка тесселяции круга", &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            if (ImGui::IsItemActive())
            {
                ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("(R = radius, N = number of segments)"_RU >> u8"(R = радиус, N = количество сегментов)");
                ImGui::Spacing();
                ImDrawList* draw_list        = ImGui::GetWindowDrawList();
                const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
                for (int n = 0; n < 8; n++)
                {
                    const float RAD_MIN = 5.0f;
                    const float RAD_MAX = 70.0f;
                    const float rad     = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

                    ImGui::BeginGroup();

                    ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

                    const float  canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
                    const float  offset_x     = floorf(canvas_width * 0.5f);
                    const float  offset_y     = floorf(RAD_MAX);

                    const ImVec2 p1           = ImGui::GetCursorScreenPos();
                    draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
                    ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

                    /*
                    const ImVec2 p2 = ImGui::GetCursorScreenPos();
                    draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
                    ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
                    */

                    ImGui::EndGroup();
                    ImGui::SameLine();
                }
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            HelpMarker("When drawing circle primitives with \"num_segments == 0\" tesselation will be calculated automatically."_RU >> u8"При рисовании круговых примитивов с \"num_segments == 0\" тесселяция будет рассчитываться автоматически.");

            ImGui::DragFloat("Global Alpha"_RU >> u8"Глобальная Альфа", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");   // Здесь не отображается ноль, чтобы пользователь не «терял» пользовательский интерфейс (нулевой альфа-канал обрезает все виджеты). Но код приложения может иметь переключатель для переключения между нулем и ненулевым значением.
            ImGui::DragFloat("Disabled Alpha"_RU >> u8"Отключенный альфа-канал", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f, "%.2f");
            ImGui::SameLine();
            HelpMarker("Additional alpha multiplier for disabled items (multiply over current value of Alpha)."_RU >> u8"Дополнительный альфа-множитель для отключенных элементов (умножается на текущее значение альфа).");
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::PopItemWidth();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: панель главного меню/ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание «главной» полноэкранной строки меню и ее заполнение.
// Обратите внимание на разницу между BeginMainMenuBar() и BeginMenuBar():
// - BeginMenuBar() = полоса меню внутри текущего окна (требуется флаг ImGuiWindowFlags_MenuBar!)
// - BeginMainMenuBar() = помощник для создания окна размером с панель меню в верхней части основного окна просмотра + вызов в нем BeginMenuBar().
static void ShowExampleAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"_RU >> u8"Правка"))
        {
            if (ImGui::MenuItem("Undo"_RU >> u8"Отменить", "CTRL+Z"))
            {}
            if (ImGui::MenuItem("Redo"_RU >> u8"Повторить", "CTRL+Y", false, false))
            {}   // Отключенный элемент
            ImGui::Separator();
            if (ImGui::MenuItem("Cut"_RU >> u8"Вырезать", "CTRL+X"))
            {}
            if (ImGui::MenuItem("Copy"_RU >> u8"Копировать", "CTRL+C"))
            {}
            if (ImGui::MenuItem("Paste"_RU >> u8"Вставить", "CTRL+V"))
            {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

// Обратите внимание, что в настоящее время ярлыки предназначены только для отображения
// (будущая версия добавит явные флаги в BeginMenu() для запроса ярлыков обработки)
static void ShowExampleMenuFile()
{
    IMGUI_DEMO_MARKER("Examples/Menu"_RU >> u8"Примеры/Меню");
    ImGui::MenuItem("(demo menu)"_RU >> u8"(демо меню)", NULL, false, false);
    if (ImGui::MenuItem("New"_RU >> u8"Новый"))
    {}
    if (ImGui::MenuItem("Open"_RU >> u8"Открыть", "Ctrl+O"))
    {}
    if (ImGui::BeginMenu("Open Recent"_RU >> u8"Открыть недавние"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."_RU >> u8"Подробнее.."))
        {
            ImGui::MenuItem("Hello"_RU >> u8"Привет");
            ImGui::MenuItem("Sailor"_RU >> u8"Моряк");
            if (ImGui::BeginMenu("Recurse.."_RU >> u8"Ресурс.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save"_RU >> u8"Сохранить", "Ctrl+S"))
    {}
    if (ImGui::MenuItem("Save As.."_RU >> u8"Сохранить как.."))
    {}

    ImGui::Separator();
    IMGUI_DEMO_MARKER("Examples/Menu/Options"_RU >> u8"Примеры/Меню/Опции");
    if (ImGui::BeginMenu("Options"_RU >> u8"Опции"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled"_RU >> u8"Включено", "", &enabled);
        ImGui::BeginChild("child"_RU >> u8"дочерний", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d"_RU >> u8"Прокрутка текста %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int   n = 0;
        ImGui::SliderFloat("Value"_RU >> u8"Значение", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input"_RU >> u8"Ввод", &f, 0.1f);
        ImGui::Combo("Combo"_RU >> u8"Комбо", &n, "Yes\0No\0Maybe\0\0"_RU >> u8"Да\0Нет\0Может быть\0\0");
        ImGui::EndMenu();
    }

    IMGUI_DEMO_MARKER("Examples/Menu/Colors"_RU >> u8"Примеры/Меню/Цвета");
    if (ImGui::BeginMenu("Colors"_RU >> u8"Цвета"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2      p    = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }

    // Здесь мы снова демонстрируем добавление в меню «Параметры» (которое мы уже создали выше)
    // Конечно, в этой демонстрации несколько глупо, что эта функция дважды вызывает BeginMenu("Options").
    // В реальной кодовой базе было бы разумно использовать эту функцию из самых разных мест кода.
    if (ImGui::BeginMenu("Options"_RU >> u8"Опции"))   // <-- Добавить!
    {
        IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu"_RU >> u8"Примеры/Меню/Добавить к существующему меню");
        static bool b = true;
        ImGui::Checkbox("SomeOption"_RU >> u8"Какой-то вариант", &b);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Disabled"_RU >> u8"Выключен", false))   // Отключенный
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked"_RU >> u8"Проверен", NULL, true))
    {}
    if (ImGui::MenuItem("Quit"_RU >> u8"Выйти", "Alt+F4"))
    {}
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: консоль отладки/ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание простого окна консоли с прокруткой, фильтрацией, завершением и историей.
// В примере с консолью мы используем более похожий на C++ подход к объявлению класса для хранения как данных, так и функций.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;   // -1: новая строка, 0.. История. История посещений Size-1.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    ExampleAppConsole()
    {
        IMGUI_DEMO_MARKER("Examples/Console"_RU >> u8"Примеры/Консоль");
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        // "КЛАССИФИКАЦИЯ" здесь, чтобы предоставить тестовый пример, где «C» + [tab] завершается до «CL» и отображает несколько совпадений.
        Commands.push_back("HELP"_RU >> u8"ПОМОЩЬ");
        Commands.push_back("HISTORY"_RU >> u8"ИСТОРИЯ");
        Commands.push_back("CLEAR"_RU >> u8"ОЧИСТИТЬ");
        Commands.push_back("CLASSIFY"_RU >> u8"КЛАССИФИКАЦИЯ");
        AutoScroll     = true;
        ScrollToBottom = false;
        AddLog("Welcome to Dear ImGui!"_RU >> u8"Добро пожаловать в Dear ImGui!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Портативные помощники
    static int Stricmp(const char* s1, const char* s2)
    {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
        }
        return d;
    }
    static int Strnicmp(const char* s1, const char* s2, int n)
    {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
            n--;
        }
        return d;
    }
    static char* Strdup(const char* s)
    {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void*  buf = malloc(len);
        IM_ASSERT(buf);
        return (char*)memcpy(buf, (const void*)s, len);
    }
    static void Strtrim(char* s)
    {
        char* str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ')
            str_end--;
        *str_end = 0;
    }

    void ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char    buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // В качестве специальной функции, гарантированной библиотекой, после вызова Begin() последний элемент представляет собой строку заголовка.
        // Итак, т. г. IsItemHovered() вернет true при наведении курсора на строку заголовка.
        // Здесь мы создаем контекстное меню, доступное только из строки заголовка.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"_RU >> u8"Закрыть консоль"))
                *p_open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped("This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
                           "implementation may want to store entries along with extra data such as timestamp, emitter, etc."_RU >>
            u8"В этом примере реализована консоль с базовой раскраской, завершением (клавиша TAB) и историей (клавиши вверх/вниз). Более продуманный"
            "реализация может захотеть сохранить записи вместе с дополнительными данными, такими как метка времени, эмиттер и т. д.");
        ImGui::TextWrapped("Enter 'HELP' for help."_RU >> u8"Введите 'HELP' для помощи.");

        // TODO: отображать элементы, начиная снизу

        if (ImGui::SmallButton("Add Debug Text"_RU >> u8"Добавить отладочный текст"))
        {
            AddLog("%d some text"_RU >> u8"%d какой-то текст", Items.Size);
            AddLog("some more text"_RU >> u8"еще немного текста");
            AddLog("display very important message here!"_RU >> u8"показать очень важное сообщение здесь!");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Add Debug Error"_RU >> u8"Добавить ошибку отладки"))
        {
            AddLog("[error] something went wrong"_RU >> u8"[error] что-то пошло не так");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear"_RU >> u8"Очистить"))
        {
            ClearLog();
        }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy"_RU >> u8"Копировать");
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Меню опций
        if (ImGui::BeginPopup("Options"_RU >> u8"Опции"))
        {
            ImGui::Checkbox("Auto-scroll"_RU >> u8"Автоматическая прокрутка", &AutoScroll);
            ImGui::EndPopup();
        }

        // Опции, Фильтр
        if (ImGui::Button("Options"_RU >> u8"Опции"))
            ImGui::OpenPopup("Options"_RU >> u8"Опции");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")"_RU >> u8"Фильтр (\"вкл,-выкл\") (\"ошибка\")", 180);
        ImGui::Separator();

        // Зарезервируйте достаточную оставшуюся высоту для 1 разделителя + 1 вводимого текста.
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion"_RU >> u8"Прокручивающийся регион", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("Clear"_RU >> u8"Очистить"))
                ClearLog();
            ImGui::EndPopup();
        }

        // Отобразите каждую строку как отдельную запись, чтобы мы могли изменить их цвет или добавить пользовательские виджеты.
        // Если вам нужен только необработанный текст, вы можете использовать ImGui:: TextUnformatted(log.begin(), log.end());
        // NB: если у вас тысячи записей, этот подход может быть слишком неэффективным и может потребовать отсечения на стороне пользователя
        // для обработки только видимых элементов. Машинка для стрижки автоматически измерит высоту вашего первого предмета, а затем
        // "искать" для отображения только элементов в видимой области.
        // Чтобы использовать клипер, мы можем заменить ваш стандартный цикл:
        //      for (int i = 0; i < Items.Size; i++)
        //   With:
        //      ImGuiListClipper clipper;
        //      clipper.Begin(Items.Size);
        //      while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // - Что ваши предметы расположены равномерно (одинаковая высота)
        // - Что у вас есть дешевый произвольный доступ к вашим элементам (вы можете получить к ним доступ по их индексу,
        // без обработки всех предыдущих)
        // Вы не можете использовать этот код как есть, если активен фильтр, потому что он нарушает свойство «дешевого произвольного доступа».
        // Нам понадобится произвольный доступ к постфильтрованному списку.
        // Типичное приложение, которому требуется грубое отсечение и фильтрация, может захотеть предварительно вычислить массив индексов
        // или смещения элементов, прошедших тест фильтрации, пересчитывая этот массив, когда пользователь меняет фильтр,
        // и добавление новых элементов по мере их вставки. Это остается задачей пользователя, пока мы не сможем управлять
        // чтобы улучшить код этого примера!
        // Если ваши элементы имеют переменную высоту:
        // - Разделить их на элементы одинаковой высоты было бы проще и облегчило бы случайный поиск в вашем списке.
        // - Рассмотрите возможность ручного вызова IsRectVisible() и исключения посторонних украшений из ваших элементов.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));   // Увеличить расстояние
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!Filter.PassFilter(item))
                continue;

            // Обычно в элементе хранится больше информации, чем просто строка.
            // (например, сделать Items[] массивом структуры, сохранить цвет/тип и т. д.)
            ImVec4 color;
            bool   has_color = false;
            if (strstr(item, "[error]"))
            {
                color     = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                has_color = true;
            }
            else if (strncmp(item, "# ", 2) == 0)
            {
                color     = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                has_color = true;
            }
            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            if (has_color)
                ImGui::PopStyleColor();
        }
        if (copy_to_clipboard)
            ImGui::LogFinish();

        if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Командная строка
        bool                reclaim_focus    = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Автофокус на появлении окна
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);   // Предыдущий виджет с автоматическим фокусом

        ImGui::End();
    }

    void ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Вставить в историю. Сначала найдите совпадение и удалите его, чтобы его можно было отодвинуть на задний план.
        // Это не попытка быть умным или оптимальным.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Команда процесса
        if (Stricmp(command_line, "CLEAR"_RU >> u8"ОЧИСТИТЬ") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP"_RU >> u8"ПОМОЩЬ") == 0)
        {
            AddLog("Commands:"_RU >> u8"Команды:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY"_RU >> u8"ИСТОРИЯ") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n"_RU >> u8"Неизвестная команда: '%s'\n", command_line);
        }

        // При вводе команды мы прокручиваем вниз, даже если AutoScroll==false
        ScrollToBottom = true;
    }

    // В С++ 11 вам лучше использовать лямбда-выражения для такого рода обратных вызовов переадресации.
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        // AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
            case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Пример ЗАВЕРШЕНИЯ ТЕКСТА

                // Найти начало текущего слова
                const char* word_end   = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Создайте список кандидатов
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // Не совпадает
                    AddLog("No match for \"%.*s\"!\n"_RU >> u8"Не подходит для \"%.*s\"!\n", (int)(word_end - word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Одиночный матч. Удалите начало слова и полностью замените его, чтобы у нас был красивый регистр.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Множественные совпадения. Завершаем как можем..
                    // Таким образом, ввод "C"+Tab приведет к "CL", а затем отобразит "CLEAR" и "CLASSIFY" как совпадения.
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int  c                      = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // Список совпадений
                    AddLog("Possible matches:\n"_RU >> u8"Возможные совпадения:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog("- %s\n", candidates[i]);
                }

                break;
            }
            case ImGuiInputTextFlags_CallbackHistory:
            {
                // Пример ИСТОРИИ
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // Лучшая реализация сохранила бы данные в текущей строке ввода вместе с позицией курсора.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw("Example: Console"_RU >> u8"Пример: Консоль", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: журнал отладки/ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Применение:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog
{
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int>   LineOffsets;   // Указатель смещения строк. Мы поддерживаем это с помощью вызовов AddLog().
    bool            AutoScroll;    // Продолжайте прокручивать, если уже внизу.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int     old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Меню опций
        if (ImGui::BeginPopup("Options"_RU >> u8"Опции"))
        {
            ImGui::Checkbox("Auto-scroll"_RU >> u8"Автоматическая прокрутка", &AutoScroll);
            ImGui::EndPopup();
        }

        // Главное окно
        if (ImGui::Button("Options"_RU >> u8"Опции"))
            ImGui::OpenPopup("Options"_RU >> u8"Опции");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear"_RU >> u8"Очистить");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy"_RU >> u8"Копировать");
        ImGui::SameLine();
        Filter.Draw("Filter"_RU >> u8"Фильтр", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling"_RU >> u8"прокрутка", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf     = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // В этом примере мы не используем ограничитель, когда фильтр включен.
            // Это потому, что у нас нет произвольного доступа к результату нашего фильтра.
            // Реальное приложение, обрабатывающее журналы с десятками тысяч записей, может захотеть сохранить результат
            // поиск/фильтр.. особенно если функция фильтрации не тривиальна (например, reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end   = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // Самый простой и легкий способ отобразить весь буфер:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // И это просто сработает. TextUnformatted() специализируется на больших текстовых блоках и выполняет ускоренную перемотку вперед.
            // чтобы пропустить невидимые строки. Здесь вместо этого мы демонстрируем использование клипера для обработки только тех строк, которые
            // в видимой области.
            // Если у вас есть десятки тысяч элементов и стоимость их обработки незначительна, грубая обрезка
            // на вашей стороне рекомендуется. Использование ImGuiListClipper требует
            // - A) произвольный доступ к вашим данным
            // - B) все предметы одинаковой высоты,
            // оба из которых мы можем обработать, поскольку мы массив, указывающий на начало каждой строки текста.
            // При использовании фильтра (в блоке кода выше) у нас нет произвольного доступа к данным для отображения
            // больше, поэтому мы не используем клипер. Сохранение или беглый просмотр результатов поиска
            // это возможно (и рекомендуется, если вы хотите выполнить поиск среди десятков тысяч записей).
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end   = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};

// Продемонстрируйте создание простого окна журнала с базовой фильтрацией.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // Для демонстрации: добавьте кнопку отладки _ДО_ обычного содержимого окна журнала
    // Мы используем редко используемую функцию: несколько вызовов Begin()/End() добавляются к _одному_ окну.
    // Большая часть содержимого окна будет добавлена ​​журналом. Вызов Рисовать().
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Example: Log"_RU >> u8"Пример: Лог", p_open);
    IMGUI_DEMO_MARKER("Examples/Log"_RU >> u8"Примеры/Лог");
    if (ImGui::SmallButton("[Debug] Add 5 entries"_RU >> u8"[Отладка] Добавить 5 записей"))
    {
        static int  counter       = 0;
        const char* categories[3] = {"info"_RU >> u8"информация", "warn"_RU >> u8"предупреждение", "error"_RU >> u8"ошибка"};
        const char* words[]       = {"Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent"};
        for (int n = 0; n < 5; n++)
        {
            const char* category = categories[counter % IM_ARRAYSIZE(categories)];
            const char* word     = words[counter % IM_ARRAYSIZE(words)];
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n"_RU >> u8"[%05d] [%s] Здравствуйте, текущее время %.1f, вот несколько слов: '%s'\n", ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
            counter++;
        }
    }
    ImGui::End();

    // На самом деле вызовите обычный помощник Log (который вызовет Begin() в том же окне, что и мы только что сделали)
    log.Draw("Example: Log"_RU >> u8"Пример: Лог", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: простой макет/ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание окна с несколькими дочерними окнами.
static void ShowExampleAppLayout(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Example: Simple layout"_RU >> u8"Пример: Простая компоновка", p_open, ImGuiWindowFlags_MenuBar))
    {
        IMGUI_DEMO_MARKER("Examples/Simple layout"_RU >> u8"Примеры/Простая компоновка");
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
            {
                if (ImGui::MenuItem("Close"_RU >> u8"Закрыть"))
                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Левый
        static int selected = 0;
        {
            ImGui::BeginChild("left pane"_RU >> u8"левая панель", ImVec2(150, 0), true);
            for (int i = 0; i < 100; i++)
            {
                // FIXME: Хороший кандидат на использование ImGuiSelectableFlags_SelectOnNav
                char label[128];
                sprintf(label, "MyObject %d"_RU >> u8"Мой объект %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Правый
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view"_RU >> u8"просмотр элемента", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));   // Оставьте место на 1 строку ниже нас
            ImGui::Text("MyObject: %d"_RU >> u8"Мой объект: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"_RU >> u8"Описание"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "_RU >> u8"Знание о том, что нужно сидеть сложа руки, заботиться о том, чтобы избавиться от лишних килограммов, откладывать на потом временные затраты на труд и долги великой аликвы. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"_RU >> u8"Подробности"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            if (ImGui::Button("Revert"_RU >> u8"Реверс"))
            {}
            ImGui::SameLine();
            if (ImGui::Button("Save"_RU >> u8"Сохранить"))
            {}
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: Редактор свойств / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------

static void ShowPlaceholderObject(const char* prefix, int uid)
{
    // Использовать uid объекта в качестве идентификатора. Чаще всего вы также можете использовать указатель объекта в качестве базового идентификатора.
    ImGui::PushID(uid);

    // Узлы текста и дерева имеют меньшую высоту, чем виджеты с фреймами, с помощью AlignTextToFramePadding() мы добавляем интервалы по вертикали, чтобы сделать линии дерева одинаковой высоты.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Object", "%s_%u"_RU >> u8"Объект", "%s_%u", prefix, uid);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("my sailor is rich"_RU >> u8"Мой моряк богат");

    if (node_open)
    {
        static float placeholder_members[8] = {0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f};
        for (int i = 0; i < 8; i++)
        {
            ImGui::PushID(i);   // Используйте индекс поля в качестве идентификатора.
            if (i < 2)
            {
                ShowPlaceholderObject("Child"_RU >> u8"Ребенок", 424242);
            }
            else
            {
                // Здесь мы используем TreeNode для выделения при наведении (мы также можем использовать, например, Selectable)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Field"_RU >> u8"Поле", flags, "Field_%d"_RU >> u8"Поле_%d", i);

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                if (i >= 5)
                    ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
                else
                    ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
                ImGui::NextColumn();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

// Продемонстрируйте создание простого редактора свойств.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Property editor"_RU >> u8"Пример: Редактор свойств", p_open))
    {
        ImGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Property Editor"_RU >> u8"Примеры/Редактор свойств");

    HelpMarker("This example shows how you may implement a property editor using two columns.\n"
               "All objects/fields data are dummies here.\n"
               "Remember that in many simple cases, you can use ImGui::SameLine(xxx) to position\n"
               "your cursor horizontally instead of using the Columns() API."_RU >>
        u8"В этом примере показано, как можно реализовать редактор свойств с использованием двух столбцов.\n"
        "Все данные объектов/полей здесь фиктивные.\n"
        "Помните, что во многих простых случаях вы можете использовать ImGui:: SameLine(xxx) для позиционирования\n"
        "ваш курсор горизонтально вместо использования Columns() API.");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("split"_RU >> u8"расколоть", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        // Итерация объектов-заполнителей (все те же данные)
        for (int obj_i = 0; obj_i < 4; obj_i++)
        {
            ShowPlaceholderObject("Object"_RU >> u8"Объект", obj_i);
            //ImGui::Separator();
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: длинный текст/ShowExampleAppLongText()
//-----------------------------------------------------------------------------

// Продемонстрируйте/протестируйте рендеринг огромного количества текста и возможность его отсечения.
static void ShowExampleAppLongText(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Long text display"_RU >> u8"Пример: Отображение длинного текста", p_open))
    {
        ImGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Long text display"_RU >> u8"Примеры/Отображение длинного текста");

    static int             test_type = 0;
    static ImGuiTextBuffer log;
    static int             lines = 0;
    ImGui::Text("Printing unusually long amount of text."_RU >> u8"Печать необычно большого количества текста.");
    ImGui::Combo("Test type"_RU >> u8"Тип теста", &test_type,
        "Single call to TextUnformatted()\0"
        "Multiple calls to Text(), clipped\0"
        "Multiple calls to Text(), not clipped (slow)\0"_RU >>
            u8"Одиночный вызов TextUnformatted()\0"
            "Множественные вызовы Text(), обрезанные\0"
            "Множественные вызовы Text(), не обрезанные (медленные)\0");
    ImGui::Text("Buffer contents: %d lines, %d bytes"_RU >> u8"Содержимое буфера: %d линий, %d байтов", lines, log.size());
    if (ImGui::Button("Clear"_RU >> u8"Очистить"))
    {
        log.clear();
        lines = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add 1000 lines"_RU >> u8"Добавьте 1000 строк"))
    {
        for (int i = 0; i < 1000; i++)
            log.appendf("%i The quick brown fox jumps over the lazy dog\n"_RU >> u8"%i Быстрая коричневая лиса прыгает через ленивую собаку\n", lines + i);
        lines += 1000;
    }
    ImGui::BeginChild("Log"_RU >> u8"Лог");
    switch (test_type)
    {
        case 0:
            // Один вызов TextUnformatted() с большим буфером
            ImGui::TextUnformatted(log.begin(), log.end());
            break;
        case 1:
        {
            // Множественные вызовы Text(), грубо обрезанные вручную — продемонстрируйте, как использовать помощник ImGuiListClipper.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiListClipper clipper;
            clipper.Begin(lines);
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    ImGui::Text("%i The quick brown fox jumps over the lazy dog"_RU >> u8"%i Быстрая коричневая лиса прыгает через ленивую собаку", i);
            ImGui::PopStyleVar();
            break;
        }
        case 2:
            // Множественные вызовы Text(), не обрезанные (медленные)
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            for (int i = 0; i < lines; i++)
                ImGui::Text("%i The quick brown fox jumps over the lazy dog"_RU >> u8"%i Быстрая коричневая лиса прыгает через ленивую собаку", i);
            ImGui::PopStyleVar();
            break;
    }
    ImGui::EndChild();
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: автоматическое изменение размера/ShowExampleAppAutoResize()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание окна, размер которого автоматически изменяется в соответствии с его содержимым.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!ImGui::Begin("Example: Auto-resizing window"_RU >> u8"Пример: Автоматическое изменение размера окна", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Auto-resizing window"_RU >> u8"Примеры/Автоматическое изменение размера окна");

    static int lines = 10;
    ImGui::TextUnformatted("Window will resize every-frame to the size of its content.\n"
                           "Note that you probably don't want to query the window size to\n"
                           "output your content because that would create a feedback loop."_RU >>
        u8"Окно будет изменять размер каждого кадра в соответствии с размером его содержимого.\n"
        "Обратите внимание, что вы, вероятно, не хотите запрашивать размер окна для\n"
        "выведите свой контент, потому что это создаст петлю обратной связи.");
    ImGui::SliderInt("Number of lines"_RU >> u8"Количество строк", &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        ImGui::Text("%*sThis is line %d"_RU >> u8"%*sэто линия %d", i * 4, "", i);   // Pad с пространством для увеличения размера по горизонтали
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: ограниченное изменение размера/ShowExampleAppConstrainedResize()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание окна с пользовательскими ограничениями изменения размера.
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints
    {
        // Вспомогательные функции для демонстрации программных ограничений
        static void Square(ImGuiSizeCallbackData* data)
        {
            data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->DesiredSize.x, data->DesiredSize.y);
        }
        static void Step(ImGuiSizeCallbackData* data)
        {
            float step        = (float)(int)(intptr_t)data->UserData;
            data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
        }
    };

    const char* test_desc[] = {
        "Resize vertical only"_RU >> u8"Изменить размер только по вертикали",
        "Resize horizontal only"_RU >> u8"Изменить размер только по горизонтали",
        "Width > 100, Height > 100"_RU >> u8"Ширина > 100, Высота > 100",
        "Width 400-500"_RU >> u8"Ширина 400-500",
        "Height 400-500"_RU >> u8"Высота 400-500",
        "Custom: Always Square"_RU >> u8"Пользовательский: Всегда квадратный",
        "Custom: Fixed Steps (100)"_RU >> u8"Пользовательский: Фиксированный Шаг (100)",
    };

    static bool auto_resize   = false;
    static int  type          = 0;
    static int  display_lines = 10;
    if (type == 0)
        ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0), ImVec2(-1, FLT_MAX));   // Только по вертикали
    if (type == 1)
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));   // Только по горизонтали
    if (type == 2)
        ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));   // Ширина > 100, Высота > 100
    if (type == 3)
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(500, -1));   // Ширина 400-500
    if (type == 4)
        ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 400), ImVec2(-1, 500));   // Высота 400-500
    if (type == 5)
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);   // Всегда квадрат
    if (type == 6)
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)(intptr_t)100);   // Фиксированный шаг

    ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
    if (ImGui::Begin("Example: Constrained Resize"_RU >> u8"Пример: Ограниченное изменение размера", p_open, flags))
    {
        IMGUI_DEMO_MARKER("Examples/Constrained Resizing window"_RU >> u8"Примеры/Ограниченное изменение размера окна");
        if (ImGui::IsWindowDocked())
            ImGui::Text("Warning: Sizing Constraints won't work if the window is docked!"_RU >> u8"Предупреждение: Ограничения размера не будут работать, если окно закреплено!");
        if (ImGui::Button("200x200"))
        {
            ImGui::SetWindowSize(ImVec2(200, 200));
        }
        ImGui::SameLine();
        if (ImGui::Button("500x500"))
        {
            ImGui::SetWindowSize(ImVec2(500, 500));
        }
        ImGui::SameLine();
        if (ImGui::Button("800x200"))
        {
            ImGui::SetWindowSize(ImVec2(800, 200));
        }
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("Constraint"_RU >> u8"Ограничение", &type, test_desc, IM_ARRAYSIZE(test_desc));
        ImGui::SetNextItemWidth(200);
        ImGui::DragInt("Lines"_RU >> u8"Строки", &display_lines, 0.2f, 1, 100);
        ImGui::Checkbox("Auto-resize"_RU >> u8"Автоматическое изменение размера", &auto_resize);
        for (int i = 0; i < display_lines; i++)
            ImGui::Text("%*sHello, sailor! Making this line long enough for the example."_RU >> u8"%*sЗдравствуй, матрос! Делаем эту строку достаточно длинной для примера.", i * 4, "");
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: простое наложение/ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание простого статического окна без оформления.
// + контекстное меню для выбора используемого угла экрана.
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    static int       corner       = 0;
    ImGuiIO&         io           = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const float          PAD       = 10.0f;
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        ImVec2               work_pos  = viewport->WorkPos;   // Используйте рабочую область, чтобы избежать панели меню/панели задач, если таковые имеются!
        ImVec2               work_size = viewport->WorkSize;
        ImVec2               window_pos, window_pos_pivot;
        window_pos.x       = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y       = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f);   // Прозрачный фон
    if (ImGui::Begin("Example: Simple overlay"_RU >> u8"Пример: Простое наложение", p_open, window_flags))
    {
        IMGUI_DEMO_MARKER("Examples/Simple Overlay"_RU >> u8"Примеры/Простое наложение");
        ImGui::Text("Simple overlay\n"
                    "in the corner of the screen.\n"
                    "(right-click to change position)"_RU >>
            u8"Простое наложение\n"
            "в углу экрана.\n"
            "(щелкните правой кнопкой мыши, чтобы изменить положение)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)"_RU >> u8"Положение мыши: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>"_RU >> u8"Положение мыши: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom"_RU >> u8"Пользовательский", NULL, corner == -1))
                corner = -1;
            if (ImGui::MenuItem("Top-left"_RU >> u8"Верхний левый", NULL, corner == 0))
                corner = 0;
            if (ImGui::MenuItem("Top-right"_RU >> u8"В правом верхнем углу", NULL, corner == 1))
                corner = 1;
            if (ImGui::MenuItem("Bottom-left"_RU >> u8"Внизу слева", NULL, corner == 2))
                corner = 2;
            if (ImGui::MenuItem("Bottom-right"_RU >> u8"Внизу справа", NULL, corner == 3))
                corner = 3;
            if (p_open && ImGui::MenuItem("Close"_RU >> u8"Закрыть"))
                *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: Полноэкранное окно/ShowExampleAppFullscreen()
//-----------------------------------------------------------------------------

// Продемонстрируйте создание окна, охватывающего весь экран/просмотр
static void ShowExampleAppFullscreen(bool* p_open)
{
    static bool             use_work_area = true;
    static ImGuiWindowFlags flags         = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    // Мы демонстрируем использование полной области просмотра или рабочей области (без панелей меню, панелей задач и т. д.)
    // В зависимости от вашего варианта использования вы можете захотеть одно из другого.
    const ImGuiViewport*    viewport      = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (ImGui::Begin("Example: Fullscreen window"_RU >> u8"Пример: Полноэкранное окно", p_open, flags))
    {
        ImGui::Checkbox("Use work area instead of main area"_RU >> u8"Используйте рабочую зону вместо основной", &use_work_area);
        ImGui::SameLine();
        HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference."_RU >> u8"Основная область = вся область просмотра,\nРабочая область = вся область просмотра за вычетом разделов, используемых панелями главного меню, панелями задач и т. д.\n\nВключите панель главного меню в меню «Примеры», чтобы увидеть разницу.");

        ImGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
        ImGui::Indent();
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
        ImGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
        ImGui::Unindent();

        if (p_open && ImGui::Button("Close this window"_RU >> u8"Закрыть это окно"))
            *p_open = false;
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: управление заголовками окон / ShowExampleAppWindowTitles()
//-----------------------------------------------------------------------------

// Продемонстрируйте использование «##» и «###» в идентификаторах для управления созданием идентификаторов.
// Это относится и ко всем обычным элементам.
// Прочтите раздел часто задаваемых вопросов "Как я могу иметь несколько виджетов с одним и тем же ярлыком?" для деталей.
static void ShowExampleAppWindowTitles(bool*)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2         base_pos = viewport->Pos;

    // По умолчанию Windows однозначно идентифицируются по названию.
    // Вы можете использовать маркеры "##" и "###" для управления отображением/идентификатором.

    // Использование «##» для отображения того же заголовка, но с уникальным идентификатором.
    ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Same title as another window##1"_RU >> u8"Тот же заголовок, что и у другого окна##1");
    IMGUI_DEMO_MARKER("Examples/Manipulating window titles"_RU >> u8"Примеры/Управление заголовками окон");
    ImGui::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique."_RU >> u8"это окно 1.\nМой заголовок такой же, как у окна 2, но мой идентификатор уникален.");
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Same title as another window##2"_RU >> u8"Тот же заголовок, что и у другого окна##2");
    ImGui::Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique."_RU >> u8"это окно 2.\nМой заголовок такой же, как у окна 1, но мой идентификатор уникален.");
    ImGui::End();

    // Использование «###» для отображения меняющегося заголовка, но с сохранением статического идентификатора «AnimatedTitle».
    char buf[128];
    sprintf(buf, "Animated title %c %d###AnimatedTitle"_RU >> u8"Анимированный заголовок %c %d###AnimatedTitle", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], ImGui::GetFrameCount());
    ImGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), ImGuiCond_FirstUseEver);
    ImGui::Begin(buf);
    ImGui::Text("This window has a changing title."_RU >> u8"Это окно имеет изменяющийся заголовок.");
    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: пользовательский рендеринг с использованием ImDrawList API/ShowExampleAppCustomRendering()
//-----------------------------------------------------------------------------

// Продемонстрируйте использование низкоуровневого ImDrawList для рисования пользовательских фигур.
static void ShowExampleAppCustomRendering(bool* p_open)
{
    if (!ImGui::Begin("Example: Custom rendering"_RU >> u8"Пример: Пользовательский рендеринг", p_open))
    {
        ImGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Custom Rendering"_RU >> u8"Примеры/Пользовательский рендеринг");

    // Совет: Если вы выполняете много пользовательских визуализаций, вы, вероятно, захотите использовать свои собственные геометрические типы и преимущества
    // перегруженные операторы и т. д. Определите IM_VEC2_CLASS_EXTRA в imconfig. h для создания неявных преобразований между вашим
    // типы и ImVec2/ImVec4. Дорогой ImGui определяет перегруженные операторы, но они являются внутренними для imgui. cpp и не
    // выставлены наружу (чтобы не путаться с вашими типами) В этом примере мы не используем математические операторы!

    if (ImGui::BeginTabBar("##TabBar"))
    {
        if (ImGui::BeginTabItem("Primitives"_RU >> u8"Примитивы"))
        {
            ImGui::PushItemWidth(-ImGui::GetFontSize() * 15);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Рисовать градиенты
            // (обратите внимание, что в настоящее время они усугубляют наши проблемы с sRGB/Linear)
            // Вызов ImGui:: GetColorU32() умножает заданные цвета на текущую альфу стиля, но вы также можете напрямую передать IM_COL32().
            ImGui::Text("Gradients"_RU >> u8"Градиенты");
            ImVec2 gradient_size = ImVec2(ImGui::CalcItemWidth(), ImGui::GetFrameHeight());
            {
                ImVec2 p0    = ImGui::GetCursorScreenPos();
                ImVec2 p1    = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32  col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
                ImU32  col_b = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                ImGui::InvisibleButton("##gradient1", gradient_size);
            }
            {
                ImVec2 p0    = ImGui::GetCursorScreenPos();
                ImVec2 p1    = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32  col_a = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
                ImU32  col_b = ImGui::GetColorU32(IM_COL32(255, 0, 0, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                ImGui::InvisibleButton("##gradient2", gradient_size);
            }

            // Нарисуйте кучу примитивов
            ImGui::Text("All primitives"_RU >> u8"Все примитивы");
            static float  sz                         = 36.0f;
            static float  thickness                  = 3.0f;
            static int    ngon_sides                 = 6;
            static bool   circle_segments_override   = false;
            static int    circle_segments_override_v = 12;
            static bool   curve_segments_override    = false;
            static int    curve_segments_override_v  = 8;
            static ImVec4 colf                       = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            ImGui::DragFloat("Size"_RU >> u8"Размер", &sz, 0.2f, 2.0f, 100.0f, "%.0f");
            ImGui::DragFloat("Thickness"_RU >> u8"Толщина", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
            ImGui::SliderInt("N-gon sides"_RU >> u8"Стороны N-угольника", &ngon_sides, 3, 12);
            ImGui::Checkbox("##circlesegmentoverride"_RU >> u8"##круг_сегмент_переопределить", &circle_segments_override);
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            circle_segments_override |= ImGui::SliderInt("Circle segments override"_RU >> u8"Переопределение сегментов круга", &circle_segments_override_v, 3, 40);
            ImGui::Checkbox("##curvessegmentoverride"_RU >> u8"##кривые_сегмент_переопределить", &curve_segments_override);
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            curve_segments_override |= ImGui::SliderInt("Curves segments override"_RU >> u8"Переопределение сегментов кривых", &curve_segments_override_v, 3, 40);
            ImGui::ColorEdit4("Color"_RU >> u8"Цвет", &colf.x);

            const ImVec2      p               = ImGui::GetCursorScreenPos();
            const ImU32       col             = ImColor(colf);
            const float       spacing         = 10.0f;
            const ImDrawFlags corners_tl_br   = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;
            const float       rounding        = sz / 5.0f;
            const int         circle_segments = circle_segments_override ? circle_segments_override_v : 0;
            const int         curve_segments  = curve_segments_override ? curve_segments_override_v : 0;
            float             x               = p.x + 4.0f;
            float             y               = p.y + 4.0f;
            for (int n = 0; n < 2; n++)
            {
                // Первая линия использует толщину 1.0f, вторая линия использует настраиваемую толщину
                float th = (n == 0) ? 1.0f : thickness;
                draw_list->AddNgon(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides, th);
                x += sz + spacing;   // N-угольник
                draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments, th);
                x += sz + spacing;   // Круг
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 0.0f, ImDrawFlags_None, th);
                x += sz + spacing;   // Квадрат
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, ImDrawFlags_None, th);
                x += sz + spacing;   // Квадрат со всеми закругленными углами
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, corners_tl_br, th);
                x += sz + spacing;   // Квадрат с двумя закругленными углами
                draw_list->AddTriangle(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col, th);
                x += sz + spacing;   // Треугольник
                //draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col, th);x+= sz*0.4f + spacing; // Тонкий треугольник
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);
                x += sz + spacing;   // Горизонтальная линия (примечание: рисовать прямоугольник с заливкой будет быстрее!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);
                x += spacing;   // Вертикальная линия (примечание: рисовать прямоугольник с заливкой будет быстрее!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);
                x += sz + spacing;   // Диагональная линия

                // Квадратичная кривая Безье (3 контрольные точки)
                ImVec2 cp3[3] = {ImVec2(x, y + sz * 0.6f), ImVec2(x + sz * 0.5f, y - sz * 0.4f), ImVec2(x + sz, y + sz)};
                draw_list->AddBezierQuadratic(cp3[0], cp3[1], cp3[2], col, th, curve_segments);
                x += sz + spacing;

                // Кубическая кривая Безье (4 контрольные точки)
                ImVec2 cp4[4] = {ImVec2(x, y), ImVec2(x + sz * 1.3f, y + sz * 0.3f), ImVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), ImVec2(x + sz, y + sz)};
                draw_list->AddBezierCubic(cp4[0], cp4[1], cp4[2], cp4[3], col, th, curve_segments);

                x = p.x + 4;
                y += sz + spacing;
            }
            draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides);
            x += sz + spacing;   // N-угольник
            draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments);
            x += sz + spacing;   // Круг
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);
            x += sz + spacing;   // Квадрат
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f);
            x += sz + spacing;   // Квадрат со всеми закругленными углами
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br);
            x += sz + spacing;   // Квадрат с двумя закругленными углами
            draw_list->AddTriangleFilled(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col);
            x += sz + spacing;   // Треугольник
            //draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; // Тонкий треугольник
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + thickness), col);
            x += sz + spacing;   // Горизонтальная линия (быстрее, чем AddLine, но обрабатывает только целочисленную толщину)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y + sz), col);
            x += spacing * 2.0f;   // Вертикальная линия (быстрее, чем AddLine, но обрабатывает только целочисленную толщину)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);
            x += sz;   // Пиксель (быстрее, чем AddLine)
            draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));

            ImGui::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
            ImGui::PopItemWidth();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Canvas"_RU >> u8"Холст"))
        {
            static ImVector<ImVec2> points;
            static ImVec2           scrolling(0.0f, 0.0f);
            static bool             opt_enable_grid         = true;
            static bool             opt_enable_context_menu = true;
            static bool             adding_line             = false;

            ImGui::Checkbox("Enable grid"_RU >> u8"Включить сетку", &opt_enable_grid);
            ImGui::Checkbox("Enable context menu"_RU >> u8"Включить контекстное меню", &opt_enable_context_menu);
            ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu."_RU >> u8"Мышь влево: перетащите, чтобы добавить линии,\nМышь вправо: перетащите для прокрутки, щелкните для контекстного меню.");

            // Обычно вы должны использовать пару BeginChild()/EndChild(), чтобы извлечь выгоду из области отсечения + собственной прокрутки.
            // Здесь мы демонстрируем, что это можно заменить простым смещением + пользовательское рисование + вызовы PushClipRect/PopClipRect().
            // Чтобы использовать дочернее окно, мы могли бы использовать, например. г:
            //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Отключить заполнение
            //      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Установить цвет фона
            //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
            //      ImGui::PopStyleColor();
            //      ImGui::PopStyleVar();
            //      [...]
            //      ImGui::EndChild();

            // Использование InvisibleButton() для удобства: 1) он перемещает курсор макета и 2) позволяет нам использовать IsItemHovered()/IsItemActive()
            ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API использует координаты экрана!
            ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Изменение размера холста до доступного
            if (canvas_sz.x < 50.0f)
                canvas_sz.x = 50.0f;
            if (canvas_sz.y < 50.0f)
                canvas_sz.y = 50.0f;
            ImVec2      canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            // Нарисуйте границу и цвет фона
            ImGuiIO&    io        = ImGui::GetIO();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

            // Это поймает наше взаимодействие
            ImGui::InvisibleButton("canvas"_RU >> u8"холст", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool   is_hovered = ImGui::IsItemHovered();                            // Наведение
            const bool   is_active  = ImGui::IsItemActive();                             // Удерживание
            const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);   // Заблокировать начало прокрутки
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            // Добавьте первый и второй пункты
            if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                points.push_back(mouse_pos_in_canvas);
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (adding_line)
            {
                points.back() = mouse_pos_in_canvas;
                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    adding_line = false;
            }

            // Панорамирование (мы используем нулевой порог мыши, когда нет контекстного меню)
            // Вы можете решить сделать этот порог динамическим в зависимости от того, зависает ли мышь над чем-либо и т. д.
            const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
            {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            // Контекстное меню (при пороге мыши по умолчанию)
            ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
            if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
                ImGui::OpenPopupOnItemClick("context"_RU >> u8"контекст", ImGuiPopupFlags_MouseButtonRight);
            if (ImGui::BeginPopup("context"_RU >> u8"контекст"))
            {
                if (adding_line)
                    points.resize(points.size() - 2);
                adding_line = false;
                if (ImGui::MenuItem("Remove one"_RU >> u8"Удалить один", NULL, false, points.Size > 0))
                {
                    points.resize(points.size() - 2);
                }
                if (ImGui::MenuItem("Remove all"_RU >> u8"Убрать все", NULL, false, points.Size > 0))
                {
                    points.clear();
                }
                ImGui::EndPopup();
            }

            // Нарисовать сетку + все линии на холсте
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid)
            {
                const float GRID_STEP = 64.0f;
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }
            for (int n = 0; n < points.Size; n += 2)
                draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
            draw_list->PopClipRect();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("BG/FG draw lists"_RU >> u8"ЗП/ПП отрисовка списков"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            ImGui::Checkbox("Draw in Background draw list"_RU >> u8"Отобразить на Фоне(ЗП) отрисовки списков", &draw_bg);
            ImGui::SameLine();
            HelpMarker("The Background draw list will be rendered below every Dear ImGui windows."_RU >> u8"Список фоновых рисунков будет отображаться под каждым окном Dear ImGui.");
            ImGui::Checkbox("Draw in Foreground draw list"_RU >> u8"Отобразить на переднем плане отрисовки списков", &draw_fg);
            ImGui::SameLine();
            HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows."_RU >> u8"Список рисунков Переднего плана будет отображаться над каждым окном Dear ImGui.");
            ImVec2 window_pos    = ImGui::GetWindowPos();
            ImVec2 window_size   = ImGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                ImGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10 + 4);
            if (draw_fg)
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: стыковка, DockSpace/ShowExampleAppDockSpace()
//-----------------------------------------------------------------------------

// Продемонстрируйте использование DockSpace() для создания явного узла стыковки в существующем окне.
// Примечание. Большинство средств Docking можно использовать без вызова какого-либо API. Вам НЕ нужно вызывать DockSpace() для использования стыковки!
// - Перетащите из строки заголовка окна или их вкладки, чтобы закрепить/открепить. Удерживайте SHIFT, чтобы отключить стыковку.
// - Перетащите из кнопки меню окна (верхняя левая кнопка), чтобы открепить весь узел (все окна).
// - Когда и.о. ConfigDockingWithShift == true, вместо этого вам нужно удерживать SHIFT, чтобы _включить_ стыковку/отстыковку.
// О доках:
// - Используйте DockSpace() для создания явного узла стыковки _внутри_ существующего окна.
// - Используйте DockSpaceOverViewport() для создания явного узла стыковки, покрывающего экран или определенное окно просмотра.
//   Это часто используется с ImGuiDockNodeFlags_PassthruCentralNode.
// - Важно: Dockspaces должны быть отправлены _перед_ любым окном, которое они могут разместить. Отправьте его заранее в своем кадре! (*)
// - Важно: Dockspaces должны оставаться активными, если они скрыты, иначе окна, прикрепленные к ним, будут отстыкованы.
//   т. е. г. если у вас есть несколько вкладок с док-пространством внутри каждой вкладки: отправьте невидимые стыковочные пространства с ImGuiDockNodeFlags_KeepAliveOnly.
//   (*) из-за этого ограничения неявное окно \"Отладка\" не может быть пристыковано к явному узлу DockSpace(),
//   потому что это окно передается как часть вызова NewFrame(). Простой обходной путь заключается в том, что вы можете создать
//   ваше собственное неявное окно "Debug##2" после вызова DockSpace() и оставить его в стеке окон для всех, кто может его использовать.
void ShowExampleAppDockSpace(bool* p_open)
{
    // Если вы удалите некоторые функции, эта демонстрация в значительной степени эквивалентна вызову DockSpaceOverViewport()!
    // В большинстве случаев вы можете просто вызвать DockSpaceOverViewport() и игнорировать весь приведенный ниже код!
    // В этой конкретной демонстрации мы не используем DockSpaceOverViewport(), потому что:
    // - мы позволяем главному окну быть плавающим/подвижным вместо того, чтобы заполнять область просмотра (когда opt_fullscreen == false)
    // - разрешаем хост-окну иметь отступы (когда opt_padding == true)
    // - у нас есть локальная панель меню в окне хоста (вместо того, чтобы вы могли использовать BeginMainMenuBar() + DockSpaceOverViewport() в вашем коде!)
    //   TL; ДР; эта демонстрация более сложна, чем то, что вы обычно используете.
    //   Если бы мы удалили все варианты, которые мы демонстрируем, эта демонстрация стала бы такой:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

    static bool               opt_fullscreen  = true;
    static bool               opt_padding     = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // Мы используем флаг ImGuiWindowFlags_NoDocking, чтобы сделать родительское окно недоступным для пристыковки.
    // потому что было бы запутанно иметь две цели стыковки друг с другом.
    ImGuiWindowFlags          window_flags    = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // При использовании ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() будет отображать наш фон
    // и обрабатывать сквозное отверстие, поэтому мы просим Begin() не отображать фон.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Важно: обратите внимание, что мы продолжаем, даже если Begin() возвращает false (окно свернуто).
    // Это потому, что мы хотим, чтобы наш DockSpace() оставался активным. Если DockSpace() неактивен,
    // все активные окна, пристыкованные к нему, потеряют своего родителя и станут отстыкованными.
    // Мы не можем сохранить отношения стыковки между активным окном и неактивным стыковкой, иначе
    // любое изменение пространства дока/настроек приведет к тому, что окна застрянут в подвешенном состоянии и никогда не будут видны.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo"_RU >> u8"Док-пространство Демо", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Отправить место для док-станции
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"_RU >> u8"Опции"))
        {
            // Отключение полноэкранного режима позволит переместить окно на передний план других окон,
            // который мы не можем отменить в данный момент без более точного управления глубиной/z окна.
            ImGui::MenuItem("Fullscreen"_RU >> u8"Полноэкранный", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding"_RU >> u8"Заполнение", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
            }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
            }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Close"_RU >> u8"Закрыть", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }
        HelpMarker("When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!"
                   "\n"
                   "- Drag from window title bar or their tab to dock/undock."
                   "\n"
                   "- Drag from window menu button (upper-left button) to undock an entire node (all windows)."
                   "\n"
                   "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)"
                   "\n"
                   "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)"
                   "\n"
                   "This demo app has nothing to do with enabling docking!"
                   "\n\n"
                   "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window."
                   "\n\n"
                   "Read comments in ShowExampleAppDockSpace() for more details."_RU >>
            u8"Когда стыковка включена, вы ВСЕГДА можете пристыковать окно MOST к другому! Попробуй это сейчас!"
            "\n"
            "- Перетащите из строки заголовка окна или их вкладки, чтобы закрепить/открепить."
            "\n"
            "- Перетащите кнопку меню окна (верхняя левая кнопка), чтобы открепить весь узел (все окна)."
            "\n"
            "- Удерживайте SHIFT, чтобы отключить стыковку (если io.ConfigDockingWithShift == false, по умолчанию)"
            "\n"
            "- Удерживайте SHIFT, чтобы включить стыковку (если io.ConfigDockingWithShift == true)"
            "\n"
            "Это демонстрационное приложение не имеет ничего общего с включением стыковки!"
            "\n\n"
            "Это демонстрационное приложение демонстрирует только использование ImGui:: DockSpace(), которое позволяет вам вручную создать узел стыковки _внутри_ другого окна."
            "\n\n"
            "Прочтите комментарии в ShowExampleAppDockSpace() для более подробной информации.");

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Пример приложения: обработка документов / ShowExampleAppDocuments()
//-----------------------------------------------------------------------------

// Упрощенная структура для имитации модели документа
struct MyDocument
{
    const char* Name;        // Заголовок документа
    bool        Open;        // Установить при открытии (мы храним массив всех доступных документов для упрощения демонстрационного кода!)
    bool        OpenPrev;    // Копия Open из последнего обновления.
    bool        Dirty;       // Установить, когда документ был изменен
    bool        WantClose;   // Установите, когда документ
    ImVec4      Color;       // Произвольная переменная, связанная с документом

    MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        Name = name;
        Open = OpenPrev = open;
        Dirty           = false;
        WantClose       = false;
        Color           = color;
    }
    void DoOpen()
    {
        Open = true;
    }
    void DoQueueClose()
    {
        WantClose = true;
    }
    void DoForceClose()
    {
        Open  = false;
        Dirty = false;
    }
    void DoSave()
    {
        Dirty = false;
    }

    // Отображение содержимого заполнителя для документа
    static void DisplayContents(MyDocument* doc)
    {
        ImGui::PushID(doc);
        ImGui::Text("Document \"%s\""_RU >> u8"Документ \"%s\"", doc->Name);
        ImGui::PushStyleColor(ImGuiCol_Text, doc->Color);
        ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."_RU >> u8"Знание о том, что нужно сидеть сложа руки, заботиться о том, чтобы избавиться от лишних килограммов, откладывать на потом временные затраты на труд и долги великой аликвы.");
        ImGui::PopStyleColor();
        if (ImGui::Button("Modify"_RU >> u8"Изменить", ImVec2(100, 0)))
            doc->Dirty = true;
        ImGui::SameLine();
        if (ImGui::Button("Save"_RU >> u8"Сохранить", ImVec2(100, 0)))
            doc->DoSave();
        ImGui::ColorEdit3("color"_RU >> u8"цвет", &doc->Color.x);   // Полезно для проверки поведения перетаскивания и удержания при перетаскивании на открытую вкладку.
        ImGui::PopID();
    }

    // Показать контекстное меню для документа
    static void DisplayContextMenu(MyDocument* doc)
    {
        if (!ImGui::BeginPopupContextItem())
            return;

        char buf[256];
        sprintf(buf, "Save %s"_RU >> u8"Сохранить %s", doc->Name);
        if (ImGui::MenuItem(buf, "CTRL+S", false, doc->Open))
            doc->DoSave();
        if (ImGui::MenuItem("Close"_RU >> u8"Закрыть", "CTRL+W", false, doc->Open))
            doc->DoQueueClose();
        ImGui::EndPopup();
    }
};

struct ExampleAppDocuments
{
    ImVector<MyDocument> Documents;

    ExampleAppDocuments()
    {
        Documents.push_back(MyDocument("Lettuce"_RU >> u8"Латук", true, ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("Eggplant"_RU >> u8"Баклажан", true, ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
        Documents.push_back(MyDocument("Carrot"_RU >> u8"Баклажан", true, ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
        Documents.push_back(MyDocument("Tomato"_RU >> u8"Помидор", false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("A Rather Long Title"_RU >> u8"Довольно длинное название", false));
        Documents.push_back(MyDocument("Some Document"_RU >> u8"Какой-то документ", false));
    }
};

// [Необязательно] Уведомлять систему о закрытии вкладок/окна, которое произошло за пределами обычного интерфейса вкладок.
// Если вкладка была закрыта программно (она же закрыта из другого источника, такого как Checkbox() в демо,
// в отличие от нажатия на обычную кнопку закрытия вкладки) и перестает отправляться, для этого потребуется кадр
// панель вкладок, чтобы заметить ее отсутствие. Во время этого кадра в панели вкладок будет пробел, и если та вкладка, которая
// исчезла выбранная, панель вкладок сообщит об отсутствии выбранной вкладки во время кадра. Это будет эффективно
// создать впечатление мерцания для одного кадра.
// Мы вызываем SetTabItemClosed(), чтобы вручную уведомить панель вкладок или систему стыковки об удаленных вкладках, чтобы избежать этого сбоя.
// Обратите внимание, что это совершенно необязательно и влияет только на панели вкладок с флагом ImGuiTabBarFlags_Reorderable.
static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments& app)
{
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (!doc->Open && doc->OpenPrev)
            ImGui::SetTabItemClosed(doc->Name);
        doc->OpenPrev = doc->Open;
    }
}

void ShowExampleAppDocuments(bool* p_open)
{
    static ExampleAppDocuments app;

    // Options
    enum Target
    {
        Target_None,
        Target_Tab,                 // Создавайте документы как локальную вкладку на локальной панели вкладок
        Target_DockSpaceAndWindow   // Создавайте документы как обычные окна и создавайте встроенное пространство для доков.
    };
    static Target           opt_target              = Target_Tab;
    static bool             opt_reorderable         = true;
    static ImGuiTabBarFlags opt_fitting_flags       = ImGuiTabBarFlags_FittingPolicyDefault_;

    // Когда (opt target == Target Dock Space And Window) возможно, что одно из наших дочерних окон документа (например, "Баклажан")
    // то, что мы испускаем, закрепляется в том же месте, что и родительское окно ("Пример: Документы").
    // Это создаст проблемный цикл обратной связи, потому что выбор вкладки «Баклажан» сделает вкладку «Пример: Документы»
    // не видно, что в свою очередь перестанет выдавать окно "Баклажан".
    // Мы избегаем этой проблемы, отправляя наше окно документов, даже если наше родительское окно в данный момент не видно.
    // Другое решение может состоять в том, чтобы заставить окно «Пример: Документы» использовать флаги окна ImGui «Нет стыковки».

    bool                    window_contents_visible = ImGui::Begin("Example: Documents"_RU >> u8"Пример: Документы", p_open, ImGuiWindowFlags_MenuBar);
    if (!window_contents_visible && opt_target != Target_DockSpaceAndWindow)
    {
        ImGui::End();
        return;
    }

    // Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"_RU >> u8"Файл"))
        {
            int open_count = 0;
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                open_count += app.Documents[doc_n].Open ? 1 : 0;

            if (ImGui::BeginMenu("Open"_RU >> u8"Открыть", open_count < app.Documents.Size))
            {
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                {
                    MyDocument* doc = &app.Documents[doc_n];
                    if (!doc->Open)
                        if (ImGui::MenuItem(doc->Name))
                            doc->DoOpen();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Close All Documents"_RU >> u8"Закрыть все документы", NULL, false, open_count > 0))
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    app.Documents[doc_n].DoQueueClose();
            if (ImGui::MenuItem("Exit"_RU >> u8"Выход", "Alt+F4"))
            {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // [Debug] Список документов с одним флажком для каждого
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (doc_n > 0)
            ImGui::SameLine();
        ImGui::PushID(doc);
        if (ImGui::Checkbox(doc->Name, &doc->Open))
            if (!doc->Open)
                doc->DoForceClose();
        ImGui::PopID();
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * 12);
    ImGui::Combo("Output"_RU >> u8"Выход", (int*)&opt_target, "None\0TabBar+Tabs\0DockSpace+Window\0");
    ImGui::PopItemWidth();
    bool redock_all = false;
    if (opt_target == Target_Tab)
    {
        ImGui::SameLine();
        ImGui::Checkbox("Reorderable Tabs"_RU >> u8"Переупорядочиваемые вкладки", &opt_reorderable);
    }
    if (opt_target == Target_DockSpaceAndWindow)
    {
        ImGui::SameLine();
        redock_all = ImGui::Button("Redock all"_RU >> u8"Перестыковать все");
    }

    ImGui::Separator();

    // Об флагах ImGuiWindowFlags_UnsavedDocument / ImGuiTabItemFlags_UnsavedDocument.
    // Они имеют несколько эффектов:
    // - Поставьте точку рядом с заголовком.
    // - Вкладка выбирается при нажатии кнопки закрытия X.
    // - Закрытие не предполагается (будет ждать, пока пользователь перестанет отправлять вкладку).
    //   В противном случае предполагается закрытие при нажатии X, поэтому, если вы продолжите отправку, вкладка может снова появиться в конце панели вкладок.
    //   Нам нужно принять закрытие по умолчанию, иначе ожидание «отсутствия представления» в следующем кадре оставит пустой
    //   отверстие для одного кадра, как в панели вкладок, так и в содержимом вкладок при закрытии вкладки/окна.
    //   Редко используемая функция SetTabItemClosed() — это способ уведомить о программном закрытии, чтобы избежать дыры в один кадр.

    // Вкладки
    if (opt_target == Target_Tab)
    {
        ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (opt_reorderable)
                NotifyOfDocumentsClosedElsewhere(app);

            // [DEBUG] Стресс-тесты
            //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Автоматически показывать/скрывать вкладку. Тестируйте различные взаимодействия, например. перетаскивание с этим дальше.
            //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Элемент вкладки TestSetSelected(), вероятно, не очень полезен как есть.

            // Submit Tabs
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (!doc->Open)
                    continue;

                ImGuiTabItemFlags tab_flags = (doc->Dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool              visible   = ImGui::BeginTabItem(doc->Name, &doc->Open, tab_flags);

                // Отменить попытку закрытия при несохраненном добавлении в очередь сохранения, чтобы мы могли отобразить всплывающее окно.
                if (!doc->Open && doc->Dirty)
                {
                    doc->Open = true;
                    doc->DoQueueClose();
                }

                MyDocument::DisplayContextMenu(doc);
                if (visible)
                {
                    MyDocument::DisplayContents(doc);
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }
    }
    else if (opt_target == Target_DockSpaceAndWindow)
    {
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            NotifyOfDocumentsClosedElsewhere(app);

            // Создайте узел DockSpace, к которому можно прикрепить любое окно.
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id);

            // Создать окна
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (!doc->Open)
                    continue;

                ImGui::SetNextWindowDockID(dockspace_id, redock_all ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
                ImGuiWindowFlags window_flags = (doc->Dirty ? ImGuiWindowFlags_UnsavedDocument : 0);
                bool             visible      = ImGui::Begin(doc->Name, &doc->Open, window_flags);

                // Отменить попытку закрытия при несохраненном добавлении в очередь сохранения, чтобы мы могли отобразить всплывающее окно.
                if (!doc->Open && doc->Dirty)
                {
                    doc->Open = true;
                    doc->DoQueueClose();
                }

                MyDocument::DisplayContextMenu(doc);
                if (visible)
                    MyDocument::DisplayContents(doc);

                ImGui::End();
            }
        }
        else
        {
            ShowDockingDisabledMessage();
        }
    }

    // Ранний доступ к другому контенту
    if (!window_contents_visible)
    {
        ImGui::End();
        return;
    }

    // Обновить очередь закрытия
    static ImVector<MyDocument*> close_queue;
    if (close_queue.empty())
    {
        // Очередь закрытия заблокирована, как только мы запустили всплывающее окно
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (doc->WantClose)
            {
                doc->WantClose = false;
                close_queue.push_back(doc);
            }
        }
    }

    // Показать интерфейс подтверждения закрытия
    if (!close_queue.empty())
    {
        int close_queue_unsaved_documents = 0;
        for (int n = 0; n < close_queue.Size; n++)
            if (close_queue[n]->Dirty)
                close_queue_unsaved_documents++;

        if (close_queue_unsaved_documents == 0)
        {
            // Закрыть документы, когда все несохраненные
            for (int n = 0; n < close_queue.Size; n++)
                close_queue[n]->DoForceClose();
            close_queue.clear();
        }
        else
        {
            if (!ImGui::IsPopupOpen("Save?"_RU >> u8"Сохранить?"))
                ImGui::OpenPopup("Save?"_RU >> u8"Сохранить?");
            if (ImGui::BeginPopupModal("Save?"_RU >> u8"Сохранить?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Save change to the following items?"_RU >> u8"Сохранить изменения для следующих элементов?");
                float item_height = ImGui::GetTextLineHeightWithSpacing();
                if (ImGui::BeginChildFrame(ImGui::GetID("frame"_RU >> u8"рамка"), ImVec2(-FLT_MIN, 6.25f * item_height)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        if (close_queue[n]->Dirty)
                            ImGui::Text("%s", close_queue[n]->Name);
                    ImGui::EndChildFrame();
                }

                ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
                if (ImGui::Button("Yes"_RU >> u8"Да", button_size))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                    {
                        if (close_queue[n]->Dirty)
                            close_queue[n]->DoSave();
                        close_queue[n]->DoForceClose();
                    }
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("No"_RU >> u8"Нет", button_size))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        close_queue[n]->DoForceClose();
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"_RU >> u8"Отменить", button_size))
                {
                    close_queue.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}

// Конец демонстрационного кода
#else

void ImGui::ShowAboutWindow(bool*) {}
void ImGui::ShowDemoWindow(bool*) {}
void ImGui::ShowUserGuide() {}
void ImGui::ShowStyleEditor(ImGuiStyle*) {}

#endif

#endif   // #ifndef IMGUI_DISABLE
