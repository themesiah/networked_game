#pragma once

#ifndef H_CHAT_MANAGER
#define H_CHAT_MANAGER

#include "imgui.h"

class CChatManager
{
public:
	CChatManager();
	virtual ~CChatManager();
	void Clear();
	void AddMessage(const char* fmt, ...);
	void Draw();
private:
	bool m_ChatWindowOpen;
	ImGuiTextBuffer m_Buffer;
	ImVector<int> m_LineOffsets;        // Index to lines offset
	bool m_ScrollToBottom;
	int m_MaxMessageSize;
	char* m_MessageBuffer;
};

#endif