#include "stdafx.h"
#include "PanelManager.h"

namespace PaleUI
{
	CPanelManager::CPanelManager()
	{
		m_HierarchyPanel = new CHierarchyPanel();
		m_InspectorPanel = new CInspectorPanel();
		m_HierarchyPanel->EventSingleSelection.subscribe(
			[this](entt::entity vId) { 
				m_InspectorPanel->OnSingleSelection(vId);
			}
		);
	}
	CPanelManager::~CPanelManager()
	{
		delete m_HierarchyPanel;
		delete m_InspectorPanel;
	}

	void CPanelManager::OnUIRender(PaleRdr::CScene& vScene)
	{
		auto& Registry = vScene.fetchRegistry();
		auto view = Registry.view<PaleRdr::SCompName>();
		std::unordered_map<std::string, entt::entity> names;
		for (auto it = view.begin(); it != view.end(); ++it)
		{
			PaleRdr::SCompName& name = view.get<PaleRdr::SCompName>(*it);
			names[name._Name] = *it;
		}
		m_HierarchyPanel->OnUIRender(names);
		m_InspectorPanel->OnUIRender(Registry);
	}
}