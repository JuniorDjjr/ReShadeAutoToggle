#include "plugin.h"
#include "IniReader/IniReader.h"

using namespace plugin;
using namespace std;

fstream lg;

bool enabled = true;
bool pressing = false;

int key = 0;
int ctrl = 0;
int shift = 0;
int alt = 0;

class ReShadeAutoToggle {
public:
    ReShadeAutoToggle() {

		lg.open("ReShadeAutoToggle.log", fstream::out | fstream::trunc);
		lg << "Build 1" << endl;
		lg.flush();

		CIniReader ini(GAME_PATH("ReShade.ini"));

		if (ini.data.size() > 0)
		{
			string keyString = ini.ReadString("INPUT", "KeyEffects", "");
			if (keyString.length() > 0) {

				sscanf(&keyString[0], "%d , %d , %d , %d", &key, &ctrl, &shift, &alt);

				if (key == 0)
				{
					ini.WriteString("INPUT", "KeyEffects", "123,0,0,0");
					key = VK_F12;
				}

			#if defined(GTASA)
				Events::drawMenuBackgroundEvent.before += [] {
			#else
				Events::menuDrawingEvent += [] {
			#endif
					if (pressing) {
						pressing = false;
						keybd_event(key, false, KEYEVENTF_KEYUP, 0);
						if (ctrl) keybd_event(VK_CONTROL, false, KEYEVENTF_KEYUP, 0);
						if (shift) keybd_event(VK_SHIFT, false, KEYEVENTF_KEYUP, 0);
						if (alt) keybd_event(VK_MENU, false, KEYEVENTF_KEYUP, 0);
						enabled = false;
					}
					if (enabled) {
						if (!pressing)
						{
							pressing = true;
							keybd_event(key, false, 0, 0);
							if (ctrl) keybd_event(VK_CONTROL, false, 0, 0);
							if (shift) keybd_event(VK_SHIFT, false, 0, 0);
							if (alt) keybd_event(VK_MENU, false, 0, 0);
						}
					}
				};

				Events::processScriptsEvent.before += [] {
					if (pressing) {
						pressing = false;
						keybd_event(key, false, KEYEVENTF_KEYUP, 0);
						if (ctrl) keybd_event(VK_CONTROL, false, KEYEVENTF_KEYUP, 0);
						if (shift) keybd_event(VK_SHIFT, false, KEYEVENTF_KEYUP, 0);
						if (alt) keybd_event(VK_MENU, false, KEYEVENTF_KEYUP, 0);
						enabled = true;
					}
					if (!enabled) {
						if (!pressing)
						{
							pressing = true;
							keybd_event(key, false, 0, 0);
							if (ctrl) keybd_event(VK_CONTROL, false, 0, 0);
							if (shift) keybd_event(VK_SHIFT, false, 0, 0);
							if (alt) keybd_event(VK_MENU, false, 0, 0);
						}
					}
				};
			}
			else {
				lg << "Unable to read 'INPUT' 'KeyEffects' from 'ReShade.ini'" << endl;
				lg.flush();
			}

		}
		else {
			lg << "Unable to find 'ReShade.ini'" << endl;
			lg.flush();
		}
    }
} reShadeAutoToggle;
