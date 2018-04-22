#include "ChatManager.h"

#include <memory>
#include <string>

#include "Engine.h"
#include "../Network/NetworkManagerClient.h"
#include "Serializer\OutputMemoryBitStream.h"
#include "Replication\Packet.h"

CChatManager::CChatManager() :
m_ChatWindowOpen(true)
, m_MaxMessageSize(200)
{
	m_MessageBuffer = static_cast<char*>(std::realloc(m_MessageBuffer, m_MaxMessageSize));
	std::memset(m_MessageBuffer, '\0', m_MaxMessageSize);
}

CChatManager::~CChatManager()
{

}

void CChatManager::Clear()
{
	m_Buffer.clear();
	m_LineOffsets.clear();
}

void CChatManager::AddMessage(const char* fmt, ...)// IM_PRINTFARGS(2)
{
	int old_size = m_Buffer.size();
	va_list args;
	va_start(args, fmt);
	m_Buffer.appendfv(fmt, args);
	m_Buffer.appendfv("\n", 0);
	va_end(args);
	for (int new_size = m_Buffer.size(); old_size < new_size; old_size++)
	{
		if (m_Buffer[old_size] == '\n')
		{
			m_LineOffsets.push_back(old_size);
		}
	}
	m_ScrollToBottom = true;
}

void CChatManager::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Chat", &m_ChatWindowOpen);
	if (ImGui::Button("Send"))
	{
		int messageSize = strnlen_s(m_MessageBuffer, m_MaxMessageSize);
		if (messageSize > 0)
		{
			std::string lMessage = std::string(m_MessageBuffer, messageSize);
			std::memset(m_MessageBuffer, '\0', m_MaxMessageSize);
			OutputMemoryBitStream lOutput;
			lOutput.Serialize(PT_RPC, PACKET_BIT_SIZE);
			lOutput.Serialize<uint32_t>('CHAT');
			lOutput.Serialize(lMessage);
			lOutput.Close();
			CEngine::GetInstance().GetNetworkManagerClient().RPCSend(lOutput);
		}
	}
	ImGui::SameLine();
	ImGui::InputText("", m_MessageBuffer, m_MaxMessageSize);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

	ImGui::TextUnformatted(m_Buffer.begin());

	if (m_ScrollToBottom)
	{
		ImGui::SetScrollHere(1.0f);
	}
	m_ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}