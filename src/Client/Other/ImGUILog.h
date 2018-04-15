#pragma once

#ifndef H_IMGUI_LOG
#define H_IMGUI_LOG

#include "imgui.h"
#include "imgui_internal.h"

#include "Utils\Singleton.h"

#define IMLOG_ERROR( x, ... )     ImguiAppLog::GetInstance().AddErrorLog(x, __VA_ARGS__ );
#define IMLOG_WARNING( x, ...  )  ImguiAppLog::GetInstance().AddWarningLog(x, __VA_ARGS__ );
#define IMLOG_INFO( x, ...  )     ImguiAppLog::GetInstance().AddInfoLog(x, __VA_ARGS__ );
#define IMLOG_NETWORK( x, ...  )     ImguiAppLog::GetInstance().AddNetworkLog(x, __VA_ARGS__ );
#define IMLOG_DRAW ImguiAppLog::GetInstance().Draw("LOG", &ImguiAppLog::GetInstance().ImguiLogWindowOpen);

class ImguiAppLog : public base::utils::CSingleton<ImguiAppLog>
{
public:
	bool ImguiLogWindowOpen;
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

	void    Clear()     { Buf.clear(); LineOffsets.clear(); }

	void AddInfoLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Buf.appendfv("[INFO]", 0);
		AddLog(fmt, args);
	}

	void AddWarningLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Buf.appendfv("[WARNING]", 0);
		AddLog(fmt, args);
	}

	void AddErrorLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Buf.appendfv("[ERROR]", 0);
		AddLog(fmt, args);
	}

	void AddNetworkLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Buf.appendfv("[NETWORK]", 0);
		AddLog(fmt, args);
	}

	void    AddLog(const char* fmt, ...)// IM_PRINTFARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		Buf.appendfv("\n", 0);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size);
		ScrollToBottom = true;
	}

	void    Draw(const char* title, bool* p_opened = NULL)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
		ImGui::Begin(title, p_opened);
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		if (copy) ImGui::LogToClipboard();

		if (Filter.IsActive())
		{
			const char* buf_begin = Buf.begin();
			const char* line = buf_begin;
			for (int line_no = 0; line != NULL; line_no++)
			{
				const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
				if (Filter.PassFilter(line, line_end)) {
					ImGui::TextUnformatted(line, line_end);
				}
				line = line_end && line_end[1] ? line_end + 1 : NULL;
			}
		}
		else
		{
			ImGui::TextUnformatted(Buf.begin());
		}

		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::End();
	}
protected:
	ImguiAppLog() :
		ImguiLogWindowOpen(true)
	{

	}
	friend class base::utils::CSingleton<ImguiAppLog>;
};

#endif