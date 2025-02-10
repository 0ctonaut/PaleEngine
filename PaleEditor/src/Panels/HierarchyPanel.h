#pragma once
#include <vector>
#include <string>
#include <entt/entt.hpp>
#include "PaleRenderer/Common/Event.h"

namespace PaleUI
{
    class CHierarchyPanel
    {
    public:
        CHierarchyPanel();
        ~CHierarchyPanel();

        void OnUIRender(std::unordered_map<std::string, entt::entity>& vNames);
        Event<entt::entity> EventSingleSelection;
    private:
    };
}
