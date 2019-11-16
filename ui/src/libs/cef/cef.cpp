#include <mainframe/libs/cef/cef.h>

#ifdef MAINFRAME_ENABLE_CEF
#include <cef_base.h>
#include <cef_app.h>
#include <cef_client.h>
#endif

namespace mainframe {
	namespace cef {
#ifdef MAINFRAME_ENABLE_CEF
		class MainframeCef : public CefApp, public CefBrowserProcessHandler {
		public:
			// CefApp methods:
			virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
				return this;
			}

			// CefBrowserProcessHandler methods:
			virtual void OnContextInitialized() override {

			}

		private:
			// Include the default reference counting implementation.
			IMPLEMENT_REFCOUNTING(MainframeCef);
		};
#endif

		int Cef::init() {
#ifdef MAINFRAME_ENABLE_CEF
			// Structure for passing command-line arguments.
			// The definition of this structure is platform-specific.
			CefMainArgs main_args;

			// Optional implementation of the CefApp interface.
			CefRefPtr<MainframeCef> app(new MainframeCef);

			// Execute the sub-process logic, if any. This will either return immediately for the browser
			// process or block until the sub-process should exit.
			int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
			if (exit_code >= 0) {
				// The sub-process terminated, exit now.
				return exit_code;
			}

			// Populate this structure to customize CEF behavior.
			CefSettings settings;

			// Initialize CEF in the main process.
			CefInitialize(main_args, settings, app.get(), nullptr);

			inited = true;
			return 0;
#endif

			return -1;
		}

		Cef::~Cef() {
			if (!inited) return;

#ifdef MAINFRAME_ENABLE_CEF
			CefShutdown();
#endif
		}

		void Cef::tick() {
#ifdef MAINFRAME_ENABLE_CEF
			CefDoMessageLoopWork();
#endif
		}
	}
}