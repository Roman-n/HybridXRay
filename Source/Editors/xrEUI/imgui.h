// dear imgui, v1.88 WIP
// (headers)

// Помощь:
// - Читать FAQ в http://dearimgui.org/faq
// - Новички, прочтите «Руководство программиста» в imgui.cpp, чтобы узнать, как настроить Dear ImGui в вашей кодовой базе.
// - Вызов и чтение ImGui::ShowDemoWindow() в imgui_demo.cpp. Все приложения в examples/ делают это.
// Подробности, ссылки и комментарии читайте в imgui.cpp..

// Ресурсы:
// - FAQ                               http://dearimgui.org/faq
// - Домашняя страница & последний     https://github.com/ocornut/imgui
// - Выпуски & changelog               https://github.com/ocornut/imgui/releases
// - Галерея                           https://github.com/ocornut/imgui/issues/5243 (пожалуйста, размещайте свои скриншоты/видео там!)
// - Вики                              https://github.com/ocornut/imgui/wiki        (там много хорошего)
// - Глоссарий                         https://github.com/ocornut/imgui/wiki/Glossary
// - Issues & support                  https://github.com/ocornut/imgui/issues

// Начнём?
// - Для начинающих пользователей, у которых возникают проблемы с компиляцией/связыванием/запуском или проблемы с загрузкой шрифтаs:
//   пожалуйста, опубликуйте в https://github.com/ocornut/imgui/discussions если вы не можете найти решение в ресурсах выше.

/*

Индекс этого файла:
// [РАЗДЕЛ] Беспорядок в заголовке
// [РАЗДЕЛ] Форвардные объявления и основные типы
// [РАЗДЕЛ] Уважаемые функции API конечного пользователя ImGui
// [РАЗДЕЛ] Флаги и перечисления
// [РАЗДЕЛ] Помощники: Макросы выделения памяти, ImVector<>
// [РАЗДЕЛ] Стиль ImGui
// [РАЗДЕЛ] ImGui IO
// [РАЗДЕЛ] Разные структуры данных (ImGuiInputTextCallbackData, ImGuiSizeCallbackData, ImGuiWindowClass, ImGuiPayload, ImGuiTableSortSpecs, ImGuiTableColumnSortSpecs)
// [РАЗДЕЛ] Помощники (ImGuiOnceUponAFrame, ImGuiTextFilter, ImGuiTextBuffer, ImGuiStorage, ImGuiListClipper, ImColor)
// [РАЗДЕЛ] API рисования (ImDrawCallback, ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawFlags, ImDrawListFlags, ImDrawList, ImDrawData)
// [РАЗДЕЛ] Font API (ImFontConfig, ImFontGlyph, ImFontGlyphRangesBuilder, ImFontAtlasFlags, ImFontAtlas, ImFont)
// [SECTION] Viewports (ImGuiViewportFlags, ImGuiViewport)
// [РАЗДЕЛ] Интерфейсы, зависящие от платформы (ImGuiPlatformIO, ImGuiPlatformMonitor, ImGuiPlatformImeData)
// [РАЗДЕЛ] Устаревшие функции и типы

*/

#pragma once

// Файл конфигурации с параметрами времени компиляции (редактировать imconfig.h или '#define IMGUI_USER_CONFIG "myfilename.h" из вашей системы сборки')
#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h"
#endif

#ifndef IMGUI_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

// Включения
#include <float.h>    // FLT_MIN, FLT_MAX
#include <stdarg.h>   // va_list, va_start, va_end
#include <stddef.h>   // ptrdiff_t, NULL
#include <string.h>   // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp

// Версия
// (Целое число, закодированное как XYYZZ для использования в #если условия препроцессора. Незавершенные версии обычно начинаются с XYY99, а затем перескакивают на XYY00, XYY01 и т. д., когда происходит пометка выпуска)
#define IMGUI_VERSION        "1.88 WIP"
#define IMGUI_VERSION_NUM    18724
#define IMGUI_CHECKVERSION() ImGui::DebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))
#define IMGUI_HAS_TABLE
#define IMGUI_HAS_VIEWPORT   // WIP-ветвь видового экрана
#define IMGUI_HAS_DOCK       // Стыковка ветки WIP

// Определить атрибуты всех объявлений символов API (например, для DLL под Windows)
// IMGUI_API используется для основных функций imgui, IMGUI_IMPL_API используется для файлов бэкенда по умолчанию (imgui_impl_xxx.h)
// С использованием dear imgui через разделяемую библиотеку не рекомендуется, потому что мы не гарантируем обратную или прямую совместимость с ABI (также накладные расходы на вызов функции, так как дорогой imgui - это API с большим количеством вызовов)
#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API IMGUI_API
#endif

// Вспомогательные макросы
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR) assert(_EXPR)   // Вы можете переопределить обработчик утверждений по умолчанию, отредактировав imconfig.h
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))   // Размер статического массива в стиле C. Не используйте указатели!
#define IM_UNUSED(_VAR)             ((void)(_VAR))                            // Используется для отключения «предупреждений о неиспользуемых переменных». Часто полезно, так как утверждения могут быть удалены из окончательных сборок.
#define IM_OFFSETOF(_TYPE, _MEMBER) offsetof(_TYPE, _MEMBER)                  // Смещение _MEMBER within _TYPE. Стандартизирован как offsetof() в C++11

// Вспомогательные макросы - IM_FMTARGS, IM_FMTLIST: Примените предупреждения в стиле printf к нашим функциям форматирования.
#if !defined(IMGUI_USE_STB_SPRINTF) && defined(__MINGW32__) && !defined(__clang__)
#define IM_FMTARGS(FMT) __attribute__((format(gnu_printf, FMT, FMT + 1)))
#define IM_FMTLIST(FMT) __attribute__((format(gnu_printf, FMT, 0)))
#elif !defined(IMGUI_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define IM_FMTARGS(FMT) __attribute__((format(printf, FMT, FMT + 1)))
#define IM_FMTLIST(FMT) __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif

// Отключите некоторые из наиболее агрессивных проверок времени выполнения отладки MSVC в заголовке/нижнем колонтитуле функции (используется в некоторых простых/низкоуровневых функциях).
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER) && !defined(IMGUI_DEBUG_PARANOID)
#define IM_MSVC_RUNTIME_CHECKS_OFF     __pragma(runtime_checks("", off)) __pragma(check_stack(off)) __pragma(strict_gs_check(push, off))
#define IM_MSVC_RUNTIME_CHECKS_RESTORE __pragma(runtime_checks("", restore)) __pragma(check_stack()) __pragma(strict_gs_check(pop))
#else
#define IM_MSVC_RUNTIME_CHECKS_OFF
#define IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Предупреждения
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:26495)   // [Статический анализатор] Переменная «XXX» не инициализирована. Всегда инициализируйте переменную-член (тип 6).
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"           // предупреждение: неизвестная опция после типа «#pragma GCC Diagnostic»
#pragma GCC diagnostic ignored "-Wclass-memaccess"   // [__GNUC__ >= 8] предупреждение: 'memset/memcpy' очищает/записывает объект типа 'xxxx' без тривиального присваивания копии; вместо этого используйте присваивание или инициализацию значения
#endif

//-----------------------------------------------------------------------------
// [SECTION] Форвардные объявления и основные типы
//-----------------------------------------------------------------------------

// Форвардные объявления
struct ImDrawChannel;                // Временное хранилище для вывода команд рисования не по порядку, используемое ImDrawListSplitter и ImDrawList:: ChannelsSplit()
struct ImDrawCmd;                    // Одна команда отрисовки в родительском ImDrawList (обычно сопоставляется с 1 вызовом отрисовки GPU, если только это не обратный вызов)
struct ImDrawData;                   // Все списки команд рисования, необходимые для рендеринга кадра + координаты положения/размера для использования в матрице проекции.
struct ImDrawList;                   // Один список команд рисования (обычно по одной на окно, концептуально вы можете рассматривать это как динамический построитель «сетки»)
struct ImDrawListSharedData;         // Данные совместно используются несколькими списками отрисовки (обычно принадлежат родительскому контексту ImGui, но вы можете создать его самостоятельно)
struct ImDrawListSplitter;           // Помощник для разделения списка рисования на разные слои, которые можно рисовать не по порядку, а затем сглаживать обратно.
struct ImDrawVert;                   // Одна вершина (по умолчанию pos + uv + col = 20 байт. Переопределить макет с помощью IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct ImFont;                       // Данные времени выполнения для одного шрифта в родительском ImFontAtlas
struct ImFontAtlas;                  // Данные времени выполнения для нескольких шрифтов, запекание нескольких шрифтов в одну текстуру, загрузчик шрифтов TTF/OTF
struct ImFontBuilderIO;              // Непрозрачный интерфейс для построителя шрифтов (stb_truetype или FreeType).
struct ImFontConfig;                 // Данные конфигурации при добавлении шрифта или объединении шрифтов
struct ImFontGlyph;                  // Один глиф шрифта (кодовая точка + координаты в ImFontAtlas + смещение)
struct ImFontGlyphRangesBuilder;     // Помощник для построения диапазонов глифов из текстовых/строковых данных
struct ImColor;                      // Вспомогательные функции для создания цвета, который можно преобразовать либо в u32, либо в float4 (*УСТАРЕЛО*, пожалуйста, избегайте использования)
struct ImGuiContext;                 // Dear ImGui контекст (непрозрачная структура, если не включает imgui_internal.h)
struct ImGuiIO;                      // Основная конфигурация и ввод-вывод между вашим приложением и ImGui
struct ImGuiInputTextCallbackData;   // Общее состояние InputText() при использовании пользовательского обратного вызова ImGuiInputText (редкое/расширенное использование)
struct ImGuiKeyData;                 // Хранилище для ImGuiIO и функций IsKeyDown(), IsKeyPressed() и т. д.
struct ImGuiListClipper;             // Помощник для ручного вырезания большого списка элементов
struct ImGuiOnceUponAFrame;          // Помощник для запуска блока кода не более одного раза за кадр
struct ImGuiPayload;                 // Пользовательские данные для операций перетаскивания
struct ImGuiPlatformIO;              // Поддержка нескольких видовых экранов: интерфейс для бэкендов платформы/рендерера + видовые экраны для рендеринга
struct ImGuiPlatformMonitor;         // Поддержка нескольких видовых экранов: пользовательские границы для каждого подключенного монитора/дисплея. Используется при позиционировании всплывающих окон и всплывающих подсказок, чтобы они не пересекали мониторы.
struct ImGuiPlatformImeData;         // Данные платформы IME для функции io.SetPlatformImeDataFn().
struct ImGuiSizeCallbackData;        // Данные обратного вызова при использовании SetNextWindowSizeConstraints() (редкое/продвинутое использование)
struct ImGuiStorage;                 // Помощник для хранения ключей->значений
struct ImGuiStyle;                   // Данные времени выполнения для стилей/цветов
struct ImGuiTableSortSpecs;          // Спецификации сортировки для таблицы (часто обработка спецификаций сортировки для одного столбца, иногда для нескольких)
struct ImGuiTableColumnSortSpecs;    // Спецификация сортировки для одного столбца таблицы
struct ImGuiTextBuffer;              // Helper для хранения и добавления в текстовый буфер (~построитель строк)
struct ImGuiTextFilter;              // Помощник для разбора и применения текстовых фильтров (например, "aaaaa[,bbbbb][,ccccc]")
struct ImGuiViewport;                // Окно платформы (всегда 1, если не включено несколько окон просмотра. По одному на окно платформы для вывода). В будущем может представлять Platform Monitor
struct ImGuiWindowClass;             // Класс окна (редкое/расширенное использование: предоставление подсказок серверной части платформы с помощью измененных флагов области просмотра и информации о родителях/потомках)

// Enums/Flags (объявлены как int для совместимости со старым C++, чтобы разрешить использование в качестве флагов без накладных расходов и не загрязнять верхнюю часть этого файла)
// - Совет: Используйте средства навигации IDE для программирования по именам в центральном столбце ниже, чтобы найти актуальные списки флагов/перечислений!
//   В Visual Studio IDE: сочетание клавиш CTRL+запятая ("Редактировать.Перейти ко всем") может следовать за символами в комментариях, тогда как сочетание клавиш CTRL+F12 ("Правка.Перейти к реализации") не может.
//   С установленным Visual Assist: ALT+G ("VAssistX.GoToImplementation") также может следовать за символами в комментариях.
typedef int ImGuiCol;                // -> enum ImGuiCol_             // Enum: Идентификатор цвета для стиля
typedef int ImGuiCond;               // -> enum ImGuiCond_            // Enum: Условие для многих функций Set*()
typedef int ImGuiDataType;           // -> enum ImGuiDataType_        // Enum: Основной тип данных
typedef int ImGuiDir;                // -> enum ImGuiDir_             // Enum: Основное направление
typedef int ImGuiKey;                // -> enum ImGuiKey_             // Enum: Идентификатор ключа
typedef int ImGuiNavInput;           // -> enum ImGuiNavInput_        // Enum: Идентификатор ввода для навигации
typedef int ImGuiMouseButton;        // -> enum ImGuiMouseButton_     // Enum: Идентификатор кнопки мыши (0=левая, 1=правая, 2=средняя)
typedef int ImGuiMouseCursor;        // -> enum ImGuiMouseCursor_     // Enum: Идентификатор курсора мыши
typedef int ImGuiSortDirection;      // -> enum ImGuiSortDirection_   // Enum: Направление сортировки (по возрастанию или по убыванию)
typedef int ImGuiStyleVar;           // -> enum ImGuiStyleVar_        // Enum: Идентификатор переменной для стиля
typedef int ImGuiTableBgTarget;      // -> enum ImGuiTableBgTarget_   // Enum: Целевой цвет для TableSetBgColor()
typedef int ImDrawFlags;             // -> enum ImDrawFlags_          // Flags: для функций ImDrawList
typedef int ImDrawListFlags;         // -> enum ImDrawListFlags_      // Flags: для экземпляра ImDrawList
typedef int ImFontAtlasFlags;        // -> enum ImFontAtlasFlags_     // Flags: для сборки ImFontAtlas
typedef int ImGuiBackendFlags;       // -> enum ImGuiBackendFlags_    // Flags: для io.BackendFlags
typedef int ImGuiButtonFlags;        // -> enum ImGuiButtonFlags_     // Flags: для InvisibleButton()
typedef int ImGuiColorEditFlags;     // -> enum ImGuiColorEditFlags_  // Flags: для ColorEdit4(), ColorPicker4() и т. д..
typedef int ImGuiConfigFlags;        // -> enum ImGuiConfigFlags_     // Flags: для io.ConfigFlags
typedef int ImGuiComboFlags;         // -> enum ImGuiComboFlags_      // Flags: для BeginCombo()
typedef int ImGuiDockNodeFlags;      // -> enum ImGuiDockNodeFlags_   // Flags: для DockSpace()
typedef int ImGuiDragDropFlags;      // -> enum ImGuiDragDropFlags_   // Flags: для BeginDragDropSource(), AcceptDragDropPayload()
typedef int ImGuiFocusedFlags;       // -> enum ImGuiFocusedFlags_    // Flags: для IsWindowFocused()
typedef int ImGuiHoveredFlags;       // -> enum ImGuiHoveredFlags_    // Flags: для IsItemHovered(), IsWindowHovered() и т. д..
typedef int ImGuiInputTextFlags;     // -> enum ImGuiInputTextFlags_  // Flags: для InputText(), InputTextMultiline()
typedef int ImGuiModFlags;           // -> enum ImGuiModFlags_        // Flags: для io.KeyMods (Ctrl/Shift/Alt/Super)
typedef int ImGuiPopupFlags;         // -> enum ImGuiPopupFlags_      // Flags: для OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
typedef int ImGuiSelectableFlags;    // -> enum ImGuiSelectableFlags_ // Flags: для Selectable()
typedef int ImGuiSliderFlags;        // -> enum ImGuiSliderFlags_     // Flags: для DragFloat(), DragInt(), SliderFloat(), SliderInt() и т. д..
typedef int ImGuiTabBarFlags;        // -> enum ImGuiTabBarFlags_     // Flags: для BeginTabBar()
typedef int ImGuiTabItemFlags;       // -> enum ImGuiTabItemFlags_    // Flags: для BeginTabItem()
typedef int ImGuiTableFlags;         // -> enum ImGuiTableFlags_      // Flags: для BeginTable()
typedef int ImGuiTableColumnFlags;   // -> enum ImGuiTableColumnFlags_// Flags: для TableSetupColumn()
typedef int ImGuiTableRowFlags;      // -> enum ImGuiTableRowFlags_   // Flags: для TableNextRow()
typedef int ImGuiTreeNodeFlags;      // -> enum ImGuiTreeNodeFlags_   // Flags: для TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int ImGuiViewportFlags;      // -> enum ImGuiViewportFlags_   // Flags: для ImGuiViewport
typedef int ImGuiWindowFlags;        // -> enum ImGuiWindowFlags_     // Flags: для Begin(), BeginChild()

// ImTexture: пользовательские данные для серверной части средства визуализации для идентификации текстуры [тип, настраиваемый во время компиляции]
// - Чтобы использовать что-то еще, кроме непрозрачного указателя void*: переопределите, например, '#define ImTextureID MyTextureType*' в вашем файле imconfig.h.
// - Это может быть все, что вы хотите, чтобы это было! прочтите FAQ о ImTextureID для получения подробной информации.
#ifndef ImTextureID
typedef void* ImTextureID;   // Default: сохранить указатель или целочисленное значение в указателе (большинство бэкэндов рендеринга подходят для этого)
#endif

// ImDrawIdx: вершинный индекс. [Тип, настраиваемый во время компиляции]
// - Чтобы использовать 16-битные индексы + разрешить большие сетки: серверу необходимо установить 'io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset' и обрабатывать ImDrawCmd::VtxOffset (рекомендуемые).
// - Чтобы использовать 32-битные индексы: переопределите с помощью '#define ImDrawIdx unsigned int' в вашем файле imconfig.h.
#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;   // Default: 16-бит (для максимальной совместимости с бэкендами рендерера)
#endif

// Скалярные типы данных
typedef unsigned int       ImGuiID;   // Уникальный идентификатор, используемый виджетами (обычно результат хеширования стека строк).
typedef signed char        ImS8;      // 8-битное целое число со знаком
typedef unsigned char      ImU8;      // 8-битное целое число без знака
typedef signed short       ImS16;     // 16-битное целое число со знаком
typedef unsigned short     ImU16;     // 16-битное целое число без знака
typedef signed int         ImS32;     // 32-битное целое число со знаком == int
typedef unsigned int       ImU32;     // 32-битное целое число без знака (часто используется для хранения упакованных цветов)
typedef signed long long   ImS64;     // 64-битное целое число со знаком
typedef unsigned long long ImU64;     // 64-битное целое число без знака

// Character types
// (обычно мы используем строку в кодировке UTF-8 в API. Это хранилище специально для декодированного символа, используемого для ввода и отображения с клавиатуры)
typedef unsigned short     ImWchar16;   // Один декодированный U16 character/code point. Мы кодируем их как многобайтовые UTF-8 при использовании в строках.
typedef unsigned int       ImWchar32;   // Один декодированный U32 character/code point. Мы кодируем их как многобайтовые UTF-8 при использовании в строках.
#ifdef IMGUI_USE_WCHAR32                // ImWchar [настраиваемый тип: переопределить в imconfig.h с '#define IMGUI_USE_WCHAR32' для поддержки плоскостей Unicode 1-16]
typedef ImWchar32 ImWchar;
#else
typedef ImWchar16 ImWchar;
#endif

// Обратный вызов и типы функций
typedef int   (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);   // Функция обратного вызова для ImGui::InputText()
typedef void  (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);             // Функция обратного вызова для ImGui::SetNextWindowSizeConstraints()
typedef void* (*ImGuiMemAllocFunc)(size_t sz, void* user_data);              // Сигнатура функции для ImGui::SetAllocatorFunctions()
typedef void  (*ImGuiMemFreeFunc)(void* ptr, void* user_data);               // Сигнатура функции для ImGui::SetAllocatorFunctions()

// ImVec2: 2D-вектор, используемый для хранения позиций, размеров и т. д. [Тип, настраиваемый во время компиляции]
// Это часто используемый тип в API. Рассмотрите возможность использования IM_VEC2_CLASS_EXTRA для создания неявного приведения от/к нашему предпочтительному типу.
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImVec2
{
    float x, y;
    constexpr ImVec2(): x(0.0f), y(0.0f) {}
    constexpr ImVec2(float _x, float _y): x(_x), y(_y) {}
    float operator[](size_t idx) const
    {
        IM_ASSERT(idx <= 1);
        return (&x)[idx];
    }   // Мы очень редко используем этот оператор [], накладные расходы на утверждение в порядке.
    float& operator[](size_t idx)
    {
        IM_ASSERT(idx <= 1);
        return (&x)[idx];
    }   // Мы очень редко используем этот оператор [], накладные расходы на утверждение в порядке.
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA   // Определите дополнительные конструкторы и операторы неявного приведения в imconfig.h для преобразования между вашими математическими типами и ImVec2.
#endif
};

// ImVec4: 4D-вектор, используемый для хранения прямоугольников отсечения, цветов и т. д. [Тип, настраиваемый во время компиляции]
struct ImVec4
{
    float x, y, z, w;
    constexpr ImVec4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    constexpr ImVec4(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w) {}
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA   // Определите дополнительные конструкторы и операторы неявного приведения в imconfig.h для преобразования между вашими математическими типами и ImVec4.
#endif
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] Dear ImGui функции API конечного пользователя
// (Обратите внимание, что ImGui:: будучи пространством имен, вы можете добавить дополнительные ImGui:: functions в вашем собственном отдельном файле. Пожалуйста, не изменяйте исходные файлы imgui!)
//-----------------------------------------------------------------------------

namespace ImGui
{
    // Создание контекста и доступ
    // - Каждый контекст создает свой собственный ImFontAtlas по умолчанию. Вы можете сами создать экземпляр и передать его CreateContext() для совместного использования атласа шрифтов между контекстами.
    // - Пользователи DLL: кучи и глобальные переменные не распределяются между границами DLL! Вам нужно будет вызвать SetCurrentContext() + SetAllocatorFunctions()
    //   для каждой статической/DLL-границы, из которой вы звоните. Читать "Распределители контекста и памяти" раздел imgui.cpp для деталей.
    IMGUI_API ImGuiContext*         CreateContext(ImFontAtlas* shared_font_atlas = NULL);
    IMGUI_API void                  DestroyContext(ImGuiContext* ctx = NULL);   // NULL = уничтожить текущий контекст
    IMGUI_API ImGuiContext*         GetCurrentContext();
    IMGUI_API void                  SetCurrentContext(ImGuiContext* ctx);

    // Main
    IMGUI_API ImGuiIO&              GetIO();         // aполучить доступ к структуре ввода-вывода (входы мыши/клавиатуры/геймпада, время, различные параметры конфигурации/флаги)
    IMGUI_API ImGuiStyle&           GetStyle();      // получить доступ к структуре стиля (цвета, размеры). Всегда используйте PushStyleCol(), PushStyleVar() изменить стиль в середине кадра!
    IMGUI_API void                  NewFrame();      // начать все заново Dear ImGui кадра, вы можете подать любую команду с этого момента до тех пор, пока Render()/EndFrame().
    IMGUI_API void                  EndFrame();      // заканчивается Dear ImGui рамка. автоматически вызывается Render(). Если вам не нужно рендерить данные (пропуская рендеринг),
                                                     // вы можете вызвать EndFrame() без Render()... но вы уже потратили ЦП впустую! Если вам не нужен рендеринг,
                                                     // лучше не создавать никаких окон и вообще не вызывать NewFrame()!
    IMGUI_API void                  Render();        // заканчивается кадр Dear ImGui, завершить данные рисования. Затем вы можете получить вызов GetDrawData().
    IMGUI_API ImDrawData*           GetDrawData();   // действует после Render() и до следующего вызова NewFrame(). это то, что у тебя есть к render.

    // Демо, Отладка, Информация
    IMGUI_API void                  ShowDemoWindow(bool* p_open = NULL);        // создать демонстрационное окно. продемонстрировать большинство функций ImGui. позвоните, чтобы узнать о библиотеке!
                                                                                // постарайтесь сделать так, чтобы он всегда был доступен в вашем приложении!
    IMGUI_API void                  ShowMetricsWindow(bool* p_open = NULL);     // создать окно Метрики/Отладчика. display Внутри Dear ImGui: окна, команды рисования, различные внутренние состояния и т.д.
    IMGUI_API void                  ShowStackToolWindow(bool* p_open = NULL);   // создать окно Stack Tool. наведите указатель мыши на элементы, чтобы запросить информацию об источнике их уникального идентификатора.
    IMGUI_API void                  ShowAboutWindow(bool* p_open = NULL);       // создать окно «О программе». display версии Dear ImGui, кредиты и информация о сборке/системе.
    IMGUI_API void                  ShowStyleEditor(ImGuiStyle* ref = NULL);    // добавить блок редактора стилей (не окно). вы можете передать эталонную структуру ImGuiStyle для сравнения,
                                                                                // возврата и сохранения (иначе он использует стиль по умолчанию)
    IMGUI_API bool                  ShowStyleSelector(const char* label);       // добавить блок селектора стиля (не окно), по сути, комбинацию, в которой перечислены стили по умолчанию.
    IMGUI_API void                  ShowFontSelector(const char* label);        // добавить блок выбора шрифта (не окно), по сути, комбинацию, в которой перечислены загруженные шрифты.
    IMGUI_API void                  ShowUserGuide();                            // добавить базовый блок справки/информации (не окно): как манипулировать ImGui в качестве конечного пользователя (управление мышью/клавиатурой).
    IMGUI_API const char*           GetVersion();                               // получить скомпилированную строку версии e. г. «1.80 WIP» (по сути, значение IMGUI_VERSION из скомпилированной версии imgui. cpp)

    // Стили
    IMGUI_API void                  StyleColorsDark(ImGuiStyle* dst = NULL);      // новый рекомендуемый стиль (по умолчанию)
    IMGUI_API void                  StyleColorsLight(ImGuiStyle* dst = NULL);     // лучше всего использовать с рамками и пользовательским, более толстым шрифтом
    IMGUI_API void                  StyleColorsClassic(ImGuiStyle* dst = NULL);   // классический стиль имгуи

    // Окна
    // - Begin() = поместить окно в стек и начать добавление к нему. End() = извлечь окно из стека.
    // - Передача 'bool* p_open != NULL' показывает виджет закрытия окна в правом верхнем углу окна,
    // какой щелчок установит логическое значение в false при нажатии.
    // - Вы можете добавить несколько раз к одному и тому же окну в одном и том же кадре, вызвав пары Begin()/End() несколько раз.
    // Некоторая информация, такая как 'flags' или 'p_open', будет учитываться только при первом вызове Begin().
    // - Begin() возвращает false, чтобы указать, что окно свернуто или полностью обрезано, поэтому вы можете выйти раньше и пропустить отправку
    // что-нибудь в окно. Всегда вызывайте соответствующий End() для каждого вызова Begin(), независимо от возвращаемого значения!
    // [Важно: из-за устаревших причин это несовместимо с большинством других функций, таких как BeginMenu/EndMenu,
    // BeginPopup/EndPopup и т. д., где вызов EndXXX должен вызываться только в том случае, если соответствующая функция BeginXXX
    // вернули истину. Begin и BeginChild — единственные нечетные. Будет исправлено в будущем обновлении.]
    // - Обратите внимание, что нижняя часть стека окон всегда содержит окно с именем "Отладка".
    IMGUI_API bool                  Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
    IMGUI_API void                  End();

    // Дочерние окна
    // - Используйте дочерние окна, чтобы начать с автономных независимых областей прокрутки/отсечения в главном окне. Дочерние окна могут встраивать своих дочерних элементов.
    // - Для каждой независимой оси 'size': ==0.0f: использовать оставшийся размер окна хоста / >0.0f: фиксированный размер / <0.0f: использовать оставшийся размер окна минус абс (размер) / Каждая ось может использовать разные режим, т. г. ИмВек2(0,400).
    // - BeginChild() возвращает false, чтобы указать, что окно свернуто или полностью обрезано, так что вы можете раньше выйти и пропустить отправку чего-либо в окно.
    // Всегда вызывайте соответствующий EndChild() для каждого вызова BeginChild(), независимо от возвращаемого значения.
    // [Важно: из-за устаревших причин это несовместимо с большинством других функций, таких как BeginMenu/EndMenu,
    // BeginPopup/EndPopup и т. д., где вызов EndXXX должен вызываться только в том случае, если соответствующая функция BeginXXX
    // вернули истину. Begin и BeginChild — единственные нечетные. Будет исправлено в будущем обновлении.]
    IMGUI_API bool                  BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API bool                  BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API void                  EndChild();

    // Утилиты Windows
    // - 'текущее окно' = окно, в которое мы добавляем, находясь внутри блока Begin()/End(). 'следующее окно' = следующее окно, в которое мы начнем Begin().
    IMGUI_API bool                  IsWindowAppearing();
    IMGUI_API bool                  IsWindowCollapsed();
    IMGUI_API bool                  IsWindowFocused(ImGuiFocusedFlags flags = 0);   // текущее окно сфокусировано? или его корень/потомок, в зависимости от флагов. см. флаги для опций.
    IMGUI_API bool                  IsWindowHovered(ImGuiHoveredFlags flags = 0);   // зависает ли текущее окно (и обычно: не блокируется всплывающим/модальным окном)? см. флаги для опций.
                                                                                    // NB: если вы пытаетесь проверить, должна ли ваша мышь быть отправлена ​​​​в imgui или в ваше приложение,
                                                                                    // вы должны использовать логическое значение для этого 'io.WantCaptureMouse'! Пожалуйста, прочтите FAQ!
    IMGUI_API ImDrawList*           GetWindowDrawList();                            // получить список рисования, связанный с текущим окном, чтобы добавить свои собственные примитивы рисования
    IMGUI_API float                 GetWindowDpiScale();                            // получить масштаб DPI, в настоящее время связанный с окном просмотра текущего окна.
    IMGUI_API ImVec2                GetWindowPos();                                 // получить текущую позицию окна в пространстве экрана (полезно, если вы хотите сделать свой собственный рисунок через API DrawList)
    IMGUI_API ImVec2                GetWindowSize();                                // получить текущий размер окна
    IMGUI_API float                 GetWindowWidth();                               // получить текущую ширину окна (ярлык для GetWindowSize().x)
    IMGUI_API float                 GetWindowHeight();                              // получить текущую высоту окна (ярлык для GetWindowSize().y)
    IMGUI_API ImGuiViewport*        GetWindowViewport();                            // получить область просмотра, связанную с текущим окном.

    // Манипуляции с окнами
    // - Лучше использовать функции SetNextXXX (до начала), а не функции SetXXX (после начала).
    IMGUI_API void                  SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));                                                                 // установить следующую позицию окна. вызов перед началом().
                                                                                                                                                                                                 // используйте pivot=(0.5f,0.5f) для центрирования в заданной точке и т. д.
    IMGUI_API void                  SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);                                                                                                   // установить следующий размер окна. установите для оси значение 0.0f,
                                                                                                                                                                                                 // чтобы принудительно выполнить автоподгонку по этой оси. вызов перед началом()
    IMGUI_API void                  SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL);   // установить следующие пределы размера окна. используйте -1,-1 по любой из осей X/Y, чтобы сохранить текущий размер.
                                                                                                                                                                                                 // Размеры будут округлены в меньшую сторону.
                                                                                                                                                                                                 // Используйте обратный вызов для применения нетривиальных программных ограничений.
    IMGUI_API void                  SetNextWindowContentSize(const ImVec2& size);                                                                                                                // установить размер содержимого следующего окна (~ прокручиваемая клиентская область, которая обеспечивает диапазон полос прокрутки).
                                                                                                                                                                                                 // Не включая украшения окна (строка заголовка, строка меню и т. д.) и WindowPadding.
                                                                                                                                                                                                 // установите ось на 0.0f, чтобы оставить ее автоматической. вызов перед началом()
    IMGUI_API void                  SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                                                                                                  // установить следующее окно в свернутом состоянии. вызов перед началом()
    IMGUI_API void                  SetNextWindowFocus();                                                                                                                                        // установить следующее окно, чтобы быть сфокусированным / самым верхним. вызов перед началом()
    IMGUI_API void                  SetNextWindowBgAlpha(float alpha);                                                                                                                           // установить альфа-цвет фона следующего окна. помощник, чтобы легко переопределить альфа-компонент ImGuiCol_WindowBg/ChildBg/PopupBg.
                                                                                                                                                                                                 // вы также можете использовать ImGuiWindowFlags_NoBackground.
    IMGUI_API void                  SetNextWindowViewport(ImGuiID viewport_id);                                                                                                                  // установить окно просмотра следующего окна
    IMGUI_API void                  SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);                                                                                                         // ((not recommended) set current window position - call within Begin()/End().)не рекомендуется)
                                                                                                                                                                                                 // установить текущую позицию окна - вызов внутри Begin()/End().
                                                                                                                                                                                                 // предпочтительнее использовать SetNextWindowPos(), так как это может привести к разрывам и побочным эффектам.
    IMGUI_API void                  SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);                                                                                                       // (не рекомендуется) установить текущий размер окна - вызов внутри Begin()/End().
                                                                                                                                                                                                 // установите ImVec2 (0, 0), чтобы принудительно выполнить автоподгонку.
                                                                                                                                                                                                 // предпочтительнее использовать SetNextWindowSize(), так как это может привести к разрывам и незначительным побочным эффектам.
    IMGUI_API void                  SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                                                                                                      // (не рекомендуется) установить текущее свернутое состояние окна. предпочитаю использовать SetNextWindowCollapsed().
    IMGUI_API void                  SetWindowFocus();                                                                                                                                            // (не рекомендуется) сделать текущее окно сфокусированным / самым верхним. предпочитаю использовать SetNextWindowFocus().
    IMGUI_API void                  SetWindowFontScale(float scale);                                                                                                                             // [УСТАРЕЛО] задать масштаб шрифта. Отрегулируйте ввод-вывод. FontGlobalScale, если вы хотите масштабировать все окна. Это старый API!
                                                                                                                                                                                                 // Для правильного масштабирования лучше перезагрузить шрифт + пересобрать ImFontAtlas + вызов style.ScaleAllSizes().
    IMGUI_API void                  SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);                                                                                       // установить именованное положение окна.
    IMGUI_API void                  SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);                                                                                     // установить именованный размер окна. установите для оси значение 0.0f,
                                                                                                                                                                                                 // чтобы принудительно выполнить автоподгонку по этой оси.
    IMGUI_API void                  SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);                                                                                    // установить именованное окно в свернутом состоянии
    IMGUI_API void                  SetWindowFocus(const char* name);                                                                                                                            // установить именованное окно в фокус / самое верхнее. используйте NULL для удаления фокуса.

    // Контентная область
    // - Получить доступное пространство из заданной точки. GetContentRegionAvail() часто бывает полезен.
    // - Эти функции должны быть переработаны (они сбивают с толку, неполны, а возвращаемые значения Min/Max находятся в локальных координатах окна, что увеличивает путаницу)
    IMGUI_API ImVec2                GetContentRegionAvail();       // == GetContentRegionMax() - GetCursorPos()
    IMGUI_API ImVec2                GetContentRegionMax();         // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
    IMGUI_API ImVec2                GetWindowContentRegionMin();   // content boundaries min for the full window (roughly (0,0)-Scroll), in window coordinates
    IMGUI_API ImVec2                GetWindowContentRegionMax();   // content boundaries max for the full window (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates

    // Windows Scrolling
    IMGUI_API float                 GetScrollX();                                                    // получить количество прокрутки [0 .. GetScrollMaxX()]
    IMGUI_API float                 GetScrollY();                                                    // получить количество прокрутки [0 .. GetScrollMaxY()]
    IMGUI_API void                  SetScrollX(float scroll_x);                                      // установить количество прокрутки [0 .. GetScrollMaxX()]
    IMGUI_API void                  SetScrollY(float scroll_y);                                      // установить количество прокрутки [0 .. GetScrollMaxY()]
    IMGUI_API float                 GetScrollMaxX();                                                 // получить максимальное количество прокрутки ~~ ContentSize.x - WindowSize.x - DecorationsSize.x
    IMGUI_API float                 GetScrollMaxY();                                                 // получить максимальное количество прокрутки ~~ ContentSize.y - WindowSize.y - DecorationsSize.y
    IMGUI_API void                  SetScrollHereX(float center_x_ratio = 0.5f);                     // отрегулируйте количество прокрутки, чтобы сделать текущую позицию курсора видимой.
                                                                                                     // center_x_ratio=0.0: левый, 0.5: центр, 1.0: право. При использовании, чтобы сделать "элемент по умолчанию/текущий элемент" видимым,
                                                                                                     // рассмотрите вместо этого использование SetItemDefaultFocus().
    IMGUI_API void                  SetScrollHereY(float center_y_ratio = 0.5f);                     // отрегулируйте количество прокрутки, чтобы сделать текущую позицию курсора видимой. center_y_ratio=0.0: верхний, 0.5: центр, 1.0: нижний.
                                                                                                     // При использовании, чтобы сделать "элемент по умолчанию/текущий элемент" видимым, рассмотрите вместо этого использование SetItemDefaultFocus().
    IMGUI_API void                  SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);   // отрегулируйте количество прокрутки, чтобы сделать данную позицию видимой.
                                                                                                     // Обычно GetCursorStartPos() + смещение для вычисления действительной позиции.
    IMGUI_API void                  SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);   // отрегулируйте количество прокрутки, чтобы сделать данную позицию видимой.
                                                                                                     // Обычно GetCursorStartPos() + смещение для вычисления действительной позиции.

    // Стеки параметров (общие)
    IMGUI_API void                  PushFont(ImFont* font);   // используйте NULL в качестве ярлыка для нажатия шрифта по умолчанию
    IMGUI_API void                  PopFont();
    IMGUI_API void                  PushStyleColor(ImGuiCol idx, ImU32 col);   // изменить цвет стиля. всегда используйте это, если вы изменяете стиль после NewFrame().
    IMGUI_API void                  PushStyleColor(ImGuiCol idx, const ImVec4& col);
    IMGUI_API void                  PopStyleColor(int count = 1);
    IMGUI_API void                  PushStyleVar(ImGuiStyleVar idx, float val);           //  изменить плавающую переменную стиля. всегда используйте это, если вы изменяете стиль после NewFrame().
    IMGUI_API void                  PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);   // изменить переменную стиля ImVec2. всегда используйте это, если вы изменяете стиль после NewFrame().
    IMGUI_API void                  PopStyleVar(int count = 1);
    IMGUI_API void                  PushAllowKeyboardFocus(bool allow_keyboard_focus);   // == включение остановки табуляции. Разрешить фокусировку с помощью TAB/Shift-TAB, включено по умолчанию,
                                                                                         // но вы можете отключить его для определенных виджетов
    IMGUI_API void                  PopAllowKeyboardFocus();
    IMGUI_API void                  PushButtonRepeat(bool repeat);   // в режиме повтора функции Button*() возвращают повторенное значение true в типизированном виде
                                                                     // (используя настройку io. KeyRepeatDelay/io. KeyRepeatRate). Обратите внимание,
                                                                     // что вы можете вызвать IsItemActive() после любой Button(), чтобы узнать, удерживается ли кнопка в текущем кадре.
    IMGUI_API void                  PopButtonRepeat();

    // Стеки параметров (текущее окно)
    IMGUI_API void                  PushItemWidth(float item_width);   // нажмите ширину элементов для обычных больших виджетов «элемент + метка». >0.0f: ширина в пикселях,
                                                                       // <0.0f выравнивает xx пикселей справа от окна (поэтому -FLT_MIN всегда выравнивает ширину по правому краю).
    IMGUI_API void                  PopItemWidth();
    IMGUI_API void                  SetNextItemWidth(float item_width);               // установить ширину _next_ общего большого виджета "item+label". >0.0f: ширина в пикселях,
                                                                                      // <0.0f выравнивает xx пикселей справа от окна (поэтому -FLT_MIN всегда выравнивает ширину по правому краю)
    IMGUI_API float                 CalcItemWidth();                                  // ширина элемента с заданными настройками и текущей позицией курсора.
                                                                                      // НЕ обязательно ширина последнего элемента, в отличие от большинства функций «Элемент».
    IMGUI_API void                  PushTextWrapPos(float wrap_local_pos_x = 0.0f);   // нажать позицию переноса слов для команд Text*(). < 0,0f: без упаковки; 0.0f: перенос до конца окна (или столбца);
                                                                                      // > 0.0f: обернуть позицию 'wrap_pos_x' в локальном пространстве окна
    IMGUI_API void                  PopTextWrapPos();

    // Стиль доступа для чтения
    // - Используйте редактор стилей (функция ShowStyleEditor()), чтобы в интерактивном режиме увидеть, какие цвета)
    IMGUI_API ImFont*               GetFont();                                           // получить текущий шрифт
    IMGUI_API float                 GetFontSize();                                       // получить текущий размер шрифта (= высота в пикселях) текущего шрифта с примененным текущим масштабом
    IMGUI_API ImVec2                GetFontTexUvWhitePixel();                            // получить координату UV для пикселя, полезного для рисования пользовательских фигур через ImDrawList API
    IMGUI_API ImU32                 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);   // получить заданный цвет стиля с примененным альфа-каналом стиля и необязательным дополнительным альфа-множителем,
                                                                                         // упакованным как 32-битное значение, подходящее для ImDrawList
    IMGUI_API ImU32                 GetColorU32(const ImVec4& col);                      // получить заданный цвет с примененным альфа-стилем, упакованным как 32-битное значение, подходящее для ImDrawList
    IMGUI_API ImU32                 GetColorU32(ImU32 col);                              // получить заданный цвет с примененным альфа-стилем, упакованным как 32-битное значение, подходящее для ImDrawList
    IMGUI_API const ImVec4&         GetStyleColorVec4(ImGuiCol idx);                     // получить цвет стиля, хранящийся в структуре ImGuiStyle. используйте для обратной связи с PushStyleColor(),
                                                                                         // в противном случае используйте GetColorU32() для получения цвета стиля с запеченным альфа-каналом стиля.

    // Курсор / Макет
    // - Под «курсором» мы подразумеваем текущую позицию вывода.
    // - Типичное поведение виджета состоит в том, чтобы вывести себя в текущей позиции курсора, а затем переместить курсор на одну строку вниз.
    // - Вы можете вызвать SameLine() между виджетами, чтобы отменить последний возврат каретки и вывести справа от предыдущего виджета.
    // - Внимание! В настоящее время у нас есть несоответствия между локальными и абсолютными позициями окна, которые мы постараемся исправить с помощью будущего API:
    // Локальные координаты окна: SameLine(), GetCursorPos(), SetCursorPos(), GetCursorStartPos(), GetContentRegionMax(), GetWindowContentRegion*(), PushTextWrapPos()
    // Абсолютная координата: GetCursorScreenPos(), SetCursorScreenPos(), все функции ImDrawList::.
    IMGUI_API void                  Separator();                                                         // сепаратор, как правило, горизонтальный. внутри строки меню или в режиме горизонтальной компоновки он становится вертикальным разделителем.
    IMGUI_API void                  SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);   // вызов между виджетами или группами, чтобы расположить их по горизонтали. Положение X указано в координатах окна.
    IMGUI_API void                  NewLine();                                                           // отменить SameLine() или принудительно ввести новую строку в контексте горизонтального макета.
    IMGUI_API void                  Spacing();                                                           // добавить интервал по вертикали.
    IMGUI_API void                  Dummy(const ImVec2& size);                                           // добавить фиктивный элемент заданного размера. в отличие от InvisibleButton(),
                                                                                                         // Dummy() не будет принимать щелчок мыши или перемещаться по нему.
    IMGUI_API void                  Indent(float indent_w = 0.0f);                                       // переместить позицию содержимого вправо, по indent_w или стилю. IndentSpacing, если indent_w <= 0
    IMGUI_API void                  Unindent(float indent_w = 0.0f);                                     // переместить позицию содержимого обратно влево, с помощью indent_w или стиля. IndentSpacing, если indent_w <= 0
    IMGUI_API void                  BeginGroup();                                                        // зафиксировать горизонтальное исходное положение
    IMGUI_API void                  EndGroup();                                                          // разблокировать начальную позицию по горизонтали + захватить всю ограничивающую рамку группы в один «элемент»
                                                                                                         // (так что вы можете использовать IsItemHovered() или примитивы макета, такие как SameLine() для всей группы и т. д.)
    IMGUI_API ImVec2                GetCursorPos();                                                      // положение курсора в координатах окна (относительно положения окна)
    IMGUI_API float                 GetCursorPosX();                                                     // (некоторые функции используют относительные координаты окна, например:
                                                                                                         // GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* и т. д..
    IMGUI_API float                 GetCursorPosY();                                                     //    другие функции, такие как GetCursorScreenPos или все в ImDrawList::
    IMGUI_API void                  SetCursorPos(const ImVec2& local_pos);                               //    используют основную, абсолютную систему координат.
    IMGUI_API void                  SetCursorPosX(float local_x);                                        //    GetWindowPos() + GetCursorPos() == GetCursorScreenPos() и т. д..)
    IMGUI_API void                  SetCursorPosY(float local_y);                                        //
    IMGUI_API ImVec2                GetCursorStartPos();                                                 // начальная позиция курсора в координатах окна
    IMGUI_API ImVec2                GetCursorScreenPos();                                                // положение курсора в абсолютных координатах (полезно для работы с ImDrawList API).
                                                                                                         // обычно верхний левый == GetMainViewport()->Pos == (0,0) в режиме одного окна просмотра
                                                                                                         // и нижний правый == GetMainViewport()->Pos+Size == io. DisplaySize в режиме одного окна просмотра.
    IMGUI_API void                  SetCursorScreenPos(const ImVec2& pos);                               // положение курсора в абсолютных координатах
    IMGUI_API void                  AlignTextToFramePadding();                                           // вертикально выровнять будущую текстовую базовую линию по FramePadding. y, чтобы он правильно выравнивался
                                                                                                         // по элементам с регулярной рамкой (вызовите, если у вас есть текст в строке перед элементом с рамкой)
    IMGUI_API float                 GetTextLineHeight();                                                 // ~ FontSize
    IMGUI_API float                 GetTextLineHeightWithSpacing();                                      // ~ FontSize + style.ItemSpacing.y (расстояние в пикселях между двумя последовательными строками текста)
    IMGUI_API float                 GetFrameHeight();                                                    // ~ FontSize + style.FramePadding.y * 2
    IMGUI_API float                 GetFrameHeightWithSpacing();                                         // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y
                                                                                                         // (расстояние в пикселях между двумя последовательными строками виджетов в рамке)

    // ID stack/scopes
    // Прочтите FAQ (docs/FAQ.md или http://dearimgui.org/faq) для получения более подробной информации о том, как ID обрабатывается в dear imgui.
    // - Ответы на эти вопросы и на них влияет понимание системы стека ID:
    //   - "Q: Почему мой виджет не реагирует, когда я нажимаю на него?"
    //   - "Q: Как я могу иметь виджеты с пустой меткой?"
    //   - "Q: Как я могу иметь несколько виджетов с одним и тем же ярлыком?»
    // - Укороченная версия: ID это хэши всего стека ID. Если вы создаете виджеты в цикле, вы, скорее всего,
    //   хотите ввести уникальный идентификатор (например, указатель объекта, индекс цикла), чтобы однозначно различать их.
    // - Вы также можете использовать синтаксис «Label##foobar» в метке виджета, чтобы отличать их друг от друга.
    // - В этом заголовочном файле мы используем терминологию «метка»/«имя» для обозначения строки, которая будет отображаться + использоваться в качестве идентификатора,
    //   тогда как «str_id» обозначает строку, которая используется только как ID и обычно не отображается.
    IMGUI_API void                  PushID(const char* str_id);                                 // поместите строку в стек ID (будет хеш-строка).
    IMGUI_API void                  PushID(const char* str_id_begin, const char* str_id_end);   // поместите строку в стек ID (будет хеш-строка).
    IMGUI_API void                  PushID(const void* ptr_id);                                 // поместите указатель в стек ID (будет хэш-указатель).
    IMGUI_API void                  PushID(int int_id);                                         // поместите целое число в стек ID (будет хешировать целое число).
    IMGUI_API void                  PopID();                                                    // вытолкнуть из стека ID.
    IMGUI_API ImGuiID               GetID(const char* str_id);                                  // вычислить уникальный ID (хэш всего стека ID + заданный параметр). е. г. если вы хотите сделать запрос в ImGuiStorage самостоятельно
    IMGUI_API ImGuiID               GetID(const char* str_id_begin, const char* str_id_end);
    IMGUI_API ImGuiID               GetID(const void* ptr_id);

    // Widgets: Text
    IMGUI_API void                  TextUnformatted(const char* text, const char* text_end = NULL);   // необработанный текст без форматирования. Приблизительно эквивалентен Text("%s", text), но:
                                                                                                      // A) не требует строки с завершающим нулем, если указано 'text_end',
                                                                                                      // B) это быстрее, не выполняется копирование памяти, нет ограничений на размер буфера, рекомендуется для длинных фрагментов текста.
    IMGUI_API void                  Text(const char* fmt, ...) IM_FMTARGS(1);                         // форматированный текст
    IMGUI_API void                  TextV(const char* fmt, va_list args) IM_FMTLIST(1);
    IMGUI_API void                  TextColored(const ImVec4& col, const char* fmt, ...) IM_FMTARGS(2);   // ярлык для PushStyleColor(ImGuiCol_Text, col); Текст(fmt, ...); PopStyleColor();
    IMGUI_API void                  TextColoredV(const ImVec4& col, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API void                  TextDisabled(const char* fmt, ...) IM_FMTARGS(1);   // ярлык для PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Текст(fmt, ...); PopStyleColor();
    IMGUI_API void                  TextDisabledV(const char* fmt, va_list args) IM_FMTLIST(1);
    IMGUI_API void                  TextWrapped(const char* fmt, ...) IM_FMTARGS(1);   // ярлык для PushTextWrapPos(0.0f); Текст(fmt, ...); PopTextWrapPos();. Обратите внимание,
                                                                                       // что это не будет работать в окне с автоматическим изменением размера, если нет других виджетов
                                                                                       // для увеличения ширины окна, может потребоваться установить размер с помощью SetNextWindowSize().
    IMGUI_API void                  TextWrappedV(const char* fmt, va_list args) IM_FMTLIST(1);
    IMGUI_API void                  LabelText(const char* label, const char* fmt, ...) IM_FMTARGS(2);   // отображать текст + метка, выровненные так же, как виджеты значение + метка
    IMGUI_API void                  LabelTextV(const char* label, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API void                  BulletText(const char* fmt, ...) IM_FMTARGS(1);   // ярлык для Bullet()+Text()
    IMGUI_API void                  BulletTextV(const char* fmt, va_list args) IM_FMTLIST(1);

    IMGUI_API void                  BulletTextColored(const ImVec4& col, const char* fmt, ...) IM_FMTARGS(1);   // ярлык для Bullet()+Text()
    IMGUI_API void                  BulletTextColoredV(const ImVec4& col, const char* fmt, va_list args) IM_FMTLIST(1);

    // Виджеты: Главное
    // - Большинство виджетов возвращают true при изменении значения или при нажатии/выборе.
    // - Вы также можете использовать одну из многих функций IsItemXXX (например, IsItemActive, IsItemHovered и т. д.) для запроса состояния виджета.
    IMGUI_API bool                  Button(const char* label, const ImVec2& size = ImVec2(0, 0));                          // кнопка
    IMGUI_API bool                  SmallButton(const char* label);                                                        // кнопка с FramePadding=(0,0) для легкого встраивания в текст
    IMGUI_API bool                  InvisibleButton(const char* str_id, const ImVec2& size, ImGuiButtonFlags flags = 0);   // гибкое поведение кнопки без визуальных элементов, часто полезное для создания настраиваемого
                                                                                                                           // поведения с использованием общедоступного API (наряду с IsItemActive, IsItemHovered и т. д.)
    IMGUI_API bool                  ArrowButton(const char* str_id, ImGuiDir dir);                                         // квадратная кнопка в форме стрелки
    IMGUI_API void                  Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
    IMGUI_API bool                  ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));   // <<0 frame_padding uses default frame padding settings. 0 for no padding>0 frame_padding использует параметры заполнения кадра по умолчанию. 0 без заполнения
    IMGUI_API bool                  Checkbox(const char* label, bool* v);
    IMGUI_API bool                  CheckboxFlags(const char* label, int* flags, int flags_value);
    IMGUI_API bool                  CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    IMGUI_API bool                  RadioButton(const char* label, bool active);            // использовать с эл. г. если (RadioButton("один", my_value==1)) { my_value = 1; }
    IMGUI_API bool                  RadioButton(const char* label, int* v, int v_button);   // ярлык для обработки вышеуказанного шаблона, когда значение является целым числом
    IMGUI_API void                  ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-FLT_MIN, 0), const char* overlay = NULL);
    IMGUI_API void                  Bullet();   // нарисуйте маленький круг + держите курсор на той же линии. переместите курсор на позицию x с помощью GetTreeNodeToLabelSpacing(), на то же расстояние, что и TreeNode()

    // Виджеты: Поле со списком
    // - API BeginCombo()/EndCombo() позволяет вам управлять своим содержимым и состоянием выбора так, как вы этого хотите, путем создания e. г. Выбираемые() элементы.
    // - Старый API Combo() является помощником по сравнению с BeginCombo()/EndCombo(), которые доступны для удобства. Это аналогично тому, как создаются ListBox.
    IMGUI_API bool                  BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API void                  EndCombo();   // Вызов EndCombo() только в том случае, если BeginCombo() возвращает значение true!
    IMGUI_API bool                  Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool                  Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);   // Разделяйте элементы с помощью \0 в строке, заканчивайте список элементов
                                                                                                                                                             // с помощью \0\0. е. г. "Один\0Два\0Три\0"
    IMGUI_API bool                  Combo(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    // Виджеты: перетаскивание ползунков
    // - CTRL+щелчок на любом поле перетаскивания, чтобы превратить их в поле ввода. Значения, введенные вручную, по умолчанию не фиксируются и могут выйти за пределы допустимого диапазона.
    // Используйте ImGuiSliderFlags_AlwaysClamp, чтобы всегда зажимать.
    // - Для всех версий Float2/Float3/Float4/Int2/Int3/Int4 каждой функции обратите внимание, что аргумент функции 'float v[X]' такой же, как 'float* v',
    // синтаксис массива — это просто способ задокументировать количество элементов, которые должны быть доступны. Вы можете передать адрес вашего первого элемента из непрерывного набора, например. г. &мой вектор. Икс
    // - Отрегулируйте строку формата, чтобы украсить значение префиксом, суффиксом или адаптировать редактирование и точность отображения e. г. "%.3f" -> 1,234; "%5.2f сек" -> 01,23 сек; "Печенье: %.0f" -> Печенье: 1; и т. д.
    // - Строка формата также может быть установлена ​​в NULL или использовать формат по умолчанию ("%f" или "%d").
    // - Скорость указана на пиксель движения мыши (v_speed=0.2f: мышь должна переместиться на 5 пикселей, чтобы увеличить значение на 1).
    // Для навигации с помощью геймпада/клавиатуры минимальная скорость равна Max(v_speed, Minimum_step_at_given_precision).
    // - Используйте v_min < v_max, чтобы зафиксировать изменения в заданных пределах. Обратите внимание, что ручной ввод CTRL+щелчок может отменить эти ограничения, если ImGuiSliderFlags_AlwaysClamp не используется.
    // - Используйте v_max = FLT_MAX / INT_MAX и т. д., чтобы избежать ограничения до максимума, то же самое с v_min = -FLT_MAX / INT_MIN, чтобы избежать ограничения до минимума.
    // - Мы используем одни и те же наборы флагов для функций DragXXX() и SliderXXX(), так как функции одинаковы и их проще поменять местами.
    // - Наследие: до версии 1.78 существуют сигнатуры функций DragXXX(), которые принимают окончательный аргумент "float power=1.0f" вместо аргумента "ImGuiSliderFlags flags=0".
    //   Если вы получаете предупреждение о преобразовании float в ImGuiSliderFlags, прочтите https://github.com/ocornut/imgui/issues/3361
    IMGUI_API bool                  DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);   // If v_min >= v_max у нас нет границ
    IMGUI_API bool                  DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);   // If v_min >= v_max у нас нет границ
    IMGUI_API bool                  DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);

    // Виджеты: обычные слайдеры
    // - CTRL+щелчок на любом ползунке, чтобы превратить их в поле ввода. Значения, введенные вручную, по умолчанию не фиксируются и могут выйти за пределы допустимого диапазона.
    // Используйте ImGuiSliderFlags_AlwaysClamp, чтобы всегда зажимать.
    // - Отрегулируйте строку формата, чтобы украсить значение префиксом, суффиксом или адаптировать редактирование и точность отображения e. г. "%.3f" -> 1,234; "%5.2f сек" -> 01,23 сек; "Печенье: %.0f" -> Печенье: 1; и т. д.
    // - Строка формата также может быть установлена ​​в NULL или использовать формат по умолчанию ("%f" или "%d").
    // - Наследие: до версии 1.78 существуют сигнатуры функций SliderXXX(), которые принимают конечный аргумент `float power=1.0f' вместо аргумента `ImGuiSliderFlags flags=0'.
    //   Если вы получаете предупреждение о преобразовании float в ImGuiSliderFlags, прочтите https://github.com/ocornut/imgui/issues/3361
    IMGUI_API bool                  SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);   // отрегулируйте формат, чтобы украсить значение префиксом или
                                                                                                                                                                   // суффиксом для меток в ползунке или отображения единиц измерения.
    IMGUI_API bool                  SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool                  VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);

    // Виджеты: ввод с клавиатуры
    // - Если вы хотите использовать InputText() с std::string или любым другим типом динамической строки, см. misc/cpp/imgui_stdlib. h и комментарии в imgui_demo. cpp.
    // - Большинство флагов ImGuiInputTextFlags полезны только для InputText(), а не для InputFloatX, InputIntX, InputDouble и т. д.
    IMGUI_API bool                  InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool                  InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool                  InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool                  InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool                  InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);

    // Виджеты: Редактор/выбор цвета (совет: функции ColorEdit* имеют небольшой цветной квадрат, по которому можно щелкнуть левой кнопкой мыши,
    // чтобы открыть средство выбора, и щелкнуть правой кнопкой мыши, чтобы открыть меню параметров).
    // - Обратите внимание, что в C++ аргумент функции 'float v[X]' является _то же_, что и 'float* v', синтаксис массива - это просто способ документировать количество элементов,
    // которые, как ожидается, будут доступны.
    // - Вы можете передать адрес первого элемента float вне непрерывной структуры, т.е. г. &мой вектор. Икс
    IMGUI_API bool                  ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool                  ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool                  ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool                  ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool                  ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));   // отображать цветной квадрат/кнопку, наводить курсор для получения
                                                                                                                                                             // подробной информации, возвращать true при нажатии.
    IMGUI_API void                  SetColorEditOptions(ImGuiColorEditFlags flags);                                                                          // инициализируйте текущие параметры (обычно при запуске приложения), если вы хотите выбрать формат по умолчанию,
                                                                                                                                                             // тип средства выбора и т. д. Пользователь сможет изменить многие параметры, если вы не передадите флаг _NoOptions своим вызовам.

    // Виджеты: Деревья
    // - Функции TreeNode возвращают true, когда узел открыт, и в этом случае вам также нужно вызвать TreePop(), когда вы закончите отображение содержимого узла дерева.
    IMGUI_API bool                  TreeNode(const char* label);
    IMGUI_API bool                  TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // вспомогательный вариант, чтобы легко декорировать идентификатор из отображаемой строки.
                                                                                                        // Прочитайте FAQ о том, почему и как использовать ID. чтобы выровнять произвольный текст на том же уровне,
                                                                                                        // что и TreeNode(), вы можете использовать Bullet().
    IMGUI_API bool                  TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   // "
    IMGUI_API bool                  TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool                  TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool                  TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
    IMGUI_API bool                  TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool                  TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool                  TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API bool                  TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API void                  TreePush(const char* str_id);                                                         // ~ Indent()+PushId(). Уже вызывается TreeNode() при возврате true, но при желании вы можете вызвать TreePush/TreePop самостоятельно.
    IMGUI_API void                  TreePush(const void* ptr_id = NULL);                                                  // "
    IMGUI_API void                  TreePop();                                                                            // ~ Unindent()+PopId()
    IMGUI_API float                 GetTreeNodeToLabelSpacing();                                                          // горизонтальное расстояние до метки при использовании TreeNode*() или Bullet() == (g. FontSize + style. FramePadding. x*2)
                                                                                                                          // для обычного узла TreeNode без рамки
    IMGUI_API bool                  CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);                    // если возвращается «true», заголовок открыт. не отступает и не нажимает на стек идентификаторов.
                                                                                                                          // пользователю не нужно вызывать TreePop().
    IMGUI_API bool                  CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0);   // когда 'p_visible != NULL': если '*p_visible==true' отображать дополнительную маленькую
                                                                                                                          // кнопку закрытия в правом верхнем углу заголовка, которая при нажатии устанавливает
                                                                                                                          // логическое значение false, если '*p_visible==false' не отображается заголовок.
    IMGUI_API void                  SetNextItemOpen(bool is_open, ImGuiCond cond = 0);                                    // установить следующее открытое состояние TreeNode/CollapsingHeader.

    // Виджеты: Selectables
    // - Selectable выделяется при наведении курсора и может отображать другой цвет при выборе.
    // - Выбираемые соседи расширяют границы подсветки, чтобы между ними не оставалось промежутка. Это сделано для того, чтобы ряд выбранных Selectable казался непрерывным.
    // "bool selected" несут состояние выбора (только для чтения). Нажатие Selectable() возвращает true, поэтому вы можете изменить состояние выбора. размер. x==0.0:
    // использовать оставшуюся ширину, размер. x>0.0: указать ширину. размер. y==0.0: использовать высоту метки, размер. y>0.0: указать высоту
    IMGUI_API bool                  Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    // "bool* p_selected" указывает на состояние выбора (чтение-запись) в качестве удобного помощника.
    IMGUI_API bool                  Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

    // Виджеты: Поля со списком
    // - По сути, это тонкая оболочка для использования BeginChild / EndChild с некоторыми стилистическими изменениями.
    // - API BeginListBox()/EndListBox() позволяет вам управлять своим содержимым и состоянием выбора так, как вы этого хотите, создавая, например, Selectable() или любые элементы.
    // - Упрощенный / старый api ListBox() - это помощники по сравнению с BeginListBox() / EndListBox(), которые остаются доступными для удобства. Это аналогично тому, как создаются комбо.
    // - Выберите ширину рамки: размер. x > 0.0f: пользовательский / размер. x < 0.0f или -FLT_MIN: выровнять по правому краю / размер. x = 0.0f (по умолчанию): использовать текущую ширину элемента
    // - Выберите высоту рамки: размер. y > 0.0f: пользовательский / размер. y < 0.0f или -FLT_MIN: выравнивание по нижнему краю / размер. y = 0.0f (по умолчанию):
    // произвольная высота по умолчанию, на которой может поместиться ~ 7 предметов
    IMGUI_API bool                  BeginListBox(const char* label, const ImVec2& size = ImVec2(0, 0));   // откройте область прокрутки в рамке
    IMGUI_API void                  EndListBox();                                                         // вызывайте EndListBox() только в том случае, если BeginListBox() вернул значение true!
    IMGUI_API bool                  ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool                  ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);

    // Виджеты: Построение графика данных
    // - Рассмотрите возможность использования ImPlot (https://github.com/epezent/implot) что намного лучше!
    IMGUI_API void                  PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void                  PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
    IMGUI_API void                  PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void                  PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

    // Виджеты: Помощники Value().
    // - Это просто ярлык для вызова Text() со строкой формата. Выведите одно значение в формате "name: value" (совет: свободно объявляйте больше в своем коде для обработки ваших типов.
    // вы можете добавлять функции в пространство имен ImGui)
    IMGUI_API void                  Value(const char* prefix, bool b);
    IMGUI_API void                  Value(const char* prefix, int v);
    IMGUI_API void                  Value(const char* prefix, unsigned int v);
    IMGUI_API void                  Value(const char* prefix, float v, const char* float_format = NULL);

    // Виджеты: Меню
    // - Используйте BeginMenuBar() в окне ImGuiWindowFlags_MenuBar для добавления в его строку меню.
    // - Используйте BeginMainMenuBar(), чтобы создать строку меню в верхней части экрана и добавить к ней.
    // - Используйте Beginnmenu() для создания меню. Вы можете вызвать beginnmenu() несколько раз с одним и тем же идентификатором, чтобы добавить к нему больше элементов.
    // - Не то, чтобы MenuItem() keyboardshortcuts отображались для удобства, но на данный момент не обрабатываются уважаемым ImGui.
    IMGUI_API bool                  BeginMenuBar();                                                                                         // добавить в строку меню текущего окна (требуется установить флаг ImGuiWindowFlags_MenuBar в родительском окне).
    IMGUI_API void                  EndMenuBar();                                                                                           // вызывайте EndMenuBar() только в том случае, если BeginMenuBar() возвращает true!
    IMGUI_API bool                  BeginMainMenuBar();                                                                                     // создайте и добавьте в полноэкранную строку меню.
    IMGUI_API void                  EndMainMenuBar();                                                                                       // вызывайте EndMainMenuBar() только в том случае, если BeginMainMenuBar() возвращает true!
    IMGUI_API bool                  BeginMenu(const char* label, bool enabled = true);                                                      // создайте пункт подменю. вызывайте EndMenu() только в том случае, если это возвращает true!
    IMGUI_API void                  EndMenu();                                                                                              // вызывайте EndMenu() только в том случае, если BeginMenu() возвращает true!
    IMGUI_API bool                  MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);   // возвращает значение true при активации.
    IMGUI_API bool                  MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);               // возвращает true при активации + переключение (*p_selected), если p_selected != NULL

    // Всплывающие подсказки
    // - Всплывающая подсказка - это окна, следующие за движением мыши. Они не отвлекают внимание.
    IMGUI_API void                  BeginTooltip();   // запустите/добавьте окно всплывающей подсказки. для создания полнофункциональной всплывающей подсказки (с любыми элементами).
    IMGUI_API void                  EndTooltip();
    IMGUI_API void                  SetTooltip(const char* fmt, ...) IM_FMTARGS(1);   // установите всплывающую подсказку только для текста, обычно используемую с ImGui:: IsItemHovered().
                                                                                      // переопределите любой предыдущий вызов SetTooltip().
    IMGUI_API void                  SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Всплывающие окна, Модальности
    //  - Они блокируют обычное обнаружение наведения мыши (и, следовательно, большинство взаимодействий с мышью) позади них.
    // - Если они не модальные: их можно закрыть, щелкнув в любом месте за их пределами или нажав ESCAPE.
    // - Их состояние видимости (~bool) сохраняется внутри, а не программистом, как мы привыкли при обычных вызовах Begin*().
    // - Приведенные выше 3 свойства связаны между собой: нам нужно сохранить состояние видимости всплывающих окон в библиотеке, потому что всплывающие окна могут быть закрыты в любое время.
    // - Вы можете обойти ограничение зависания, используя ImGuiHoveredFlags_AllowWhenBlockedByPopup при вызове IsItemHovered() или IsWindowHovered().
    // - ВАЖНО: Идентификаторы всплывающих окон относятся к текущему стеку идентификаторов, поэтому openPopup и BeginPopup обычно должны находиться на одном уровне стека.
    // Иногда это приводит к запутанным ошибкам. Возможно, это будет переработано в будущем.

    // Всплывающие окна: функции начала/завершения
    //  - BeginPopup(): запрашивает состояние всплывающего окна, если открыто, начинает добавление в окно. После этого вызовите EndPopup(). ImGuiWindowFlags перенаправляются в окно.
    //  - BeginPopupModal(): блокирует все взаимодействия за окном, не может быть закрыт пользователем, добавляет затемняющий фон, имеет строку заголовка.
    IMGUI_API bool                  BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);                           // верните значение true, если всплывающее окно открыто, и вы можете начать вывод в него.
    IMGUI_API bool                  BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);   // верните значение true, если модал открыт, и вы можете начать вывод в него.
    IMGUI_API void                  EndPopup();                                                                           // вызывайте EndPopup() только в том случае, если BeginPopupXXX() возвращает true!

    // Всплывающие окна: функции открытия/закрытия
    //  - OpenPopup(): установите всплывающее окно в состояние открыто. Для открытия опций доступны ImGuiPopupFlags.
    //  - Если они не модальные: их можно закрыть, щелкнув в любом месте за их пределами или нажав ESCAPE.
    //  - CloseCurrentPopup(): используйте внутри области BeginPopup()/EndPopup() для закрытия вручную.
    //  - CloseCurrentPopup() вызывается по умолчанию с помощью Selectable()/MenuItem() при активации (FIXME: нужны некоторые параметры).
    //  - Используйте ImGuiPopupFlags_NoOpenOverExistingPopup, чтобы избежать открытия всплывающего окна, если оно уже есть на том же уровне.
    //  Это эквивалентно, например, тестированию на ! IsAnyPopupOpen() предшествует openPopup().
    // - Используйте IsWindowAppearing() после BeginPopup(), чтобы определить, только что открылось окно.
    // - ВАЖНО: Обратите внимание, что для OpenPopupOnItemClick() мы устанавливаем флаги по умолчанию равными 1 (== ImGuiPopupFlags_MouseButtonRight)
    //  для обратной совместимости со старым API, принимающим параметр 'int mouse_button = 1'.
    IMGUI_API void                  OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);                     // вызовите, чтобы пометить всплывающее окно как открытое (не вызывайте каждый кадр!).
    IMGUI_API void                  OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);                             // перегрузка идентификатора для облегчения вызова из вложенных стеков
    IMGUI_API void                  OpenPopupOnItemClick(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // помощник для открытия всплывающего окна при нажатии на последний элемент.
                                                                                                                        // Значение по умолчанию ImGuiPopupFlags_MouseButtonRight == 1. (примечание: на самом деле
                                                                                                                        // запускает событие mouse _released_, чтобы соответствовать поведению всплывающих окон)
    IMGUI_API void                  CloseCurrentPopup();                                                                // вручную закройте всплывающее окно, с которого мы начали.

    // Всплывающие окна: открыть+начать комбинированные функции помощников
    //  - Помощники для выполнения openPopup +BeginPopup, где действие открытия запускается, например, наведением курсора на элемент и щелчком правой кнопки мыши.
    //  - Они удобны для легкого создания контекстных меню, отсюда и название.
    //  - ВАЖНО: Обратите внимание, что BeginPopupContextXXX принимает ImGuiPopupFlags так же, как openPopup() и в отличие от BeginPopup(). Для обеспечения полной согласованности
    //  в будущем мы можем добавить ImGuiWindowFlags к функциям BeginPopupContextXXX.
    //  - ВАЖНО: Обратите внимание, что мы в исключительных случаях устанавливаем по умолчанию их флаги равными 1 (== ImGuiPopupFlags_MouseButtonRight) для обратной совместимости со старым API,
    //  использующим параметр 'int mouse_button = 1', поэтому, если вы добавляете другие флаги, не забудьте повторно добавить ImGuiPopupFlags_MouseButtonRight.
    IMGUI_API bool                  BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);     // открыть + начать всплывающее окно при нажатии на последний элемент. Используйте str_id==NULL,
                                                                                                                           // чтобы связать всплывающее окно с предыдущим элементом. Если вы хотите использовать это
                                                                                                                           // для неинтерактивного элемента, такого как Text(), вам нужно передать здесь явный идентификатор.
                                                                                                                           // читайте комментарии в .cpp!
    IMGUI_API bool                  BeginPopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // открыть +начать всплывающее окно при нажатии на текущее окно.
    IMGUI_API bool                  BeginPopupContextVoid(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);     // открыть + начать всплывающее окно при нажатии в пустоте (где нет окон).

    // Всплывающие окна: функции запроса
    //  - IsPopupOpen(): возвращает значение true, если всплывающее окно открыто на текущем уровне BeginPopup() стека всплывающих окон.
    //  - IsPopupOpen() с ImGuiPopupFlags_AnyPopupId: возвращает true, если какое-либо всплывающее окно открыто на текущем уровне BeginPopup() стека всплывающих окон.
    //  - IsPopupOpen() с ImGuiPopupFlags_AnyPopupId + ImGuiPopupFlags_AnyPopupLevel: возвращает true, если открыто какое-либо всплывающее окно.
    IMGUI_API bool                  IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0);   // верните значение true, если всплывающее окно открыто.

    // Таблицы
    // - Полнофункциональная замена старому API Columns.
    // - Демонстрационный код приведен в разделе Демо->Таблицы. Смотрите верхнюю часть imgui_tables. cpp для общего комментария.
    // - Смотрите перечисления ImGuiTableFlags_ и ImGuiTableColumnFlags_ для описания доступных флагов.
    //   Типичным потоком вызовов является:
    // - 1. Вызов BeginTable(), завершите работу раньше, если возвращает false.
    // - 2. Опциональный вызов TableSetupColumn() чтобы отправить имя столбца/флаги/значения по умолчанию.
    // - 3. Опциональный вызов TableSetupScrollFreeze() чтобы запросить замораживание прокрутки столбцов/строк.
    // - 4. Опциональный вызов TableHeadersRow() чтобы отправить строку заголовка. Имена извлекаются из данных TableSetupColumn().
    // - 5. Заполнить содержимое:
    //    - В большинстве ситуаций вы можете использовать TableNextRow() + TableSetColumnIndex(N), чтобы начать добавление в столбец.
    //    - Если вы используете таблицы как своего рода сетку, где все столбцы содержат содержимое одного и того же типа,
    //      возможно, вы предпочтете использовать TableNextColumn() вместо TableNextRow() + TableSetColumnIndex().
    //      TableNextColumn() при необходимости автоматически перенесет в следующую строку.
    //    - ВАЖНО: По сравнению со старыми столбцами() API, нам нужно вызвать TableNextColumn() для первого столбца!
    //    - Краткое описание возможного потока вызовов:
    //        --------------------------------------------------------------------------------------------------------
    //        TableNextRow() -> TableSetColumnIndex(0) -> Text("Hello 0") -> TableSetColumnIndex(1) -> Text("Hello 1")  // OK
    //        TableNextRow() -> TableNextColumn()      -> Text("Hello 0") -> TableNextColumn()      -> Text("Hello 1")  // OK
    //                          TableNextColumn()      -> Text("Hello 0") -> TableNextColumn()      -> Text("Hello 1")  // OK: TableNextColumn() автоматически переходит к следующему ряду!
    //        TableNextRow()                           -> Text("Hello 0")                                               // Не OK! Отсутствует TableSetColumnIndex() или TableNextColumn()! Текст не появится!
    //        --------------------------------------------------------------------------------------------------------
    // - 5. Call EndTable()
    IMGUI_API bool                  BeginTable(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);
    IMGUI_API void                  EndTable();                                                                    // вызывайте EndTable() только в том случае, если BeginTable() возвращает true!
    IMGUI_API void                  TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f);   // добавить в первую ячейку новой строки.
    IMGUI_API bool                  TableNextColumn();                                                             // добавить в следующий столбец (или первый столбец следующей строки, если в данный момент находится в последнем столбце). Возвращает значение true, когда столбец виден.
    IMGUI_API bool                  TableSetColumnIndex(int column_n);                                             // добавить в указанный столбец. Возвращает значение true, когда столбец виден.

    // Таблицы: Объявление заголовков и столбцов
    // - Используйте TableSetupColumn(), чтобы указать метку, политику изменения размера, ширину / вес по умолчанию, идентификатор, различные другие флаги и т.д.
    // - Используйте TableHeadersRow(), чтобы создать строку заголовка и автоматически отправить TableHeader() для каждого столбца.
    //   Заголовки необходимы для выполнения: изменения порядка, сортировки и открытия контекстного меню.
    //   Контекстное меню также можно сделать доступным в теле столбцов, используя ImGuiTableFlags_ContextMenuInBody.
    // - Вы можете вручную отправлять заголовки, используя вызовы TableNextRow() + TableHeader(), но это полезно только в
    //   некоторые расширенные варианты использования (например, добавление пользовательских виджетов в строку заголовка).
    // - Используйте TableSetupScrollFreeze() для блокировки столбцов / строк, чтобы они оставались видимыми при прокрутке.
    IMGUI_API void                  TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
    IMGUI_API void                  TableSetupScrollFreeze(int cols, int rows);   // заблокируйте столбцы / строки, чтобы они оставались видимыми при прокрутке.
    IMGUI_API void                  TableHeadersRow();                            // отправить все ячейки заголовков на основе данных, предоставленных TableSetupColumn() + контекстное меню отправки
    IMGUI_API void                  TableHeader(const char* label);               // отправьте одну ячейку заголовка вручную (используется редко)

    // Таблицы: Сортировка & Разнообразные функции
    // - Сортировка: вызовите TableGetSortSpecs(), чтобы получить последние спецификации сортировки для таблицы. NULL, если не выполняется сортировка.
    //   Когда 'sort_specs->SpecsDirty == true', вы должны отсортировать свои данные. Это будет верно, когда спецификации сортировки будут иметь
    //   изменилось с момента последнего вызова или в первый раз. Обязательно установите 'SpecsDirty = false' после сортировки,
    //   иначе вы можете расточительно сортировать свои данные в каждом кадре!
    // - Функции args 'int column_n' обрабатывают значение по умолчанию, равное -1, как то же самое, что и передача индекса текущего столбца.
    IMGUI_API ImGuiTableSortSpecs*  TableGetSortSpecs();                                                          // получите последние спецификации сортировки для таблицы (NULL, если не выполняется сортировка).
                                                                                                                  // Время жизни: не удерживайте этот указатель на протяжении нескольких кадров или после любого последующего вызова BeginTable().
    IMGUI_API int                   TableGetColumnCount();                                                        // возвращает количество столбцов (значение, переданное в начальную таблицу)
    IMGUI_API int                   TableGetColumnIndex();                                                        // возвращает индекс текущего столбца.
    IMGUI_API int                   TableGetRowIndex();                                                           // возвращает индекс текущей строки.
    IMGUI_API const char*           TableGetColumnName(int column_n = -1);                                        // возвращает "", если у столбца не было имени, объявленного TableSetupColumn(). Передайте значение -1, чтобы использовать текущий столбец.
    IMGUI_API ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);                                       // верните флаги столбцов, чтобы вы могли запросить их флаги статуса "Включено" / "Видно" / "Отсортировано" / "Зависло".
                                                                                                                  // Передайте значение -1, чтобы использовать текущий столбец.
    IMGUI_API void                  TableSetColumnEnabled(int column_n, bool v);                                  // измените доступное пользователю включенное/отключенное состояние столбца. Установите значение false, чтобы скрыть столбец.
                                                                                                                  // Пользователь может использовать контекстное меню, чтобы изменить это самостоятельно (щелкните правой кнопкой мыши
                                                                                                                  // в заголовках или щелкните правой кнопкой мыши в теле столбцов с помощью ImGuiTableFlags_ContextMenuInBody)
    IMGUI_API void                  TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1);   // измените цвет ячейки, строки или столбца. Смотрите ImGuiTableBgTarget_ флаги для получения подробной информации.

    // Устаревший API столбцов (предпочитаю использовать таблицы!)
    // - Вы также можете использовать sameLine(pos_x) для имитации упрощенных столбцов.
    IMGUI_API void                  Columns(int count = 1, const char* id = NULL, bool border = true);
    IMGUI_API void                  NextColumn();                                        // следующий столбец, по умолчанию используется текущая строка или следующая строка, если текущая строка завершена
    IMGUI_API int                   GetColumnIndex();                                    // получить индекс текущего столбца
    IMGUI_API float                 GetColumnWidth(int column_index = -1);               // получаем ширину столбца (в пикселях). передайте значение -1, чтобы использовать текущий столбец
    IMGUI_API void                  SetColumnWidth(int column_index, float width);       // установите ширину столбца (в пикселях). передайте значение -1, чтобы использовать текущий столбец
    IMGUI_API float                 GetColumnOffset(int column_index = -1);              // получить положение строки столбца (в пикселях, с левой стороны области содержимого). передайте значение -1,
                                                                                         // чтобы использовать текущий столбец, в противном случае 0.. GetColumnsCount() включительно. столбец 0 обычно равен 0.0f
    IMGUI_API void                  SetColumnOffset(int column_index, float offset_x);   // установите положение строки столбца (в пикселях, с левой стороны области содержимого).
                                                                                         // передайте значение -1, чтобы использовать текущий столбец
    IMGUI_API int                   GetColumnsCount();

    // Панели вкладок, вкладки
    // Примечание: Вкладки автоматически создаются системой стыковки. Используйте это, чтобы самостоятельно создавать панели вкладок без необходимости стыковки.
    IMGUI_API bool                  BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);                         // создавать и добавлять на панель вкладок
    IMGUI_API void                  EndTabBar();                                                                         // вызывайте EndTabBar() только в том случае, если BeginTabBar() возвращает true!
    IMGUI_API bool                  BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);   // создайте вкладку. Возвращает значение true, если выбрана вкладка.
    IMGUI_API void                  EndTabItem();                                                                        // вызывайте EndTabItem() только в том случае, если BeginTabItem() возвращает true!
    IMGUI_API bool                  TabItemButton(const char* label, ImGuiTabItemFlags flags = 0);                       // создайте вкладку, которая ведет себя как кнопка. возвращает значение true при нажатии кнопки.
                                                                                                                         // не может быть выбран на панели вкладок.
    IMGUI_API void                  SetTabItemClosed(const char* tab_or_docked_window_label);                            // уведомлять панель вкладок или систему стыковки о предстоящей закрытой вкладке / окне
                                                                                                                         // (полезно для уменьшения визуального мерцания на переупорядочиваемых панелях вкладок).
                                                                                                                         // Для панели вкладок: вызывается после BeginTabBar() и перед отправкой вкладки.
                                                                                                                         // В противном случае вызывайте с именем окна.

    // Docking
    // [BETA API] Включить с помощью ввода-вывода. ConfigFlags |= ImGuiConfigFlags_DockingEnable.
    // Примечание: Вы можете использовать большинство средств стыковки без вызова какого-либо API. Вам НЕ нужно вызывать DockSpace(), чтобы использовать Docking!
    // - Перетащите из строки заголовка окна или их вкладки, чтобы закрепить / отстыковать. Удерживайте клавишу SHIFT, чтобы отключить стыковку/расстыковку.
    // - Перетащите из кнопки меню окна (верхняя левая кнопка), чтобы отстыковать весь узел (все окна).
    // - При вводе-выводе. ConfigDockingWithShift == true, вместо этого вам нужно удерживать SHIFT в положении _enable_ стыковка/расстыковка.
    // О dockspaces:
    // - Используйте DockSpace() для создания явного узла dock внутри существующего окна. Подробности смотрите в демо-версии Docking.
    // - Используйте DockSpaceOverViewport() для создания явного узла dock, закрывающего экран или определенный видовой экран.
    //   Это часто используется с ImGuiDockNodeFlags_PassthruCentralNode.
    // - Важно: Док-пространства должны быть отправлены _ перед_ любым окном, которое они могут разместить. Отправьте его пораньше в свой кадр!
    // - Важно: Док-пространства необходимо поддерживать активными, если они скрыты, в противном случае пристыкованные к ним окна будут отстыкованы.
    // например, если у вас есть несколько вкладок с dockspace внутри каждой вкладки: отправьте невидимые dockspaces с помощью ImGuiDockNodeFlags_KeepAliveOnly.
    IMGUI_API ImGuiID               DockSpace(ImGuiID id, const ImVec2& size = ImVec2(0, 0), ImGuiDockNodeFlags flags = 0, const ImGuiWindowClass* window_class = NULL);
    IMGUI_API ImGuiID               DockSpaceOverViewport(const ImGuiViewport* viewport = NULL, ImGuiDockNodeFlags flags = 0, const ImGuiWindowClass* window_class = NULL);
    IMGUI_API void                  SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0);   // установите идентификатор док-станции следующего окна
    IMGUI_API void                  SetNextWindowClass(const ImGuiWindowClass* window_class);   // установите следующий класс окна (управляйте совместимостью с док-станцией + предоставляйте подсказки
                                                                                                // серверной части платформы с помощью пользовательских флагов видового экрана и отношений родитель/потомок платформы)
    IMGUI_API ImGuiID               GetWindowDockID();
    IMGUI_API bool                  IsWindowDocked();   // закреплено ли текущее окно в другом окне?

    // Logging/Capture
    // - Весь текст, выводимый из интерфейса, может быть сохранен в tty /файл / буфер обмена. По умолчанию узлы дерева автоматически открываются во время ведения журнала.
    IMGUI_API void                  LogToTTY(int auto_open_depth = -1);                                 // начать вход в tty (stdout)
    IMGUI_API void                  LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // начать запись в файл
    IMGUI_API void                  LogToClipboard(int auto_open_depth = -1);                           // начать запись в буфер обмена операционной системы
    IMGUI_API void                  LogFinish();                                                        // прекратить ведение журнала (закрыть файл и т.д.)
    IMGUI_API void                  LogButtons();                                                       // помощник для отображения кнопок для входа в tty/файл/буфер обмена
    IMGUI_API void                  LogText(const char* fmt, ...) IM_FMTARGS(1);                        // передавать текстовые данные непосредственно в журнал (без отображения)
    IMGUI_API void                  LogTextV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Drag and Drop(Перетаскивание)
    // - Для исходных элементов вызовите BeginDragDropSource(), если он возвращает true, также вызовите SetDragDropPayload() + EndDragDropSource().
    // - Для целевых кандидатов вызовите beginndragdroptarget(), если он возвращает true, также вызовите AcceptDragDropPayload() + EndDragDropTarget().
    // - Если вы прекратите вызывать BeginDragDropSource(), полезная нагрузка сохранится, однако у нее не будет всплывающей подсказки предварительного просмотра
    //  (в настоящее время мы показываем резервную всплывающую подсказку "...", см. #1725)
    // - Элемент может быть как источником перетаскивания, так и целью перетаскивания.
    IMGUI_API bool                  BeginDragDropSource(ImGuiDragDropFlags flags = 0);                                       // вызовите после отправки элемента, который можно перетащить. когда это возвращает
                                                                                                                             // значение true, вы можете вызвать SetDragDropPayload() + EndDragDropSource()
    IMGUI_API bool                  SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0);   // тип - это определенная пользователем строка, состоящая максимум из 32 символов.
                                                                                                                             // Строки, начинающиеся с '_', зарезервированы для внутренних типов dear imgui.
                                                                                                                             // Данные копируются и хранятся в imgui. Возвращает значение true, когда полезная нагрузка была принята.
    IMGUI_API void                  EndDragDropSource();                                                                     // вызывайте EndDragDropSource() только в том случае, если BeginDragDropSource() возвращает true!
    IMGUI_API bool                  BeginDragDropTarget();                                                                   // вызовите после отправки элемента, который может получить полезную нагрузку.
                                                                                                                             // Если это возвращает true, вы можете вызвать AcceptDragDropPayload() + EndDragDropTarget()
    IMGUI_API const ImGuiPayload*   AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);                   // принимайте содержимое заданного типа. Если установлено значение ImGuiDragDropFlags_AcceptBeforeDelivery,
                                                                                                                             // вы можете просмотреть полезную нагрузку до того, как будет отпущена кнопка мыши.
    IMGUI_API void                  EndDragDropTarget();                                                                     // вызывайте EndDragDropTarget() только в том случае, если BeginDragDropTarget() возвращает true!
    IMGUI_API const ImGuiPayload*   GetDragDropPayload();                                                                    // просматривайте текущую полезную нагрузку непосредственно из любого места.
                                                                                                                             // может возвращать значение NULL. используйте ImGuiPayload:: IsDataType() для проверки типа полезной нагрузки.

    // Отключение [BETA API]
    // - Отключите все взаимодействия с пользователем и затемните визуальные элементы элементов (применяя стиль. Отключена АЛЬФА поверх текущих цветов)
    // - Они могут быть вложенными, но их нельзя использовать для включения уже отключенного раздела (одного BeginDisabled(true) в стеке достаточно, чтобы все было отключено)
    // - BeginDisabled(false), по сути, не делает ничего полезного, но предоставляется для облегчения использования логических выражений.
    //   Если вы можете избежать вызова BeginDisabled(False)/EndDisabled(), лучше всего этого избежать.
    IMGUI_API void                  BeginDisabled(bool disabled = true);
    IMGUI_API void                  EndDisabled();

    // Clipping
    // - На наведение курсора мыши влияют вызовы ImGui:: PushClipRect(), в отличие от прямых вызовов ImDrawList:: PushClipRect(), которые предназначены только для рендеринга.
    IMGUI_API void                  PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    IMGUI_API void                  PopClipRect();

    // Focus, Activation
    // - Предпочитаю использовать "SetItemDefaultFocus()" вместо "if (IsWindowAppearing()) SetScrollHereY()", когда это применимо, для обозначения "это элемент по умолчанию".
    IMGUI_API void                  SetItemDefaultFocus();                  // сделайте последний элемент сфокусированным элементом окна по умолчанию.
    IMGUI_API void                  SetKeyboardFocusHere(int offset = 0);   // сфокусируйте клавиатуру на следующем виджете. Используйте положительное "смещение" для доступа к подкомпонентам
                                                                            // многокомпонентного виджета. Используйте -1, чтобы получить доступ к предыдущему виджету.

    // Item/Widgets Утилиты и функции запросов
    // - Большинство функций относятся к предыдущему отправленному элементу.
    // - Смотрите демонстрационное окно в разделе "Виджеты-> Статус запроса" для интерактивной визуализации большинства этих функций.
    IMGUI_API bool                  IsItemHovered(ImGuiHoveredFlags flags = 0);         // завис ли последний элемент? (и пригодный для использования, он же не блокируется всплывающим окном и т.д.).
                                                                                        // Дополнительные параметры см. в разделе ImGuiHoveredFlags.
    IMGUI_API bool                  IsItemActive();                                     // активен ли последний элемент? (например, кнопка удерживается, текстовое поле редактируется.
                                                                                        // Это будет постоянно возвращать значение true при удерживании кнопки мыши на элементе.
                                                                                        // Элементы, которые не взаимодействуют, всегда будут возвращать значение false)
    IMGUI_API bool                  IsItemFocused();                                    // предназначен ли последний элемент для навигации по клавиатуре / геймпаду?
    IMGUI_API bool                  IsItemClicked(ImGuiMouseButton mouse_button = 0);   // наведен ли указатель мыши на последний элемент? (**) == IsMouseClicked(mouse_button) && IsItemHovered()Важно.
                                                                                        // (**) это не эквивалентно поведению, например, Button(). Прочитайте комментарии в определении функции.
    IMGUI_API bool                  IsItemVisible();                                    // виден ли последний элемент? (элементы могут быть скрыты из виду из-за обрезки / прокрутки)
    IMGUI_API bool                  IsItemEdited();                                     // изменил ли последний элемент свое базовое значение в этом фрейме? или на него оказали давление?
                                                                                        // Обычно это то же самое, что и возвращаемое значение "bool" многих виджетов.
    IMGUI_API bool                  IsItemActivated();                                  // был последним элементом, который только что стал активным (ранее элемент был неактивен).
    IMGUI_API bool                  IsItemDeactivated();                                // был ли последний элемент только что сделан неактивным (элемент ранее был активен).
                                                                                        // Полезно для отмены / повтора шаблонов с виджетами, требующих непрерывного редактирования.
    IMGUI_API bool                  IsItemDeactivatedAfterEdit();                       // был ли последний элемент просто сделан неактивным и изменил значение, когда он был активен?
                                                                                        // (например, перемещен ползунок / перетаскивание). Полезно для отмены / повтора шаблонов с виджетами,
                                                                                        // требующих непрерывного редактирования. Обратите внимание, что вы можете получать ложные срабатывания
                                                                                        // (некоторые виджеты, такие как Combo() /ListBox() /Selectable(), возвращают значение true даже при нажатии на уже выбранный элемент).
    IMGUI_API bool                  IsItemToggledOpen();                                // был ли переключен последний элемент в открытое состояние? устанавливается с помощью TreeNode().
    IMGUI_API bool                  IsAnyItemHovered();                                 // завис ли какой-либо элемент?
    IMGUI_API bool                  IsAnyItemActive();                                  // активен ли какой-либо элемент?
    IMGUI_API bool                  IsAnyItemFocused();                                 // сфокусирован ли какой-либо элемент?
    IMGUI_API ImVec2                GetItemRectMin();                                   // получаем верхний левый ограничивающий прямоугольник последнего элемента (пространство экрана)
    IMGUI_API ImVec2                GetItemRectMax();                                   // получаем правый нижний ограничивающий прямоугольник последнего элемента (пространство экрана)
    IMGUI_API ImVec2                GetItemRectSize();                                  // получить размер последнего элемента
    IMGUI_API void                  SetItemAllowOverlap();                              // разрешить последнему элементу перекрываться последующим элементом. иногда полезно с невидимыми кнопками,
                                                                                        // элементами выбора и т.д. чтобы захватить неиспользуемую область.

    // Viewports
    // - В настоящее время представляет собой окно платформы, созданное приложением, в котором размещены наши дорогие окна ImGui.
    // - В ветке "стыковка" с включенным мульти-видовым экраном мы расширяем эту концепцию, чтобы иметь несколько активных видовых экранов.
    // - В будущем мы расширим эту концепцию еще больше, чтобы также представлять Platform Monitor и поддерживать режим работы "без главного окна платформы".
    IMGUI_API ImGuiViewport*        GetMainViewport();   // верните основной видовой экран/экран по умолчанию. Это никогда не может быть NULL.

    // Background/Foreground Draw Lists
    IMGUI_API ImDrawList*           GetBackgroundDrawList();                          // получите список фоновой прорисовки для видового экрана, связанного с текущим окном.
                                                                                      // этот список отрисовок будет первым списком рендеринга.
                                                                                      // Полезно для быстрого рисования фигур / текста за содержимым imgui.
    IMGUI_API ImDrawList*           GetForegroundDrawList();                          // получите список отрисовки переднего плана для видового экрана, связанного с текущим окном.
                                                                                      // этот список отрисовок будет последним отрисованным.
                                                                                      // Полезно для быстрого рисования фигур / текста поверх содержимого imgui.
    IMGUI_API ImDrawList*           GetBackgroundDrawList(ImGuiViewport* viewport);   // получите список фоновой прорисовки для данного видового экрана. этот список отрисовок будет.
                                                                                      // первым списком рендеринга Полезно для быстрого рисования фигур / текста за содержимым imgui.
    IMGUI_API ImDrawList*           GetForegroundDrawList(ImGuiViewport* viewport);   // получите список отрисовки переднего плана для данного видового экрана. этот список отрисовок будет
                                                                                      // последним отрисованным. Полезно для быстрого рисования фигур / текста поверх содержимого imgui.

    // Разнообразные Утилиты
    IMGUI_API bool                  IsRectVisible(const ImVec2& size);                               // проверьте, виден ли прямоугольник (заданного размера, начиная с позиции курсора) / не обрезан.
    IMGUI_API bool                  IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);   // проверьте, виден ли прямоугольник (в пространстве экрана) / не обрезан.
                                                                                                     // для выполнения грубой обрезки со стороны пользователя.
    IMGUI_API double                GetTime();                                                       // получите глобальное время imgui. увеличивается на ввод-вывод. Уточняйте каждый кадр.
    IMGUI_API int                   GetFrameCount();                                                 // получите глобальное количество кадров imgui. увеличивается на 1 в каждом кадре.
    IMGUI_API ImDrawListSharedData* GetDrawListSharedData();                                         // вы можете использовать это при создании своих собственных экземпляров ImDrawList.
    IMGUI_API const char*           GetStyleColorName(ImGuiCol idx);                                 // получите строку, соответствующую значению enum (для отображения, сохранения и т.д.).
    IMGUI_API void                  SetStateStorage(ImGuiStorage* storage);                          // замените текущее хранилище окон нашим собственным (если вы хотите управлять им самостоятельно, обычно очищайте его подраздел)
    IMGUI_API ImGuiStorage*         GetStateStorage();
    IMGUI_API bool                  BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags flags = 0);   // помощник для создания дочернего окна/области прокрутки, которая выглядит как обычная рамка виджета
    IMGUI_API void                  EndChildFrame();                                                               // всегда вызывать EndChildFrame() независимо от возвращаемых значений BeginChildFrame() (что указывает на свернутое/обрезанное окно)

    // Text Utilities
    IMGUI_API ImVec2                CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    // Color Utilities
    IMGUI_API ImVec4                ColorConvertU32ToFloat4(ImU32 in);
    IMGUI_API ImU32                 ColorConvertFloat4ToU32(const ImVec4& in);
    IMGUI_API void                  ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    IMGUI_API void                  ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    // Входные данные Утилиты: Клавиатура
    // Без IMGUI_DISABLE_OBSOLETE_KEYIO: (устаревшая поддержка)
    // - Для 'ImGuiKey key' вы по-прежнему можете использовать свои устаревшие нативные/пользовательские индексы в соответствии с тем, как ваш бэкенд/движок сохранил их в io. Клавиши Вниз[].
    //   С IMGUI_DISABLE_OBSOLETE_KEYIO: (это путь вперед)
    // - Любое использование 'ImGuiKey' будет подтверждено, когда будет передан ключ < 512, ранее зарезервированный как индексы собственных/пользовательских ключей
    // - GetKeyIndex() является сквозным и поэтому устарел (исчез, если определен IMGUI_DISABLE_OBSOLETE_KEYIO)
    IMGUI_API bool                  IsKeyDown(ImGuiKey key);                                             // держится ключ.
    IMGUI_API bool                  IsKeyPressed(ImGuiKey key, bool repeat = true);                      // была ли нажата клавиша (перешла от ! Вниз к Вниз)? если repeat=true, использует io. KeyRepeatDelay / KeyRepeatRate
    IMGUI_API bool                  IsKeyReleased(ImGuiKey key);                                         // была ли клавиша отпущена (перешла из Вниз в ! Вниз)?
    IMGUI_API int                   GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate);   // использует предоставленную скорость повторения/задержку. возвращает количество, чаще всего 0 или 1,
                                                                                                         // но может быть > 1, если RepeatRate достаточно мал, чтобы DeltaTime > RepeatRate
    IMGUI_API const char*           GetKeyName(ImGuiKey key);                                            // [DEBUG] возвращает английское имя ключа. Эти имена предоставлены для целей отладки
                                                                                                         // и не предназначены для постоянного сохранения без сравнения.
    IMGUI_API void                  SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);         // Переопределить ио. Флаг WantCaptureKeyboard для следующего кадра (упомянутый флаг остается
                                                                                                         // для обработки вашим приложением, обычно, когда он равен true, он указывает вашему приложению
                                                                                                         // игнорировать входные данные). е. г. принудительно захватывать клавиатуру при наведении
                                                                                                         // курсора на виджет. Это эквивалентно установке "io. WantCaptureKeyboard = want_capture_keyboard";
                                                                                                         // после следующего вызова NewFrame().

    // Inputs Utilities: Mouse
    // - Чтобы сослаться на кнопку мыши, вы можете использовать именованные перечисления в своем коде. e. г. ImGuiMouseButton_Left, ImGuiMouseButton_Right.
    // - Вы также можете использовать обычное целое число: всегда гарантируется, что 0 = левое, 1 = правое, 2 = среднее.
    // - Об операциях перетаскивания сообщается только после того, как мышь переместилась на определенное расстояние от начальной позиции щелчка (см. 'lock_threshold' и 'io. MouseDraggingThreshold')
    IMGUI_API bool                  IsMouseDown(ImGuiMouseButton button);                                              // кнопка мыши удерживается?
    IMGUI_API bool                  IsMouseClicked(ImGuiMouseButton button, bool repeat = false);                      // кнопка мыши нажала? (перешел от ! Вниз к Внизу). То же, что GetMouseClickedCount() == 1.
    IMGUI_API bool                  IsMouseReleased(ImGuiMouseButton button);                                          // кнопка мыши отпущена? (перешел от Вниз к ! Вниз)
    IMGUI_API bool                  IsMouseDoubleClicked(ImGuiMouseButton button);                                     // кнопка мыши дважды щелкнула? То же, что и GetMouseClickedCount() == 2. (обратите внимание,
                                                                                                                       // что двойной щелчок также сообщает IsMouseClicked() == true)
    IMGUI_API int                   GetMouseClickedCount(ImGuiMouseButton button);                                     // вернуть количество последовательных щелчков мышью в то время, когда произошел щелчок (иначе 0).
    IMGUI_API bool                  IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);   // это зависание мыши с заданным ограничивающим прямоугольником (в пространстве экрана).
                                                                                                                       // обрезается текущими настройками отсечения, но игнорирует другие соображения
                                                                                                                       // фокуса/порядка окон/блока всплывающих окон.
    IMGUI_API bool                  IsMousePosValid(const ImVec2* mouse_pos = NULL);                                   // по соглашению мы используем (-FLT_MAX,-FLT_MAX) для обозначения отсутствия доступной мыши.
    IMGUI_API bool                  IsAnyMouseDown();                                                                  // [УСТАРЕЕТ] удерживается ли какая-либо кнопка мыши? Это было разработано для бэкендов, но лучше, чтобы бэкэнд
                                                                                                                       // поддерживал маску удерживаемых кнопок мыши, потому что будущая система очереди ввода сделает это недействительным.
    IMGUI_API ImVec2                GetMousePos();                                                                     // ярлык для ImGui:: GetIO(). MousePos, предоставленный пользователем, для согласования с другими вызовами
    IMGUI_API ImVec2                GetMousePosOnOpeningCurrentPopup();                                                // получить позицию мыши во время открытия всплывающего окна, в которое у нас есть BeginPopup()
                                                                                                                       // (помощник, чтобы пользователь не поддерживал это значение самостоятельно)
    IMGUI_API bool                  IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f);            // мышь тащит? (если lock_threshold < -1.0f, используется io. MouseDraggingThreshold)
    IMGUI_API ImVec2                GetMouseDragDelta(ImGuiMouseButton button = 0, float lock_threshold = -1.0f);      // вернуть дельту из исходной позиции щелчка, когда кнопка мыши нажата или только что отпущена.
                                                                                                                       // Это заблокировано и возвращает 0.0f до тех пор, пока мышь не преодолеет пороговое расстояние
                                                                                                                       // хотя бы один раз (если lock_threshold < -1.0f, используется io. MouseDraggingThreshold)
    IMGUI_API void                  ResetMouseDragDelta(ImGuiMouseButton button = 0);                                  //
    IMGUI_API ImGuiMouseCursor      GetMouseCursor();                                                                  // получить желаемый тип курсора, сбросить в ImGui:: NewFrame(), он обновляется во время кадра. действителен до Render().
                                                                                                                       // Если вы используете программный рендеринг, установив io. MouseDrawCursor ImGui сделает это за вас.
    IMGUI_API void                  SetMouseCursor(ImGuiMouseCursor cursor_type);                                      // установить желаемый тип курсора
    IMGUI_API void                  SetNextFrameWantCaptureMouse(bool want_capture_mouse);                             // Переопределить ио. WantCaptureMouse флаг следующего кадра (указанный флаг остается для обработки вашим приложением,
                                                                                                                       // обычно, когда он равен true, он указывает вашему приложению игнорировать входные данные). Это эквивалентно
                                                                                                                       // установке "io. WantCaptureMouse = want_capture_mouse;" после следующего вызова NewFrame().

    // Утилиты буфера обмена
    // - Также смотрите функцию LogToClipboard() для захвата графического интерфейса пользователя в буфер обмена или простого вывода текстовых данных в буфер обмена.
    IMGUI_API const char*           GetClipboardText();
    IMGUI_API void                  SetClipboardText(const char* text);

    // Настройки/. Утилиты Ini
    // - Дисковые функции вызываются автоматически, если io. IniFilename != NULL (по умолчанию "imgui.ini").
    // - Установить ио. IniFilename в NULL для загрузки/сохранения вручную. Читай ио. Описание WantSaveIniSettings по обработке . ini сохранение вручную.
    // - Важно: значение по умолчанию "imgui.ini" относится к текущему рабочему каталогу! Большинство приложений захотят заблокировать это абсолютным путем (например, тем же путем, что и исполняемые файлы).
    IMGUI_API void                  LoadIniSettingsFromDisk(const char* ini_filename);                      // вызов после CreateContext() и перед первым вызовом NewFrame(). NewFrame() автоматически вызывает
                                                                                                            // LoadIniSettingsFromDisk(io.IniFilename).
    IMGUI_API void                  LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size = 0);   // вызов после CreateContext() и перед первым вызовом NewFrame() для предоставления .
                                                                                                            // ini данные из собственного источника данных.
    IMGUI_API void                  SaveIniSettingsToDisk(const char* ini_filename);                        // это вызывается автоматически (если io.IniFilename не пусто) через несколько секунд после любой модификации,
                                                                                                            // которая должна быть отражена в файле . ini (а также DestroyContext).
    IMGUI_API const char*           SaveIniSettingsToMemory(size_t* out_ini_size = NULL);                   // вернуть строку с завершающим нулем с расширением . ini данные, которые вы можете сохранить своими средствами.
                                                                                                            // позвони когда ио. Значение WantSaveIniSettings установлено, затем сохраните данные
                                                                                                            // по своему усмотрению и очистите io. Хотите СохранитьIniSettings.

    // Утилиты отладки
    IMGUI_API void                  DebugTextEncoding(const char* text);
    IMGUI_API bool                  DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx);   // Это вызывается макрос IMGUI_CHECKVERSION().

    // Распределители памяти
    // - Эти функции не зависят от текущего контекста.
    // - Пользователи DLL: кучи и глобальные переменные не распределяются между границами DLL! Вам нужно будет вызвать SetCurrentContext() + SetAllocatorFunctions()
    // для каждой границы статики/DLL, с которой вы звоните. Прочтите раздел imgui «Распределители контекста и памяти». cpp для более подробной информации.
    IMGUI_API void                  SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data = NULL);
    IMGUI_API void                  GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data);
    IMGUI_API void*                 MemAlloc(size_t size);
    IMGUI_API void                  MemFree(void* ptr);

    // (Optional) Интерфейс платформы/ОС для поддержки нескольких окон просмотра
    // Прочтите комментарии к структуре ImGuiPlatformIO, чтобы узнать больше.
    // Примечание: вы можете использовать GetWindowViewport() для получения текущего окна просмотра текущего окна.
    IMGUI_API ImGuiPlatformIO&      GetPlatformIO();                                                                                    // функции платформы/рендерера, для бэкенда для настройки + список видовых экранов.
    IMGUI_API void                  UpdatePlatformWindows();                                                                            // вызов в основном цикле. будет вызывать CreateWindow/ResizeWindow/etc. платформенные функции
                                                                                                                                        // для каждого вторичного окна просмотра и DestroyWindow для каждого неактивного окна просмотра.
    IMGUI_API void                  RenderPlatformWindowsDefault(void* platform_render_arg = NULL, void* renderer_render_arg = NULL);   // вызов в основном цикле. будет вызывать функции платформы RenderWindow/SwapBuffers для каждого
                                                                                                                                        // вторичного окна просмотра, для которого не установлен флаг ImGuiViewportFlags_Minimized.
                                                                                                                                        // Может быть повторно реализован пользователем для пользовательских нужд рендеринга.
    IMGUI_API void                  DestroyPlatformWindows();                                                                           // вызывать функции платформы DestroyWindow для всех окон просмотра. вызовите из бэкенда Shutdown(), если вам нужно
                                                                                                                                        // закрыть окна платформы перед выключением imgui. в противном случае будет вызываться DestroyContext().
    IMGUI_API ImGuiViewport*        FindViewportByID(ImGuiID id);                                                                       // это помощник для бэкэндов.
    IMGUI_API ImGuiViewport*        FindViewportByPlatformHandle(void* platform_handle);                                                // это помощник для бэкэндов. тип platform_handle определяется серверной частью (например, HWND, MyWindow*, GLFWwindow* и т. д.)
}   // namespace ImGui

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------

// Флаги для ImGui:: Begin()
enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None                      = 0,
    ImGuiWindowFlags_NoTitleBar                = 1 << 0,    // Отключить строку заголовка
    ImGuiWindowFlags_NoResize                  = 1 << 1,    // Отключить изменение размера пользователя правой нижней ручкой
    ImGuiWindowFlags_NoMove                    = 1 << 2,    // Запретить пользователю перемещать окно
    ImGuiWindowFlags_NoScrollbar               = 1 << 3,    // Отключить полосы прокрутки (окно по-прежнему можно прокручивать с помощью мыши или программно)
    ImGuiWindowFlags_NoScrollWithMouse         = 1 << 4,    // Отключить вертикальную прокрутку пользователем колесиком мыши. В дочернем окне колесико мыши будет
                                                            // переадресовано родительскому, если также не установлен параметр NoScrollbar.
    ImGuiWindowFlags_NoCollapse                = 1 << 5,    // Отключите сворачивающееся окно пользователя, дважды щелкнув по нему. Также называется кнопкой меню окна (например, в стыковочном узле).
    ImGuiWindowFlags_AlwaysAutoResize          = 1 << 6,    // Изменение размера каждого окна в соответствии с его содержимым в каждом кадре
    ImGuiWindowFlags_NoBackground              = 1 << 7,    // Отключите цвет фона рисования (WindowBg и т. д.) и внешнюю границу. Подобно использованию SetNextWindowBgAlpha(0.0f).
    ImGuiWindowFlags_NoSavedSettings           = 1 << 8,    // Никогда не загружайте/сохраняйте настройки в формате . ini-файл
    ImGuiWindowFlags_NoMouseInputs             = 1 << 9,    // Отключить ловлю мыши, тест зависания с пропуском.
    ImGuiWindowFlags_MenuBar                   = 1 << 10,   // Имеет строку меню
    ImGuiWindowFlags_HorizontalScrollbar       = 1 << 11,   // Разрешить появление горизонтальной полосы прокрутки (по умолчанию отключено). Вы можете использовать
                                                            // SetNextWindowContentSize(ImVec2(width,0.0f)); перед вызовом Begin() для указания ширины.
                                                            // Прочитайте код в imgui_demo в разделе "Горизонтальная прокрутка".
    ImGuiWindowFlags_NoFocusOnAppearing        = 1 << 12,   // Отключить получение фокуса при переходе из скрытого в видимое состояние
    ImGuiWindowFlags_NoBringToFrontOnFocus     = 1 << 13,   // Отключить вывод окна на передний план при наведении фокуса (например, щелкнув по нему или программно передав ему фокус)
    ImGuiWindowFlags_AlwaysVerticalScrollbar   = 1 << 14,   // Всегда показывать вертикальную полосу прокрутки (даже если ContentSize. y < Size. y)
    ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,   // Всегда показывать горизонтальную полосу прокрутки (даже если ContentSize. x < Size. x)
    ImGuiWindowFlags_AlwaysUseWindowPadding    = 1 << 16,   // Убедитесь, что дочерние окна без рамки используют стиль. WindowPadding (по умолчанию игнорируется для дочерних окон без рамки, так как это удобнее)
    ImGuiWindowFlags_NoNavInputs               = 1 << 18,   // Нет навигации с помощью геймпада/клавиатуры в окне
    ImGuiWindowFlags_NoNavFocus                = 1 << 19,   // Не фокусируется на этом окне при навигации с помощью геймпада/клавиатуры (например, пропускается с помощью CTRL+TAB)
    ImGuiWindowFlags_UnsavedDocument           = 1 << 20,   // Поставьте точку рядом с заголовком. При использовании в контексте вкладки/стыковки вкладка выбирается при нажатии X + закрытие
                                                            // не предполагается (будет ждать, пока пользователь прекратит отправку вкладки). В противном случае предполагается закрытие
                                                            // при нажатии X, поэтому, если вы продолжите отправку, вкладка может снова появиться в конце панели вкладок.
    ImGuiWindowFlags_NoDocking                 = 1 << 21,   // Отключить стыковку этого окна

    ImGuiWindowFlags_NoNav                     = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration              = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs                  = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    // [Internal]
    ImGuiWindowFlags_NavFlattened              = 1 << 23,   // [BETA] В дочернем окне: разрешить навигацию геймпада/клавиатуры пересекать родительскую границу в этом дочернем окне или между одноуровневыми дочерними окнами.
    ImGuiWindowFlags_ChildWindow               = 1 << 24,   // Не используйте! Для внутреннего использования BeginChild()
    ImGuiWindowFlags_Tooltip                   = 1 << 25,   // Не используйте! Для внутреннего использования с помощью BeginTooltip()
    ImGuiWindowFlags_Popup                     = 1 << 26,   // Не используйте! Для внутреннего использования BeginPopup()
    ImGuiWindowFlags_Modal                     = 1 << 27,   // Не используйте! Для внутреннего использования с помощью BeginPopupModal()
    ImGuiWindowFlags_ChildMenu                 = 1 << 28,   // Не используйте! Для внутреннего использования с помощью BeginMenu()
    ImGuiWindowFlags_DockNodeHost              = 1 << 29    // Не используйте! Для внутреннего использования с помощью Begin()/NewFrame()

    // [Устаревший]
    //ImGuiWindowFlags_ResizeFromAnySide    = 1 << 17,  // [Устаревший] --> Set io.ConfigWindowsResizeFromEdges=true и убедитесь, что курсоры мыши поддерживаются серверной частью (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)
};

// Флаги для ImGui:: InputText()
enum ImGuiInputTextFlags_
{
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,    // Разрешить 0123456789.+-*/
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,    // Разрешить 0123456789ABCDEF abcdef
    ImGuiInputTextFlags_CharsUppercase      = 1 << 2,    // Превратите a.. z в A.. Z
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 3,    // Отфильтровать пробелы, вкладки
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 4,    // Выделить весь текст при первом фокусе мыши
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 5,    // Возвращает «true» при нажатии Enter (в отличие от каждого изменения значения). Рассмотрим функцию IsItemDeactivatedAfterEdit().
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 6,    // Обратный вызов при нажатии TAB (для обработки завершения)
    ImGuiInputTextFlags_CallbackHistory     = 1 << 7,    // Обратный вызов при нажатии стрелок вверх/вниз (для обработки истории)
    ImGuiInputTextFlags_CallbackAlways      = 1 << 8,    // Обратный вызов на каждой итерации. Пользовательский код может запрашивать позицию курсора, изменять текстовый буфер.
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 9,    // Обратный вызов при вводе символов, чтобы заменить или отбросить их. Измените 'EventChar', чтобы заменить или отбросить,
                                                         // или верните 1 в обратном вызове, чтобы отбросить.
    ImGuiInputTextFlags_AllowTabInput       = 1 << 10,   // Нажав клавишу TAB, введите символ '\t' в текстовое поле.
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,   // В многострочном режиме расфокусируйте с помощью Enter, добавьте новую строку с помощью Ctrl+Enter (по умолчанию наоборот:
                                                         // расфокусируйте с помощью Ctrl+Enter, добавьте строку с помощью Enter).
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 12,   // Отключить следование за курсором по горизонтали
    ImGuiInputTextFlags_AlwaysOverwrite     = 1 << 13,   // Режим перезаписи
    ImGuiInputTextFlags_ReadOnly            = 1 << 14,   // Режим только для чтения
    ImGuiInputTextFlags_Password            = 1 << 15,   // Режим пароля, отображать все символы как «*»
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,   // Отключить отмену/повтор. Обратите внимание, что входной текст владеет текстовыми данными, пока он активен, если вы хотите
                                                         // предоставить свой собственный стек отмены/повтора, вам понадобится e. г. для вызова ClearActiveID().
    ImGuiInputTextFlags_CharsScientific     = 1 << 17,   // Разрешить 0123456789.+-*/eE (ввод научной нотации)
    ImGuiInputTextFlags_CallbackResize      = 1 << 18,   // Обратный вызов при запросе изменения емкости буфера (за пределами значения параметра 'buf_size'), что позволяет увеличить строку.
                                                         // Уведомлять, когда требуется изменить размер строки (для типов строк, которые содержат кэш своего размера). Вам будет предоставлен
                                                         // новый BufSize в обратном вызове, и вам НЕОБХОДИМО соблюдать его. (см. misc/cpp/imgui_stdlib.h для примера использования этого)
    ImGuiInputTextFlags_CallbackEdit        = 1 << 19    // Обратный вызов при любом редактировании (обратите внимание, что InputText() уже возвращает true при редактировании,
                                                         // обратный вызов полезен в основном для управления базовым буфером, когда фокус активен)

    // Устаревшие имена (скоро будут удалены)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ,
    ImGuiInputTextFlags_AlwaysInsertMode = ImGuiInputTextFlags_AlwaysOverwrite   // [[renamed in 1.82] name was not matching behavior]переименовано в 1.82] имя не соответствовало поведению
#endif
};

// Флаги для ImGui::TreeNodeEx(), ImGui::CollapsingHeader*()
enum ImGuiTreeNodeFlags_
{
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,    // Рисуйте так, как выбрано
    ImGuiTreeNodeFlags_Framed               = 1 << 1,    // Отрисовка рамки с фоном (например, для CollapsingHeader)
    ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,    // Тестирование попадания, чтобы последующие виджеты могли перекрывать этот.
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,    // Не делайте TreePush() при открытии (например, для CollapsingHeader) = без дополнительного отступа и без нажатия на стек идентификаторов
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,    // Не открывать узел автоматически и временно, когда ведение журнала активно (по умолчанию ведение журнала автоматически открывает узлы дерева)
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,    // Узел по умолчанию должен быть открытым
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,    // Нужно дважды щелкнуть, чтобы открыть узел
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,    // Открывается только при нажатии на часть со стрелкой. Если ImGuiTreeNodeFlags_OpenOnDoubleClick также установлен, щелкните стрелку один раз или дважды щелкните все поле, чтобы открыть его.
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,    // Нет сворачивания, нет стрелки (используйте для удобства листовых узлов).
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,    // Отображать маркер вместо стрелки
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,   // Используйте FramePadding (даже для текстового узла без рамки), чтобы вертикально выровнять базовую линию текста по стандартной высоте виджета. Эквивалентно вызову AlignTextToFramePadding().
    ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,   // Расширьте поле попадания до самого правого края, даже если оно не в рамке. Это не значение по умолчанию, позволяющее добавлять другие элементы в ту же строку.
                                                         // В будущем мы можем реорганизовать систему попаданий, чтобы она шла от начала к концу, допуская естественные перекрытия, и тогда это может стать системой по умолчанию.
    ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,   // Расширьте область поиска до самого левого и самого правого краев (обходя область с отступом).
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,   // (WIP) Nav: левое направление может двигаться к этому TreeNode() из любого его дочернего элемента (элементы, представленные между TreeNode и TreePop)
    //ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: отключить автоматическую прокрутку в TreePop(), если узел только что открылся, а содержимое не видно.
    ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};

//  Флаги для функций OpenPopup*(), BeginPopupContext*(), IsPopupOpen().
// - Чтобы быть обратно совместимым со старым API, который принимал аргумент 'int mouse_button = 1', нам нужно обработать
//   маленькие значения флагов как индекс кнопки мыши, поэтому мы кодируем кнопку мыши в первых нескольких битах флагов.
//   Таким образом, передача индекса кнопки мыши в ImGuiPopupFlags гарантируется.
// - По той же причине мы в исключительных случаях по умолчанию используем для аргумента ImGuiPopupFlags функций BeginPopupContextXXX значение 1 вместо 0.
//   ВАЖНО: поскольку параметр по умолчанию равен 1 (==ImGuiPopupFlags_MouseButtonRight), если вы полагаетесь на параметр по умолчанию
//   и хотите другой флаг, вам нужно передать флаг ImGuiPopupFlags_MouseButtonRight.
// - Несколько кнопок в настоящее время не могут быть объединены/или объединены в этих функциях (мы могли бы разрешить это позже).
enum ImGuiPopupFlags_
{
    ImGuiPopupFlags_None                    = 0,
    ImGuiPopupFlags_MouseButtonLeft         = 0,   // Для BeginPopupContext*(): открыть при отпускании левой кнопки мыши. Гарантировано всегда == 0 (так же, как ImGuiMouseButton_Left)
    ImGuiPopupFlags_MouseButtonRight        = 1,   // Для BeginPopupContext*(): открыть при отпускании правой кнопки мыши. Гарантировано всегда == 1 (так же, как ImGuiMouseButton_Right)
    ImGuiPopupFlags_MouseButtonMiddle       = 2,   // Для BeginPopupContext*(): открыть при выпуске средней кнопки мыши. Гарантированно всегда == 2 (так же, как ImGuiMouseButton_Middle)
    ImGuiPopupFlags_MouseButtonMask_        = 0x1F,
    ImGuiPopupFlags_MouseButtonDefault_     = 1,
    ImGuiPopupFlags_NoOpenOverExistingPopup = 1 << 5,   // Для OpenPopup*(), BeginPopupContext*(): не открывать, если на том же уровне стека всплывающих окон уже есть всплывающее окно.
    ImGuiPopupFlags_NoOpenOverItems         = 1 << 6,   // Для BeginPopupContextWindow(): не возвращать true при наведении курсора на элементы, только при наведении курсора на пустое место.
    ImGuiPopupFlags_AnyPopupId              = 1 << 7,   // Для IsPopupOpen(): игнорируйте параметр ImGuiID и проверяйте всплывающие окна.
    ImGuiPopupFlags_AnyPopupLevel           = 1 << 8,   // Для IsPopupOpen(): поиск/проверка на любом уровне стека всплывающих окон (проверка по умолчанию на текущем уровне)
    ImGuiPopupFlags_AnyPopup                = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel
};

// Флаги для ImGui:: Selectable()
enum ImGuiSelectableFlags_
{
    ImGuiSelectableFlags_None             = 0,
    ImGuiSelectableFlags_DontClosePopups  = 1 << 0,   // Нажатие на это не закрывает родительское всплывающее окно
    ImGuiSelectableFlags_SpanAllColumns   = 1 << 1,   // Selectable кадр может охватывать все столбцы (текст по-прежнему будет помещаться в текущем столбце)
    ImGuiSelectableFlags_AllowDoubleClick = 1 << 2,   // Создавайте пресс-события и по двойным кликам
    ImGuiSelectableFlags_Disabled         = 1 << 3,   // Невозможно выбрать, отображать текст серым цветом
    ImGuiSelectableFlags_AllowItemOverlap = 1 << 4    // (WIP) Тестирование попадания, чтобы последующие виджеты могли перекрывать этот.
};

// Флаги для ImGui::BeginCombo()
enum ImGuiComboFlags_
{
    ImGuiComboFlags_None           = 0,
    ImGuiComboFlags_PopupAlignLeft = 1 << 0,   // По умолчанию выровняйте всплывающее окно по левому краю.
    ImGuiComboFlags_HeightSmall    = 1 << 1,   // Максимум ~4 видимых элемента. Совет: если вы хотите, чтобы всплывающее окно с комбо было определенного размера,
                                               // вы можете использовать SetNextWindowSizeConstraints() перед вызовом BeginCombo().
    ImGuiComboFlags_HeightRegular  = 1 << 2,   // Максимум ~8 видимых элементов (по умолчанию)
    ImGuiComboFlags_HeightLarge    = 1 << 3,   // Макс. ~20 видимых элементов
    ImGuiComboFlags_HeightLargest  = 1 << 4,   // Как можно больше подходящих предметов
    ImGuiComboFlags_NoArrowButton  = 1 << 5,   // Отображать в поле предварительного просмотра без кнопки с квадратной стрелкой
    ImGuiComboFlags_NoPreview      = 1 << 6,   // Отображать только кнопку с квадратной стрелкой
    ImGuiComboFlags_HeightMask_    = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest
};

// Флаги для ImGui::BeginTabBar()
enum ImGuiTabBarFlags_
{
    ImGuiTabBarFlags_None                         = 0,
    ImGuiTabBarFlags_Reorderable                  = 1 << 0,   // Разрешить перетаскивание вкладок вручную для изменения их порядка + Новые вкладки добавляются в конец списка
    ImGuiTabBarFlags_AutoSelectNewTabs            = 1 << 1,   // Автоматически выбирать новые вкладки при их появлении
    ImGuiTabBarFlags_TabListPopupButton           = 1 << 2,   // Отключить кнопки для открытия всплывающего окна со списком вкладок
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,   // Отключить поведение закрытия вкладок (которые отправляются с p_open != NULL) средней кнопкой мыши.
                                                              // Вы все еще можете воспроизвести это поведение на стороне пользователя с помощью
                                                              // if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ImGuiTabBarFlags_NoTabListScrollingButtons    = 1 << 4,   // Отключить кнопки прокрутки (применяется, когда политика подбора — ImGuiTabBarFlags_FittingPolicyScroll)
    ImGuiTabBarFlags_NoTooltip                    = 1 << 5,   // Отключить всплывающие подсказки при наведении на вкладку
    ImGuiTabBarFlags_FittingPolicyResizeDown      = 1 << 6,   // Изменение размера вкладок, если они не подходят
    ImGuiTabBarFlags_FittingPolicyScroll          = 1 << 7,   // Добавьте кнопки прокрутки, если вкладки не подходят
    ImGuiTabBarFlags_FittingPolicyMask_           = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_        = ImGuiTabBarFlags_FittingPolicyResizeDown
};

// Флаги для ImGui::BeginTabItem()
enum ImGuiTabItemFlags_
{
    ImGuiTabItemFlags_None                         = 0,
    ImGuiTabItemFlags_UnsavedDocument              = 1 << 0,   // Отображение точки рядом с заголовком + вкладка выбрана при нажатии X + закрытие не предполагается
                                                               // (будет ждать, пока пользователь перестанет отправлять вкладку). В противном случае предполагается закрытие
                                                               // при нажатии X, поэтому, если вы продолжите отправку, вкладка может снова появиться в конце панели вкладок.
    ImGuiTabItemFlags_SetSelected                  = 1 << 1,   // Флаг триггера для программного выбора вкладки при вызове BeginTabItem()
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,   // Отключить поведение закрытия вкладок (которые отправляются с p_open != NULL) средней кнопкой мыши.
                                                               // Вы все еще можете воспроизвести это поведение на стороне пользователя с помощью if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ImGuiTabItemFlags_NoPushId                     = 1 << 3,   // Не вызывать PushID(tab->ID)/PopID() для BeginTabItem()/EndTabItem()
    ImGuiTabItemFlags_NoTooltip                    = 1 << 4,   // Отключить всплывающую подсказку для данной вкладки
    ImGuiTabItemFlags_NoReorder                    = 1 << 5,   // Отключить изменение порядка этой вкладки или пересечение этой вкладки другой вкладкой
    ImGuiTabItemFlags_Leading                      = 1 << 6,   // Принудительное расположение вкладки слева от панели вкладок (после всплывающей кнопки со списком вкладок)
    ImGuiTabItemFlags_Trailing                     = 1 << 7    // Принудительное расположение вкладки справа от панели вкладок (перед кнопками прокрутки)
};

// Флаги для ImGui::BeginTable()
// - Важный! Политика изменения размера имеет сложные и незаметные побочные эффекты, гораздо более серьезные, чем можно было бы ожидать.
//   Внимательно прочитайте комментарии/демонстрации + поэкспериментируйте с живыми демонстрациями, чтобы ознакомиться с ними.
// - Политики размеров ПО УМОЛЧАНИЮ:
// - По умолчанию ImGuiTableFlags_SizingFixedFit, если ScrollX включен или если окно хоста имеет ImGuiWindowFlags_AlwaysAutoResize.
// - По умолчанию ImGuiTableFlags_SizingStretchSame, если ScrollX выключен.
// - Когда ScrollX выключен:
// - Таблица по умолчанию имеет значение ImGuiTableFlags_SizingStretchSame -> все столбцы по умолчанию имеют значение ImGuiTableColumnFlags_WidthStretch с одинаковым весом.
// - Допустимая политика размеров столбцов: Растянуть (по умолчанию), Фиксированная/Авто.
// - Фиксированные столбцы (если они есть) обычно получают требуемую ширину (если только таблица не может вместить их все).
// - Растянутые столбцы разделят оставшуюся ширину в соответствии с их соответствующим весом.
// — Смешанные фиксированные/растянутые столбцы возможны, но имеют различные побочные эффекты при изменении размера.
//   Типичное использование политик смешивания размеров: любое количество ВЕДУЩИХ фиксированных столбцов, за которыми следуют один или два ТРЕЙЛИНГОВЫХ растянутых столбца.
//   (это потому, что видимый порядок столбцов имеет тонкое, но необходимое влияние на то, как они реагируют на ручное изменение размера).
// - Когда ScrollX включен:
// - Таблица по умолчанию имеет значение ImGuiTableFlags_SizingFixedFit -> все столбцы по умолчанию имеют значение ImGuiTableColumnFlags_WidthFixed
// - Допустимая политика размеров столбцов: в основном фиксированная/автоматическая.
// - Фиксированные столбцы могут быть увеличены по мере необходимости. Таблица покажет горизонтальную полосу прокрутки, если это необходимо.
// - При использовании фиксированных столбцов с автоматическим изменением размера (неизменяемого размера) запрос ширины содержимого для использования выравнивания элемента
//   по правому краю e. г. SetNextItemWidth(-FLT_MIN) не имеет смысла, создаст цикл обратной связи.
// - Использование столбцов Stretch ЧАСТО НЕ ИМЕЕТ СМЫСЛА, если ScrollX включен, ЕСЛИ вы не указали значение для 'inner_width' в BeginTable().
//   Если вы укажете значение для 'inner_width', тогда фактически пространство прокрутки будет известно, и столбцы Stretch или смешанные столбцы Fixed/Stretch снова станут значимыми.
// - Прочтите документацию вверху imgui_tables. cpp для получения подробной информации.
enum ImGuiTableFlags_
{
    // Функции
    ImGuiTableFlags_None                       = 0,
    ImGuiTableFlags_Resizable                  = 1 << 0,   // Включить изменение размера столбцов.
    ImGuiTableFlags_Reorderable                = 1 << 1,   // Eвозможность переупорядочивания столбцов в строке заголовка (необходим вызов TableSetupColumn() + TableHeadersRow() для отображения заголовков)
    ImGuiTableFlags_Hideable                   = 1 << 2,   // Включить скрытие/отключение столбцов в контекстном меню.
    ImGuiTableFlags_Sortable                   = 1 << 3,   // Включить сортировку. Вызовите TableGetSortSpecs(), чтобы получить спецификации сортировки. См. также ImGuiTableFlags_SortMulti и ImGuiTableFlags_SortTristate.
    ImGuiTableFlags_NoSavedSettings            = 1 << 4,   // Отключите сохраняющиеся параметры порядка, ширины и сортировки столбцов в файле . ини файл.
    ImGuiTableFlags_ContextMenuInBody          = 1 << 5,   // Щелкните правой кнопкой мыши тело/содержимое столбцов, чтобы отобразить контекстное меню таблицы. По умолчанию он доступен в TableHeadersRow().
    // украшения
    ImGuiTableFlags_RowBg                      = 1 << 6,                                                          // Установите каждый цвет RowBg с помощью ImGuiCol_TableRowBg или ImGuiCol_TableRowBgAlt (эквивалент вызова TableSetBgColor с ImGuiTableBgFlags_RowBg0 для каждой строки вручную)
    ImGuiTableFlags_BordersInnerH              = 1 << 7,                                                          // Нарисуйте горизонтальные границы между рядами.
    ImGuiTableFlags_BordersOuterH              = 1 << 8,                                                          // Нарисуйте горизонтальные границы сверху и снизу.
    ImGuiTableFlags_BordersInnerV              = 1 << 9,                                                          // Нарисуйте вертикальные границы между столбцами.
    ImGuiTableFlags_BordersOuterV              = 1 << 10,                                                         // Нарисуйте вертикальные границы слева и справа.
    ImGuiTableFlags_BordersH                   = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH,   // Нарисуйте горизонтальные границы.
    ImGuiTableFlags_BordersV                   = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV,   // Нарисуйте вертикальные границы.
    ImGuiTableFlags_BordersInner               = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH,   // Нарисуйте внутренние границы.
    ImGuiTableFlags_BordersOuter               = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH,   // Нарисуйте внешние границы.
    ImGuiTableFlags_Borders                    = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter,     // Нарисуйте все границы.
    ImGuiTableFlags_NoBordersInBody            = 1 << 11,                                                         // [ALPHA] Отключите вертикальные границы в основной части столбцов (границы всегда будут отображаться в заголовках). -> Может перейти в стиль
    ImGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,                                                         // [ALPHA] Отключите вертикальные границы в столбцах Body, пока не наведете курсор для изменения размера
                                                                                                                  // (границы всегда будут отображаться в заголовках). -> Может перейти в стиль
    // Политика изменения размера (о значениях по умолчанию см. выше)
    ImGuiTableFlags_SizingFixedFit             = 1 << 13,   // Столбцы по умолчанию имеют значение _WidthFixed или _WidthAuto (если размер может изменяться или не изменяться), что соответствует ширине содержимого.
    ImGuiTableFlags_SizingFixedSame            = 2 << 13,   // Столбцы по умолчанию имеют значение _WidthFixed или _WidthAuto (если размер изменяется или не изменяется), что соответствует
                                                            // максимальной ширине содержимого всех столбцов. Неявно включите ImGuiTableFlags_NoKeepColumnsVisible.
    ImGuiTableFlags_SizingStretchProp          = 3 << 13,   // Столбцы по умолчанию имеют значение _WidthStretch с весами по умолчанию, пропорциональными ширине содержимого каждого столбца.
    ImGuiTableFlags_SizingStretchSame          = 4 << 13,   // Столбцы по умолчанию имеют значение _WidthStretch с одинаковыми весами по умолчанию, если только они не переопределены с помощью TableSetupColumn().
    // Дополнительные параметры размера
    ImGuiTableFlags_NoHostExtendX              = 1 << 16,   // Сделайте так, чтобы внешняя ширина автоматически подгонялась под столбцы, переопределяя external_size. х значение.
                                                            // Доступно, только если ScrollX/ScrollY отключены и столбцы Stretch не используются.
    ImGuiTableFlags_NoHostExtendY              = 1 << 17,   // Сделайте так, чтобы внешняя высота останавливалась точно на external_size. y (предотвратить автоматическое расширение таблицы за предел).
                                                            // Доступно, только если ScrollX/ScrollY отключены. Данные ниже предела будут обрезаны и не видны.
    ImGuiTableFlags_NoKeepColumnsVisible       = 1 << 18,   // Отключить отображение столбца всегда минимально видимым, когда ScrollX выключен, а таблица становится слишком маленькой.
                                                            // Не рекомендуется, если размер столбцов можно изменить.
    ImGuiTableFlags_PreciseWidths              = 1 << 19,   // Отключить распределение остаточной ширины на растянутые столбцы (выделение ширины для таблицы шириной 100 с 3 столбцами:
                                                            // Без этого флага: 33,33,34. С этим флагом: 33,33,33). При большем количестве столбцов изменение размера будет менее плавным.
    // Clipping
    ImGuiTableFlags_NoClip                     = 1 << 20,   // Отключите прямоугольник обрезки для каждого отдельного столбца (уменьшите количество команд рисования,
                                                            // элементы смогут перетекать в другие столбцы). Обычно несовместим с TableSetupScrollFreeze().
    // Padding
    ImGuiTableFlags_PadOuterX                  = 1 << 21,   // По умолчанию, если BordersOuterV включен. Включить самое внешнее заполнение. Вообще желательно, если у вас есть заголовки.
    ImGuiTableFlags_NoPadOuterX                = 1 << 22,   // По умолчанию, если BordersOuterV выключен. Отключите самое внешнее заполнение.
    ImGuiTableFlags_NoPadInnerX                = 1 << 23,   // Отключите внутреннее заполнение между столбцами (двойное внутреннее заполнение, если BordersOuterV включено,
                                                            // одиночное внутреннее заполнение, если BordersOuterV отключено).
    // Scrolling
    ImGuiTableFlags_ScrollX                    = 1 << 24,   // Включите горизонтальную прокрутку. Требовать параметр «outer_size» в BeginTable() для указания размера контейнера.
                                                            // Изменяет политику размеров по умолчанию.
                                                            // Поскольку при этом создается дочернее окно, ScrollY в настоящее время обычно рекомендуется при использовании ScrollX.
    ImGuiTableFlags_ScrollY                    = 1 << 25,   // Включить вертикальную прокрутку. Требовать параметр «outer_size» в BeginTable() для указания размера контейнера.
    // Sorting
    ImGuiTableFlags_SortMulti                  = 1 << 26,   // Удерживайте Shift при нажатии на заголовки для сортировки по нескольким столбцам. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount > 1).
    ImGuiTableFlags_SortTristate               = 1 << 27,   // Разрешить сортировку, отключить сортировку по умолчанию. TableGetSortSpecs() может возвращать спецификации, где (SpecsCount == 0).

    // [Internal] Комбинации и маски
    ImGuiTableFlags_SizingMask_                = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame

    // Устаревшие имена (скоро будут удалены)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    //, ImGuiTableFlags_ColumnsWidthFixed = ImGuiTableFlags_SizingFixedFit, ImGuiTableFlags_ColumnsWidthStretch = ImGuiTableFlags_SizingStretchSame   // WIP Таблицы 2020/12
    //, ImGuiTableFlags_SizingPolicyFixed = ImGuiTableFlags_SizingFixedFit, ImGuiTableFlags_SizingPolicyStretch = ImGuiTableFlags_SizingStretchSame   // WIP Таблицы 2021/01
#endif
};

// Флаги для ImGui::TableSetupColumn()
enum ImGuiTableColumnFlags_
{
    // Входные флаги конфигурации
    ImGuiTableColumnFlags_None                 = 0,
    ImGuiTableColumnFlags_Disabled             = 1 << 0,    // Флаг переопределения/основного отключения: скрыть столбец, не будет отображаться в контекстном меню
                                                            // (в отличие от вызова TableSetColumnEnabled(), который манипулирует доступным для пользователя состоянием)
    ImGuiTableColumnFlags_DefaultHide          = 1 << 1,    // По умолчанию как скрытый/отключенный столбец.
    ImGuiTableColumnFlags_DefaultSort          = 1 << 2,    // По умолчанию в качестве столбца сортировки.
    ImGuiTableColumnFlags_WidthStretch         = 1 << 3,    // Колонка растянется. Предпочтительно с отключенной горизонтальной прокруткой
                                                            // (по умолчанию, если политика размера таблицы — _SizingStretchSame или _SizingStretchProp).
    ImGuiTableColumnFlags_WidthFixed           = 1 << 4,    // Колонка не растянется. Предпочтительно с включенной горизонтальной прокруткой
                                                            // (по умолчанию, если политика размера таблицы _SizingFixedFit и таблица может изменять размер).
    ImGuiTableColumnFlags_NoResize             = 1 << 5,    // Отключите ручное изменение размера.
    ImGuiTableColumnFlags_NoReorder            = 1 << 6,    // Отключите ручное изменение порядка этого столбца, это также предотвратит пересечение этого столбца другими столбцами.
    ImGuiTableColumnFlags_NoHide               = 1 << 7,    // Отключить возможность скрыть/отключить этот столбец.
    ImGuiTableColumnFlags_NoClip               = 1 << 8,    // Отключите обрезку для этого столбца (все столбцы NoClip будут отображаться в одной и той же команде рисования).
    ImGuiTableColumnFlags_NoSort               = 1 << 9,    // Отключить возможность сортировки по этому полю (даже если для таблицы установлен ImGuiTableFlags_Sortable).
    ImGuiTableColumnFlags_NoSortAscending      = 1 << 10,   // Отключить возможность сортировки по возрастанию.
    ImGuiTableColumnFlags_NoSortDescending     = 1 << 11,   // Отключить возможность сортировки по убыванию.
    ImGuiTableColumnFlags_NoHeaderLabel        = 1 << 12,   // TableHeadersRow() не будет отправлять метку для этого столбца. Удобно для некоторых небольших колонок. Имя по-прежнему будет отображаться в контекстном меню.
    ImGuiTableColumnFlags_NoHeaderWidth        = 1 << 13,   // Отключить вклад ширины текста заголовка в автоматическую ширину столбца.
    ImGuiTableColumnFlags_PreferSortAscending  = 1 << 14,   // Сделать начальное направление сортировки по возрастанию при первой сортировке в этом столбце (по умолчанию).
    ImGuiTableColumnFlags_PreferSortDescending = 1 << 15,   // Сделайте начальное направление сортировки по убыванию при первой сортировке в этом столбце.
    ImGuiTableColumnFlags_IndentEnable         = 1 << 16,   // Использовать текущее значение отступа при вводе ячейки (по умолчанию для столбца 0).
    ImGuiTableColumnFlags_IndentDisable        = 1 << 17,   // Игнорировать текущее значение отступа при вводе ячейки (по умолчанию для столбцов > 0). Изменения отступа _внутри_ ячейки будут по-прежнему учитываться.

    // Флаги состояния вывода, только для чтения через TableGetColumnFlags()
    ImGuiTableColumnFlags_IsEnabled            = 1 << 24,   // Состояние: включено == не скрыто флагами пользователя/API (называемого «Скрыть» в _DefaultHide и _NoHide).
    ImGuiTableColumnFlags_IsVisible            = 1 << 25,   // Статус: виден == включен И не обрезается прокруткой.
    ImGuiTableColumnFlags_IsSorted             = 1 << 26,   // Статус: в настоящее время является частью спецификации сортировки
    ImGuiTableColumnFlags_IsHovered            = 1 << 27,   // Статус: наведен мышкой

    // [Internal] Комбинации и маски
    ImGuiTableColumnFlags_WidthMask_           = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed,
    ImGuiTableColumnFlags_IndentMask_          = ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable,
    ImGuiTableColumnFlags_StatusMask_          = ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered,
    ImGuiTableColumnFlags_NoDirectResize_      = 1 << 30   // [Internal] Отключить непосредственное изменение пользователем размера этого столбца
                                                           // (однако мы можем косвенно изменить размер этого столбца от его левого края)

    // Устаревшие имена (скоро будут удалены)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    //ImGuiTableColumnFlags_WidthAuto           = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, // Столбец не будет растягиваться и будет изменять размер в зависимости от отправленного содержимого.
#endif
};

// Флаги для ImGui::TableNextRow()
enum ImGuiTableRowFlags_
{
    ImGuiTableRowFlags_None    = 0,
    ImGuiTableRowFlags_Headers = 1 << 0   // Определите строку заголовка (установите цвет фона по умолчанию + ширина его содержимого, учитываемая по-разному для автоматической ширины столбца)
};

// Enum for ImGui::TableSetBgColor()
// Цвета фона рендерятся в 3 слоя:
//  - Слой 0: рисуйте цветом RowBg0, если установлен, в противном случае рисуйте цветом ColumnBg0, если установлен.
//  - Слой 1: рисовать цветом RowBg1, если установлен, в противном случае рисовать цветом ColumnBg1, если установлен.
//  - Слой 2: рисовать цветом CellBg, если он установлен.
// Цель двух слоев строк/столбцов — позволить вам решить, должны ли изменения цвета фона переопределять или смешиваться с существующим цветом.
// При использовании ImGuiTableFlags_RowBg в таблице каждая строка имеет цвет RowBg0, автоматически устанавливаемый для нечетных/четных строк.
// Если вы установите цвет цели RowBg0, ваш цвет переопределит существующий цвет RowBg0.
// Если вы установите цвет цели RowBg1 или ColumnBg1, ваш цвет будет смешиваться с цветом RowBg0.
enum ImGuiTableBgTarget_
{
    ImGuiTableBgTarget_None   = 0,
    ImGuiTableBgTarget_RowBg0 = 1,   // Установите цвет фона строки 0 (обычно используется для фона, устанавливается автоматически при использовании ImGuiTableFlags_RowBg)
    ImGuiTableBgTarget_RowBg1 = 2,   // Установите цвет фона строки 1 (обычно используется для маркировки выбора)
    ImGuiTableBgTarget_CellBg = 3    // Установить цвет фона ячейки (самый верхний цвет)
};

// Флаги для ImGui::IsWindowFocused()
enum ImGuiFocusedFlags_
{
    ImGuiFocusedFlags_None                = 0,
    ImGuiFocusedFlags_ChildWindows        = 1 << 0,   // Возвратите true, если какие-либо дочерние элементы окна сфокусированы
    ImGuiFocusedFlags_RootWindow          = 1 << 1,   // Тестировать из корневого окна (самого верхнего родителя текущей иерархии)
    ImGuiFocusedFlags_AnyWindow           = 1 << 2,   // Возвращает true, если какое-либо окно находится в фокусе. Важно: если вы пытаетесь указать, как отправлять
                                                      // низкоуровневые входные данные, НЕ используйте это. Используйте «ио. Вместо этого WantCaptureMouse! Пожалуйста, прочтите FAQ!
    ImGuiFocusedFlags_NoPopupHierarchy    = 1 << 3,   // Не учитывать иерархию всплывающих окон (не рассматривать эмиттер всплывающих окон как родитель всплывающего окна)
                                                      // (при использовании с _ChildWindows или _RootWindow)
    ImGuiFocusedFlags_DockHierarchy       = 1 << 4,   // Рассмотрите иерархию стыковки (рассматривайте хост пространства док-станции как родительский для закрепленного окна)
                                                      // (при использовании с _ChildWindows или _RootWindow)
    ImGuiFocusedFlags_RootAndChildWindows = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows
};

// Флаги для ImGui::IsItemHovered(), ImGui::IsWindowHovered()
// Примечание: если вы пытаетесь проверить, должна ли ваша мышь быть отправлена ​​​​в Dear ImGui или в ваше приложение, вы должны использовать «io. Вместо этого WantCaptureMouse! Пожалуйста, прочтите FAQ!
// Примечание: окна с флагом ImGuiWindowFlags_NoInputs игнорируются вызовами IsWindowHovered().
enum ImGuiHoveredFlags_
{
    ImGuiHoveredFlags_None                         = 0,        // Вернуть true, если прямо над элементом/окном, не загороженным другим окном, не заблокированным активным
                                                               // всплывающим окном или модальным блокирующим вводом под ними.
    ImGuiHoveredFlags_ChildWindows                 = 1 << 0,   // IsWindowHovered() only: Возвращает true, если какие-либо дочерние элементы окна наведены
    ImGuiHoveredFlags_RootWindow                   = 1 << 1,   // IsWindowHovered() only: Тестировать из корневого окна (самого верхнего родителя текущей иерархии)
    ImGuiHoveredFlags_AnyWindow                    = 1 << 2,   // IsWindowHovered() only: Возвращает true, если какое-либо окно наведено
    ImGuiHoveredFlags_NoPopupHierarchy             = 1 << 3,   // IsWindowHovered() only: Не учитывать иерархию всплывающих окон (не рассматривать эмиттер всплывающих окон
                                                               // как родитель всплывающего окна) (при использовании с _ChildWindows или _RootWindow)
    ImGuiHoveredFlags_DockHierarchy                = 1 << 4,   // IsWindowHovered() only: Рассмотрите иерархию стыковки (рассматривайте хост пространства док-станции
                                                               // как родительский для закрепленного окна) (при использовании с _ChildWindows или _RootWindow)
    ImGuiHoveredFlags_AllowWhenBlockedByPopup      = 1 << 5,   // Вернуть true, даже если всплывающее окно обычно блокирует доступ к этому элементу/окну.
    //ImGuiHoveredFlags_AllowWhenBlockedByModal    = 1 << 6,  // Верните true, даже если модальное всплывающее окно обычно блокирует доступ к этому элементу/окну. FIXME-TODO: пока недоступно.
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,    // Возвращает true, даже если активный элемент блокирует доступ к этому элементу/окну. Полезно для шаблонов перетаскивания.
    ImGuiHoveredFlags_AllowWhenOverlapped          = 1 << 8,    // IsItemHovered() only: Возвращает true, даже если позиция закрыта или перекрывается другим окном
    ImGuiHoveredFlags_AllowWhenDisabled            = 1 << 9,    // IsItemHovered() only: Вернуть true, даже если элемент отключен
    ImGuiHoveredFlags_NoNavOverride                = 1 << 10,   // Отключить использование состояния навигации геймпада/клавиатуры, когда активно, всегда запрашивать мышь.
    ImGuiHoveredFlags_RectOnly                     = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows          = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows
};

// Флаги для ImGui::DockSpace(), совместно используются/унаследованы дочерними узлами.
// (Некоторые флаги могут быть применены непосредственно к отдельным узлам)
// FIXME-DOCK: См. также ImGuiDockNodeFlagsPrivate_, который может включать использование WIP и внутреннего API DockBuilder.
enum ImGuiDockNodeFlags_
{
    ImGuiDockNodeFlags_None                   = 0,
    ImGuiDockNodeFlags_KeepAliveOnly          = 1 << 0,   // Shared  // Не отображайте узел док-пространства, но оставьте его активным. Windows, пристыкованные к этому узлу пространства доков, не будут отстыкованы.
    //ImGuiDockNodeFlags_NoCentralNode              = 1 << 1,   // Shared  // Отключить центральный узел (узел, который может оставаться пустым)
    ImGuiDockNodeFlags_NoDockingInCentralNode = 1 << 2,   // Shared  // Отключите стыковку внутри центрального узла, который всегда будет оставаться пустым.
    ImGuiDockNodeFlags_PassthruCentralNode    = 1 << 3,   // Shared  // Включить сквозное пространство дока:
                                                          // 1) DockSpace() будет отображать фон ImGuiCol_WindowBg, покрывающий все, кроме центрального узла, когда он пуст.
                                                          // Это означает, что окно хоста, вероятно, должно использовать SetNextWindowBgAlpha(0.0f) до Begin() при использовании этого.
                                                          // 2) Когда центральный узел пуст: разрешить прохождение входных данных + не отображать фон DockingEmptyBg. Подробности смотрите в демо.
    ImGuiDockNodeFlags_NoSplit                = 1 << 4,   // Shared/Local // Отключите разделение узла на более мелкие узлы. Полезный эл. г. при встраивании док-пространств
                                                          // в основное корневое (в корневом может быть отключено разделение, чтобы избежать путаницы).
                                                          // Примечание: при отключении существующие разделения будут сохранены.
    ImGuiDockNodeFlags_NoResize               = 1 << 5,   // Shared/Local // Отключите изменение размера узла с помощью разделителя/разделителей. Полезно с программно настроенными пространствами доков.
    ImGuiDockNodeFlags_AutoHideTabBar         = 1 << 6    // Shared/Local // Панель вкладок автоматически скрывается, когда в узле док-станции есть одно окно.
};

// Флаги для ImGui::BeginDragDropSource(), ImGui::AcceptDragDropPayload()
enum ImGuiDragDropFlags_
{
    ImGuiDragDropFlags_None                     = 0,
    // BeginDragDropSource() flags
    ImGuiDragDropFlags_SourceNoPreviewTooltip   = 1 << 0,   // По умолчанию успешный вызов BeginDragDropSource открывает всплывающую подсказку, позволяющую отобразить предварительный просмотр или описание исходного содержимого. Этот флаг отключает это поведение.
    ImGuiDragDropFlags_SourceNoDisableHover     = 1 << 1,   // По умолчанию при перетаскивании мы очищаем данные, чтобы IsItemHovered() возвращал false, чтобы избежать отправки всплывающих подсказок последующим кодом пользователя. Этот флаг отключает это поведение, чтобы вы могли по-прежнему вызывать IsItemHovered() для исходного элемента.
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,   // Отключить поведение, которое позволяет открывать узлы дерева и сворачивать заголовок, удерживая их при перетаскивании исходного элемента.
    ImGuiDragDropFlags_SourceAllowNullID        = 1 << 3,   // Разрешить использование таких элементов, как Text(), Image(), не имеющих уникального идентификатора, в качестве источника перетаскивания путем создания временного идентификатора на основе их положения относительно окна. Это крайне необычно для дорогой экосистемы imgui, поэтому мы сделали это явным.
    ImGuiDragDropFlags_SourceExtern             = 1 << 4,   // Внешний источник (за пределами дорогой imgui), не будет пытаться прочитать информацию о текущем элементе/окне. Всегда будет возвращать true. Одновременно может быть активен только один внешний источник.
    ImGuiDragDropFlags_SourceAutoExpirePayload  = 1 << 5,   // Автоматически завершать полезную нагрузку, если источник перестает отправляться (в противном случае полезные нагрузки сохраняются при перетаскивании)
    // AcceptDragDropPayload() flags
    ImGuiDragDropFlags_AcceptBeforeDelivery     = 1 << 10,                                                                               // AcceptDragDropPayload() вернет true еще до того, как кнопка мыши будет отпущена. Затем вы можете вызвать IsDelivery(), чтобы проверить, нужно ли доставлять полезную нагрузку.
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect  = 1 << 11,                                                                               // Не рисовать прямоугольник выделения по умолчанию при наведении курсора на цель.
    ImGuiDragDropFlags_AcceptNoPreviewTooltip   = 1 << 12,                                                                               // Запрос на скрытие всплывающей подсказки BeginDragDropSource с сайта BeginDragDropTarget.
    ImGuiDragDropFlags_AcceptPeekOnly           = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect   // Для просмотра и проверки полезной нагрузки перед доставкой.
};

// Стандартные типы полезной нагрузки Drag and Drop. Вы можете определить свои собственные типы полезной нагрузки, используя короткие строки. Типы, начинающиеся с '_', определяются Уважаемым ImGui.
#define IMGUI_PAYLOAD_TYPE_COLOR_3F "_COL3F"   // float[3]: Стандартный тип для цветов, без альфы. Код пользователя может использовать этот тип.
#define IMGUI_PAYLOAD_TYPE_COLOR_4F "_COL4F"   // float[4]: Стандартный тип для цветов. Код пользователя может использовать этот тип.

// Основной тип данных
enum ImGuiDataType_
{
    ImGuiDataType_S8,       // подписанный char/char (с толковыми компиляторами)
    ImGuiDataType_U8,       // беззнаковый символ
    ImGuiDataType_S16,      // короткий
    ImGuiDataType_U16,      // неподписанный короткий
    ImGuiDataType_S32,      // int
    ImGuiDataType_U32,      // unsigned int
    ImGuiDataType_S64,      // long long / __int64
    ImGuiDataType_U64,      // unsigned long long / unsigned __int64
    ImGuiDataType_Float,    // float
    ImGuiDataType_Double,   // double
    ImGuiDataType_COUNT
};

// Основное направление
enum ImGuiDir_
{
    ImGuiDir_None  = -1,
    ImGuiDir_Left  = 0,
    ImGuiDir_Right = 1,
    ImGuiDir_Up    = 2,
    ImGuiDir_Down  = 3,
    ImGuiDir_COUNT
};

// Направление сортировки
enum ImGuiSortDirection_
{
    ImGuiSortDirection_None       = 0,
    ImGuiSortDirection_Ascending  = 1,   // По возрастанию = 0->9, A->Z и т.д.
    ImGuiSortDirection_Descending = 2    // По убыванию = 9->0, Z->A и т.д.
};

enum ImGuiKey_
{
    // Клавиатура
    ImGuiKey_None = 0,
    ImGuiKey_Tab  = 512,   // == ImGuiKey_NamedKey_BEGIN
    ImGuiKey_LeftArrow,
    ImGuiKey_RightArrow,
    ImGuiKey_UpArrow,
    ImGuiKey_DownArrow,
    ImGuiKey_PageUp,
    ImGuiKey_PageDown,
    ImGuiKey_Home,
    ImGuiKey_End,
    ImGuiKey_Insert,
    ImGuiKey_Delete,
    ImGuiKey_Backspace,
    ImGuiKey_Space,
    ImGuiKey_Enter,
    ImGuiKey_Escape,
    ImGuiKey_LeftCtrl,
    ImGuiKey_LeftShift,
    ImGuiKey_LeftAlt,
    ImGuiKey_LeftSuper,
    ImGuiKey_RightCtrl,
    ImGuiKey_RightShift,
    ImGuiKey_RightAlt,
    ImGuiKey_RightSuper,
    ImGuiKey_Menu,
    ImGuiKey_0,
    ImGuiKey_1,
    ImGuiKey_2,
    ImGuiKey_3,
    ImGuiKey_4,
    ImGuiKey_5,
    ImGuiKey_6,
    ImGuiKey_7,
    ImGuiKey_8,
    ImGuiKey_9,
    ImGuiKey_A,
    ImGuiKey_B,
    ImGuiKey_C,
    ImGuiKey_D,
    ImGuiKey_E,
    ImGuiKey_F,
    ImGuiKey_G,
    ImGuiKey_H,
    ImGuiKey_I,
    ImGuiKey_J,
    ImGuiKey_K,
    ImGuiKey_L,
    ImGuiKey_M,
    ImGuiKey_N,
    ImGuiKey_O,
    ImGuiKey_P,
    ImGuiKey_Q,
    ImGuiKey_R,
    ImGuiKey_S,
    ImGuiKey_T,
    ImGuiKey_U,
    ImGuiKey_V,
    ImGuiKey_W,
    ImGuiKey_X,
    ImGuiKey_Y,
    ImGuiKey_Z,
    ImGuiKey_F1,
    ImGuiKey_F2,
    ImGuiKey_F3,
    ImGuiKey_F4,
    ImGuiKey_F5,
    ImGuiKey_F6,
    ImGuiKey_F7,
    ImGuiKey_F8,
    ImGuiKey_F9,
    ImGuiKey_F10,
    ImGuiKey_F11,
    ImGuiKey_F12,
    ImGuiKey_Apostrophe,     // '
    ImGuiKey_Comma,          // ,
    ImGuiKey_Minus,          // -
    ImGuiKey_Period,         // .
    ImGuiKey_Slash,          // /
    ImGuiKey_Semicolon,      // ;
    ImGuiKey_Equal,          // =
    ImGuiKey_LeftBracket,    // [
    ImGuiKey_Backslash,      // \ (этот текст запрещает многострочный комментарий, вызванный обратной косой чертой)
    ImGuiKey_RightBracket,   // ]
    ImGuiKey_GraveAccent,    // `
    ImGuiKey_CapsLock,
    ImGuiKey_ScrollLock,
    ImGuiKey_NumLock,
    ImGuiKey_PrintScreen,
    ImGuiKey_Pause,
    ImGuiKey_Keypad0,
    ImGuiKey_Keypad1,
    ImGuiKey_Keypad2,
    ImGuiKey_Keypad3,
    ImGuiKey_Keypad4,
    ImGuiKey_Keypad5,
    ImGuiKey_Keypad6,
    ImGuiKey_Keypad7,
    ImGuiKey_Keypad8,
    ImGuiKey_Keypad9,
    ImGuiKey_KeypadDecimal,
    ImGuiKey_KeypadDivide,
    ImGuiKey_KeypadMultiply,
    ImGuiKey_KeypadSubtract,
    ImGuiKey_KeypadAdd,
    ImGuiKey_KeypadEnter,
    ImGuiKey_KeypadEqual,

    // Геймпад (некоторые из них являются аналоговыми значениями, 0.0f to 1.0f)                              // NAVIGATION action
    ImGuiKey_GamepadStart,         // Menu (Xbox)          + (Switch)   Start/Options (PS) // --
    ImGuiKey_GamepadBack,          // View (Xbox)          - (Switch)   Share (PS)         // --
    ImGuiKey_GamepadFaceUp,        // Y (Xbox)             X (Switch)   Triangle (PS)      // -> ImGuiNavInput_Input
    ImGuiKey_GamepadFaceDown,      // A (Xbox)             B (Switch)   Cross (PS)         // -> ImGuiNavInput_Activate
    ImGuiKey_GamepadFaceLeft,      // X (Xbox)             Y (Switch)   Square (PS)        // -> ImGuiNavInput_Menu
    ImGuiKey_GamepadFaceRight,     // B (Xbox)             A (Switch)   Circle (PS)        // -> ImGuiNavInput_Cancel
    ImGuiKey_GamepadDpadUp,        // D-pad Up                                             // -> ImGuiNavInput_DpadUp
    ImGuiKey_GamepadDpadDown,      // D-pad Down                                           // -> ImGuiNavInput_DpadDown
    ImGuiKey_GamepadDpadLeft,      // D-pad Left                                           // -> ImGuiNavInput_DpadLeft
    ImGuiKey_GamepadDpadRight,     // D-pad Right                                          // -> ImGuiNavInput_DpadRight
    ImGuiKey_GamepadL1,            // L Bumper (Xbox)      L (Switch)   L1 (PS)            // -> ImGuiNavInput_FocusPrev + ImGuiNavInput_TweakSlow
    ImGuiKey_GamepadR1,            // R Bumper (Xbox)      R (Switch)   R1 (PS)            // -> ImGuiNavInput_FocusNext + ImGuiNavInput_TweakFast
    ImGuiKey_GamepadL2,            // L Trigger (Xbox)     ZL (Switch)  L2 (PS) [Analog]
    ImGuiKey_GamepadR2,            // R Trigger (Xbox)     ZR (Switch)  R2 (PS) [Analog]
    ImGuiKey_GamepadL3,            // L Thumbstick (Xbox)  L3 (Switch)  L3 (PS)
    ImGuiKey_GamepadR3,            // R Thumbstick (Xbox)  R3 (Switch)  R3 (PS)
    ImGuiKey_GamepadLStickUp,      // [Analog]                                             // -> ImGuiNavInput_LStickUp
    ImGuiKey_GamepadLStickDown,    // [Analog]                                             // -> ImGuiNavInput_LStickDown
    ImGuiKey_GamepadLStickLeft,    // [Analog]                                             // -> ImGuiNavInput_LStickLeft
    ImGuiKey_GamepadLStickRight,   // [Analog]                                             // -> ImGuiNavInput_LStickRight
    ImGuiKey_GamepadRStickUp,      // [Analog]
    ImGuiKey_GamepadRStickDown,    // [Analog]
    ImGuiKey_GamepadRStickLeft,    // [Analog]
    ImGuiKey_GamepadRStickRight,   // [Analog]

    // Модификаторы клавиатуры (явно отправленные серверной частью через вызовы AddKeyEvent())
    // - Это зеркальное отображение данных, также записанных в io. KeyCtrl, ио. KeyShift, ио. KeyAlt, и.о. KeySuper в формате, позволяющем
    //   к ним можно получить доступ через стандартный API ключей, что позволяет выполнять такие вызовы, как IsKeyPressed(), IsKeyReleased(), продолжительность запроса и т. д.
    // - код, опрашивающий каждую клавишу (например, интерфейс для обнаружения нажатия клавиши для сопоставления ввода), может захотеть игнорировать эти
    //   и предпочитаем использовать настоящие клавиши (например, ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl вместо ImGuiKey_ModCtrl).
    // - Теоретически значение модификаторов клавиатуры должно быть примерно эквивалентно логической или эквивалентной левой/правой клавишам.
    //   На практике: сложно; моды часто предоставляются из разных источников. Раскладка клавиатуры, IME, залипание клавиш и
    //   серверные части имеют тенденцию вмешиваться и нарушать эту эквивалентность. Более безопасное решение - передать эту двусмысленность конечному пользователю...
    ImGuiKey_ModCtrl,
    ImGuiKey_ModShift,
    ImGuiKey_ModAlt,
    ImGuiKey_ModSuper,

    // Конец списка
    ImGuiKey_COUNT,   // Ни один допустимый ImGuiKey не превышает это значение.

    // [Internal] До версии 1.87 мы требовали от пользователя заполнения io. KeysDown[512] с использованием собственного собственного индекса + a io. Массив KeyMap[].
    // Мы отказываемся от этого метода, но сохраняем устаревший путь для пользовательского кода, выполняющего e. г. IsKeyPressed(MY_NATIVE_KEY_CODE)
    ImGuiKey_NamedKey_BEGIN = 512,
    ImGuiKey_NamedKey_END   = ImGuiKey_COUNT,
    ImGuiKey_NamedKey_COUNT = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
    ImGuiKey_KeysData_SIZE   = ImGuiKey_NamedKey_COUNT,   // Размер KeysData[]: хранить только именованные ключи
    ImGuiKey_KeysData_OFFSET = ImGuiKey_NamedKey_BEGIN    // Первый ключ хранится в KeysData[0]
#else
    ImGuiKey_KeysData_SIZE   = ImGuiKey_COUNT,   // Размер KeysData[]: содержит устаревшие коды клавиш 0..512 + именованные клавиши
    ImGuiKey_KeysData_OFFSET = 0                 // Первый ключ хранится в KeysData[0]
#endif

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ,
    ImGuiKey_KeyPadEnter = ImGuiKey_KeypadEnter   // Переименован в 1.87
#endif
};

// Помощник «помечает» версии ключевых модов, чтобы легко хранить и сравнивать несколько ключевых модов. Иногда используется для хранения (например, io. KeyMods), но в остальном мало используется в общедоступном API.
enum ImGuiModFlags_
{
    ImGuiModFlags_None  = 0,
    ImGuiModFlags_Ctrl  = 1 << 0,
    ImGuiModFlags_Shift = 1 << 1,
    ImGuiModFlags_Alt   = 1 << 2,   // Меню
    ImGuiModFlags_Super = 1 << 3    // Cmd/Super/Windows key
};

// Навигация с помощью геймпада/клавиатуры
// Клавиатура: Установить io. ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard для включения. NewFrame() автоматически заполнит io. NavInputs[] на основе вашего io. AddKeyEvent() вызывает.
// Геймпад: Установить io. ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad для включения. Бэкэнд: установите ImGuiBackendFlags_HasGamepad и заполните файл io. поля NavInputs[] перед вызовом NewFrame().
// Обратите внимание, что ио. NavInputs[] очищается EndFrame().
// Читайте инструкцию в imgui. cpp для более подробной информации. Скачать PNG/PSD на http://dearimgui.org/controls_sheets.
enum ImGuiNavInput_
{
    // Отображение геймпада
    ImGuiNavInput_Activate,      // Activate / Open / Toggle / Tweak value       // e.g. Cross  (PS4), A (Xbox), A (Switch), Space (Keyboard)
    ImGuiNavInput_Cancel,        // Cancel / Close / Exit                        // e.g. Circle (PS4), B (Xbox), B (Switch), Escape (Keyboard)
    ImGuiNavInput_Input,         // Text input / On-Screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
    ImGuiNavInput_Menu,          // Tap: Toggle menu / Hold: Focus, Move, Resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
    ImGuiNavInput_DpadLeft,      // Move / Tweak / Resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
    ImGuiNavInput_DpadRight,     //
    ImGuiNavInput_DpadUp,        //
    ImGuiNavInput_DpadDown,      //
    ImGuiNavInput_LStickLeft,    // Scroll / Move window (w/ PadMenu)            // e.g. Left Analog Stick Left/Right/Up/Down
    ImGuiNavInput_LStickRight,   //
    ImGuiNavInput_LStickUp,      //
    ImGuiNavInput_LStickDown,    //
    ImGuiNavInput_FocusPrev,     // Focus Next window (w/ PadMenu)               // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    ImGuiNavInput_FocusNext,     // Focus Prev window (w/ PadMenu)               // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
    ImGuiNavInput_TweakSlow,     // Slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    ImGuiNavInput_TweakFast,     // Faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

    // [Internal] Не используйте напрямую! Это используется внутренне, чтобы отличить клавиатуру от ввода геймпада для поведения, которое требует их различения.
    // Поведение клавиатуры, не имеющее соответствующего сопоставления с геймпадом (например, CTRL+TAB), будет напрямую считываться с клавиш клавиатуры, а не с io. НавИнпутс[].
    ImGuiNavInput_KeyLeft_,    // Move left                                    // = Клавиши со стрелками
    ImGuiNavInput_KeyRight_,   // Move right
    ImGuiNavInput_KeyUp_,      // Move up
    ImGuiNavInput_KeyDown_,    // Move down
    ImGuiNavInput_COUNT
};

// Флаги конфигурации, хранящиеся в файле io. Флаги конфигурации. Устанавливается пользователем/приложением.
enum ImGuiConfigFlags_
{
    ImGuiConfigFlags_None                    = 0,
    ImGuiConfigFlags_NavEnableKeyboard       = 1 << 0,   // Флаг включения навигации с основной клавиатуры. NewFrame() автоматически заполнит io.NavInputs[] на основе io. AddKeyEvent() вызывает
    ImGuiConfigFlags_NavEnableGamepad        = 1 << 1,   // Флаг включения основной навигации геймпада. Это в основном для того, чтобы указать вашему бэкэнду imgui
                                                         // заполнить io.NavInputs[]. Backend также необходимо установить ImGuiBackendFlags_HasGamepad.
    ImGuiConfigFlags_NavEnableSetMousePos    = 1 << 2,   // Укажите навигацию для перемещения курсора мыши. Может быть полезно в телевизионных/консольных системах,
                                                         // где неудобно перемещать виртуальную мышь. Обновлю io.MousePos и ​​установите io.WantSetMousePos=true.
                                                         // Если включено, вы ДОЛЖНЫ учитывать Запросы io. WantSetMousePos в вашем бэкенде, иначе ImGui будет реагировать так,
                                                         // как будто мышь прыгает туда-сюда.
    ImGuiConfigFlags_NavNoCaptureKeyboard    = 1 << 3,   // Проинструктируйте навигацию не устанавливать Флаг io.WantCaptureKeyboard, когда io.NavActive установлен.
    ImGuiConfigFlags_NoMouse                 = 1 << 4,   // Проинструктируйте imgui очистить положение/кнопки мыши в NewFrame(). Это позволяет игнорировать информацию о мыши, установленную серверной частью.
    ImGuiConfigFlags_NoMouseCursorChange     = 1 << 5,   // Проинструктируйте серверную часть не изменять форму и видимость курсора мыши. Используйте,
                                                         // если изменения внутреннего курсора мешают вашим, и вы не хотите использовать SetMouseCursor()
                                                         // для изменения курсора мыши. Вы можете выполнить запросы от imgui, прочитав GetMouseCursor() самостоятельно.

    // [BETA] Docking
    ImGuiConfigFlags_DockingEnable           = 1 << 6,   // Флаги включения стыковки.

    // [BETA] Viewports
    // При использовании видовых экранов рекомендуется, чтобы значение по умолчанию для ImGuiCol_WindowBg было непрозрачным (альфа = 1,0), чтобы переход в видовой экран не был заметен.
    ImGuiConfigFlags_ViewportsEnable         = 1 << 10,   // Флаги включения области просмотра (требуются оба ImGuiBackendFlags_PlatformHasViewports + ImGuiBackendFlags_RendererHasViewports,
                                                          // установленные соответствующими бэкэндами)
    ImGuiConfigFlags_DpiEnableScaleViewports = 1 << 14,   // [BETA: Не используйте] FIXME-DPI: Изменение положения и размера окон imgui при изменении DpiScale окна просмотра
                                                          // (в основном полезно для основного окна просмотра, на котором размещено другое окно). Обратите внимание,
                                                          // что изменение размера самого главного окна зависит от вашего приложения.
    ImGuiConfigFlags_DpiEnableScaleFonts     = 1 << 15,   // [BETA: Не используйте] FIXME-DPI: Запросите шрифты с растровым масштабированием, чтобы они соответствовали DpiScale.
                                                          // Это очень низкокачественный обходной путь. Правильный способ обработки DPI — _в настоящее время_ заменить атлас и/или
                                                          // шрифты в обратном вызове Platform_OnChangedViewport, но все это находится на ранней стадии разработки.

    // Хранилище пользователя (чтобы ваш бэкенд/движок мог взаимодействовать с кодом, который может совместно использоваться несколькими проектами. Эти флаги не используются ядром Уважаемый ImGui)
    ImGuiConfigFlags_IsSRGB                  = 1 << 20,   // Приложение поддерживает SRGB.
    ImGuiConfigFlags_IsTouchScreen           = 1 << 21    // Приложение использует сенсорный экран вместо мыши.
};

// Флаги возможностей серверной части, хранящиеся в файле io. BackendFlags. Устанавливается imgui_impl_xxx или пользовательским бэкендом.
enum ImGuiBackendFlags_
{
    ImGuiBackendFlags_None                    = 0,
    ImGuiBackendFlags_HasGamepad              = 1 << 0,   // Backend Platform поддерживает геймпад и в настоящее время один из них подключен.
    ImGuiBackendFlags_HasMouseCursors         = 1 << 1,   // Backend Platform поддерживает значение GetMouseCursor() для изменения формы курсора ОС.
    ImGuiBackendFlags_HasSetMousePos          = 1 << 2,   // Backend Platform поддерживает io.WantSetMousePos запросы на изменение положения мыши ОС
                                                          // (используется, только если установлено значение ImGuiConfigFlags_NavEnableSetMousePos).
    ImGuiBackendFlags_RendererHasVtxOffset    = 1 << 3,   // Backend Renderer поддерживает ImDrawCmd::VtxOffset. Это позволяет выводить большие сетки (64K+ вершин) при использовании 16-битных индексов.

    // [BETA] Viewports
    ImGuiBackendFlags_PlatformHasViewports    = 1 << 10,   // Backend Platform поддерживает несколько окон просмотра.
    ImGuiBackendFlags_HasMouseHoveredViewport = 1 << 11,   // Backend Platform поддерживает вызов io.AddMouseViewportEvent() с областью просмотра под мышью.
                                                           // ЕСЛИ ВОЗМОЖНО, игнорируйте окна просмотра с флагом ImGuiViewportFlags_NoInputs
                                                           // (бэкэнд Win32, бэкэнд GLFW 3.30+ может это сделать, бэкэнд SDL не может). Если это невозможно сделать,
                                                           // Дорогой ImGui должен использовать ошибочную эвристику, чтобы найти область просмотра под.
    ImGuiBackendFlags_RendererHasViewports    = 1 << 12    // Backend Renderer поддерживает несколько окон просмотра.
};

// Перечисление для PushStyleColor()/PopStyleColor()
enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,   // Фон обычных окон
    ImGuiCol_ChildBg,    // Фон дочерних окон
    ImGuiCol_PopupBg,    // Фон всплывающих окон, меню, всплывающих подсказок
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,   // Фон флажка, переключатель, сюжет, ползунок, ввод текста
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,   // Header* цвета используются для CollapsingHeader, TreeNode, Selectable, MenuItem
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_DockingPreview,   // Предварительный просмотр цвета наложения при стыковке чего-либо
    ImGuiCol_DockingEmptyBg,   // Цвет фона для пустого узла (например, CentralNode без прикрепленного к нему окна)
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,       // Фон заголовка таблицы
    ImGuiCol_TableBorderStrong,   // Внешние границы таблицы и границы заголовка (здесь лучше использовать Alpha=1.0)
    ImGuiCol_TableBorderLight,    // Внутренние границы таблицы (здесь лучше использовать Alpha=1.0)
    ImGuiCol_TableRowBg,          // Фон строки таблицы (четные строки)
    ImGuiCol_TableRowBgAlt,       // Фон строки таблицы (нечетные строки)
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,            // Геймпад/клавиатура: текущий выделенный элемент
    ImGuiCol_NavWindowingHighlight,   // Подсветка окна при использовании CTRL+TAB
    ImGuiCol_NavWindowingDimBg,       // Затемнить/окрасить весь экран за списком окон CTRL+TAB, когда он активен
    ImGuiCol_ModalWindowDimBg,        // Затемнять/раскрашивать весь экран за модальным окном, когда оно активно
    ImGuiCol_COUNT
};

// Перечисление для PushStyleVar() / PopStyleVar() для временного изменения структуры ImGuiStyle.
// - Перечисление относится только к полям ImGuiStyle, которые имеет смысл помещать/извлекать внутри кода пользовательского интерфейса.
//   Во время инициализации или между кадрами не стесняйтесь просто вставлять ImGuiStyle напрямую.
// — Совет: используйте средства навигации в IDE для программирования по именам во _втором столбце_ ниже, чтобы найти фактические элементы и их описание.
//   В Visual Studio IDE: CTRL+запятая ("Edit. GoToAll") может следовать за символами в комментариях, тогда как CTRL+F12 ("Edit. GoToImplementation") не может.
//   При установленном Visual Assist: ALT+G ("VAssistX. GoToImplementation") также может следовать за символами в комментариях.
// - При изменении этого перечисления вам необходимо соответствующим образом обновить связанную с ним внутреннюю таблицу GStyleVarInfo[]. Здесь мы связываем значения перечисления со смещением/типом членов.
enum ImGuiStyleVar_
{
    // Enum name --------------------- // Член структуры ImGuiStyle (описания см. в ImGuiStyle)
    ImGuiStyleVar_Alpha,                 // float     Alpha
    ImGuiStyleVar_DisabledAlpha,         // float     DisabledAlpha
    ImGuiStyleVar_WindowPadding,         // ImVec2    WindowPadding
    ImGuiStyleVar_WindowRounding,        // float     WindowRounding
    ImGuiStyleVar_WindowBorderSize,      // float     WindowBorderSize
    ImGuiStyleVar_WindowMinSize,         // ImVec2    WindowMinSize
    ImGuiStyleVar_WindowTitleAlign,      // ImVec2    WindowTitleAlign
    ImGuiStyleVar_ChildRounding,         // float     ChildRounding
    ImGuiStyleVar_ChildBorderSize,       // float     ChildBorderSize
    ImGuiStyleVar_PopupRounding,         // float     PopupRounding
    ImGuiStyleVar_PopupBorderSize,       // float     PopupBorderSize
    ImGuiStyleVar_FramePadding,          // ImVec2    FramePadding
    ImGuiStyleVar_FrameRounding,         // float     FrameRounding
    ImGuiStyleVar_FrameBorderSize,       // float     FrameBorderSize
    ImGuiStyleVar_ItemSpacing,           // ImVec2    ItemSpacing
    ImGuiStyleVar_ItemInnerSpacing,      // ImVec2    ItemInnerSpacing
    ImGuiStyleVar_IndentSpacing,         // float     IndentSpacing
    ImGuiStyleVar_CellPadding,           // ImVec2    CellPadding
    ImGuiStyleVar_ScrollbarSize,         // float     ScrollbarSize
    ImGuiStyleVar_ScrollbarRounding,     // float     ScrollbarRounding
    ImGuiStyleVar_GrabMinSize,           // float     GrabMinSize
    ImGuiStyleVar_GrabRounding,          // float     GrabRounding
    ImGuiStyleVar_TabRounding,           // float     TabRounding
    ImGuiStyleVar_ButtonTextAlign,       // ImVec2    ButtonTextAlign
    ImGuiStyleVar_SelectableTextAlign,   // ImVec2    SelectableTextAlign
    ImGuiStyleVar_COUNT
};

// Флаги для InvisibleButton() [продлен в imgui_internal.h]
enum ImGuiButtonFlags_
{
    ImGuiButtonFlags_None                = 0,
    ImGuiButtonFlags_MouseButtonLeft     = 1 << 0,   // Реагировать на левую кнопку мыши (по умолчанию)
    ImGuiButtonFlags_MouseButtonRight    = 1 << 1,   // Реагировать на правую кнопку мыши
    ImGuiButtonFlags_MouseButtonMiddle   = 1 << 2,   // Реагировать на центральную кнопку мыши

    // [Internal]
    ImGuiButtonFlags_MouseButtonMask_    = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle,
    ImGuiButtonFlags_MouseButtonDefault_ = ImGuiButtonFlags_MouseButtonLeft
};

// Флаги для ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()
enum ImGuiColorEditFlags_
{
    ImGuiColorEditFlags_None             = 0,
    ImGuiColorEditFlags_NoAlpha          = 1 << 1,    //              // ColorEdit, ColorPicker, ColorButton: игнорировать альфа-компонент (будет считывать только 3 компонента из указателя ввода).
    ImGuiColorEditFlags_NoPicker         = 1 << 2,    //              // ColorEdit: отключить средство выбора при нажатии на цветной квадрат.
    ImGuiColorEditFlags_NoOptions        = 1 << 3,    //              // ColorEdit: отключить меню параметров переключения при щелчке правой кнопкой мыши на входах/малом предварительном просмотре.
    ImGuiColorEditFlags_NoSmallPreview   = 1 << 4,    //              // ColorEdit, ColorPicker: отключить предварительный просмотр цветного квадрата рядом с входами. (например, чтобы показать только входы)
    ImGuiColorEditFlags_NoInputs         = 1 << 5,    //              // ColorEdit, ColorPicker: отключить ползунки ввода / текстовые виджеты
                                                      // (например, чтобы отображать только небольшой цветной квадрат предварительного просмотра).
    ImGuiColorEditFlags_NoTooltip        = 1 << 6,    //              // ColorEdit, ColorPicker, ColorButton: отключить всплывающую подсказку при наведении превью.
    ImGuiColorEditFlags_NoLabel          = 1 << 7,    //              // ColorEdit, ColorPicker: отключить отображение встроенной текстовой метки
                                                      // (метка по-прежнему перенаправляется во всплывающую подсказку и средство выбора).
    ImGuiColorEditFlags_NoSidePreview    = 1 << 8,    //              // ColorPicker: отключите предварительный просмотр большего цвета в правой части средства выбора,
                                                      // вместо этого используйте предварительный просмотр маленького цветного квадрата.
    ImGuiColorEditFlags_NoDragDrop       = 1 << 9,    //              // ColorEdit: отключить перетаскивание цели. ColorButton: отключить источник перетаскивания.
    ImGuiColorEditFlags_NoBorder         = 1 << 10,   //              // ColorButton: отключить границу (которая применяется по умолчанию)

    // Параметры пользователя (щелкните правой кнопкой мыши виджет, чтобы изменить некоторые из них).
    ImGuiColorEditFlags_AlphaBar         = 1 << 16,   //              // ColorEdit, ColorPicker: показать вертикальную альфа-полосу/градиент в средстве выбора.
    ImGuiColorEditFlags_AlphaPreview     = 1 << 17,   //              // ColorEdit, ColorPicker, ColorButton: отображать предварительный просмотр в виде прозрачного цвета на шахматной доске вместо непрозрачного.
    ImGuiColorEditFlags_AlphaPreviewHalf = 1 << 18,   //              // ColorEdit, ColorPicker, ColorButton: отображать наполовину непрозрачный / наполовину в шахматном порядке, вместо непрозрачного.
    ImGuiColorEditFlags_HDR              = 1 << 19,   //              // (WIP) ColorEdit: В настоящее время отключите ограничения 0.0f..1.0f только в версии RGBA
                                                      // (примечание: вы, вероятно, также захотите использовать флаг ImGuiColorEditFlags_Float).
    ImGuiColorEditFlags_DisplayRGB       = 1 << 20,   // [Display]    // ColorEdit: переопределить тип _display_ среди RGB/HSV/Hex. ColorPicker: выберите любую комбинацию, используя один или несколько RGB/HSV/Hex.
    ImGuiColorEditFlags_DisplayHSV       = 1 << 21,   // [Display]    // "
    ImGuiColorEditFlags_DisplayHex       = 1 << 22,   // [Display]    // "
    ImGuiColorEditFlags_Uint8            = 1 << 23,   // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ значения в формате 0..255.
    ImGuiColorEditFlags_Float            = 1 << 24,   // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ значения в формате 0.0f..1.0f с плавающей запятой вместо целых чисел 0..255.
                                                      // Нет кругового пути значения через целые числа.
    ImGuiColorEditFlags_PickerHueBar     = 1 << 25,   // [Picker]     // ColorPicker: полоса для оттенка, прямоугольник для насыщенности/значения.
    ImGuiColorEditFlags_PickerHueWheel   = 1 << 26,   // [Picker]     // ColorPicker: колесо для Hue, треугольник для Sat/Value.
    ImGuiColorEditFlags_InputRGB         = 1 << 27,   // [Input]      // ColorEdit, ColorPicker: входные и выходные данные в формате RGB.
    ImGuiColorEditFlags_InputHSV         = 1 << 28,   // [Input]      // ColorEdit, ColorPicker: входные и выходные данные в формате HSV.

    // Параметры по умолчанию. Вы можете установить значения по умолчанию для приложения, используя SetColorEditOptions(). Цель состоит в том, что вы, вероятно, не хотите
    // переопределить их в большинстве ваших вызовов. Позвольте пользователю выбрать через меню опций и/или вызвать SetColorEditOptions() один раз во время запуска.
    ImGuiColorEditFlags_DefaultOptions_  = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar,

    // [Internal] Маски
    ImGuiColorEditFlags_DisplayMask_     = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex,
    ImGuiColorEditFlags_DataTypeMask_    = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags_PickerMask_      = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_InputMask_       = ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV

    // Устаревшие имена (будут удалены)
    // ImGuiColorEditFlags_RGB = ImGuiColorEditFlags_DisplayRGB, ImGuiColorEditFlags_HSV = ImGuiColorEditFlags_DisplayHSV, ImGuiColorEditFlags_HEX = ImGuiColorEditFlags_DisplayHex  // [переименован в 1.69]
};

// Флаги для DragFloat(), DragInt(), SliderFloat(), SliderInt() и т. д..
// Мы используем одни и те же наборы флагов для функций DragXXX() и SliderXXX(), так как функции одинаковы и их проще поменять местами.
enum ImGuiSliderFlags_
{
    ImGuiSliderFlags_None            = 0,
    ImGuiSliderFlags_AlwaysClamp     = 1 << 4,      // Зафиксируйте значение в минимальных/максимальных границах при вводе вручную с помощью CTRL+Click. По умолчанию CTRL+Click позволяет выйти за границы.
    ImGuiSliderFlags_Logarithmic     = 1 << 5,      // Сделайте виджет логарифмическим (линейным в противном случае). Рассмотрите возможность использования ImGuiSliderFlags_NoRoundToFormat с этим,
                                                    // если вы используете строку формата с небольшим количеством цифр.
    ImGuiSliderFlags_NoRoundToFormat = 1 << 6,      // Отключите округление базового значения, чтобы оно соответствовало точности строки формата отображения (например, значения %.3f округляются до этих трех цифр).
    ImGuiSliderFlags_NoInput         = 1 << 7,      // Отключите CTRL+щелчок или клавишу Enter, чтобы вводить текст непосредственно в виджет
    ImGuiSliderFlags_InvalidMask_    = 0x7000000F   // [Internal] Мы рассматриваем использование этих битов как потенциально аргумент «плавающей мощности» из предыдущего API,
                                                    // который был неправильно преобразован в это перечисление и при необходимости вызовет утверждение.

    // Устаревшие имена (будут удалены)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ,
    ImGuiSliderFlags_ClampOnInput = ImGuiSliderFlags_AlwaysClamp   // [переименован в 1.79]
#endif
};

// Определите кнопку мыши.
// Эти значения гарантированно стабильны, и мы часто используем 0/1 напрямую. Именованные перечисления предоставлены для удобства.
enum ImGuiMouseButton_
{
    ImGuiMouseButton_Left   = 0,
    ImGuiMouseButton_Right  = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT  = 5
};

// Перечисление для GetMouseCursor()
// Пользовательский код может запросить серверную часть для отображения данного курсора, вызвав SetMouseCursor(), поэтому у нас есть некоторые курсоры, которые помечены здесь как неиспользуемые.
enum ImGuiMouseCursor_
{
    ImGuiMouseCursor_None  = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,     // При наведении курсора на InputText и т. д.
    ImGuiMouseCursor_ResizeAll,     // ((Unused by Dear ImGui functions))Не используется функциями Dear ImGui)
    ImGuiMouseCursor_ResizeNS,      // При наведении на горизонтальную границу
    ImGuiMouseCursor_ResizeEW,      // При наведении на вертикальную границу или столбец
    ImGuiMouseCursor_ResizeNESW,    // При наведении курсора на левый нижний угол окна
    ImGuiMouseCursor_ResizeNWSE,    // При наведении курсора на правый нижний угол окна
    ImGuiMouseCursor_Hand,          // (Не используется функциями Dear ImGui. Используйте для эл. г. гиперссылки)
    ImGuiMouseCursor_NotAllowed,    // При наведении курсора на что-то с запрещенным взаимодействием. Обычно перечеркнутый круг.
    ImGuiMouseCursor_СrossSelect,   // Перекрестие.
    ImGuiMouseCursor_Size,          // Спец. выделение(стрелка)
    ImGuiMouseCursor_Help,          // Выбор справки(стрелка с вопросом)
    ImGuiMouseCursor_COUNT
};

// Перечисление для ImGui::SetWindow***(), SetNextWindow***(), SetNextItem***() функции
// Представлять условие.
// Важно: рассматривать как обычное перечисление! НЕ объединяйте несколько значений с помощью бинарных операторов! Все вышеперечисленные функции обрабатывают 0 как ярлык ImGuiCond_Always.
enum ImGuiCond_
{
    ImGuiCond_None         = 0,        // Без условия (всегда задавайте переменную), то же, что и _Always
    ImGuiCond_Always       = 1 << 0,   // Без условия (всегда задавайте переменную)
    ImGuiCond_Once         = 1 << 1,   // Установите переменную один раз за сеанс выполнения (успешен только первый вызов)
    ImGuiCond_FirstUseEver = 1 << 2,   // Установите переменную, если объект/окно не имеет постоянно сохраненных данных (нет записи в .ini файле)
    ImGuiCond_Appearing    = 1 << 3    // Установите переменную, если объект/окно появляется после того, как он был скрыт/неактивен (или в первый раз)
};

//-----------------------------------------------------------------------------
// [SECTION] Помощники: Макросы выделения памяти, ImVector<>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// IM_MALLOC(), IM_FREE(), IM_NEW(), IM_PLACEMENT_NEW(), IM_DELETE()
// Мы вызываем конструктор C++ для собственной выделенной памяти с помощью синтаксиса размещения "new(ptr) Type()".
// Определение пользовательского места размещения new() с пользовательским параметром позволяет нам обойти включение <new>, которое на некоторых платформах жалуется, когда пользователь отключил исключения.
//-----------------------------------------------------------------------------

struct ImNewWrapper
{};
inline void* operator new(size_t, ImNewWrapper, void* ptr)
{
    return ptr;
}
inline void operator delete(void*, ImNewWrapper, void*) {}   // Это необходимо только для того, чтобы мы могли использовать симметричный метод new().
#define IM_ALLOC(_SIZE)        ImGui::MemAlloc(_SIZE)
#define IM_FREE(_PTR)          ImGui::MemFree(_PTR)
#define IM_PLACEMENT_NEW(_PTR) new (ImNewWrapper(), _PTR)
#define IM_NEW(_TYPE)          new (ImNewWrapper(), ImGui::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void IM_DELETE(T* p)
{
    if (p)
    {
        p->~T();
        ImGui::MemFree(p);
    }
}

//-----------------------------------------------------------------------------
// ImVector<>
// Lightweight std::vector<>-как класс, чтобы избежать перетаскивания зависимостей (кроме того, некоторые реализации STL с включенной отладкой абсурдно медленны, мы обходим его, чтобы наш код работал быстро в отладке).
//-----------------------------------------------------------------------------
// - Как правило, вам НЕ нужно заботиться или использовать это когда-либо. Но нам нужно сделать его доступным в imgui. h, потому что на него полагаются некоторые из наших общественных структур.
// - Здесь мы используем стандартное соглашение об именах, что немного необычно для этой кодовой базы.
// - Важно: clear() освобождает память, resize(0) сохраняет выделенный буфер. Мы часто используем resize(0) для намеренного повторного использования выделенных буферов между кадрами и амортизации наших затрат.
// - Важно: наша реализация НЕ вызывает конструкторы/деструкторы C++, мы обрабатываем все как необработанные данные! Это сделано намеренно, но помните об этом,
//   НЕ используйте этот класс в качестве замены std::vector в собственном коде! Многие структуры, используемые дорогим imgui, можно безопасно инициализировать нулевым набором памяти.
//-----------------------------------------------------------------------------

IM_MSVC_RUNTIME_CHECKS_OFF
template<typename T> struct ImVector
{
    int                       Size;
    int                       Capacity;
    T*                        Data;

    // Предоставляем стандартные определения типов, но сами мы их не используем.
    typedef T                 value_type;
    typedef value_type*       iterator;
    typedef const value_type* const_iterator;

    // Конструкторы, деструктор
    inline ImVector()
    {
        Size = Capacity = 0;
        Data            = NULL;
    }
    inline ImVector(const ImVector<T>& src)
    {
        Size = Capacity = 0;
        Data            = NULL;
        operator=(src);
    }
    inline ImVector<T>& operator=(const ImVector<T>& src)
    {
        clear();
        resize(src.Size);
        memcpy(Data, src.Data, (size_t)Size * sizeof(T));
        return *this;
    }
    inline ~ImVector()
    {
        if (Data)
            IM_FREE(Data);
    }   // Важно: ничего не разрушает

    inline void clear()
    {
        if (Data)
        {
            Size = Capacity = 0;
            IM_FREE(Data);
            Data = NULL;
        }
    }   // Важно: ничего не разрушает
    inline void clear_delete()
    {
        for (int n = 0; n < Size; n++)
            IM_DELETE(Data[n]);
        clear();
    }   // Важно: никогда не вызывается автоматически! всегда явный.
    inline void clear_destruct()
    {
        for (int n = 0; n < Size; n++)
            Data[n].~T();
        clear();
    }   // Важно: никогда не вызывается автоматически! всегда явный.

    inline bool empty() const
    {
        return Size == 0;
    }
    inline int size() const
    {
        return Size;
    }
    inline int size_in_bytes() const
    {
        return Size * (int)sizeof(T);
    }
    inline int max_size() const
    {
        return 0x7FFFFFFF / (int)sizeof(T);
    }
    inline int capacity() const
    {
        return Capacity;
    }
    inline T& operator[](int i)
    {
        IM_ASSERT(i >= 0 && i < Size);
        return Data[i];
    }
    inline const T& operator[](int i) const
    {
        IM_ASSERT(i >= 0 && i < Size);
        return Data[i];
    }

    inline T* begin()
    {
        return Data;
    }
    inline const T* begin() const
    {
        return Data;
    }
    inline T* end()
    {
        return Data + Size;
    }
    inline const T* end() const
    {
        return Data + Size;
    }
    inline T& front()
    {
        IM_ASSERT(Size > 0);
        return Data[0];
    }
    inline const T& front() const
    {
        IM_ASSERT(Size > 0);
        return Data[0];
    }
    inline T& back()
    {
        IM_ASSERT(Size > 0);
        return Data[Size - 1];
    }
    inline const T& back() const
    {
        IM_ASSERT(Size > 0);
        return Data[Size - 1];
    }
    inline void swap(ImVector<T>& rhs)
    {
        int rhs_size = rhs.Size;
        rhs.Size     = Size;
        Size         = rhs_size;
        int rhs_cap  = rhs.Capacity;
        rhs.Capacity = Capacity;
        Capacity     = rhs_cap;
        T* rhs_data  = rhs.Data;
        rhs.Data     = Data;
        Data         = rhs_data;
    }

    inline int _grow_capacity(int sz) const
    {
        int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8;
        return new_capacity > sz ? new_capacity : sz;
    }
    inline void resize(int new_size)
    {
        if (new_size > Capacity)
            reserve(_grow_capacity(new_size));
        Size = new_size;
    }
    inline void resize(int new_size, const T& v)
    {
        if (new_size > Capacity)
            reserve(_grow_capacity(new_size));
        if (new_size > Size)
            for (int n = Size; n < new_size; n++)
                memcpy(&Data[n], &v, sizeof(v));
        Size = new_size;
    }
    inline void shrink(int new_size)
    {
        IM_ASSERT(new_size <= Size);
        Size = new_size;
    }   // Измените размер вектора на меньший размер, чтобы гарантированно не вызвать перераспределение
    inline void reserve(int new_capacity)
    {
        if (new_capacity <= Capacity)
            return;
        T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T));
        if (Data)
        {
            memcpy(new_data, Data, (size_t)Size * sizeof(T));
            IM_FREE(Data);
        }
        Data     = new_data;
        Capacity = new_capacity;
    }

    // NB: Незаконно вызывать push_back/push_front/insert со ссылкой, указывающей внутри самих данных ImVector! е. г. v. push_back(v[10]) запрещен.
    inline void push_back(const T& v)
    {
        if (Size == Capacity)
            reserve(_grow_capacity(Size + 1));
        memcpy(&Data[Size], &v, sizeof(v));
        Size++;
    }
    inline void pop_back()
    {
        IM_ASSERT(Size > 0);
        Size--;
    }
    inline void push_front(const T& v)
    {
        if (Size == 0)
            push_back(v);
        else
            insert(Data, v);
    }
    inline T* erase(const T* it)
    {
        IM_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T));
        Size--;
        return Data + off;
    }
    inline T* erase(const T* it, const T* it_last)
    {
        IM_ASSERT(it >= Data && it < Data + Size && it_last >= it && it_last <= Data + Size);
        const ptrdiff_t count = it_last - it;
        const ptrdiff_t off   = it - Data;
        memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T));
        Size -= (int)count;
        return Data + off;
    }
    inline T* erase_unsorted(const T* it)
    {
        IM_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        if (it < Data + Size - 1)
            memcpy(Data + off, Data + Size - 1, sizeof(T));
        Size--;
        return Data + off;
    }
    inline T* insert(const T* it, const T& v)
    {
        IM_ASSERT(it >= Data && it <= Data + Size);
        const ptrdiff_t off = it - Data;
        if (Size == Capacity)
            reserve(_grow_capacity(Size + 1));
        if (off < (int)Size)
            memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T));
        memcpy(&Data[off], &v, sizeof(v));
        Size++;
        return Data + off;
    }
    inline bool contains(const T& v) const
    {
        const T* data     = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data++ == v)
                return true;
        return false;
    }
    inline T* find(const T& v)
    {
        T*       data     = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }
    inline const T* find(const T& v) const
    {
        const T* data     = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }
    inline bool find_erase(const T& v)
    {
        const T* it = find(v);
        if (it < Data + Size)
        {
            erase(it);
            return true;
        }
        return false;
    }
    inline bool find_erase_unsorted(const T& v)
    {
        const T* it = find(v);
        if (it < Data + Size)
        {
            erase_unsorted(it);
            return true;
        }
        return false;
    }
    inline int index_from_ptr(const T* it) const
    {
        IM_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        return (int)off;
    }
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] ImGuiStyle
//-----------------------------------------------------------------------------
// Вы можете модифицировать главный экземпляр ImGui:: GetStyle() во время инициализации и перед NewFrame().
// Во время кадра используйте ImGui::PushStyleVar(ImGuiStyleVar_XXXX)/PopStyleVar() для изменения основных значений стиля,
// и ImGui::PushStyleColor(ImGuiCol_XXX)/PopStyleColor() для цветов.
//-----------------------------------------------------------------------------

struct ImGuiStyle
{
    float          Alpha;                        // Глобальная альфа применяется ко всему в Dear ImGui.
    float          DisabledAlpha;                // Дополнительный альфа-множитель, применяемый BeginDisabled(). Умножить на текущее значение альфы.
    ImVec2         WindowPadding;                // Заполнение внутри окна.
    float          WindowRounding;               // Радиус закругления углов окна. Установите 0.0f, чтобы иметь прямоугольные окна. Большие значения, как правило, приводят к множеству артефактов и не рекомендуются.
    float          WindowBorderSize;             // Толщина окантовки вокруг окон. Обычно устанавливается на 0.0f или 1.0f. (Другие значения не проверены и требуют больших затрат на ЦП/ГП).
    ImVec2         WindowMinSize;                // Минимальный размер окна. Это глобальная настройка. Если вы хотите ограничить отдельные окна, используйте SetNextWindowSizeConstraints().
    ImVec2         WindowTitleAlign;             // Выравнивание текста в заголовке. По умолчанию (0.0f,0.5f) для выравнивания по левому краю, по центру по вертикали.
    ImGuiDir       WindowMenuButtonPosition;     // Сторона кнопки сворачивания/стыковки в строке заголовка (Нет/Слева/Справа). По умолчанию ImGuiDir_Left.
    float          ChildRounding;                // Радиус скругления углов дочернего окна. Установите 0.0f, чтобы иметь прямоугольные окна.
    float          ChildBorderSize;              // Толщина границы вокруг дочерних окон. Обычно устанавливается на 0.0f или 1.0f. (Другие значения не проверены и требуют больших затрат на ЦП/ГП).
    float          PopupRounding;                // Радиус скругления углов всплывающего окна. (Обратите внимание, что окна всплывающих подсказок используют WindowRounding)
    float          PopupBorderSize;              // Толщина границы вокруг всплывающих окон/всплывающих подсказок. Обычно устанавливается на 0.0f или 1.0f. (Другие значения не проверены и требуют больших затрат на ЦП/ГП).
    ImVec2         FramePadding;                 // Отступ внутри прямоугольника с рамкой (используется большинством виджетов).
    float          FrameRounding;                // Радиус скругления углов кадра. Установите 0.0f, чтобы иметь прямоугольную рамку (используется большинством виджетов).
    float          FrameBorderSize;              // Толщина границы вокруг рамок. Обычно устанавливается на 0.0f или 1.0f. (Другие значения не проверены и требуют больших затрат на ЦП/ГП).
    ImVec2         ItemSpacing;                  // Горизонтальное и вертикальное расстояние между виджетами/стрками.
    ImVec2         ItemInnerSpacing;             // Расстояние по горизонтали и вертикали между внутренними элементами составного виджета (например, ползунком и его меткой).
    ImVec2         CellPadding;                  // Заполнение в ячейке таблицы
    ImVec2         TouchExtraPadding;            // Расширьте реактивную ограничивающую рамку для сенсорной системы, где положение касания недостаточно точное.
                                                 // К сожалению, мы не сортируем виджеты, поэтому приоритет при перекрытии всегда будет отдаваться первому виджету.
                                                 // Так что не выращивайте это слишком много!
    float          IndentSpacing;                // Горизонтальный отступ, когда e. г. вход в узел дерева. Обычно == (FontSize + FramePadding. x*2).
    float          ColumnsMinSpacing;            // Минимальное горизонтальное расстояние между двумя столбцами. Предпочтительно > (FramePadding.x+1).
    float          ScrollbarSize;                // Ширина вертикальной полосы прокрутки, Высота горизонтальной полосы прокрутки.
    float          ScrollbarRounding;            // Радиус углов захвата полосы прокрутки.
    float          GrabMinSize;                  // Минимальная ширина/высота захвата для ползунка/полосы прокрутки.
    float          GrabRounding;                 // Радиус скругления углов захватов. Установите 0.0f, чтобы иметь прямоугольные захваты ползунка.
    float          LogSliderDeadzone;            // Размер в пикселях мертвой зоны вокруг нуля на логарифмических ползунках, пересекающих ноль.
    float          TabRounding;                  // Радиус верхних углов вкладки. Установите 0.0f, чтобы иметь прямоугольные вкладки.
    float          TabBorderSize;                // Толщина границы вокруг вкладок.
    float          TabMinWidthForCloseButton;    // Минимальная ширина, при которой кнопка закрытия отображается на невыбранной вкладке при наведении курсора.
                                                 // Установите значение 0.0f, чтобы всегда показывать при наведении курсора, установите значение FLT_MAX,
                                                 // чтобы никогда не отображать кнопку закрытия, если она не выбрана.
    ImGuiDir       ColorButtonPosition;          // Сторона кнопки цвета в виджете ColorEdit4 (слева/справа). По умолчанию ImGuiDir_Right.
    ImVec2         ButtonTextAlign;              // Выравнивание текста кнопки, когда кнопка больше текста. По умолчанию (0,5f, 0,5f) (по центру).
    ImVec2         SelectableTextAlign;          // Выравнивание выделяемого текста. По умолчанию (0.0f, 0.0f) (выравнивание по верхнему левому краю).
                                                 // Как правило, важно выровнять это по левому краю, если вы хотите расположить несколько элементов на одной линии.
    ImVec2         DisplayWindowPadding;         // Положение окна фиксируется, чтобы быть видимым в пределах области отображения или мониторов, по крайней мере, на эту величину.
                                                 // Подходит только для обычных окон.
    ImVec2         DisplaySafeAreaPadding;       // Если вы не видите края экрана (например, на телевизоре), увеличьте отступ безопасной области.
                                                 // Применяется к всплывающим окнам/подсказкам, а также к обычным окнам. NB: Предпочтите правильную настройку телевизоров!
    float          MouseCursorScale;             // Масштабирование отображаемого программным обеспечением курсора мыши (когда включен io. MouseDrawCursor).
                                                 // Мы применяем масштабирование DPI для каждого монитора по этому масштабу. Может быть удален позже.
    bool           AntiAliasedLines;             // Включить сглаженные линии/границы. Отключите, если вы сильно напрягаете CPU/GPU. Защелкивается в начале кадра (скопировано в ImDrawList).
    bool           AntiAliasedLinesUseTex;       // Включите сглаженные линии/границы с использованием текстур, где это возможно. Требовать от бэкэнда рендеринга
                                                 // с билинейной фильтрацией (НЕ точечная/ближайшая фильтрация). Защелкивается в начале кадра (скопировано в ImDrawList).
    bool           AntiAliasedFill;              // Включите сглаживание краев вокруг заполненных фигур (прямоугольников со скругленными углами, кругов и т. д.).
                                                 // Отключите, если вы сильно напрягаете CPU/GPU. Защелкивается в начале кадра (скопировано в ImDrawList).
    float          CurveTessellationTol;         // Допуск тесселяции при использовании PathBezierCurveTo() без определенного количества сегментов.
                                                 // Уменьшите для сильно мозаичных кривых (более высокое качество, больше полигонов), увеличьте, чтобы уменьшить качество.
    float          CircleTessellationMaxError;   // Максимальная ошибка (в пикселях), допустимая при использовании AddCircle()/AddCircleFilled() или рисовании прямоугольников
                                                 // со скругленными углами без указания явного количества сегментов. Уменьшение для более высокого качества, но больше геометрии.
    ImVec4         Colors[ImGuiCol_COUNT];

    IMGUI_API      ImGuiStyle();
    IMGUI_API void ScaleAllSizes(float scale_factor);
};

//-----------------------------------------------------------------------------
// [SECTION] ImGuiIO
//-----------------------------------------------------------------------------
// Передавайте большинство настроек и вводов/выводов в Dear ImGui, используя эту структуру.
// Доступ через ImGui::GetIO(). Прочтите раздел «Руководство программиста» в . cpp для общего использования.
//-----------------------------------------------------------------------------

// [Internal] Хранилище, используемое функциями IsKeyDown(), IsKeyPressed() и т. д.
// Если до 1.87 вы использовали io.KeysDownDuration[] (который был помечен как внутренний), вы должны использовать GetKeyData(key)->DownDuration, а не io. KeysData[ключ]->DownDuration.
struct ImGuiKeyData
{
    bool  Down;               // Истинно, если ключ нажат
    float DownDuration;       // Продолжительность нажатия клавиши (<0,0f: не нажата, 0,0f: только что нажата, >0,0f: время удерживания)
    float DownDurationPrev;   // Длительность последнего кадра, когда ключ был нажат
    float AnalogValue;        // 0.0f..1.0f для значений геймпада
};

struct ImGuiIO
{
    //------------------------------------------------------------------
    // Configuration                            // Значение по умолчанию
    //------------------------------------------------------------------

    ImGuiConfigFlags  ConfigFlags;               // = 0              // См. перечисление ImGuiConfigFlags_. Устанавливается пользователем/приложением. Параметры навигации с помощью геймпада/клавиатуры и т. д.
    ImGuiBackendFlags BackendFlags;              // = 0              // См. перечисление ImGuiBackendFlags_. Устанавливается бэкендом (файлы imgui_impl_xxx или пользовательский бэкэнд)
                                                 // для передачи функций, поддерживаемых бэкендом.
    ImVec2            DisplaySize;               // <unset>          // Размер основного экрана в пикселях (обычно == GetMainViewport()->Size). Может меняться каждый кадр.
    float             DeltaTime;                 // = 1.0f/60.0f     // Время, прошедшее с момента последнего кадра, в секундах. Может меняться каждый кадр.
    float             IniSavingRate;             // = 5.0f           // Минимальное время между сохранением позиций/размеров в . ini-файл в секундах.
    const char*       IniFilename;               // = "imgui.ini"    // Путь к . ini (важно: по умолчанию "imgui.ini" относится к текущему рабочему каталогу!).
                                                 // Установите NULL, чтобы отключить автоматический . ini загрузки/сохранения или если вы хотите
                                                 // вручную вызвать функции LoadIniSettingsXXX() / SaveIniSettingsXXX().
    const char*       LogFilename;               // = "imgui_log.txt"// Путь к . log (параметр по умолчанию для ImGui::LogToFile, если файл не указан).
    float             MouseDoubleClickTime;      // = 0.30f          // Время двойного щелчка в секундах.
    float             MouseDoubleClickMaxDist;   // = 6.0f           // Пороговое значение расстояния, на котором следует оставаться для проверки двойного щелчка, в пикселях.
    float             MouseDragThreshold;        // = 6.0f           // Пороговое расстояние, прежде чем считать, что мы перетаскиваем.
    float             KeyRepeatDelay;            // = 0.250f         // При удержании клавиши/кнопки время до начала повторения в секундах (для кнопок в режиме повтора и т. д.).
    float             KeyRepeatRate;             // = 0.050f         // При удержании клавиши/кнопки частота повторения в секундах.
    void*             UserData;                  // = NULL           // Храните свои собственные данные для извлечения с помощью обратных вызовов.

    ImFontAtlas*      Fonts;                     // <auto>           // Атлас шрифтов: загружайте, растрируйте и упаковывайте один или несколько шрифтов в одну текстуру.
    float             FontGlobalScale;           // = 1.0f           // Глобальный масштаб всех шрифтов
    bool              FontAllowUserScaling;      // = false          // Разрешить пользователю масштабировать текст отдельного окна с помощью CTRL+Wheel.
    ImFont*           FontDefault;               // = NULL           // Шрифт для использования в NewFrame(). Используйте NULL для использования Fonts->Fonts[0].
    ImVec2            DisplayFramebufferScale;   // = (1, 1)         // Fили дисплей сетчатки или другие ситуации, когда координаты окна отличаются от координат буфера кадра.
                                                 // Обычно это заканчивается в ImDrawData::FramebufferScale.

    // Параметры стыковки (если установлено значение ImGuiConfigFlags_DockingEnable)
    bool              ConfigDockingNoSplit;              // = false          // Упрощенный режим стыковки: отключите разделение окон, чтобы стыковка ограничивалась объединением нескольких окон вместе в панели вкладок.
    bool              ConfigDockingWithShift;            // = false          // Включить стыковку, удерживая клавишу Shift (уменьшает визуальный шум, позволяет размещать в более широком пространстве)
    bool              ConfigDockingAlwaysTabBar;         // = false          // [BETA] [FIXME: В настоящее время это создает регрессию с автоматическим изменением размера и общими накладными расходами]
                                                         // Сделайте так, чтобы каждое отдельное плавающее окно отображалось в стыковочном узле.
    bool              ConfigDockingTransparentPayload;   // = false          // [BETA] Сделайте окно или область просмотра прозрачными при стыковке и отображайте стыковочные блоки только в целевом окне просмотра.
                                                         // Полезно, если рендеринг нескольких видовых экранов не может быть синхронизирован. Лучше всего использовать с ConfigViewportsNoAutoMerge.

    // Параметры области просмотра (если установлено значение ImGuiConfigFlags_ViewportsEnable)
    bool              ConfigViewportsNoAutoMerge;       // = false;         // Установите, чтобы все плавающие окна imgui всегда создавали свои собственные области просмотра.
                                                        // В противном случае они сливаются с основным окном просмотра хоста при его перекрытии.
                                                        // Также можно установить ImGuiViewportFlags_NoAutoMerge для отдельного окна просмотра.
    bool              ConfigViewportsNoTaskBarIcon;     // = false          // Отключите флаг значка панели задач ОС по умолчанию для дополнительных окон просмотра.
                                                        // Когда видовому экрану не нужен значок на панели задач, для него будет установлен ImGuiViewportFlags_NoTaskBarIcon.
    bool              ConfigViewportsNoDecoration;      // = true           // Отключите флаг оформления окна ОС по умолчанию для дополнительных видовых экранов.
                                                        // Когда видовой экран не хочет украшать окна, для него будет установлен ImGuiViewportFlags_NoDecoration.
                                                        // Включение оформления может создать последующие проблемы на уровне ОС (например, минимальный размер окна).
    bool              ConfigViewportsNoDefaultParent;   // = false          // Отключите родитель ОС по умолчанию для основного видового экрана для вторичных видовых экранов.
                                                        // По умолчанию окна просмотра помечаются ParentViewportId = <main_viewport>, ожидая, что серверная часть платформы
                                                        // установит отношение родитель/потомок между окнами ОС (некоторые серверные части могут игнорировать это).
                                                        // Установите значение true, если вы хотите, чтобы значение по умолчанию было равно 0, тогда все области просмотра будут окнами ОС верхнего уровня.

    // Разнообразные Опции
    bool              MouseDrawCursor;                     // = false          // Попросите ImGui нарисовать для вас курсор мыши (если вы работаете на платформе без курсора мыши).
                                                           // Не может быть легко переименован в «io. ConfigXXX», потому что он часто используется в серверных реализациях.
    bool              ConfigMacOSXBehaviors;               // = defined(__APPLE__) // Стиль OS X: перемещение курсора при редактировании текста с использованием Alt вместо Ctrl, ярлыки с использованием
                                                           // Cmd/Super вместо Ctrl, начало и конец строки/текста с использованием Cmd+стрелки вместо Home/End, двойной
                                                           // щелчок по слову вместо выделения всего текста, Множественный выбор в списках использует Cmd/Super вместо Ctrl.
    bool              ConfigInputTrickleEventQueue;        // = true           // Включить просачивание очереди ввода: некоторые типы событий, отправленные в течение одного кадра (например, нажатие кнопки + вверх),
                                                           // будут распределены по нескольким кадрам, улучшая взаимодействие с низкой частотой кадров.
    bool              ConfigInputTextCursorBlink;          // = true           // Включить мигающий курсор (необязательно, так как некоторые пользователи считают, что это отвлекает).
    bool              ConfigDragClickToInputText;          // = false          // [BETA] Включите преобразование виджетов DragXXX в текстовый ввод простым щелчком мыши (без перемещения).
                                                           // Не желательно на устройствах без клавиатуры.
    bool              ConfigWindowsResizeFromEdges;        // = true           // Включить изменение размера окон от их краев и от нижнего левого угла. Для этого требуется
                                                           // (io. BackendFlags & ImGuiBackendFlags_HasMouseCursors), потому что ему нужна обратная связь с курсором мыши.
                                                           // (Раньше это был флаг ImGuiWindowFlags_ResizeFromAnySide для каждого окна)
    bool              ConfigWindowsMoveFromTitleBarOnly;   // = false       // Включите разрешение перемещать окна только при нажатии на их строку заголовка. Не применяется к окнам без строки заголовка.
    float             ConfigMemoryCompactTimer;            // = 60.0f          // Таймер (в секундах) для освобождения временных буферов памяти окон/таблиц, когда они не используются. Установите на -1.0f, чтобы отключить.

    //------------------------------------------------------------------
    // Функции платформы
    // (внутренние файлы imgui_impl_xxxx настраивают их для вас)
    //------------------------------------------------------------------

    // Optional: Имя бэкенда платформы/рендерера (только для информации! Будет отображаться в окне «О программе») + Пользовательские данные для бэкенда/обертки для хранения своих собственных материалов.
    const char*       BackendPlatformName;       // = NULL
    const char*       BackendRendererName;       // = NULL
    void*             BackendPlatformUserData;   // = NULL           // Пользовательские данные для серверной части платформы
    void*             BackendRendererUserData;   // = NULL           // Пользовательские данные для бэкенда рендерера
    void*             BackendLanguageUserData;   // = NULL           // Пользовательские данные для серверной части языка программирования, отличной от C++

    // Optional: Доступ к буферу обмена ОС
    // (по умолчанию используется собственный буфер обмена Win32 в Windows, в противном случае используется частный буфер обмена. Переопределить доступ к буферу обмена ОС на других архитектурах)
    const char*       (*GetClipboardTextFn)(void* user_data);
    void              (*SetClipboardTextFn)(void* user_data, const char* text);
    void*             ClipboardUserData;

    // Optional: Сообщите редактору метода ввода ОС о положении курсора на экране для положения ввода текста (например, при использовании японского/китайского IME в Windows)
    // (по умолчанию используется собственный API imm32 в Windows)
    void              (*SetPlatformImeDataFn)(ImGuiViewport* viewport, ImGuiPlatformImeData* data);
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    void* ImeWindowHandle;   // = NULL // [Устаревший] Вместо этого установите ImGuiViewport::PlatformHandleRaw. Установите это на свой HWND, чтобы получить автоматическое позиционирование курсора IME.
#else
    void* _UnusedPadding;   // Неиспользуемое поле для сохранения структуры данных того же размера.
#endif

    //------------------------------------------------------------------
    // Input - Вызов перед вызовом NewFrame()
    //------------------------------------------------------------------

    // Input Функции
    IMGUI_API void AddKeyEvent(ImGuiKey key, bool down);                  // Поставить в очередь новое событие нажатия/нажатия клавиши. Ключ должен быть «переведен»
                                                                          // (например, обычно ImGuiKey_A соответствует ключу, который конечный пользователь будет использовать для создания символа «A»)
    IMGUI_API void AddKeyAnalogEvent(ImGuiKey key, bool down, float v);   // Поставьте в очередь новое событие нажатия/нажатия клавиши для аналоговых значений
                                                                          // (например, значений ImGuiKey_Gamepad_). Мертвые зоны должны обрабатываться серверной частью.
    IMGUI_API void AddMousePosEvent(float x, float y);                    // Поставьте в очередь обновление положения мыши. Используйте -FLT_MAX, -FLT_MAX,
                                                                          // чтобы обозначить отсутствие мыши (например, приложение не сфокусировано и не зависло)
    IMGUI_API void AddMouseButtonEvent(int button, bool down);            // Поставить в очередь смену кнопки мыши
    IMGUI_API void AddMouseWheelEvent(float wh_x, float wh_y);            // Поставить в очередь обновление колеса мыши
    IMGUI_API void AddMouseViewportEvent(ImGuiID id);                     // Очередь в окне просмотра, наведенном мышью. Требуется, чтобы серверная часть установила
                                                                          // ImGuiBackendFlags_HasMouseHoveredViewport для вызова этого (для поддержки нескольких окон просмотра).
    IMGUI_API void AddFocusEvent(bool focused);                           // Поставьте в очередь получение/потерю фокуса для приложения (обычно в зависимости от фокуса ОС/платформы вашего окна)
    IMGUI_API void AddInputCharacter(unsigned int c);                     // Очередь ввода нового символа
    IMGUI_API void AddInputCharacterUTF16(ImWchar16 c);                   // Поставьте в очередь ввод нового символа из символа UTF-16, это может быть суррогат
    IMGUI_API void AddInputCharactersUTF8(const char* str);               // Поставить в очередь ввод новых символов из строки UTF-8

    IMGUI_API void ClearInputCharacters();                                                                                       // [Internal] Очистить буфер ввода текста вручную
    IMGUI_API void ClearInputKeys();                                                                                             // [Internal] Отпустите все ключи
    IMGUI_API void SetKeyEventNativeData(ImGuiKey key, int native_keycode, int native_scancode, int native_legacy_index = -1);   // [Optional] Укажите индекс для устаревших функций <1.87 IsKeyXXX()
                                                                                                                                 // с собственными индексами + укажите собственный код клавиши, код сканирования.

    //------------------------------------------------------------------
    // Output - Обновляется NewFrame() или EndFrame()/Render()
    // (при чтении из io.WantCaptureMouse, io.WantCaptureKeyboard флаги для отправки ваших входов, это
    //  вообще проще и правильнее использовать их состояние ДО вызова NewFrame(). Подробности смотрите в FAQ!)
    //------------------------------------------------------------------

    bool           WantCaptureMouse;           // Установите, когда Уважаемый ImGui будет использовать ввод с помощью мыши, в этом случае не отправляйте их в вашу основную
                                               // игру/приложение (в любом случае всегда передавайте ввод с мыши в imgui). (например, ненажатая мышь находится над окном imgui,
                                               // виджет активен, мышь нажата над окном imgui и т. д.).
    bool           WantCaptureKeyboard;        // Установите, когда Уважаемый ImGui будет использовать ввод с клавиатуры, в этом случае не отправляйте их в вашу основную
                                               // игру/приложение (в любом случае всегда передавайте ввод с клавиатуры в imgui).
                                               // (например, активен InputText или сфокусировано окно imgui и включена навигация и т. д.).
    bool           WantTextInput;              // Мобильный телефон/консоль: если установлено, вы можете отображать экранную клавиатуру. Это устанавливается Уважаемым ImGui,
                                               // когда он хочет, чтобы ввод текста с клавиатуры происходил (например, когда активен виджет InputText).
    bool           WantSetMousePos;            // MousePos был изменен, серверная часть должна перемещать мышь в следующем кадре. Редко используемый! Устанавливается только тогда,
                                               // когда флаг ImGuiConfigFlags_NavEnableSetMousePos включен.
    bool           WantSaveIniSettings;        // Когда ручной. ini активна загрузка/сохранение (io. IniFilename == NULL), это будет установлено для уведомления вашего приложения о том, что вы
                                               // можете вызвать SaveIniSettingsToMemory() и сохранить себя. Важно: очистить io. Хотите сохранить IniSettings самостоятельно после сохранения!
    bool           NavActive;                  // Навигация с клавиатуры/геймпада в настоящее время разрешена (будет обрабатывать события ImGuiKey_NavXXX) = окно
                                               // находится в фокусе и не использует флаг ImGuiWindowFlags_NoNavInputs.
    bool           NavVisible;                 // Навигация с клавиатуры/геймпада видна и разрешена (будет обрабатывать события ImGuiKey_NavXXX).
    float          Framerate;                  // Приблизительная оценка частоты кадров приложения, в кадрах в секунду. Исключительно для удобства. Оценка скользящего среднего на основе io.DeltaTime более 120 кадров.
    int            MetricsRenderVertices;      // Вывод вершин во время последнего вызова Render()
    int            MetricsRenderIndices;       // Вывод индексов во время последнего вызова Render() = количество треугольников * 3
    int            MetricsRenderWindows;       // Количество видимых окон
    int            MetricsActiveWindows;       // Количество активных окон
    int            MetricsActiveAllocations;   // Количество активных распределений, обновляемых MemAlloc/MemFree в зависимости от текущего контекста. Может быть выключен, если у вас несколько контекстов imgui.
    ImVec2         MouseDelta;                 // Дельта мыши. Обратите внимание, что это значение равно нулю, если текущая или предыдущая позиция недействительны (-FLT_MAX, -FLT_MAX),
                                               // поэтому у исчезающей/возвращающейся мыши не будет большой разницы.

    // Наследие: до версии 1.87 нам требовался бэкенд для заполнения io. KeyMap[] (imgui->native map) во время инициализации и io.KeysDown[] (собственные индексы) каждый кадр.
    // Это по-прежнему временно поддерживается как устаревшая функция. Однако новая предпочтительная схема заключается в том, что серверная часть вызывает io.AddKeyEvent().
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    int  KeyMap[ImGuiKey_COUNT];     // [НАСЛЕДИЕ] Input: отображение индексов в массив записей KeysDown[512], которые представляют ваше «родное» состояние клавиатуры. Первые 512 теперь
                                     // не используются и должны быть нулевыми. Устаревший бэкенд будет записывать в KeyMap[] с использованием индексов ImGuiKey_, которые всегда >512.
    bool KeysDown[ImGuiKey_COUNT];   // [НАСЛЕДИЕ] Input: Клавиши клавиатуры, которые нажаты (в идеале оставить в «родном» порядке, ваш движок имеет доступ к клавишам клавиатуры,
                                     // поэтому вы можете использовать свои собственные определения/перечисления для клавиш). Раньше это был размер [512].
                                     // Теперь ImGuiKey_COUNT позволяет использовать устаревший io. KeysDown[GetKeyIndex(...)] для работы без переполнения.
#endif

    //------------------------------------------------------------------
    // [Internal] Dear ImGui будет поддерживать эти поля. Прямая совместимость не гарантируется!
    //------------------------------------------------------------------

    // Состояние основного входа
    // (этот блок раньше писался бэкендом, так как 1.87 лучше НЕ писать в них напрямую, вместо этого вызывайте функции AddXXX выше)
    // (чтение из этих переменных является честной игрой, так как крайне маловероятно, что они будут двигаться куда-либо)
    ImVec2            MousePos;                         // Положение мыши в пикселях. Установите ImVec2(-FLT_MAX, -FLT_MAX), если мышь недоступна (на другом экране и т. д.)
    bool              MouseDown[5];                     // Кнопки мыши: 0=левая, 1=правая, 2=средняя + дополнительные (ImGuiMouseButton_COUNT == 5). Дорогой ImGui в основном использует левую и правую кнопки.
                                                        // Другие кнопки позволяют нам отслеживать, используется ли мышь вашим приложением + доступны для пользователя для удобства через IsMouse** API.
    float             MouseWheel;                       // Колесико мыши Вертикально: 1 единица прокручивает примерно 5 строк текста.
    float             MouseWheelH;                      // Колесо мыши Горизонтальное. У большинства пользователей нет мыши с горизонтальным колесиком, может быть не заполнено всеми бэкендами.
    ImGuiID           MouseHoveredViewport;             // (Optional) Изменить с помощью io. ДобавитьMouseViewportEvent(). С несколькими окнами просмотра: окно просмотра, на котором находится мышь операционной системы.
                                                        // Если возможно, _IGNORING_ области просмотра с флагом ImGuiViewportFlags_NoInputs намного лучше (немногие бэкенды могут это обработать).
                                                        // Установить ио. BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport, если вы можете предоставить эту информацию.
                                                        // Если вы этого не сделаете, imgui выведет значение, используя прямоугольники и время последней фокусировки в окнах просмотра,
                                                        // о которых он знает (игнорируя другие окна ОС).
    bool              KeyCtrl;                          // Модификатор клавиатуры вниз: Control
    bool              KeyShift;                         // Модификатор клавиатуры вниз: Shift
    bool              KeyAlt;                           // Модификатор клавиатуры вниз: Alt
    bool              KeySuper;                         // Модификатор клавиатуры вниз: Cmd/Super/Windows
    float             NavInputs[ImGuiNavInput_COUNT];   // Входы геймпада. Сбрасывается до нуля с помощью EndFrame(). Клавиши клавиатуры будут автоматически сопоставлены и записаны здесь с помощью NewFrame().

    // Другое состояние, поддерживаемое из данных выше + вызовы функций ввода-вывода
    ImGuiModFlags     KeyMods;                             // Ключевые флаги модов (такие же, как io. KeyCtrl/KeyShift/KeyAlt/KeySuper, но объединены в флаги), обновляются NewFrame()
    ImGuiKeyData      KeysData[ImGuiKey_KeysData_SIZE];    // Состояние ключа для всех известных ключей. Используйте функции IsKeyXXX() для доступа к этому.
    bool              WantCaptureMouseUnlessPopupClose;    // Альтернатива WantCaptureMouse: (WantCaptureMouse == true && WantCaptureMouseUnlessPopupClose == false),
                                                           // когда ожидается, что щелчок по пустоте закроет всплывающее окно.
    ImVec2            MousePosPrev;                        // Предыдущая позиция мыши (обратите внимание, что MouseDelta не обязательно == MousePos-MousePosPrev, если какая-либо позиция недействительна)
    ImVec2            MouseClickedPos[5];                  // Позиция во время нажатия
    double            MouseClickedTime[5];                 // Время последнего щелчка (используется для определения двойного щелчка)
    bool              MouseClicked[5];                     // Кнопка мыши ушла из ! Вниз вниз (аналогично MouseClickedCount[x] != 0)
    bool              MouseDoubleClicked[5];               // Была ли кнопка мыши дважды нажата? (то же, что и MouseClickedCount[x] == 2)
    ImU16             MouseClickedCount[5];                // == 0 (без щелчка), == 1 (то же, что и MouseClicked[]), == 2 (двойной щелчок), == 3 (тройной щелчок) и т. д. при переходе от ! Вниз вниз
    ImU16             MouseClickedLastCount[5];            // Подсчитайте количество последовательных кликов. Остается в силе после отпускания мыши. Сброс после очередного щелчка.
    bool              MouseReleased[5];                    // Кнопка мыши изменилась с Вниз на ! Вниз
    bool              MouseDownOwned[5];                   // Отслеживайте, была ли кнопка нажата внутри окна imgui или над пустотой, заблокированной всплывающим окном.
                                                           // Мы не запрашиваем захват мыши из приложения, если щелчок начался за пределами ImGui.
    bool              MouseDownOwnedUnlessPopupClose[5];   // Отслеживайте, была ли нажата кнопка в дорогом окне imgui.
    float             MouseDownDuration[5];                // Длительность нажатия кнопки мыши (0.0f == только что нажата)
    float             MouseDownDurationPrev[5];            // Предыдущий раз, когда кнопка мыши была нажата
    ImVec2            MouseDragMaxDistanceAbs[5];          // Максимальное расстояние, абсолютное, по каждой оси, сколько мышь прошла от точки щелчка
    float             MouseDragMaxDistanceSqr[5];          // Возведенное в квадрат максимальное расстояние того, сколько мышь прошла от точки щелчка (используется для перемещения порогов).
    float             NavInputsDownDuration[ImGuiNavInput_COUNT];
    float             NavInputsDownDurationPrev[ImGuiNavInput_COUNT];
    float             PenPressure;   // Давление касания/пера (от 0,0f до 1,0f, должно быть >0,0f, только если MouseDown[0] == true).
                                     // Вспомогательное хранилище в настоящее время не используется Dear ImGui.
    bool              AppFocusLost;
    ImS8              BackendUsingLegacyKeyArrays;       // -1: неизвестно, 0: с использованием AddKeyEvent(), 1: с использованием устаревшего ввода-вывода. KeysDown[]
    bool              BackendUsingLegacyNavInputArray;   // 0: используя AddKeyAnalogEvent(), 1: запись в устаревший io.NavInputs[] напрямую
    ImWchar16         InputQueueSurrogate;               // Для AddInputCharacterUTF16()
    ImVector<ImWchar> InputQueueCharacters;              // Очередь ввода _characters_ (получена серверной частью платформы). Заполните с помощью помощника AddInputCharacter().

    IMGUI_API         ImGuiIO();
};

//-----------------------------------------------------------------------------
// [SECTION] Разные структуры данных
//-----------------------------------------------------------------------------

// Общее состояние InputText(), переданное в качестве аргумента вашему обратному вызову при использовании флага ImGuiInputTextFlags_Callback*.
// Функция обратного вызова по умолчанию должна возвращать 0.
// Обратные вызовы (следуйте имени флага и смотрите комментарии в объявлениях ImGuiInputTextFlags_ для более подробной информации)
// - ImGuiInputTextFlags_CallbackEdit:        Обратный вызов при редактировании буфера (обратите внимание, что InputText() уже возвращает true при редактировании,
//                                            обратный вызов полезен в основном для управления базовым буфером, когда фокус активен)
// - ImGuiInputTextFlags_CallbackAlways:      Обратный вызов на каждой итерации
// - ImGuiInputTextFlags_CallbackCompletion:  Обратный вызов по нажатию TAB
// - ImGuiInputTextFlags_CallbackHistory:     Обратный вызов при нажатии стрелок вверх/вниз
// - ImGuiInputTextFlags_CallbackCharFilter:  Обратный вызов при вводе символов, чтобы заменить или отбросить их. Измените 'EventChar', чтобы заменить или отбросить, или верните 1 в обратном вызове, чтобы отбросить.
// - ImGuiInputTextFlags_CallbackResize:      Обратный вызов при запросе изменения емкости буфера (за пределами значения параметра 'buf_size'), что позволяет увеличить строку.
struct ImGuiInputTextCallbackData
{
    ImGuiInputTextFlags EventFlag;   // Один ImGuiInputTextFlags_Callback*       // Только для чтения
    ImGuiInputTextFlags Flags;       // Какой пользователь передал InputText()   // Только для чтения
    void*               UserData;    // Какой пользователь передал InputText()   // Только для чтения

    //Аргументы для различных событий обратного вызова
    // - Чтобы изменить текстовый буфер в обратном вызове, лучше использовать функцию InsertChars() / DeleteChars(). InsertChars() позаботится о вызове обратного вызова изменения размера, если это необходимо.
    // - Если вы знаете, что ваши правки не приведут к изменению размера основного буфера, вы можете напрямую изменить содержимое 'Buf[]'.
    //  Вам необходимо соответствующим образом обновить «BufTextLen» (0 <= BufTextLen < BufSize) и установить для «BufDirty» значение true, чтобы InputText мог обновлять свое внутреннее состояние.
    ImWchar             EventChar;        // Ввод символов                // Read-write   // [CharFilter] Замените символ другим или установите ноль, чтобы удалить его. return 1 эквивалентен установке EventChar=0;
    ImGuiKey            EventKey;         // Клавиша нажата (Up/Down/TAB) // Read-only    // [Completion,History]
    char*               Buf;              // Текстовый буфер              // Read-write   // [Resize] Может заменить указатель / [Завершение, История, Всегда] Записывать только указанные данные, не заменять фактический указатель!
    int                 BufTextLen;       // Длина текста (в байтах)      // Read-write   // [Resize,Completion,History,Always] Исключить хранилище с нулевым терминатором. В стране C: == strlen(some_text), в стране C++: строка. длина()
    int                 BufSize;          // Размер буфера (в байтах) = capacity+1  // Read-only    // [Resize,Completion,History,Always] Включите хранилище с нулевым терминатором. В языке C == ARRAYSIZE(my_char_array), в языке C++: строка. емкость()+1
    bool                BufDirty;         // Установите, если вы изменяете Buf/BufTextLen!  // Write        // [Completion,History,Always]
    int                 CursorPos;        //                                                // Read-write   // [Completion,History,Always]
    int                 SelectionStart;   //                                                // Read-write   // [Completion,History,Always] == to SelectionEnd when no selection)
    int                 SelectionEnd;     //                                                // Read-write   // [Completion,History,Always]

    // Вспомогательные функции для работы с текстом.
    // Используйте эти функции, чтобы извлечь выгоду из поведения CallbackResize. Вызов этих функций сбрасывает выбор.
    IMGUI_API           ImGuiInputTextCallbackData();
    IMGUI_API void      DeleteChars(int pos, int bytes_count);
    IMGUI_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    void                SelectAll()
    {
        SelectionStart = 0;
        SelectionEnd   = BufTextLen;
    }
    void ClearSelection()
    {
        SelectionStart = SelectionEnd = BufTextLen;
    }
    bool HasSelection() const
    {
        return SelectionStart != SelectionEnd;
    }
};

// Изменение размера данных обратного вызова для применения настраиваемого ограничения. Как разрешено SetNextWindowSizeConstraints(). Обратный вызов вызывается во время следующего метода Begin().
// NB: для базового ограничения минимального/максимального размера по каждой оси вам не нужно использовать обратный вызов! Параметров SetNextWindowSizeConstraints() достаточно.
struct ImGuiSizeCallbackData
{
    void*  UserData;      // Только для чтения.   Какой пользователь передал SetNextWindowSizeConstraints()
    ImVec2 Pos;           // Только для чтения.   Положение окна, для справки.
    ImVec2 CurrentSize;   // Только для чтения.   Текущий размер окна.
    ImVec2 DesiredSize;   // Читай-пиши.          Желаемый размер в зависимости от положения мыши пользователя. Напишите в это поле, чтобы ограничить изменение размера.
};

// [ALPHA] Только редко используемое / очень продвинутое использование. Используйте с функциями SetNextWindowClass() и DockSpace().
// Важно: содержимое этого класса по-прежнему находится в стадии разработки и, вероятно, будет изменено и подвергнуто рефакторингу.
// прежде чем мы стабилизируем функции стыковки. Пожалуйста, будьте внимательны при использовании этого.
// Предоставляем подсказки:
// - На серверную часть платформы через измененные флаги области просмотра (включение/выключение оформления ОС, значков панели задач ОС и т. д.)
// - К серверной части платформы для родительских/дочерних отношений области просмотра на уровне ОС.
// - В систему стыковки для различных опций и фильтрации.
struct ImGuiWindowClass
{
    ImGuiID            ClassId;                      // Данные пользователя. 0 = класс по умолчанию (неклассифицированный). Окна разных классов не могут быть состыкованы друг с другом.
    ImGuiID            ParentViewportId;             // Подсказка для бэкенда платформы. -1: использовать по умолчанию. 0: запрашивать серверную часть платформы, чтобы она не была родительской.
                                                     // != 0: запрашивать серверную часть платформы для создания отношения «родительский<>дочерний» между окнами платформы.
                                                     // Несоответствующие серверные части могут быть бесплатными для e. г. родительский каждый видовой экран к основному видовому экрану или нет.
    ImGuiViewportFlags ViewportFlagsOverrideSet;     // Флаги области просмотра для установки, когда окно этого класса владеет областью просмотра. Это позволяет вам применять оформление ОС
                                                     // или значок панели задач, переопределяя значения по умолчанию для каждого окна.
    ImGuiViewportFlags ViewportFlagsOverrideClear;   // Флаги окна просмотра, которые нужно сбросить, когда окно этого класса владеет окном просмотра. Это позволяет вам применять оформление ОС
                                                     // или значок панели задач, переопределяя значения по умолчанию для каждого окна.
    ImGuiTabItemFlags  TabItemFlagsOverrideSet;      // [EXPERIMENTAL] Флаги TabItem, которые нужно установить, когда окно этого класса отправляется на панель вкладок узла док-станции.
                                                     // Может использоваться с ImGuiTabItemFlags_Leading или ImGuiTabItemFlags_Trailing.
    ImGuiDockNodeFlags DockNodeFlagsOverrideSet;     // [EXPERIMENTAL] Флаги узла стыковки для установки, когда окно этого класса размещено на узле стыковки (его не нужно выбирать!)
    bool               DockingAlwaysTabBar;          // Установите значение true, чтобы отдельные плавающие окна этого класса всегда имели собственный узел стыковки
                                                     // (эквивалентно настройке глобального io. ConfigDockingAlwaysTabBar).
    bool               DockingAllowUnclassed;        // Установите значение true, чтобы разрешить стыковку/объединение окон этого класса с неклассифицированным окном.
                                                     // // FIXME-DOCK: перейти к переопределению DockNodeFlags?

    ImGuiWindowClass()
    {
        memset(this, 0, sizeof(*this));
        ParentViewportId      = (ImGuiID)-1;
        DockingAllowUnclassed = true;
    }
};

// Полезные данные для операций перетаскивания: AcceptDragDropPayload(), GetDragDropPayload()
struct ImGuiPayload
{
    // Члены
    void*   Data;       // Данные (скопированы и принадлежат уважаемому imgui)
    int     DataSize;   // Размер данных

    // [Internal]
    ImGuiID SourceId;           // Идентификатор исходного элемента
    ImGuiID SourceParentId;     // Родительский идентификатор источника (если доступен)
    int     DataFrameCount;     // Временная метка данных
    char    DataType[32 + 1];   // Тег типа данных (короткая строка, вводимая пользователем, не более 32 символов)
    bool    Preview;            // Устанавливается при вызове AcceptDragDropPayload() и наведении мыши на целевой элемент (примечание: обработка перекрывающихся целей перетаскивания)
    bool    Delivery;           // Устанавливается при вызове AcceptDragDropPayload() и отпускании кнопки мыши над целевым элементом.

    ImGuiPayload()
    {
        Clear();
    }
    void Clear()
    {
        SourceId = SourceParentId = 0;
        Data                      = NULL;
        DataSize                  = 0;
        memset(DataType, 0, sizeof(DataType));
        DataFrameCount = -1;
        Preview = Delivery = false;
    }
    bool IsDataType(const char* type) const
    {
        return DataFrameCount != -1 && strcmp(type, DataType) == 0;
    }
    bool IsPreview() const
    {
        return Preview;
    }
    bool IsDelivery() const
    {
        return Delivery;
    }
};

// Спецификация сортировки для одного столбца таблицы (sizeof == 12 байт)
struct ImGuiTableColumnSortSpecs
{
    ImGuiID            ColumnUserID;      // Идентификатор пользователя столбца (если указан вызовом TableSetupColumn())
    ImS16              ColumnIndex;       // Индекс столбца
    ImS16              SortOrder;         // Индекс в родительском ImGuiTableSortSpecs (всегда хранится в порядке, начиная с 0, таблицы, отсортированные по одному критерию, всегда будут иметь здесь 0)
    ImGuiSortDirection SortDirection:8;   // ImGuiSortDirection_Ascending или ImGuiSortDirection_Descending (вы можете использовать это или SortSign, в зависимости от того, что более удобно для вашей функции сортировки)

    ImGuiTableColumnSortSpecs()
    {
        memset(this, 0, sizeof(*this));
    }
};

// Спецификации сортировки для таблицы (часто обработка спецификаций сортировки для одного столбца, иногда для нескольких)
// Получено вызовом TableGetSortSpecs().
// Когда «SpecsDirty == true», вы можете сортировать данные. Это будет верно, если спецификации сортировки изменились с момента последнего вызова или в первый раз.
// Не забудьте после сортировки установить SpecsDirty = false, иначе вы можете напрасно сортировать данные в каждом кадре!
struct ImGuiTableSortSpecs
{
    const ImGuiTableColumnSortSpecs* Specs;        // Указатель для сортировки массива спецификаций.
    int                              SpecsCount;   // Сортировка количества спецификаций. Чаще всего 1. Может быть > 1, если ImGuiTableFlags_SortMulti включен. Может быть == 0, когда ImGuiTableFlags_SortTristate включен.
    bool                             SpecsDirty;   // Установите значение true, если характеристики изменились с прошлого раза! Используйте это, чтобы отсортировать снова, затем снимите флажок.

    ImGuiTableSortSpecs()
    {
        memset(this, 0, sizeof(*this));
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Помощники (ImGuiOnceUponAFrame, ImGuiTextFilter, ImGuiTextBuffer, ImGuiStorage, ImGuiListClipper, ImColor)
//-----------------------------------------------------------------------------

// Помощник: Unicode определяет
#define IM_UNICODE_CODEPOINT_INVALID 0xFFFD   // Недопустимая кодовая точка Unicode (стандартное значение).
#ifdef IMGUI_USE_WCHAR32
#define IM_UNICODE_CODEPOINT_MAX 0x10FFFF   // Максимальная кодовая точка Unicode, поддерживаемая этой сборкой.
#else
#define IM_UNICODE_CODEPOINT_MAX 0xFFFF   // Максимальная кодовая точка Unicode, поддерживаемая этой сборкой.
#endif

// Помощник: выполнение блока кода не более одного раза за кадр. Удобно, если вы хотите быстро создать пользовательский интерфейс в глубоко вложенном коде, который запускается несколько раз в каждом кадре.
// Использование: статический ImGuiOnceUponAFrame oaf; if (oaf) ImGui:: Text("Это будет вызываться только один раз за кадр");
struct ImGuiOnceUponAFrame
{
    ImGuiOnceUponAFrame()
    {
        RefFrame = -1;
    }
    mutable int RefFrame;
    operator bool() const
    {
        int current_frame = ImGui::GetFrameCount();
        if (RefFrame == current_frame)
            return false;
        RefFrame = current_frame;
        return true;
    }
};

// Помощник: Анализ и применение текстовых фильтров. В формате "ааааа[, bbbb][, ccccc]"
struct ImGuiTextFilter
{
    IMGUI_API      ImGuiTextFilter(const char* default_filter = "");
    IMGUI_API bool Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);   // Помощник, вызывающий InputText+Build
    IMGUI_API bool PassFilter(const char* text, const char* text_end = NULL) const;
    IMGUI_API void Build();
    void           Clear()
    {
        InputBuf[0] = 0;
        Build();
    }
    bool IsActive() const
    {
        return !Filters.empty();
    }

    // [Internal]
    struct ImGuiTextRange
    {
        const char* b;
        const char* e;

        ImGuiTextRange()
        {
            b = e = NULL;
        }
        ImGuiTextRange(const char* _b, const char* _e)
        {
            b = _b;
            e = _e;
        }
        bool empty() const
        {
            return b == e;
        }
        IMGUI_API void split(char separator, ImVector<ImGuiTextRange>* out) const;
    };
    char                     InputBuf[256];
    ImVector<ImGuiTextRange> Filters;
    int                      CountGrep;
};

// Helper: растущий текстовый буфер для регистрации/накопления текста
// (это можно назвать 'ImGuiTextBuilder' / 'ImGuiStringBuilder')
struct ImGuiTextBuffer
{
    ImVector<char>        Buf;
    IMGUI_API static char EmptyString[1];

    ImGuiTextBuffer() {}
    inline char operator[](int i) const
    {
        IM_ASSERT(Buf.Data != NULL);
        return Buf.Data[i];
    }
    const char* begin() const
    {
        return Buf.Data ? &Buf.front() : EmptyString;
    }
    const char* end() const
    {
        return Buf.Data ? &Buf.back() : EmptyString;
    }   // Buf завершается нулем, поэтому end() будет указывать на нулевое завершение.
    int size() const
    {
        return Buf.Size ? Buf.Size - 1 : 0;
    }
    bool empty() const
    {
        return Buf.Size <= 1;
    }
    void clear()
    {
        Buf.clear();
    }
    void reserve(int capacity)
    {
        Buf.reserve(capacity);
    }
    const char* c_str() const
    {
        return Buf.Data ? Buf.Data : EmptyString;
    }
    IMGUI_API void append(const char* str, const char* str_end = NULL);
    IMGUI_API void appendf(const char* fmt, ...) IM_FMTARGS(2);
    IMGUI_API void appendfv(const char* fmt, va_list args) IM_FMTLIST(2);
};

// Помощник: Ключ-> Хранение значений
// Как правило, вам не нужно беспокоиться об этом, так как хранилище содержится в каждом окне.
// Мы используем его для e. г. сохранить состояние коллапса для дерева (Int 0/1)
// Это оптимизировано для эффективного поиска (дихотомия в непрерывном буфере) и редкой вставки (обычно привязанной к взаимодействиям с пользователем, т. е. не более одного раза в кадре)
// Вы можете использовать его как пользовательское хранилище для временных значений. Объявите собственное хранилище, если, например:
// - Вы хотите манипулировать состоянием открытия/закрытия определенного поддерева в вашем интерфейсе (узел дерева использует Int 0/1 для хранения своего состояния).
// - Вы хотите легко хранить пользовательские отладочные данные без добавления или редактирования структур в коде (вероятно, неэффективно, но удобно)
// Типы НЕ сохраняются, поэтому вы должны убедиться, что ваш ключ не конфликтует с другими типами.
struct ImGuiStorage
{
    // [Internal]
    struct ImGuiStoragePair
    {
        ImGuiID key;
        union
        {
            int   val_i;
            float val_f;
            void* val_p;
        };
        ImGuiStoragePair(ImGuiID _key, int _val_i)
        {
            key   = _key;
            val_i = _val_i;
        }
        ImGuiStoragePair(ImGuiID _key, float _val_f)
        {
            key   = _key;
            val_f = _val_f;
        }
        ImGuiStoragePair(ImGuiID _key, void* _val_p)
        {
            key   = _key;
            val_p = _val_p;
        }
    };

    ImVector<ImGuiStoragePair> Data;

    // - Get***() функции находят пару, никогда не добавляют/не выделяют. Пары сортируются, поэтому запрос равен O (log N)
    // - Set***() функции поиска пары, вставка по запросу, если отсутствует.
    // - Сортированная вставка стоит дорого, оплачивается один раз. В типичный кадр не нужно вставлять новую пару.
    void                       Clear()
    {
        Data.clear();
    }
    IMGUI_API int    GetInt(ImGuiID key, int default_val = 0) const;
    IMGUI_API void   SetInt(ImGuiID key, int val);
    IMGUI_API bool   GetBool(ImGuiID key, bool default_val = false) const;
    IMGUI_API void   SetBool(ImGuiID key, bool val);
    IMGUI_API float  GetFloat(ImGuiID key, float default_val = 0.0f) const;
    IMGUI_API void   SetFloat(ImGuiID key, float val);
    IMGUI_API void*  GetVoidPtr(ImGuiID key) const;   // default_val is NULL
    IMGUI_API void   SetVoidPtr(ImGuiID key, void* val);

    // - Get***Ref() функции находят пару, вставляют по запросу, если отсутствуют, возвращают указатель. Полезно, если вы собираетесь использовать Get+Set.
    // - Ссылки действительны только до тех пор, пока в хранилище не будет добавлено новое значение. Вызов функции Set***() или Get***Ref() делает указатель недействительным.
    // - типичный вариант использования, когда это удобно для быстрого взлома (например, добавить хранилище во время сеанса редактирования и продолжения в реальном времени,
    //   если вы не можете изменить существующую структуру)
    // float* pvar = ImGui:: GetFloatRef(key); ImGui:: SliderFloat("var", pvar, 0, 100.0f); some_var += *pvar;
    IMGUI_API int*   GetIntRef(ImGuiID key, int default_val = 0);
    IMGUI_API bool*  GetBoolRef(ImGuiID key, bool default_val = false);
    IMGUI_API float* GetFloatRef(ImGuiID key, float default_val = 0.0f);
    IMGUI_API void** GetVoidPtrRef(ImGuiID key, void* default_val = NULL);

    // Используйте в своем собственном хранилище, если вы знаете, что хранятся только целые числа (открыть/закрыть все узлы дерева)
    IMGUI_API void   SetAllInt(int val);

    // Для более быстрой полной перестройки хранилища (вместо инкрементной) вы можете добавить все свое содержимое, а затем отсортировать один раз.
    IMGUI_API void   BuildSortByKey();
};

// Помощник: вручную обрезать большой список элементов.
// Если у вас много равномерно расположенных элементов и у вас есть произвольный доступ к списку, вы можете выполнить грубое
// отсечение на основе видимости, чтобы отправлять только те элементы, которые находятся в поле зрения.
// Ограничитель вычисляет диапазон видимых элементов и перемещает курсор, чтобы компенсировать невидимые элементы, которые мы пропустили.
// (Уважаемый ImGui уже обрезает элементы на основе их границ, но: для этого ему нужно сначала разместить элемент, и, как правило,
// получение/отправка ваших собственных данных влечет за собой дополнительные расходы. Грубая обрезка с использованием ImGuiListClipper позволяет легко
// без проблем масштабируем с помощью списков с десятками тысяч элементов)
// Применение:
//   ImGuiListClipper clipper;
//   clipper.Begin(1000);         // We have 1000 elements, evenly spaced.
//   while (clipper.Step())
//       for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
//           ImGui::Text("line number %d", i);
// Вообще то, что происходит:
// - Clipper позволяет обрабатывать первый элемент (DisplayStart = 0, DisplayEnd = 1) независимо от того, виден он или нет.
// - Пользовательский код отправляет этот один элемент.
// - Clipper может измерить высоту первого элемента
// - Clipper вычисляет фактический диапазон отображаемых элементов на основе текущего прямоугольника отсечения, позиционирует курсор перед первым видимым элементом.
// - Пользовательский код отправляет видимые элементы.
// - Clipper также обрабатывает различные тонкости, связанные с навигацией клавиатуры/геймпада, обтеканием и т. д.
struct ImGuiListClipper
{
    int       DisplayStart;   // Первый отображаемый элемент, обновляемый при каждом вызове Step()
    int       DisplayEnd;     // Конец элементов для отображения (эксклюзивно)
    int       ItemsCount;     // [Internal] Количество предметов
    float     ItemsHeight;    // [Internal] Высота предмета после первого шага и отправка предмета могут рассчитать его
    float     StartPosY;      // [Internal] Положение курсора во время Begin() или после обработки всех замороженных строк таблицы
    void*     TempData;       // [Internal] Внутренние данные

    // items_count: используйте INT_MAX, если вы не знаете, сколько у вас предметов (в этом случае курсор не будет перемещаться на последнем шаге)
    // items_height: Используйте -1.0f для автоматического расчета на первом шаге. В противном случае укажите расстояние между вашими элементами, как правило, GetTextLineHeightWithSpacing() или GetFrameHeightWithSpacing().
    IMGUI_API ImGuiListClipper();
    IMGUI_API ~ImGuiListClipper();
    IMGUI_API void Begin(int items_count, float items_height = -1.0f);
    IMGUI_API void End();    // Автоматически вызывается при последнем вызове Step(), который возвращает false.
    IMGUI_API bool Step();   // Звоните до тех пор, пока он не вернет false. Поля DisplayStart/DisplayEnd будут установлены, и вы сможете обрабатывать/отрисовывать эти элементы.

    // Вызовите ForceDisplayRangeByIndices() перед первым вызовом Step(), если вам нужно, чтобы диапазон элементов отображался независимо от видимости.
    // item_max является эксклюзивным e. г. используйте (42, 42+1), чтобы сделать элемент 42 всегда видимым, НО из-за выравнивания/дополнения некоторых элементов вполне вероятно,
    // что дополнительный элемент может быть включен на любом конце диапазона отображения.
    IMGUI_API void ForceDisplayRangeByIndices(int item_min, int item_max);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    inline ImGuiListClipper(int items_count, float items_height = -1.0f)
    {
        memset(this, 0, sizeof(*this));
        ItemsCount = -1;
        Begin(items_count, items_height);
    }   // [removed in 1.79]
#endif
};

// Вспомогательные макросы для генерации 32-битных закодированных цветов
// Пользователь может объявить свой собственный формат, #определив 5 макросов _SHIFT/_MASK в своем файле imconfig.
#ifndef IM_COL32_R_SHIFT
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT 16
#define IM_COL32_G_SHIFT 8
#define IM_COL32_B_SHIFT 0
#define IM_COL32_A_SHIFT 24
#define IM_COL32_A_MASK  0xFF000000
#else
#define IM_COL32_R_SHIFT 0
#define IM_COL32_G_SHIFT 8
#define IM_COL32_B_SHIFT 16
#define IM_COL32_A_SHIFT 24
#define IM_COL32_A_MASK  0xFF000000
#endif
#endif
#define IM_COL32(R, G, B, A) (((ImU32)(A) << IM_COL32_A_SHIFT) | ((ImU32)(B) << IM_COL32_B_SHIFT) | ((ImU32)(G) << IM_COL32_G_SHIFT) | ((ImU32)(R) << IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255, 255, 255, 255)   // Непрозрачный белый = 0xFFFFFFFF
#define IM_COL32_BLACK       IM_COL32(0, 0, 0, 255)         // Непрозрачный черный
#define IM_COL32_BLACK_TRANS IM_COL32(0, 0, 0, 0)           // Прозрачный черный = 0x00000000

// Помощник: ImColor() неявно преобразует цвета либо в ImU32 (упакованный 4x1 байт), либо в ImVec4 (4x1 с плавающей запятой).
// Предпочитайте использование макросов IM_COL32(), если вам нужен ImU32 с гарантированным временем компиляции для использования с ImDrawList API.
// ** Избегайте хранения ImColor! Сохраните либо u32, либо ImVec4. Это не полнофункциональный цветовой класс. МОЖЕТ УСТАРЕТЬ.
// ** Ни один из API ImGui не использует ImColor напрямую, но вы можете использовать его для удобства передачи цветов в форматах ImU32 или ImVec4. Явное приведение к ImU32 или ImVec4, если это необходимо.
struct ImColor
{
    ImVec4 Value;

    constexpr ImColor() {}
    constexpr ImColor(float r, float g, float b, float a = 1.0f): Value(r, g, b, a) {}
    constexpr ImColor(const ImVec4& col): Value(col) {}
    ImColor(int r, int g, int b, int a = 255)
    {
        float sc = 1.0f / 255.0f;
        Value.x  = (float)r * sc;
        Value.y  = (float)g * sc;
        Value.z  = (float)b * sc;
        Value.w  = (float)a * sc;
    }
    ImColor(ImU32 rgba)
    {
        float sc = 1.0f / 255.0f;
        Value.x  = (float)((rgba >> IM_COL32_R_SHIFT) & 0xFF) * sc;
        Value.y  = (float)((rgba >> IM_COL32_G_SHIFT) & 0xFF) * sc;
        Value.z  = (float)((rgba >> IM_COL32_B_SHIFT) & 0xFF) * sc;
        Value.w  = (float)((rgba >> IM_COL32_A_SHIFT) & 0xFF) * sc;
    }
    inline operator ImU32() const
    {
        return ImGui::ColorConvertFloat4ToU32(Value);
    }
    inline operator ImVec4() const
    {
        return Value;
    }

    // FIXME-OBSOLETE: может потребоваться удалить/очистить эти помощники.
    inline void SetHSV(float h, float s, float v, float a = 1.0f)
    {
        ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z);
        Value.w = a;
    }
    static ImColor HSV(float h, float s, float v, float a = 1.0f)
    {
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return ImColor(r, g, b, a);
    }
};

//-----------------------------------------------------------------------------
// [SECTION] API рисования (ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawListFlags, ImDrawList, ImDrawData)
// Держите серию команд рисования. Пользователь предоставляет средство визуализации для ImDrawData, которое по существу содержит массив ImDrawList.
//-----------------------------------------------------------------------------

// Максимальная ширина линии для запекания сглаженных текстур. Создайте атлас с ImFontAtlasFlags_NoBakedLines, чтобы отключить запекание.
#ifndef IM_DRAWLIST_TEX_LINES_WIDTH_MAX
#define IM_DRAWLIST_TEX_LINES_WIDTH_MAX (63)
#endif

// ImDrawCallback: Нарисуйте обратные вызовы для расширенного использования [настраиваемый тип: переопределить в imconfig. час]
// NB: вам, скорее всего, НЕ нужно использовать обратные вызовы отрисовки только для создания собственного виджета или настраиваемого рендеринга пользовательского интерфейса,
// для этого вы можете ткнуть в список отрисовки! Обратный вызов Draw может быть полезен, например, для:
// A) Измените состояние рендеринга графического процессора,
// B) визуализировать сложную 3D-сцену внутри элемента пользовательского интерфейса без промежуточной текстуры/цели рендеринга и т. д.
// Ожидаемое поведение от вашей функции рендеринга: 'if (cmd. UserCallback != NULL) { cmd. UserCallback(parent_list, cmd); } еще {RenderTriangles() }'
// Если вы хотите переопределить подпись ImDrawCallback, вы можете просто использовать e. г. '#define ImDrawCallback MyDrawCallback' (в imconfig.h) + соответствующим образом обновить серверную часть рендеринга.
#ifndef ImDrawCallback
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
#endif

// Специальное значение обратного вызова Draw, чтобы запросить у бэкенда рендерера сброс состояния графики/рендеринга.
// Бэкенд рендерера должен обрабатывать это специальное значение, иначе произойдет сбой при попытке вызвать функцию по этому адресу.
// Это полезно, например, если вы отправили обратные вызовы, которые, как вы знаете, изменили состояние рендеринга, и вы хотите, чтобы оно было восстановлено.
// Это не делается по умолчанию, потому что это очень полезный способ изменить состояние рендеринга для содержимого imgui (например, изменить настройки шейдера/смешивания перед вызовом изображения).
#define ImDrawCallback_ResetRenderState (ImDrawCallback)(-1)

// Как правило, 1 команда = 1 вызов отрисовки графического процессора (если только команда не является обратным вызовом).
// - VtxOffset: Когда 'io. BackendFlags и ImGuiBackendFlags_RendererHasVtxOffset включены,
// эти поля позволяют нам визуализировать сетки размером более 64 000 вершин, сохраняя при этом 16-битные индексы.
// Бэкэнды сделаны для <1.71. обычно игнорирует поля VtxOffset.
// - Поля ClipRect/TextureId/VtxOffset должны быть непрерывными, так как мы memcmp() их вместе (это утверждается для).
struct ImDrawCmd
{
    ImVec4         ClipRect;           // 4*4  // Отсекающий прямоугольник (x1, y1, x2, y2). Вычтите ImDrawData->DisplayPos, чтобы получить прямоугольник обрезки в координатах «видового экрана».
    ImTextureID    TextureId;          // 4-8  // Пользовательский идентификатор текстуры. Устанавливается пользователем в ImfontAtlas::SetTexID() для шрифтов или передается функциям Image*().
                                       // Игнорируйте, если никогда не используете изображения или атлас с несколькими шрифтами.
    unsigned int   VtxOffset;          // 4    // Начальное смещение в буфере вершин. ImGuiBackendFlags_RendererHasVtxOffset: всегда 0, в противном случае может быть > 0
                                       // для поддержки сеток размером более 64 000 вершин с 16-битными индексами.
    unsigned int   IdxOffset;          // 4    // Начальное смещение в индексном буфере.
    unsigned int   ElemCount;          // 4    // Количество индексов (кратное 3), которые должны отображаться в виде треугольников. Вершины хранятся в массиве vtx_buffer[] вызываемого ImDrawList, индексы в idx_buffer[].
    ImDrawCallback UserCallback;       // 4-8  // If != NULL, вызовите функцию вместо рендеринга вершин. clip_rect и texture_id будут установлены нормально.
    void*          UserCallbackData;   // 4-8  // Код обратного вызова отрисовки может получить к нему доступ.

    ImDrawCmd()
    {
        memset(this, 0, sizeof(*this));
    }   // Также убедитесь, что наши поля заполнения обнулены.

    // Начиная с версии 1.83: возвращает ImTextureID, связанный с этим вызовом отрисовки. Предупреждение: НЕ предполагайте, что это всегда то же самое, что и «TextureId» (мы изменим эту функцию для будущей функции)
    inline ImTextureID GetTexID() const
    {
        return TextureId;
    }
};

// Компоновка вершин
#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
    ImVec2 pos;
    ImVec2 uv;
    ImU32  col;
};
#else
// Вы можете переопределить макет формата вершин, определив IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT в imconfig. час
// Код ожидает ImVec2 pos (8 байт), ImVec2 uv (8 байт), ImU32 col (4 байта), но вы можете изменить их порядок или добавить другие поля по мере необходимости, чтобы упростить интеграцию в ваш движок.
// Тип должен быть описан внутри макроса (вы можете либо объявить структуру, либо использовать typedef). Это связано с тем, что ImVec2/ImU32, скорее всего, не объявлены в то время, когда вы хотите настроить свой тип.
// ПРИМЕЧАНИЕ: IMGUI НЕ ОЧИЩАЕТ СТРУКТУРУ И НЕ ВЫЗЫВАЕТ КОНСТРУКТОР, ПОЭТОМУ ЛЮБОЕ ПОЛЬЗОВАТЕЛЬСКОЕ ПОЛЕ БУДЕТ НЕИНИЦИАЛИЗИРОВАНО.
// ЕСЛИ ВЫ ДОБАВЛЯЕТЕ ДОПОЛНИТЕЛЬНЫЕ ПОЛЯ (НАПРИМЕР, КООРДИНАТЫ 'Z'), ВАМ НЕОБХОДИМО ОЧИСТИТЬ ИХ ВО ВРЕМЯ ВИЗУАЛИЗАЦИИ ИЛИ ИГНОРИРОВАТЬ ИХ.
IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

// [Internal] Для использования ImDrawList
struct ImDrawCmdHeader
{
    ImVec4       ClipRect;
    ImTextureID  TextureId;
    unsigned int VtxOffset;
};

// [Internal] Для использования ImDrawListSplitter
struct ImDrawChannel
{
    ImVector<ImDrawCmd> _CmdBuffer;
    ImVector<ImDrawIdx> _IdxBuffer;
};

// Функции разделения/объединения используются для разделения списка рисования на разные слои, которые можно рисовать не по порядку.
// Это используется API Columns/Tables, поэтому элементы каждого столбца могут быть объединены вместе в одном вызове отрисовки.
struct ImDrawListSplitter
{
    int                     _Current;    // Текущий номер канала (0)
    int                     _Count;      // Количество активных каналов (1+)
    ImVector<ImDrawChannel> _Channels;   // Отрисовка каналов (размер не уменьшается, поэтому _Count может быть < Channels. Size)

    inline ImDrawListSplitter()
    {
        memset(this, 0, sizeof(*this));
    }
    inline ~ImDrawListSplitter()
    {
        ClearFreeMemory();
    }
    inline void Clear()
    {
        _Current = 0;
        _Count   = 1;
    }   // Не очищайте Channels[], чтобы наши выделения использовались повторно в следующем кадре.
    IMGUI_API void ClearFreeMemory();
    IMGUI_API void Split(ImDrawList* draw_list, int count);
    IMGUI_API void Merge(ImDrawList* draw_list);
    IMGUI_API void SetCurrentChannel(ImDrawList* draw_list, int channel_idx);
};

// Флаги для функций ImDrawList
// (Наследие: бит 0 всегда должен соответствовать ImDrawFlags_Closed для обратной совместимости со старым API с использованием логического значения. Биты 1..3 не должны использоваться)
enum ImDrawFlags_
{
    ImDrawFlags_None                    = 0,
    ImDrawFlags_Closed                  = 1 << 0,   // PathStroke(), AddPolyline(): укажите, что форма должна быть закрыта (Важно: это всегда == 1 по устаревшей причине)
    ImDrawFlags_RoundCornersTopLeft     = 1 << 4,   // AddRect(), AddRectFilled(), PathRect(): включить округление только верхнего левого угла (при округлении > 0.0f мы по умолчанию используем все углы). Был 0x01.
    ImDrawFlags_RoundCornersTopRight    = 1 << 5,   // AddRect(), AddRectFilled(), PathRect(): включить скругление только правого верхнего угла (при округлении > 0,0f мы по умолчанию используем все углы). Был 0x02.
    ImDrawFlags_RoundCornersBottomLeft  = 1 << 6,   // AddRect(), AddRectFilled(), PathRect(): включить скругление только нижнего левого угла (при округлении > 0.0f по умолчанию используются все углы). Был 0x04.
    ImDrawFlags_RoundCornersBottomRight = 1 << 7,   // AddRect(), AddRectFilled(), PathRect(): включить скругление только нижнего правого угла (при округлении > 0.0f мы по умолчанию используем все углы). Воск 0x08.
    ImDrawFlags_RoundCornersNone        = 1 << 8,   // AddRect(), AddRectFilled(), PathRect(): отключить округление на всех углах (при округлении > 0.0f). Это НЕ ноль, НЕ неявный флаг!
    ImDrawFlags_RoundCornersTop         = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersBottom      = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersLeft        = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft,
    ImDrawFlags_RoundCornersRight       = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersAll         = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersDefault_    = ImDrawFlags_RoundCornersAll,   // По умолчанию для ВСЕХ углов, если ни один из флагов _RoundCornersXX не указан.
    ImDrawFlags_RoundCornersMask_       = ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone
};

// Флаги для экземпляра ImDrawList. Они устанавливаются автоматически функциями ImGui:: из настроек ImGuiIO и, как правило, не манипулируются напрямую.
// Однако можно временно изменить флаги между вызовами функций ImDrawList::.
enum ImDrawListFlags_
{
    ImDrawListFlags_None                   = 0,
    ImDrawListFlags_AntiAliasedLines       = 1 << 0,   // Включить сглаженные линии/границы (*2 количество треугольников для линии шириной 1,0f или линий, достаточно тонких,
                                                       // чтобы их можно было рисовать с помощью текстур, в противном случае *3 количество треугольников)
    ImDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,   // По возможности включите сглаженные линии/границы с использованием текстур.
                                                       // Требовать от бэкэнда рендеринга с билинейной фильтрацией (НЕ точечная/ближайшая фильтрация).
    ImDrawListFlags_AntiAliasedFill        = 1 << 2,   // Включите сглаживание краев вокруг заполненных фигур (прямоугольников со скругленными углами, кругов).
    ImDrawListFlags_AllowVtxOffset         = 1 << 3    // Может выдавать «VtxOffset> 0», чтобы разрешить большие сетки. Устанавливается, когда ImGuiBackendFlags_RendererHasVtxOffset включен.
};

// Список команд рисования
// Это низкоуровневый список полигонов, которые заполняют функции ImGui::. В конце кадра,
// все списки команд передаются в вашу функцию ImGuiIO::RenderDrawListFn для рендеринга.
// Каждое дорогое окно imgui содержит свой собственный ImDrawList. Вы можете использовать ImGui:: GetWindowDrawList() для
// доступ к списку отрисовки текущего окна и отрисовка пользовательских примитивов.
// Вы можете чередовать обычные вызовы ImGui:: и добавлять примитивы в текущий список отрисовки.
// В режиме одного окна просмотра верхний левый == GetMainViewport()->Pos (обычно 0,0), нижний правый == GetMainViewport()->Pos+Size (обычно io. DisplaySize).
// Вы абсолютно свободны применять любую матрицу преобразования к данным (в зависимости от использования преобразования вы можете захотеть применить его и к ClipRect!)
// Важно: примитивы всегда добавляются в список, а не отбраковываются (отбор выполняется на более высоком уровне с помощью функций ImGui::),
// если вы часто используете этот API, подумайте о грубом отбраковывании ваших нарисованных объектов.
struct ImDrawList
{
    // Это то, что вы должны сделать
    ImVector<ImDrawCmd>         CmdBuffer;   // Команды рисования. Обычно 1 команда = 1 вызов отрисовки графического процессора, если только команда не является обратным вызовом.
    ImVector<ImDrawIdx>         IdxBuffer;   // Индексный буфер. Каждая команда потребляет ImDrawCmd::ElemCount из тех
    ImVector<ImDrawVert>        VtxBuffer;   // Буфер вершин.
    ImDrawListFlags             Flags;       // Флаги, вы можете использовать их, чтобы настроить параметры сглаживания для каждого примитива.

    // [Внутренний, используется при построении списков]
    unsigned int                _VtxCurrentIdx;    // [Internal] вообще == VtxBuffer. Размер, если мы не превысим 64K вершин, в этом случае он сбрасывается до 0.
    const ImDrawListSharedData* _Data;             // Указатель на общие данные рисования (вы можете использовать ImGui:: GetDrawListSharedData(), чтобы получить его из текущего контекста ImGui)
    const char*                 _OwnerName;        // Указатель на имя окна владельца для отладки
    ImDrawVert*                 _VtxWritePtr;      // [Internal] точка в VtxBuffer. Данные после каждой команды добавления (чтобы избежать чрезмерного использования операторов ImVector<>)
    ImDrawIdx*                  _IdxWritePtr;      // [Internal] точка в IdxBuffer. Данные после каждой команды добавления (чтобы избежать чрезмерного использования операторов ImVector<>)
    ImVector<ImVec4>            _ClipRectStack;    // [Internal]
    ImVector<ImTextureID>       _TextureIdStack;   // [Internal]
    ImVector<ImVec2>            _Path;             // [Internal] Текущее построение пути
    ImDrawCmdHeader             _CmdHeader;        // [Internal] шаблон активных команд. Поля должны соответствовать полям CmdBuffer. назад().
    ImDrawListSplitter          _Splitter;         // [Internal] для каналов API (примечание: лучше использовать собственный постоянный экземпляр ImDrawListSplitter!)
    float                       _FringeScale;      // [Internal] это значение масштабирует полосу сглаживания, это помогает сохранять резкость при масштабировании содержимого буфера вершин.

    // Если вы хотите создать экземпляры ImDrawList, передайте им ImGui:: GetDrawListSharedData() или создайте и используйте свои собственные ImDrawListSharedData (чтобы вы могли использовать ImDrawList без ImGui)
    ImDrawList(const ImDrawListSharedData* shared_data)
    {
        memset(this, 0, sizeof(*this));
        _Data = shared_data;
    }

    ~ImDrawList()
    {
        _ClearFreeMemory();
    }
    // Ножницы на уровне рендеринга. Это передается вашей функции рендеринга, но не используется для грубого отсечения на стороне процессора.
    // Предпочитаю использовать высокоуровневый ImGui:: PushClipRect() для воздействия на логику (проверка на попадание и отбраковка виджетов).
    IMGUI_API void PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect = false);
    IMGUI_API void PushClipRectFullScreen();
    IMGUI_API void PopClipRect();
    IMGUI_API void PushTextureID(ImTextureID texture_id);
    IMGUI_API void PopTextureID();
    inline ImVec2  GetClipRectMin() const
    {
        const ImVec4& cr = _ClipRectStack.back();
        return ImVec2(cr.x, cr.y);
    }
    inline ImVec2 GetClipRectMax() const
    {
        const ImVec4& cr = _ClipRectStack.back();
        return ImVec2(cr.z, cr.w);
    }

    // примитивы
    // - Заполненные формы всегда должны использовать порядок намотки по часовой стрелке. От этого зависит полоса сглаживания. Фигуры против часовой стрелки будут иметь «внутреннее» сглаживание.
    // - Для прямоугольных примитивов "p_min" и "p_max" представляют верхний левый и нижний правый углы.
    // - Для примитивов круга используйте "num_segments == 0" для автоматического вычисления тесселяции (предпочтительно).
    //   В более старых версиях (до Дорогой ImGui 1.77) функции AddCircle по умолчанию имели значение num_segments == 12.
    //   В будущих версиях мы будем использовать текстуры, чтобы обеспечить более дешевые и качественные круги.
    //   Используйте функции AddNgon() и AddNgonFilled(), если вам нужно гарантировать определенное количество сторон.
    IMGUI_API void AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);
    IMGUI_API void AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size)
    IMGUI_API void AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0);                     // a: upper-left, b: lower-right (== upper-left + size)
    IMGUI_API void AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    IMGUI_API void AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f);
    IMGUI_API void AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col);
    IMGUI_API void AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
    IMGUI_API void AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);
    IMGUI_API void AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f);
    IMGUI_API void AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 0);
    IMGUI_API void AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f);
    IMGUI_API void AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments);
    IMGUI_API void AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
    IMGUI_API void AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
    IMGUI_API void AddPolyline(const ImVec2* points, int num_points, ImU32 col, ImDrawFlags flags, float thickness);
    IMGUI_API void AddConvexPolyFilled(const ImVec2* points, int num_points, ImU32 col);
    IMGUI_API void AddBezierCubic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0);   // Кубический Безье (4 контрольные точки)
    IMGUI_API void AddBezierQuadratic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness, int num_segments = 0);                 // Квадратичный Безье (3 контрольные точки)

    // Примитивы изображения
    // - Прочтите FAQ, чтобы понять, что такое ImTextureID.
    // - "p_min" и "p_max" представляют верхний левый и нижний правый углы прямоугольника.
    // - "uv_min" и "uv_max" представляют нормализованные координаты текстуры для использования в этих углах. Использование координат текстуры (0,0)->(1,1) обычно отображает всю текстуру.
    IMGUI_API void AddImage(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void AddImageQuad(ImTextureID user_texture_id, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1 = ImVec2(0, 0), const ImVec2& uv2 = ImVec2(1, 0), const ImVec2& uv3 = ImVec2(1, 1), const ImVec2& uv4 = ImVec2(0, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void AddImageRounded(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawFlags flags = 0);

    // API пути с отслеживанием состояния, добавьте точки, затем завершите с помощью PathFillConvex() или PathStroke()
    // - Заполненные фигуры всегда должны использовать порядок намотки по часовой стрелке. От этого зависит полоса сглаживания. Фигуры против часовой стрелки будут иметь «внутреннее» сглаживание.
    inline void    PathClear()
    {
        _Path.Size = 0;
    }
    inline void PathLineTo(const ImVec2& pos)
    {
        _Path.push_back(pos);
    }
    inline void PathLineToMergeDuplicate(const ImVec2& pos)
    {
        if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0)
            _Path.push_back(pos);
    }
    inline void PathFillConvex(ImU32 col)
    {
        AddConvexPolyFilled(_Path.Data, _Path.Size, col);
        _Path.Size = 0;
    }
    inline void PathStroke(ImU32 col, ImDrawFlags flags = 0, float thickness = 1.0f)
    {
        AddPolyline(_Path.Data, _Path.Size, col, flags, thickness);
        _Path.Size = 0;
    }
    IMGUI_API void        PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments = 0);
    IMGUI_API void        PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12);                  // Используйте предварительно рассчитанные углы для круга из 12 шагов
    IMGUI_API void        PathBezierCubicCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0);   // Кубический Безье (4 контрольные точки)
    IMGUI_API void        PathBezierQuadraticCurveTo(const ImVec2& p2, const ImVec2& p3, int num_segments = 0);                 // Квадратичный Безье (3 контрольные точки)
    IMGUI_API void        PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, ImDrawFlags flags = 0);

    // Advanced
    IMGUI_API void        AddCallback(ImDrawCallback callback, void* callback_data);   // Ваша функция рендеринга должна проверять «UserCallback» в ImDrawCmd и вызывать функцию вместо рендеринга треугольников.
    IMGUI_API void        AddDrawCmd();                                                // Это полезно, если вам нужно принудительно создать новый вызов отрисовки (чтобы разрешить зависимый рендеринг/смешивание).
                                                                                       // В противном случае примитивы максимально объединяются в один и тот же вызов отрисовки.
    IMGUI_API ImDrawList* CloneOutput() const;                                         // Создайте клон CmdBuffer/IdxBuffer/VtxBuffer.

    // Advanced: Channels
    // - Используйте для разделения рендера на слои. Переключая каналы, чтобы можно было отображать не по порядку (например, отправлять примитивы FG перед примитивами BG)
    // — Используйте, чтобы свести к минимуму вызовы отрисовки (например, при переходе между несколькими прямоугольниками отсечения предпочитайте добавлять их в отдельные каналы, а затем объединять в конце)
    // - FIXME-OBSOLETE: Этот API изначально не должен был быть в ImDrawList!
    //   Предпочитаю использовать собственный постоянный экземпляр ImDrawListSplitter, так как вы можете складывать их друг в друга.
    //   Используя ImDrawList::ChannelsXXXX, вы не можете накладывать один фрагмент на другой.
    inline void           ChannelsSplit(int count)
    {
        _Splitter.Split(this, count);
    }
    inline void ChannelsMerge()
    {
        _Splitter.Merge(this);
    }
    inline void ChannelsSetCurrent(int n)
    {
        _Splitter.SetCurrentChannel(this, n);
    }

    // Advanced: Распределение примитивов
    // - Отрисовываем треугольники (три вершины)
    // - Все примитивы должны быть предварительно зарезервированы через PrimReserve().
    IMGUI_API void PrimReserve(int idx_count, int vtx_count);
    IMGUI_API void PrimUnreserve(int idx_count, int vtx_count);
    IMGUI_API void PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);   // Выровненный по оси прямоугольник (состоит из двух треугольников)
    IMGUI_API void PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
    IMGUI_API void PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
    inline void    PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)
    {
        _VtxWritePtr->pos = pos;
        _VtxWritePtr->uv  = uv;
        _VtxWritePtr->col = col;
        _VtxWritePtr++;
        _VtxCurrentIdx++;
    }
    inline void PrimWriteIdx(ImDrawIdx idx)
    {
        *_IdxWritePtr = idx;
        _IdxWritePtr++;
    }
    inline void PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)
    {
        PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx);
        PrimWriteVtx(pos, uv, col);
    }   // Запишите вершину с уникальным индексом

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    inline void AddBezierCurve(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0)
    {
        AddBezierCubic(p1, p2, p3, p4, col, thickness, num_segments);
    }   // УСТАРЕЛО в версии 1.80 (январь 2021 г.)
    inline void PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0)
    {
        PathBezierCubicCurveTo(p2, p3, p4, num_segments);
    }   // УСТАРЕЛО в версии 1.80 (январь 2021 г.)
#endif

    // [Внутренние помощники]
    IMGUI_API void _ResetForNewFrame();
    IMGUI_API void _ClearFreeMemory();
    IMGUI_API void _PopUnusedDrawCmd();
    IMGUI_API void _TryMergeDrawCmds();
    IMGUI_API void _OnChangedClipRect();
    IMGUI_API void _OnChangedTextureID();
    IMGUI_API void _OnChangedVtxOffset();
    IMGUI_API int  _CalcCircleAutoSegmentCount(float radius) const;
    IMGUI_API void _PathArcToFastEx(const ImVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step);
    IMGUI_API void _PathArcToN(const ImVec2& center, float radius, float a_min, float a_max, int num_segments);
};

// Все данные отрисовки для рендеринга фрейма Dear ImGui
// (Примечание: стиль и соглашение об именах здесь немного противоречивы, в настоящее время мы сохраняем их для целей обратной совместимости,
// так как это одна из старейших структур, предоставляемых библиотекой! По сути, ImDrawList == CmdList)
struct ImDrawData
{
    bool           Valid;              // Действителен только после вызова Render() и до вызова следующего NewFrame().
    int            CmdListsCount;      // Количество ImDrawList* для рендеринга
    int            TotalIdxCount;      // Для удобства сумма всех IdxBuffer ImDrawList. Размер
    int            TotalVtxCount;      // Для удобства сумма всех VtxBuffer ImDrawList. Размер
    ImDrawList**   CmdLists;           // Массив ImDrawList* для рендеринга. ImDrawList принадлежит ImGuiContext и указывает на него только отсюда.
    ImVec2         DisplayPos;         // Верхнее левое положение окна просмотра для рендеринга (== верхний левый угол используемой матрицы ортогональной проекции)
                                       // (== GetMainViewport()->Pos для основного окна просмотра, == (0.0) в большинстве приложений с одним окном просмотра)
    ImVec2         DisplaySize;        // Размер области просмотра для рендеринга (== GetMainViewport()->Размер основного окна просмотра, == io. DisplaySize в большинстве приложений с одним окном просмотра)
    ImVec2         FramebufferScale;   // Количество пикселей для каждой единицы DisplaySize. На основе ио. DisplayFramebufferScale. Обычно (1,1) на обычном дисплее, (2,2) на OSX с дисплеем Retina.
    ImGuiViewport* OwnerViewport;      // Область просмотра, несущая экземпляр ImDrawData, может быть полезна для средства визуализации (как правило, нет).

    // Functions
    ImDrawData()
    {
        Clear();
    }
    void Clear()
    {
        memset(this, 0, sizeof(*this));
    }                                                        // ImDrawList принадлежит ImGuiContext!
    IMGUI_API void DeIndexAllBuffers();                      // Помощник для преобразования всех буферов из индексированных в неиндексированные, если вы не можете сделать индексированные.
                                                             // Примечание: это медленно и, скорее всего, пустая трата ресурсов. Всегда предпочитайте индексированный рендеринг!
    IMGUI_API void ScaleClipRects(const ImVec2& fb_scale);   // Помощник для масштабирования поля ClipRect каждого ImDrawCmd. Используйте, если ваш окончательный выходной буфер имеет другой масштаб,
                                                             // чем ожидает Уважаемый ImGui, или если есть разница между разрешением вашего окна и разрешением буфера кадра.
};

//-----------------------------------------------------------------------------
// [SECTION] Font API (ImFontConfig, ImFontGlyph, ImFontAtlasFlags, ImFontAtlas, ImFontGlyphRangesBuilder, ImFont)
//-----------------------------------------------------------------------------

struct ImFontConfig
{
    void*          FontData;               //          // TTF/OTF data
    int            FontDataSize;           //          // TTF/OTF размер данных
    bool           FontDataOwnedByAtlas;   // true     // TTF/OTF владение данными, полученное контейнером ImFontAtlas (удалит саму память).
    int            FontNo;                 // 0        // Индекс шрифта в файле TTF/OTF
    float          SizePixels;             //          // Размер в пикселях для растеризатора (более или менее соответствует результирующей высоте шрифта).
    int            OversampleH;            // 3        // Растрируйте с более высоким качеством для субпиксельного позиционирования. Обратите внимание, что разница между 2 и 3 минимальна,
                                           // поэтому вы можете уменьшить это значение до 2 для экономии памяти. Читать https://github.com/nothings/stb/blob/master/tests/oversample/README.md для деталей.
    int            OversampleV;            // 1        // Растрируйте с более высоким качеством для субпиксельного позиционирования. Это не очень полезно, так как мы не используем субпиксельные позиции по оси Y.
    bool           PixelSnapH;             // false    // Выровняйте каждый глиф по границе пикселя. Полезный эл. г. если вы объединяете шрифт, не выровненный по пикселям, со шрифтом по умолчанию.
                                           // Если включено, вы можете установить OversampleH/V на 1.
    ImVec2         GlyphExtraSpacing;      // 0, 0     // Дополнительный интервал (в пикселях) между глифами. На данный момент поддерживается только ось X.
    ImVec2         GlyphOffset;            // 0, 0     // Сместите все глифы из этого ввода шрифта.
    const ImWchar* GlyphRanges;            // NULL     // Указатель на предоставленный пользователем список диапазонов Unicode (2 значения на диапазон, значения включаются, список завершается нулем).
                                           // ДАННЫЕ МАССИВА ДОЛЖНЫ СОХРАНЯТЬСЯ, ПОКА ЖИВЕТ ШРИФТ.
    float          GlyphMinAdvanceX;       // 0        // Минимум AdvanceX для глифов, установите Min для выравнивания значков шрифта, установите Min/Max для принудительного моноширинного шрифта.
    float          GlyphMaxAdvanceX;       // FLT_MAX  // Максимальный AdvanceX для глифов
    bool           MergeMode;              // false    // Слияние с предыдущим ImFont, чтобы вы могли объединить несколько входных шрифтов в один ImFont (например, шрифт ASCII + значки + японские глифы).
                                           // Вы можете использовать GlyphOffset. y при слиянии шрифтов разной высоты.
    unsigned int   FontBuilderFlags;       // 0        // Настройки для построителя пользовательских шрифтов. ЭТО ЗАВИСИТ ОТ РЕАЛИЗАЦИИ БИЛДЕРА. Оставьте как ноль, если не уверены.
    float          RasterizerMultiply;     // 1.0f     // Осветлить (>1.0f) или затемнить (<1.0f) вывод шрифта. Осветление мелких шрифтов может быть хорошим обходным путем, чтобы сделать их более читабельными.
    ImWchar        EllipsisChar;           // -1       // Явно укажите кодовую точку Юникода для символа многоточия. При объединении шрифтов будет использоваться первое указанное многоточие.

    // [Internal]
    char           Name[40];   // Имя (строго для облегчения отладки)
    ImFont*        DstFont;

    IMGUI_API      ImFontConfig();
};

// Хранить данные рендеринга для одного глифа.
// (Примечание: некоторые синтаксические анализаторы языка могут не преобразовать элементы битового поля 31+1, в этом случае может быть удалено сохранение одного u32, или мы можем переработать это)
struct ImFontGlyph
{
    unsigned int Colored  :1;      // Флаг, указывающий на то, что глиф окрашен и обычно должен игнорировать тонировку (сделайте его пригодным для использования без сдвига на прямом порядке байтов, поскольку он используется в циклах)
    unsigned int Visible  :1;      // Флаг, указывающий, что глиф не имеет видимых пикселей (например, пробел). Разрешить ранний выход при рендеринге.
    unsigned int Codepoint:30;     // 0x0000..0x10FFFF
    float        AdvanceX;         // Расстояние до следующего символа (= данные из шрифта + ImFontConfig::GlyphExtraSpacing.x встроенный)
    float        X0, Y0, X1, Y1;   // Углы глифов
    float        U0, V0, U1, V1;   // Координаты текстуры
};

// Помощник для построения диапазонов глифов из текстовых/строковых данных. Передайте ему строки/символы вашего приложения, затем вызовите BuildRanges().
// По сути, это плотно упакованный вектор из 64 КБ логических значений = 8 КБ памяти.
struct ImFontGlyphRangesBuilder
{
    ImVector<ImU32> UsedChars;   // Хранить 1 бит на кодовую точку Unicode (0 = не используется, 1 = используется)

    ImFontGlyphRangesBuilder()
    {
        Clear();
    }
    inline void Clear()
    {
        int size_in_bytes = (IM_UNICODE_CODEPOINT_MAX + 1) / 8;
        UsedChars.resize(size_in_bytes / (int)sizeof(ImU32));
        memset(UsedChars.Data, 0, (size_t)size_in_bytes);
    }
    inline bool GetBit(size_t n) const
    {
        int   off  = (int)(n >> 5);
        ImU32 mask = 1u << (n & 31);
        return (UsedChars[off] & mask) != 0;
    }   // Получить бит n в массиве
    inline void SetBit(size_t n)
    {
        int   off  = (int)(n >> 5);
        ImU32 mask = 1u << (n & 31);
        UsedChars[off] |= mask;
    }   // Установить бит n в массиве
    inline void AddChar(ImWchar c)
    {
        SetBit(c);
    }                                                                        // Добавлять character
    IMGUI_API void AddText(const char* text, const char* text_end = NULL);   // Добавить строку (каждый символ строки UTF-8 добавляется)
    IMGUI_API void AddRanges(const ImWchar* ranges);                         // Добавьте диапазоны, например. г. строитель. AddRanges(ImFontAtlas:: GetGlyphRangesDefault()) для принудительного добавления всех ASCII/Latin+Ext
    IMGUI_API void BuildRanges(ImVector<ImWchar>* out_ranges);               // Вывод новых диапазонов
};

// См. функции ImFontAtlas::AddCustomRectXXX.
struct ImFontAtlasCustomRect
{
    unsigned short Width, Height;   // Input    // Желаемый размер прямоугольника
    unsigned short X, Y;            // Output   // Упакованная позиция в Atlas
    unsigned int   GlyphID;         // Input    // Только для глифов пользовательского шрифта (ID < 0x110000)
    float          GlyphAdvanceX;   // Input    // Только для глифов пользовательского шрифта: glyph xadvance
    ImVec2         GlyphOffset;     // Input    // Только для глифов пользовательского шрифта: смещение отображения глифов
    ImFont*        Font;            // Input    // Fили только глифы пользовательского шрифта: целевой шрифт
    ImFontAtlasCustomRect()
    {
        Width = Height = 0;
        X = Y         = 0xFFFF;
        GlyphID       = 0;
        GlyphAdvanceX = 0.0f;
        GlyphOffset   = ImVec2(0, 0);
        Font          = NULL;
    }
    bool IsPacked() const
    {
        return X != 0xFFFF;
    }
};

// Флаги для сборки ImFontAtlas
enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Не округляйте высоту до следующей степени двойки
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1,   // Не встраивайте программные курсоры мыши в атлас (сэкономьте немного памяти текстур)
    ImFontAtlasFlags_NoBakedLines       = 1 << 2    // Не встраивайте в атлас текстуры с толстыми линиями (сэкономьте немного памяти для текстур, разрешите поддержку фильтрации точек/ближайших).
                                                    // Функции AntiAliasedLinesUseTex используют их, иначе они будут отображаться с использованием полигонов (более дорогие для CPU/GPU).
};

// Загрузите и растеризуйте несколько шрифтов TTF/OTF в одну и ту же текстуру. Атлас шрифтов создаст единую текстуру, содержащую:
//  - Один или несколько шрифтов.
//  - Пользовательские графические данные, необходимые для рендеринга фигур, необходимых Dear ImGui.
//  - Формы курсора мыши для программного рендеринга курсора (если не установлено значение «Flags |= ImFontAtlasFlags_NoMouseCursors» в атласе шрифтов).
//  Ответственность за настройку/создание атласа, а затем загрузку данных пикселей в текстуру, доступную вашему графическому API, лежит на пользовательском коде.
//  - При желании вызовите любую из функций AddFont***. Если вы ничего не вызовете, для вас будет загружен шрифт по умолчанию, встроенный в код.
//  - Вызов GetTexDataAsAlpha8() или GetTexDataAsRGBA32() для создания и получения данных о пикселях.
//  - Загрузите данные о пикселях в текстуру в вашей графической системе (см. примеры imgui_impl_xxxx. cpp)
//  - Вызов SetTexID(my_tex_id); и передайте указатель/идентификатор вашей текстуре в формате, естественном для вашего графического API.
//  Это значение будет возвращено вам во время рендеринга для идентификации текстуры. Прочтите запись FAQ об ImTextureID для более подробной информации.
// Распространенные ловушки:
// - Если вы передаете массив glyph_ranges функциям AddFont***, вам нужно убедиться, что ваш массив сохраняется до тех пор, пока
//   атлас построен (при вызове GetTexData*** или Build()). Мы копируем только указатель, а не данные.
// - Важно: по умолчанию AddFontFromMemoryTTF() берет на себя ответственность за данные. Хоть мы и не пишем в него, мы освободим указатель при уничтожении.
//   Вы можете установить font_cfg->FontDataOwnedByAtlas=false, чтобы сохранить право собственности на ваши данные, и они не будут освобождены,
// - Несмотря на то, что многие функции имеют суффикс "TTF", данные OTF также поддерживаются.
// - Это старый API, и в настоящее время он неудобен по тем и другим причинам! Будем обращаться к ним в будущем!
struct ImFontAtlas
{
    IMGUI_API ImFontAtlas();
    IMGUI_API ~ImFontAtlas();
    IMGUI_API ImFont* AddFont(const ImFontConfig* font_cfg);
    IMGUI_API ImFont* AddFontDefault(const ImFontConfig* font_cfg = NULL);
    IMGUI_API ImFont* AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    // Примечание. Передайте право собственности на ttf_data компании ImFontAtlas! Будет удалено после уничтожения атласа.
    // Установите font_cfg->FontDataOwnedByAtlas=false, чтобы сохранить право собственности на ваши данные, и они не будут освобождены.
    IMGUI_API ImFont* AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    // 'compressed_font_data' по-прежнему принадлежит вызывающей стороне. Сжать с помощью binary_to_compressed_c.cpp.
    IMGUI_API ImFont* AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    // 'compressed_font_data_base85' все еще принадлежит вызывающему абоненту. Сжать с помощью binary_to_compressed_c.cpp с параметром -base85.
    IMGUI_API ImFont* AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    IMGUI_API void    ClearInputData();   // Очистить входные данные (все структуры ImFontConfig, включая размеры, данные TTF, диапазоны глифов и т. д.) = все данные, используемые для построения текстуры и шрифтов.
    IMGUI_API void    ClearTexData();     // Очистить выходные данные текстуры (на стороне процессора). Экономит оперативную память после копирования текстуры в графическую память.
    IMGUI_API void    ClearFonts();       // Очистить выходные данные шрифта (хранение глифов, координаты UV).
    IMGUI_API void    Clear();            // Очистите все входные и выходные данные.

    // Создайте атлас, извлеките данные о пикселях.
    // Пользователь отвечает за копирование пикселей в графическую память (например, создание текстуры с помощью вашего движка). Затем сохраните дескриптор текстуры с помощью SetTexID().
    // Шаг всегда = ширина * байты на пиксели (1 или 4)
    // Построение в формате RGBA32 предусмотрено для удобства и совместимости, но учтите, что если вы вручную не манипулируете данными о цвете или не копируете их в
    // текстура (например, при использовании API AddCustomRect***), то испускаемые пиксели RGB всегда будут белыми (~75% потерь памяти/пропускной способности.
    IMGUI_API bool    Build();                                                                                                            // Построить данные о пикселях. Это автоматически вызывается функциями GetTexData***.
    IMGUI_API void    GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);   // 1 byte per-pixel
    IMGUI_API void    GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);   // 4 bytes-per-pixel
    // Немного неоднозначно: используется для определения, когда пользователь не создал текстуру, но фактически мы должны проверить TexID != 0, за исключением того, что это будет зависеть от бэкэнда...
    bool              IsBuilt() const
    {
        return Fonts.Size > 0 && TexReady;
    }
    void SetTexID(ImTextureID id)
    {
        TexID = id;
    }

    //-------------------------------------------
    // Диапазоны глифов
    //-------------------------------------------

    // Помощники для получения списка распространенных диапазонов Unicode (2 значения на диапазон, значения включаются, список завершается нулем)
    // NB: Убедитесь, что ваша строка имеет кодировку UTF-8, а НЕ кодовую страницу вашей локальной системы. В C++11 вы можете создать строковый литерал UTF-8,
    // используя синтаксис u8 "Hello world". Подробности см. в разделе часто задаваемых вопросов.
    // NB: рассмотрите возможность использования ImFontGlyphRangesBuilder для создания диапазонов глифов из текстовых данных.
    IMGUI_API const ImWchar* GetGlyphRangesDefault();                   // Базовая латынь, Расширенная латынь
    IMGUI_API const ImWchar* GetGlyphRangesKorean();                    // По умолчанию + корейские символы
    IMGUI_API const ImWchar* GetGlyphRangesJapanese();                  // По умолчанию + хирагана, катакана, половинная ширина, набор из 2999 иероглифов
    IMGUI_API const ImWchar* GetGlyphRangesChineseFull();               // По умолчанию + половинная ширина + японская хирагана/катакана + полный набор около 21000 унифицированных иероглифов CJK
    IMGUI_API const ImWchar* GetGlyphRangesChineseSimplifiedCommon();   // По умолчанию + половинная ширина + японская хирагана/катакана + набор из 2500 унифицированных иероглифов CJK для общего упрощенного китайского языка
    IMGUI_API const ImWchar* GetGlyphRangesCyrillic();                  // По умолчанию + около 400 символов кириллицы
    IMGUI_API const ImWchar* GetGlyphRangesThai();                      // По умолчанию + тайские символы
    IMGUI_API const ImWchar* GetGlyphRangesVietnamese();                // По умолчанию + вьетнамские символы

    //-------------------------------------------
    // [BETA] API пользовательских прямоугольников/глифов
    //-------------------------------------------

    //   Вы можете запросить укладку в атлас произвольных прямоугольников для своих целей.
    // - После вызова Build() вы можете запросить положение прямоугольника и отобразить ваши пиксели.
    // - Если вы визуализируете цветной вывод, установите 'atlas->TexPixelsUseColors = true', так как это может помочь некоторым бэкэндам выбрать предпочтительный формат текстуры.
    // - Вы также можете запросить отображение ваших прямоугольников в виде глифа шрифта (учитывая шрифт + точку Unicode),
    //   чтобы вы могли визуализировать e. г. пользовательские красочные значки и использовать их как обычные глифы.
    // - Читать документы/ШРИФТЫ. md для получения более подробной информации об использовании цветных значков.
    // - Примечание: этот API может быть переработан позже, чтобы поддерживать различные настройки DPI для нескольких мониторов.
    IMGUI_API int            AddCustomRectRegular(int width, int height);
    IMGUI_API int            AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0, 0));
    ImFontAtlasCustomRect*   GetCustomRectByIndex(int index)
    {
        IM_ASSERT(index >= 0);
        return &CustomRects[index];
    }

    // [Internal]
    IMGUI_API void                  CalcCustomRectUV(const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) const;
    IMGUI_API bool                  GetMouseCursorTexData(ImGuiMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]);

    //-------------------------------------------
    // Члены
    //-------------------------------------------

    ImFontAtlasFlags                Flags;             // Флаги сборки (см. ImFontAtlasFlags_)
    ImTextureID                     TexID;             // Пользовательские данные для ссылки на текстуру после ее загрузки в графические системы пользователя. Он передается вам во время рендеринга через структуру ImDrawCmd.
    int                             TexDesiredWidth;   // Ширина текстуры, желаемая пользователем перед Build(). Должна быть степень двойки.
                                                       // Если у вашего графического API много глифов, есть ограничения на размер текстуры, вы можете увеличить ширину текстуры, чтобы уменьшить высоту.
    int                             TexGlyphPadding;   // Отступы между глифами в текстуре в пикселях. По умолчанию 1. Если ваш метод рендеринга не полагается на билинейную фильтрацию,
                                                       // вы можете установить это значение на 0 (также потребуется установить AntiAliasedLinesUseTex = false).
    bool                            Locked;            // Помечено ImGui:: NewFrame() как заблокированное, поэтому попытка изменить атлас будет подтверждена.

    // [Internal]
    // NB: Доступ к данным текстуры через вызовы GetTexData*()! Который установит для вас шрифт по умолчанию.
    bool                            TexReady;                                          // Установить, когда текстура была построена в соответствии с текущим вводом шрифта
    bool                            TexPixelsUseColors;                                // Сообщите, известно ли, что наши данные текстуры используют цвета (а не только альфа-канал), чтобы помочь серверной части выбрать формат.
    unsigned char*                  TexPixelsAlpha8;                                   // 1 компонент на пиксель, каждый компонент беззнаковый 8-битный. Общий размер = TexWidth * TexHeight
    unsigned int*                   TexPixelsRGBA32;                                   // 4 компонента на пиксель, каждый компонент беззнаковый 8-битный. Общий размер = TexWidth * TexHeight * 4
    int                             TexWidth;                                          // Ширина текстуры рассчитывается во время Build().
    int                             TexHeight;                                         // Высота текстуры рассчитывается во время Build().
    ImVec2                          TexUvScale;                                        // = (1.0f/TexWidth, 1.0f/TexHeight)
    ImVec2                          TexUvWhitePixel;                                   // Координаты текстуры в белый пиксель
    ImVector<ImFont*>               Fonts;                                             // Сохраните все шрифты, возвращенные AddFont*. Fonts[0] является шрифтом по умолчанию при вызове ImGui:: NewFrame(),
                                                                                       // используйте ImGui:: PushFont()/PopFont() для изменения текущего шрифта.
    ImVector<ImFontAtlasCustomRect> CustomRects;                                       // Прямоугольники для упаковки данных пользовательских текстур в атлас.
    ImVector<ImFontConfig>          ConfigData;                                        // Данные конфигурации
    ImVec4                          TexUvLines[IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];   // UV для запеченных сглаженных линий

    // [Internal] Конструктор шрифтов
    const ImFontBuilderIO*          FontBuilderIO;      // Непрозрачный интерфейс для построителя шрифтов (по умолчанию stb_truetype, можно изменить на использование FreeType, определив IMGUI_ENABLE_FREETYPE).
    unsigned int                    FontBuilderFlags;   // Общие флаги (для всех шрифтов) для построителя пользовательских шрифтов. ЭТО СТРОЙКА ЗАВИСИТ ОТ РЕАЛИЗАЦИИ. Переопределение для каждого шрифта также доступно в ImFontConfig.

    // [Internal] Данные упаковки
    int                             PackIdMouseCursors;   // Пользовательский идентификатор прямоугольника текстуры для белых пикселей и курсоров мыши
    int                             PackIdLines;          // Пользовательский идентификатор прямоугольника текстуры для запеченных сглаженных линий

    // [Устаревший]
    //typedef ImFontAtlasCustomRect    CustomRect;         // УСТАРЕЛО в 1.72+
    //typedef ImFontGlyphRangesBuilder GlyphRangesBuilder; // УСТАРЕЛО в 1.67+
};

// Данные времени выполнения шрифта и рендеринг
// ImFontAtlas автоматически загружает встроенный шрифт по умолчанию, когда вы вызываете GetTexDataAsAlpha8() или GetTexDataAsRGBA32().
struct ImFont
{
    // Члены: Горячие ~20/24 байта (для CalcTextSize)
    ImVector<float>       IndexAdvanceX;      // 12-16 // out //  // Редкий. Glyphs->AdvanceX напрямую индексируемым способом
                                              // (удобен для кэширования для функций CalcTextSize, которые только обрабатывают эту информацию
                                              // и часто являются узким местом в большом пользовательском интерфейсе).
    float                 FallbackAdvanceX;   // 4     // out // = FallbackGlyph->AdvanceX
    float                 FontSize;           // 4     // in  //            // Высота символов/строк, задается при загрузке (не меняется после загрузки)

    // Участники: Hot ~28/40 байт (для CalcTextSize + цикл рендеринга)
    ImVector<ImWchar>     IndexLookup;     // 12-16 // out //            // Редкий. Глифы индекса по кодовой точке Unicode.
    ImVector<ImFontGlyph> Glyphs;          // 12-16 // out //            // Все глифы.
    const ImFontGlyph*    FallbackGlyph;   // 4-8   // out // = FindGlyph(FontFallbackChar)

    // Члены: Холодные ~32/40 байт
    ImFontAtlas*          ContainerAtlas;                                              // 4-8   // out //            // Во что мы были загружены
    const ImFontConfig*   ConfigData;                                                  // 4-8   // in  //            // Указатель в ContainerAtlas->ConfigData
    short                 ConfigDataCount;                                             // 2     // in  // ~ 1        // Количество ImFontConfig, участвующих в создании этого шрифта. Больше 1 при объединении нескольких источников шрифтов в один ImFont.
    ImWchar               FallbackChar;                                                // 2     // out // = FFFD/'?' // Символ, используемый, если глиф не найден.
    ImWchar               EllipsisChar;                                                // 2     // out // = '...'    // Символ, используемый для отображения многоточия.
    ImWchar               DotChar;                                                     // 2     // out // = '.'      // Символ, используемый для рендеринга многоточия (если не найден ни один символ '...')
    bool                  DirtyLookupTables;                                           // 1     // out //
    float                 Scale;                                                       // 4     // in  // = 1.f      // Базовый масштаб шрифта, умноженный на масштаб шрифта для каждого окна, который можно настроить с помощью SetWindowFontScale().
    float                 Ascent, Descent;                                             // 4+4   // out //            // Восхождение: расстояние от вершины до низа e. г. 'A' [0.. Размер шрифта]
    int                   MetricsTotalSurface;                                         // 4     // out //            // Общая поверхность в пикселях, чтобы получить представление о стоимости растеризации/текстуры шрифта
                                                                                       // (не точно, мы приблизительно оцениваем стоимость отступов между глифами)
    ImU8                  Used4kPagesMap[(IM_UNICODE_CODEPOINT_MAX + 1) / 4096 / 8];   // 2 байта, если ImWchar=ImWchar16, 34 байта, если ImWchar==ImWchar32. Сохраняйте 1 бит для каждого блока
                                                                                       // кодовых точек 4K, который имеет один активный глиф. Это в основном используется для облегчения итераций
                                                                                       // по всем используемым кодовым точкам.

    // Методы
    IMGUI_API             ImFont();
    IMGUI_API ~ImFont();
    IMGUI_API const ImFontGlyph* FindGlyph(ImWchar c) const;
    IMGUI_API const ImFontGlyph* FindGlyphNoFallback(ImWchar c) const;
    float                        GetCharAdvance(ImWchar c) const
    {
        return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX;
    }
    bool IsLoaded() const
    {
        return ContainerAtlas != NULL;
    }
    const char* GetDebugName() const
    {
        return ConfigData ? ConfigData->Name : "<unknown>";
    }

    // 'max_width' прекращает рендеринг после определенной ширины (можно превратить в 2d-размер). FLT_MAX для отключения.
    // 'wrap_width' включить автоматический перенос слов между несколькими строками, чтобы они соответствовали заданной ширине. 0.0f для отключения.
    IMGUI_API ImVec2      CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const;   // utf8
    IMGUI_API const char* CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    IMGUI_API void        RenderChar(ImDrawList* draw_list, float size, const ImVec2& pos, ImU32 col, ImWchar c) const;
    IMGUI_API void        RenderText(ImDrawList* draw_list, float size, const ImVec2& pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    // [Internal] Не используйте!
    IMGUI_API void        BuildLookupTable();
    IMGUI_API void        ClearOutputData();
    IMGUI_API void        GrowIndex(int new_size);
    IMGUI_API void        AddGlyph(const ImFontConfig* src_cfg, ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    IMGUI_API void        AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true);   // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
    IMGUI_API void        SetGlyphVisible(ImWchar c, bool visible);
    IMGUI_API bool        IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

//-----------------------------------------------------------------------------
// [SECTION] Viewports
//-----------------------------------------------------------------------------

// Flags stored in ImGuiViewport::Flags, giving indications to the platform backends.
enum ImGuiViewportFlags_
{
    ImGuiViewportFlags_None                = 0,
    ImGuiViewportFlags_IsPlatformWindow    = 1 << 0,    // Представлять окно платформы
    ImGuiViewportFlags_IsPlatformMonitor   = 1 << 1,    // Представлять монитор платформы (еще не используется)
    ImGuiViewportFlags_OwnedByApp          = 1 << 2,    // Окно платформы: создается/управляется приложением (а не дорогим бэкэндом imgui)
    ImGuiViewportFlags_NoDecoration        = 1 << 3,    // Окно платформы: отключить украшения платформы: панель заголовка, границы и т. д. (обычно устанавливается для всех окон,
                                                        // но если установлено значение ImGuiConfigFlags_ViewportsDecoration, мы устанавливаем это только для всплывающих окон/подсказок)
    ImGuiViewportFlags_NoTaskBarIcon       = 1 << 4,    // Окно платформы: отключить значок панели задач платформы (обычно устанавливается во всплывающих окнах/подсказках
                                                        // или во всех окнах, если установлено значение ImGuiConfigFlags_ViewportsNoTaskBarIcon)
    ImGuiViewportFlags_NoFocusOnAppearing  = 1 << 5,    // Окно платформы: не принимать фокус при создании.
    ImGuiViewportFlags_NoFocusOnClick      = 1 << 6,    // Окно платформы: не принимать фокус при нажатии.
    ImGuiViewportFlags_NoInputs            = 1 << 7,    // Окно платформы: проведите мышь, чтобы мы могли перетаскивать это окно, заглядывая за него.
    ImGuiViewportFlags_NoRendererClear     = 1 << 8,    // Окно платформы: рендереру не нужно заранее очищать буфер кадра (потому что мы заполним его полностью).
    ImGuiViewportFlags_TopMost             = 1 << 9,    // Окно платформы: отображается сверху (только для всплывающих подсказок).
    ImGuiViewportFlags_Minimized           = 1 << 10,   // Окно платформы: Окно свернуто, можно пропустить рендеринг. В свернутом виде мы стараемся не использовать положение/размер
                                                        // области просмотра для обрезки окна или проверки того, содержатся ли они в области просмотра.
    ImGuiViewportFlags_NoAutoMerge         = 1 << 11,   // Окно платформы: избегайте слияния этого окна с другим окном хоста. Это может быть установлено только с помощью переопределения флагов
                                                        // области просмотра ImGuiWindowClass (потому что нам нужно сделать это заранее, если мы собираемся создать область просмотра в первую очередь!).
    ImGuiViewportFlags_CanHostOtherWindows = 1 << 12    // Основное окно просмотра: может содержать несколько окон imgui (дополнительные окна просмотра связаны с одним окном).
};

// - В настоящее время представляет собой окно платформы, созданное приложением, в котором размещаются наши дорогие окна ImGui.
// - С включенным несколькими окнами просмотра мы расширяем эту концепцию, чтобы иметь несколько активных окон просмотра.
// - В будущем мы расширим эту концепцию, чтобы также представлять монитор платформы и поддерживать режим работы «без основного окна платформы».
// - О основной области и рабочей области:
// - Основная область = вся область просмотра.
// - Рабочая область = вся область просмотра за вычетом разделов, используемых панелями главного меню (для окон платформы) или панелью задач (для монитора платформы).
// - Окна обычно стараются оставаться в пределах рабочей области окна просмотра хоста.
struct ImGuiViewport
{
    ImGuiID            ID;                 // Уникальный идентификатор области просмотра
    ImGuiViewportFlags Flags;              // См. ImGuiViewportFlags_
    ImVec2             Pos;                // Основная область: положение области просмотра (координаты Дорогой ImGui такие же, как координаты рабочего стола ОС/собственные координаты)
    ImVec2             Size;               // Основная область: размер окна просмотра.
    ImVec2             WorkPos;            // Рабочая область: положение области просмотра за вычетом панелей задач, строк меню, строк состояния (>= Pos)
    ImVec2             WorkSize;           // Рабочая область: размер области просмотра минус панель задач, строка меню, строки состояния (<= Размер)
    float              DpiScale;           // 1.0f = 96 DPI = Без лишних масштабов.
    ImGuiID            ParentViewportId;   // (Дополнительно) 0: нет родителя. Дайте серверной части платформы указание настроить отношение родитель/потомок между окнами платформы.
    ImDrawData*        DrawData;           // ImDrawData, соответствующий этому видовому экрану. Действителен после Render() и до следующего вызова NewFrame().

    // Данные, зависящие от платформы/серверной части
    // Наш дизайн разделяет бэкэнды рендерера и платформы, чтобы облегчить объединение бэкэндов по умолчанию друг с другом.
    // Когда мы создаем свой собственный бэкенд для пользовательского движка, возможно, что будут обрабатываться и рендерер, и платформа
    // одной и той же системой, и вам может не понадобиться использовать все поля UserData/Handle.
    // Библиотека никогда не использует эти поля, они просто служат для хранения, чтобы упростить реализацию серверной части.
    void*              RendererUserData;        // void* для хранения пользовательской структуры данных для средства визуализации (например, цепочка обмена, буферы кадров и т. д.).
                                                // обычно устанавливается вашей функцией Renderer_CreateWindow.
    void*              PlatformUserData;        // void* для хранения пользовательской структуры данных для ОС/платформы (например, информация об окнах, контекст рендеринга).
                                                // обычно устанавливается вашей функцией Platform_CreateWindow.
    void*              PlatformHandle;          // void* для FindViewportByPlatformHandle(). (например, предлагается использовать естественный дескриптор платформы, такой как HWND, GLFWWindow*, SDL_Window*)
    void*              PlatformHandleRaw;       // void* для хранения низкоуровневого дескриптора окна, встроенного в платформу (под Win32 ожидается, что это будет HWND, не используемый для других платформ),
                                                // при использовании уровня абстракции, такого как GLFW или SDL (где PlatformHandle будет SDL_Window*)
    bool               PlatformRequestMove;     // Окно платформы запросило перемещение (например, окно было перемещено оконным менеджером ОС/хоста, авторитарной позицией будет позиция окна ОС)
    bool               PlatformRequestResize;   // Запрошено изменение размера окна платформы (например, размер окна был изменен оконным менеджером ОС/хоста, официальным размером будет размер окна ОС)
    bool               PlatformRequestClose;    // Окно платформы запросило закрытие (например, окно было перемещено оконным менеджером ОС/узла, например, нажатием ALT-F4)

    ImGuiViewport()
    {
        memset(this, 0, sizeof(*this));
    }
    ~ImGuiViewport()
    {
        IM_ASSERT(PlatformUserData == NULL && RendererUserData == NULL);
    }

    // Помощники
    ImVec2 GetCenter() const
    {
        return ImVec2(Pos.x + Size.x * 0.5f, Pos.y + Size.y * 0.5f);
    }
    ImVec2 GetWorkCenter() const
    {
        return ImVec2(WorkPos.x + WorkSize.x * 0.5f, WorkPos.y + WorkSize.y * 0.5f);
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Интерфейсы, зависящие от платформы (например, для поддержки нескольких окон просмотра)
//-----------------------------------------------------------------------------
// [BETA] (Optional) Это совершенно необязательно, для продвинутых пользователей!
// Если вы новичок в Уважаемом ImGui и пытаетесь интегрировать его в свой движок, вы, вероятно, можете пока проигнорировать это.
//
// Эта функция позволяет легко перетаскивать окна Dear ImGui за пределы области просмотра вашего приложения.
// Это достигается за счет создания новых окон платформы/ОС на лету и рендеринга в них.
// Уважаемый ImGui управляет структурами области просмотра, а серверная часть создает и поддерживает одно окно платформы/ОС для каждой из этих областей просмотра.
//
// См. глоссарий https://github.com/ocornut/imgui/wiki/Glossary для получения подробной информации о некоторых терминах.
// См. Тему https://github.com/ocornut/imgui/issues/1542 для GIF, новостей и вопросов об этой развивающейся функции.
//
// О системе координат:
// - Когда мультипросмотр включен, все координаты Dear ImGui становятся абсолютными координатами (такими же, как координаты ОС!)
// - Так т.е. г. ImGui:: SetNextWindowPos(ImVec2(0,0)) позиционирует окно относительно вашего основного монитора!
// - Если вы хотите расположить окна относительно вашего основного окна просмотра приложения, используйте ImGui:: GetMainViewport()->Pos в качестве базовой позиции.
//
// Шаги по использованию нескольких видовых экранов в вашем приложении при использовании бэкенда по умолчанию из папки examples/:
// - Application:  Включите функцию с помощью 'io. ConfigFlags |= ImGuiConfigFlags_ViewportsEnable'.
// - Backend:      Инициализация бэкенда установит все необходимые функции ImGuiPlatformIO и обновит информацию о мониторах в каждом кадре.
// - Application:  В основном цикле вызовите ImGui::UpdatePlatformWindows(), ImGui::RenderPlatformWindowsDefault() после EndFrame() или Render().
// - Application:  Исправлены абсолютные координаты, используемые в вызовах ImGui::SetWindowPos() или ImGui::SetNextWindowPos().
//
// Шаги по использованию нескольких видовых экранов в вашем приложении при использовании пользовательского бэкенда:
// - Important:    ЭТО НЕ ЛЕГКО СДЕЛАТЬ и имеет много тонкостей, которые здесь не описаны!
//                 Это также экспериментальная функция, поэтому некоторые требования могут измениться.
//                 Рассмотрите возможность использования бэкэндов по умолчанию, если можете. В любом случае внимательно следите за деталями и обращайтесь к примерам/бэкендам.
// - Application:  Включите функцию с помощью 'io. ConfigFlags |= ImGuiConfigFlags_ViewportsEnable'.
// - Backend:      Перехватите обратные вызовы ImGuiPlatformIO Platform_* и Renderer_* (см. ниже).
//                 Установить 'io. BackendFlags |= ImGuiBackendFlags_PlatformHasViewports» и «io. BackendFlags |= ImGuiBackendFlags_PlatformHasViewports'.
//                 Обновлять список мониторов ImGuiPlatformIO каждый кадр.
//                 Обновлять MousePos каждый кадр в абсолютных координатах.
// - Application:  В основном цикле вызовите ImGui::UpdatePlatformWindows(), ImGui::RenderPlatformWindowsDefault() после EndFrame() или Render().
//                 Вы можете пропустить вызов RenderPlatformWindowsDefault(), если его API не подходит для ваших нужд. Читайте комментарии ниже.
// - Application:  Исправлены абсолютные координаты, используемые в вызовах ImGui::SetWindowPos() или ImGui::SetNextWindowPos().
//
// Об ImGui:: RenderPlatformWindowsDefault():
// - Эта функция в основном является _helper_ для наиболее распространенных случаев и для облегчения использования бэкэндов по умолчанию.
// - Вы можете проверить его простой исходный код, чтобы понять, что он делает.
// Он в основном перебирает вторичные области просмотра и вызывает 4 функции, настроенные в ImGuiPlatformIO, если они доступны:
// Platform_RenderWindow(), Renderer_RenderWindow(), Platform_SwapBuffers(), Renderer_SwapBuffers()
// Эти указатели на функции существуют только для пользы RenderPlatformWindowsDefault().
// - Если у вас очень специфические потребности в рендеринге (например, одновременное переключение нескольких цепочек обмена, необычные проблемы с синхронизацией/поточностью и т. д.),
// у вас может возникнуть соблазн игнорировать RenderPlatformWindowsDefault() и написать собственный код для выполнения рендерингаg.
// Вы можете установить указатели *RenderWindow и *SwapBuffers платформы_io и вызывать свои функции через эти указатели,
// или вы можете решить никогда не устанавливать эти указатели и напрямую вызывать свой код. Это удобство, а не обязательный интерфейс.
//-----------------------------------------------------------------------------

// (Optional) Доступ через ImGui::GetPlatformIO()
struct ImGuiPlatformIO
{
    //------------------------------------------------------------------
    // Input - Серверный интерфейс/функции + список мониторов
    //------------------------------------------------------------------

    // (Optional) Функции платформы (например, Win32, GLFW, SDL2)
    // Для справки, во втором столбце показано, какая функция обычно вызывает функции платформы:
    //   N = ImGui::NewFrame()                        ~ начало кадра dear imgui: чтение информации из окон платформы/ОС (последний размер/положение)
    //   F = ImGui::Begin(), ImGui::EndFrame()        ~ во время кадра dear imgui
    //   U = ImGui::UpdatePlatformWindows()           ~ после кадра dear imgui: создайте и обновите все окна платформы/ОС
    //   R = ImGui::RenderPlatformWindowsDefault()    ~ render
    //   D = ImGui::DestroyPlatformWindows()          ~ shutdown
    // Общая идея заключается в том, что NewFrame() мы будем считывать текущее состояние платформы/ОС, а UpdatePlatformWindows() будет писать в него.
    //
    // TФункции спроектированы таким образом, что мы можем смешивать и сопоставлять 2 файла imgui_impl_xxxx, один для платформы (обработка ~окна/ввода), один для рендерера.
    // Пользовательские серверные части движка часто предоставляют интерфейсы как для платформы, так и для рендерера, и поэтому могут не нуждаться в использовании всех функций.
    // Платформенные функции обычно вызываются перед их аналогом Renderer, за исключением Destroy, которые вызываются другим способом.

    // Функция платформы --------------------------------------------------- Called by -----
    void                           (*Platform_CreateWindow)(ImGuiViewport* vp);                                                                       // . . U . .  // Создать новое окно платформы для данного окна просмотра
    void                           (*Platform_DestroyWindow)(ImGuiViewport* vp);                                                                      // N . U . D  //
    void                           (*Platform_ShowWindow)(ImGuiViewport* vp);                                                                         // . . U . .  // Вновь созданные окна изначально скрыты, поэтому для них можно вызвать SetWindowPos/Size/Title перед отображением окна.
    void                           (*Platform_SetWindowPos)(ImGuiViewport* vp, ImVec2 pos);                                                           // . . U . .  // Установите положение окна платформы (учитывая левый верхний угол клиентской области)
    ImVec2                         (*Platform_GetWindowPos)(ImGuiViewport* vp);                                                                       // N . . . .  //
    void                           (*Platform_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);                                                         // . . U . .  // Установите размер клиентской области окна платформы (игнорируя украшения ОС, такие как строка заголовка ОС и т. д.)
    ImVec2                         (*Platform_GetWindowSize)(ImGuiViewport* vp);                                                                      // N . . . .  // Получить размер клиентской области окна платформы
    void                           (*Platform_SetWindowFocus)(ImGuiViewport* vp);                                                                     // N . . . .  // Переместить окно вперед и установить фокус ввода
    bool                           (*Platform_GetWindowFocus)(ImGuiViewport* vp);                                                                     // . . U . .  //
    bool                           (*Platform_GetWindowMinimized)(ImGuiViewport* vp);                                                                 // N . . . .  // Получить свернутое состояние окна платформы. При сворачивании мы, как правило, не пытаемся получить/установить размер, и содержимое будет легче отбраковываться.
    void                           (*Platform_SetWindowTitle)(ImGuiViewport* vp, const char* str);                                                    // . . U . .  // Установить заголовок окна платформы (с учетом строки UTF-8)
    void                           (*Platform_SetWindowAlpha)(ImGuiViewport* vp, float alpha);                                                        // . . U . .  // (Optional) Настройка глобальной прозрачности (не попиксельной прозрачности)
    void                           (*Platform_UpdateWindow)(ImGuiViewport* vp);                                                                       // . . U . .  // (Optional) Вызывается функцией UpdatePlatformWindows(). Дополнительный хук, позволяющий серверной части платформы вести общую бухгалтерию в каждом кадре.
    void                           (*Platform_RenderWindow)(ImGuiViewport* vp, void* render_arg);                                                     // . . . R .  // (Optional) Основной рендеринг (сторона платформы! Часто не используется или просто устанавливает «текущий» контекст для привязок OpenGL). 'render_arg' — это значение, переданное RenderPlatformWindowsDefault().
    void                           (*Platform_SwapBuffers)(ImGuiViewport* vp, void* render_arg);                                                      // . . . R .  // (Optional) Вызов Present/SwapBuffers (со стороны платформы! Это часто не используется!). 'render_arg' — это значение, переданное RenderPlatformWindowsDefault().
    float                          (*Platform_GetWindowDpiScale)(ImGuiViewport* vp);                                                                  // N . . . .  // (Optional) [BETA] FIXME-DPI: обработка DPI: возвращает масштаб DPI для этого окна просмотра. 1,0f = 96 точек на дюйм.
    void                           (*Platform_OnChangedViewport)(ImGuiViewport* vp);                                                                  // . F . . .  // (Optional) [BETA] FIXME-DPI: обработка DPI: вызывается во время Begin() каждый раз, когда область просмотра, которую мы выводим в, изменяется, поэтому у серверной части есть возможность поменять местами шрифты для настройки стиля.
    int                            (*Platform_CreateVkSurface)(ImGuiViewport* vp, ImU64 vk_inst, const void* vk_allocators, ImU64* out_vk_surface);   // (Optional) Для Vulkan Renderer для вызова кода платформы (поскольку создание поверхности должно связать их обоих).

    // (Optional) Функции рендеринга (например, DirectX, OpenGL, Vulkan)
    void                           (*Renderer_CreateWindow)(ImGuiViewport* vp);                     // . . U . .  // Создать цепочку подкачки, буферы кадров и т. д. (вызывается после Platform_CreateWindow)
    void                           (*Renderer_DestroyWindow)(ImGuiViewport* vp);                    // N . U . D  // Уничтожить цепочку подкачки, буферы кадров и т. д. (вызывается перед Platform_DestroyWindow)
    void                           (*Renderer_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);       // . . U . .  // Изменение размера цепочки подкачки, буферов кадров и т. д. (вызывается после Platform_SetWindowSize)
    void                           (*Renderer_RenderWindow)(ImGuiViewport* vp, void* render_arg);   // . . . R .  // (Optional) Очистите фреймбуфер, настройте цель рендеринга, затем выполните рендеринг окна просмотра->DrawData. 'render_arg' — это значение, переданное RenderPlatformWindowsDefault().
    void                           (*Renderer_SwapBuffers)(ImGuiViewport* vp, void* render_arg);    // . . . R .  // (Optional) Вызов Present/SwapBuffers. 'render_arg' — это значение, переданное RenderPlatformWindowsDefault().

    // (Optional) Список мониторов
    // - Обновлено: app/backend. Обновляйте каждый кадр, чтобы динамически поддерживать изменение конфигурации монитора или DPI.
    // - Используется: дорогой imgui для запроса информации о DPI, зажима всплывающих окон / всплывающих подсказок на одном мониторе, а не для того, чтобы они располагались между мониторами.
    ImVector<ImGuiPlatformMonitor> Monitors;

    //------------------------------------------------------------------
    // Output - Список видовых экранов для рендеринга на платформе windows
    //------------------------------------------------------------------

    // Список видовых экранов (список обновляется вызовом ImGui::EndFrame или ImGui::Render)
    // (в будущем мы попытаемся организовать эту функцию, чтобы убрать необходимость в «основном окне просмотра»)
    ImVector<ImGuiViewport*>       Viewports;   // Основные видовые экраны, за которыми следуют все второстепенные видовые экраны.
    ImGuiPlatformIO()
    {
        memset(this, 0, sizeof(*this));
    }   // Нулевой сброс
};

// (Optional) Это необходимо при включении мультипросмотра. Представляют границы каждого подключенного монитора/дисплея и их DPI.
// Мы используем эту информацию для поддержки нескольких DPI + фиксируем положение всплывающих окон и всплывающих подсказок, чтобы они не перекрывали несколько мониторов.
struct ImGuiPlatformMonitor
{
    ImVec2 MainPos, MainSize;   // Coordinates of the area displayed on this monitor (Min = upper left, Max = bottom right)
    ImVec2 WorkPos, WorkSize;   // Координаты без панелей задач/боковых панелей/панелей меню. Используется, чтобы избежать размещения всплывающих окон/подсказок внутри этой области.
                                // Если у вас нет этой информации, скопируйте значение для MainPos/MainSize.
    float  DpiScale;            // 1.0f = 96 DPI
    ImGuiPlatformMonitor()
    {
        MainPos = MainSize = WorkPos = WorkSize = ImVec2(0, 0);
        DpiScale                                = 1.0f;
    }
};

// (Optional) Поддержка IME (редактор метода ввода) через интерфейс io. Функция SetPlatformImeDataFn().
struct ImGuiPlatformImeData
{
    bool   WantVisible;       // Виджет хочет, чтобы IME был виден
    ImVec2 InputPos;          // Позиция курсора ввода
    float  InputLineHeight;   // Высота линии

    ImGuiPlatformImeData()
    {
        memset(this, 0, sizeof(*this));
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Устаревшие функции и типы
// (Будет удалено! Подробнее читайте в разделе «API BREAKING CHANGES» в imgui. cpp)
// Пожалуйста, обновляйте свою копию дорогой imgui! Время от времени устанавливайте '#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS' в imconfig. ч, чтобы оставаться впереди.
//-----------------------------------------------------------------------------

namespace ImGui
{
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    IMGUI_API int GetKeyIndex(ImGuiKey key);   // сопоставить значения ImGuiKey_* с устаревшим собственным индексом ключа. == io.KeyMap[key]
#else
    static inline int GetKeyIndex(ImGuiKey key)
    {
        IM_ASSERT(key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END && "ImGuiKey and native_index was merged together and native_index is disabled by IMGUI_DISABLE_OBSOLETE_KEYIO. Please switch to ImGuiKey.");
        return key;
    }
#endif
}   // namespace ImGui

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
namespace ImGui
{
    // УСТАРЕЛО в версии 1.88 (с мая 2022 г.)
    static inline void CaptureKeyboardFromApp(bool want_capture_keyboard = true)
    {
        SetNextFrameWantCaptureKeyboard(want_capture_keyboard);
    }   // Переименовано, поскольку имя вводило в заблуждение + удалено значение по умолчанию.
    static inline void CaptureMouseFromApp(bool want_capture_mouse = true)
    {
        SetNextFrameWantCaptureMouse(want_capture_mouse);
    }   // Переименовано, поскольку имя вводило в заблуждение + удалено значение по умолчанию.
    // УСТАРЕЛО в версии 1.86 (с ноября 2021 г.)
    IMGUI_API void      CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);   // Вычислите грубое отсечение для большого списка элементов одинакового размера. Лучше использовать ImGuiListClipper.
    // УСТАРЕЛО в версии 1.85 (с августа 2021 г.)
    static inline float GetWindowContentRegionWidth()
    {
        return GetWindowContentRegionMax().x - GetWindowContentRegionMin().x;
    }
    // УСТАРЕЛО в версии 1.81 (с февраля 2021 г.)
    IMGUI_API bool     ListBoxHeader(const char* label, int items_count, int height_in_items = -1);   // Помощник для расчета размера из items_count и height_in_items
    static inline bool ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0, 0))
    {
        return BeginListBox(label, size);
    }
    static inline void ListBoxFooter()
    {
        EndListBox();
    }
    // УСТАРЕЛО в версии 1.79 (с августа 2020 г.)
    static inline void OpenPopupContextItem(const char* str_id = NULL, ImGuiMouseButton mb = 1)
    {
        OpenPopupOnItemClick(str_id, mb);
    }   // Возвращаемое значение Bool удалено. Используйте IsWindowAppearing() вместо этого BeginPopup(). Переименован в 1.77, переименован обратно в 1.79. Извини!
    // УСТАРЕЛО в версии 1.78 (с июня 2020 г.)
    // Старые функции перетаскивания/ползунков, которые вместо флагов принимали аргумент 'float power = 1.0'.
    // Для общего кода вы можете проверить версию во время компиляции с помощью `#if IMGUI_VERSION_NUM >= 17704`.
    IMGUI_API bool     DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, float power);
    IMGUI_API bool     DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, float power);
    static inline bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
    {
        return DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, power);
    }
    static inline bool DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        return DragScalarN(label, ImGuiDataType_Float, v, 2, v_speed, &v_min, &v_max, format, power);
    }
    static inline bool DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        return DragScalarN(label, ImGuiDataType_Float, v, 3, v_speed, &v_min, &v_max, format, power);
    }
    static inline bool DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        return DragScalarN(label, ImGuiDataType_Float, v, 4, v_speed, &v_min, &v_max, format, power);
    }
    IMGUI_API bool     SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power);
    IMGUI_API bool     SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format, float power);
    static inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power)
    {
        return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, power);
    }
    static inline bool SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, float power)
    {
        return SliderScalarN(label, ImGuiDataType_Float, v, 2, &v_min, &v_max, format, power);
    }
    static inline bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power)
    {
        return SliderScalarN(label, ImGuiDataType_Float, v, 3, &v_min, &v_max, format, power);
    }
    static inline bool SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, float power)
    {
        return SliderScalarN(label, ImGuiDataType_Float, v, 4, &v_min, &v_max, format, power);
    }
    // OBSOLETED in 1.77 (from June 2020)
    static inline bool BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items)
    {
        return BeginPopupContextWindow(str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
    }

    // Некоторые из старых устаревших имен вместе с их заменой (закомментированы, поэтому они не отображаются в IDE)
    //static inline void  TreeAdvanceToLabelPos()               { SetCursorPosX(GetCursorPosX() + GetTreeNodeToLabelSpacing()); }   // УСТАРЕЛО в версии 1.72 (с июля 2019 г.)
    //static inline void  SetNextTreeNodeOpen(bool open, ImGuiCond cond = 0) { SetNextItemOpen(open, cond); }                       // УСТАРЕЛО в версии 1.71 (с июня 2019 г.)
    //static inline float GetContentRegionAvailWidth()          { return GetContentRegionAvail().x; }                               // УСТАРЕЛО в версии 1.70 (с мая 2019 г.)
    //static inline ImDrawList* GetOverlayDrawList()            { return GetForegroundDrawList(); }                                 // УСТАРЕЛО в версии 1.69 (с марта 2019 г.)
    //static inline void  SetScrollHere(float ratio = 0.5f)     { SetScrollHereY(ratio); }                                          // УСТАРЕЛО в версии 1.66 (с ноября 2018 г.)
    //static inline bool  IsItemDeactivatedAfterChange()        { return IsItemDeactivatedAfterEdit(); }                            // УСТАРЕЛО в версии 1.63 (с августа 2018 г.)
    //static inline bool  IsAnyWindowFocused()                  { return IsWindowFocused(ImGuiFocusedFlags_AnyWindow); }            // УСТАРЕЛО в версии 1.60 (с апреля 2018 г.)
    //static inline bool  IsAnyWindowHovered()                  { return IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }            // УСТАРЕЛО в версии 1.60 (с декабря 2017 г. по апрель 2018 г.)
    //static inline void  ShowTestWindow()                      { return ShowDemoWindow(); }                                        // УСТАРЕЛО в версии 1.53 (с октября 2017 г. по декабрь 2017 г.)
    //static inline bool  IsRootWindowFocused()                 { return IsWindowFocused(ImGuiFocusedFlags_RootWindow); }           // УСТАРЕЛО в версии 1.53 (с октября 2017 г. по декабрь 2017 г.)
    //static inline bool  IsRootWindowOrAnyChildFocused()       { return IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows); }  // УСТАРЕЛО в версии 1.53 (с октября 2017 г. по декабрь 2017 г.)
    //static inline void  SetNextWindowContentWidth(float w)    { SetNextWindowContentSize(ImVec2(w, 0.0f)); }                      // УСТАРЕЛО в версии 1.53 (с октября 2017 г. по декабрь 2017 г.)
    //static inline float GetItemsLineHeightWithSpacing()       { return GetFrameHeightWithSpacing(); }                             // УСТАРЕЛО в версии 1.53 (с октября 2017 г. по декабрь 2017 г.)
}   // namespace ImGui

// УСТАРЕЛО в версии 1.82 (с Mars 2021): флаги для AddRect(), AddRectFilled(), AddImageRounded(), PathRect()
typedef ImDrawFlags ImDrawCornerFlags;
enum ImDrawCornerFlags_
{
    ImDrawCornerFlags_None     = ImDrawFlags_RoundCornersNone,          // Было == 0 до 1.82, теперь это == ImDrawFlags_RoundCornersNone, что равно != 0 и не является неявным
    ImDrawCornerFlags_TopLeft  = ImDrawFlags_RoundCornersTopLeft,       // Было == 0x01 (1 << 0) до 1.82. Порядок соответствует флагу ImDrawFlags_NoRoundCorner* (мы используем это внутри).
    ImDrawCornerFlags_TopRight = ImDrawFlags_RoundCornersTopRight,      // Было == 0x02 (1 << 1) до 1.82.
    ImDrawCornerFlags_BotLeft  = ImDrawFlags_RoundCornersBottomLeft,    // Было == 0x04 (1 << 2) до 1.82.
    ImDrawCornerFlags_BotRight = ImDrawFlags_RoundCornersBottomRight,   // Было == 0x08 (1 << 3) до 1.82.
    ImDrawCornerFlags_All      = ImDrawFlags_RoundCornersAll,           // Было == 0x0F до 1.82
    ImDrawCornerFlags_Top      = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,
    ImDrawCornerFlags_Bot      = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,
    ImDrawCornerFlags_Left     = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,
    ImDrawCornerFlags_Right    = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight
};

// ПЕРЕИМЕНОВАНО ImGuiKeyModFlags -> ImGuiModFlags в версии 1.88 (с апреля 2022 г.)
typedef int ImGuiKeyModFlags;
enum ImGuiKeyModFlags_
{
    ImGuiKeyModFlags_None  = ImGuiModFlags_None,
    ImGuiKeyModFlags_Ctrl  = ImGuiModFlags_Ctrl,
    ImGuiKeyModFlags_Shift = ImGuiModFlags_Shift,
    ImGuiKeyModFlags_Alt   = ImGuiModFlags_Alt,
    ImGuiKeyModFlags_Super = ImGuiModFlags_Super
};

#endif   // #ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//-----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Включите imgui_user. h в конце imgui. h (удобно, чтобы пользователь явно включал только vanilla imgui. h)
#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif

#endif   // #ifndef IMGUI_DISABLE
