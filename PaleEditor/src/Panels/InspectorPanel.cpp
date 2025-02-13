#include "stdafx.h"
#include "InspectorPanel.h"

namespace PaleUI
{
    CInspectorPanel::CInspectorPanel()
    {
    }
    CInspectorPanel::~CInspectorPanel()
    {
    }

    void CInspectorPanel::OnUIRender(entt::registry& vRegistry)
    {
        ImGui::Begin("Inspector");
        __renderUICompName(vRegistry, m_SelectionEntity);
		ImGui::Separator();
        __renderUICompTrans(vRegistry, m_SelectionEntity);
		ImGui::Separator();
		__renderUICompLight(vRegistry, m_SelectionEntity);
		ImGui::Separator();

        ImGui::End();
    }

    void CInspectorPanel::OnSingleSelection(const entt::entity& vId)
    {
        m_SelectionEntity = vId;
    }

    void CInspectorPanel::__renderUICompName(entt::registry& vRegistry, const entt::entity& vId)
    {
        if (auto* name = vRegistry.try_get<PaleRdr::SCompName>(vId))
        {
            ImGui::Text(name->_Name.c_str());
        }
    }

    void CInspectorPanel::__renderUICompTrans(entt::registry& vRegistry, const entt::entity& vId)
    {
		ImGui::Text("Transfrom");
        if (auto* trans = vRegistry.try_get<PaleRdr::SCompTransform>(vId))
        {
			__drawVec3Control("Position", trans->_Position, trans->_InitPosition, -10.f, 10.f, 0.1f);
			__drawVec3Control("Rotation", trans->_Rotation, trans->_InitRotation, -360.f, 360.f, 2.f);
			__drawVec3Control("Scale", trans->_Scale, trans->_InitScale, 0.1f, 5.f, 0.1f);
        }
    }

	void CInspectorPanel::__renderUICompLight(entt::registry& vRegistry, const entt::entity& vId)
	{
		ImGui::Text("PointLight");

		if (auto* light = vRegistry.try_get<PaleRdr::SCompPointLight>(vId))
		{
			ImGui::ColorEdit3("Color", glm::value_ptr(light->_Color));
			ImGui::DragFloat("Intensity", &light->_Intensity, 0.002f, 0.0f, 1.0f, "%.2f");
		}
	}

    // -------------

	void CInspectorPanel::__drawVec3Control(
		const std::string& vLabel,
		glm::vec3& vValues,
		const glm::vec3& vResetValue,
		float vMin,
		float vMax,
		float vSpeed,
		float vColumnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(vLabel.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, vColumnWidth);
		ImGui::Text(vLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		if (ImGui::Button("X"))
			vValues.x = vResetValue.x;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &vValues.x, vSpeed, vMin, vMax, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

		if (ImGui::Button("Y"))
			vValues.y = vResetValue.y;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vValues.y, vSpeed, vMin, vMax, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.2f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.3f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.2f, 0.7f, 1.0f));

		if (ImGui::Button("Z"))
			vValues.z = vResetValue.z;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vValues.z, vSpeed, vMin, vMax, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopStyleVar();
		ImGui::PopID();
	}
}