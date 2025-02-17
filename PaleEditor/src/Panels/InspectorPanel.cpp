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
        __renderUICompTrans(vRegistry, m_SelectionEntity);
		__renderUICompLight(vRegistry, m_SelectionEntity);
		__renderUICompMeshRdr(vRegistry, m_SelectionEntity);
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
			ImGui::Separator();
        }
    }

    void CInspectorPanel::__renderUICompTrans(entt::registry& vRegistry, const entt::entity& vId)
    {
        if (auto* trans = vRegistry.try_get<PaleRdr::SCompTransform>(vId))
        {
			if (ImGui::TreeNodeEx("Transfrom", ImGuiTreeNodeFlags_DefaultOpen))
			{
				__drawVec3Control("Position", trans->_Position, trans->_InitPosition, -10.f, 10.f, 0.1f);
				__drawVec3Control("Rotation", trans->_Rotation, trans->_InitRotation, -360.f, 360.f, 2.f);
				__drawVec3Control("Scale", trans->_Scale, trans->_InitScale, 0.1f, 5.f, 0.1f);
				ImGui::TreePop();
			}
			ImGui::Separator();
        }
    }

	void CInspectorPanel::__renderUICompLight(entt::registry& vRegistry, const entt::entity& vId)
	{
		if (auto* light = vRegistry.try_get<PaleRdr::SCompPointLight>(vId))
		{
			if (ImGui::TreeNodeEx("PointLight", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorEdit3("Color", glm::value_ptr(light->_Color));
				ImGui::DragFloat("Intensity", &light->_Intensity, 0.002f, 0.0f, 1.0f, "%.2f");
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
	}

	void CInspectorPanel::__renderUICompMeshRdr(entt::registry& vRegistry, const entt::entity& vId)
	{
		if (auto* meshRdr = vRegistry.try_get<PaleRdr::SCompMeshRenderer>(vId))
		{
			if (ImGui::TreeNodeEx("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Sub mesh number: %d", meshRdr->_Meshes.size());
				if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float imageSize = 16.0f;
					float previewImageSize = 256.0f;
					for (auto it = meshRdr->_Materials.begin(); it != meshRdr->_Materials.end(); ++it)
					{
						auto DisplayTextureButton = [&previewImageSize](const std::string& label, unsigned int textureID, float imageSize) {
							if (ImGui::ImageButton(label.c_str(), textureID, ImVec2(imageSize, imageSize))) {
								ImGui::OpenPopup((label + "Preview").c_str());
							}
							ImGui::SameLine();
							ImGui::Text(label.c_str());

							if (ImGui::BeginPopup((label + "Preview").c_str())) {
								ImGui::Text((label + " Preview").c_str());
								ImGui::Image(textureID, ImVec2(previewImageSize, previewImageSize));
								ImGui::EndPopup();
							}
						};

						DisplayTextureButton("Albedo", it->second->fetchTextureOfType(PaleRdr::ETexture::Albedo)[0]->getID(), imageSize);
						DisplayTextureButton("Normal", it->second->fetchTextureOfType(PaleRdr::ETexture::Normal)[0]->getID(), imageSize);
						DisplayTextureButton("Metallic", it->second->fetchTextureOfType(PaleRdr::ETexture::Metallic)[0]->getID(), imageSize);
						DisplayTextureButton("Roughness", it->second->fetchTextureOfType(PaleRdr::ETexture::Roughness)[0]->getID(), imageSize);
						DisplayTextureButton("AO", it->second->fetchTextureOfType(PaleRdr::ETexture::AO)[0]->getID(), imageSize);
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
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