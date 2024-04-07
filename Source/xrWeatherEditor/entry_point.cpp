#include "pch.hpp"

#pragma managed(push, off)
#include <windows.h>
#include "Public/engine.hpp"
#include "ide_impl.hpp"
#pragma managed(pop)

#include "window_ide.h"

#pragma comment(lib, "user32.lib")

ide_impl* g_ide = nullptr;
namespace XRay
{
    namespace xrWeatherEditor
    {
    private
        ref class window_ide_final: public ::xrWeatherEditor::window_ide
        {
        public:
            window_ide_final(::xrWeatherEditor::ide*& ide, ::xrWeatherEditor::engine* engine): ::xrWeatherEditor::window_ide(engine)
            {
                m_ide = ide;
                Application::Idle += gcnew System::EventHandler(this, &window_ide_final::on_idle);
            }

            ~window_ide_final()
            {
                Application::Idle -= gcnew System::EventHandler(this, &window_ide_final::on_idle);
                m_engine = nullptr;
                m_ide    = nullptr;
            }

        protected:
            virtual void WndProc(Message % m) override
            {
                LRESULT result;
                if (m_engine && m_engine->on_message((HWND)m.HWnd.ToInt32(), m.Msg, m.WParam.ToInt32(), m.LParam.ToInt32(), result))
                    return;

                ::xrWeatherEditor::window_ide::WndProc(m);
            }

        private:
            void on_idle(System::Object ^ sender, System::EventArgs ^ event_args)
            {
                ide_impl* impl = dynamic_cast<ide_impl*>(m_ide);
                impl->on_idle_start();

                MSG message;
                do
                {
                    m_engine->on_idle();
                    impl->on_idle();
                }
                while (m_engine && !PeekMessage(&message, HWND(0), 0, 0, 0));

                impl->on_idle_end();
            }
        };

        void initialize_impl(::xrWeatherEditor::ide*& ide, ::xrWeatherEditor::engine* engine)
        {
            VERIFY(!g_ide);
            g_ide = new ide_impl(engine);
            ide   = g_ide;
            g_ide->window(gcnew window_ide_final(ide, engine));
        }
    }   // namespace xrWeatherEditor
}   // namespace XRay

#pragma managed(push, off)
#include <objbase.h>
WINOLEAPI CoInitializeEx(IN LPVOID pvReserved, IN DWORD dwCoInit);
#pragma comment(lib, "ole32.lib")

extern "C" __declspec(dllexport) void initialize(xrWeatherEditor::ide*& ide, xrWeatherEditor::engine* engine)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    XRay::xrWeatherEditor::initialize_impl(ide, engine);
}

extern "C" __declspec(dllexport) void finalize(xrWeatherEditor::ide*& ide)
{
    delete (ide);
    ide   = nullptr;
    g_ide = nullptr;
}
#pragma managed(pop)
