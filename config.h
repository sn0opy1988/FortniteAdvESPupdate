
#ifndef __CONFIG_H
#define __CONFIG_H

#define AUTOFIRE_TOGGLE_KEY VK_F7
#define CHAMS_KEY VK_F8
#define ESP_KEY VK_F9
#define EXTENDED_ESP_KEY VK_XBUTTON2

#define DEFAULT_FONT L"Verdana"
#define ENEMY_TEXT_COLOR Color{ 0.9f, 0.9f, 0.15f, 0.95f }

class Config
{
public:
	bool m_EnableESP = true;
	bool m_EnableExtendedESP = false;
	bool m_EnableChams = true;
};

#endif
