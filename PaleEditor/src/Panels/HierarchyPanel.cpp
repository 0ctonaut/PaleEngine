#include "stdafx.h"
#include "HierarchyPanel.h"

namespace PaleUI
{
    CHierarchyPanel::CHierarchyPanel()
    {
    }
    CHierarchyPanel::~CHierarchyPanel()
    {
    }

    struct ExampleSelectionWithDeletion : ImGuiSelectionBasicStorage
    {
        // Find which item should be Focused after deletion.
        // Call _before_ item submission. Retunr an index in the before-deletion item list, your item loop should call SetKeyboardFocusHere() on it.
        // The subsequent ApplyDeletionPostLoop() code will use it to apply Selection.
        // - We cannot provide this logic in core Dear ImGui because we don't have access to selection data.
        // - We don't actually manipulate the ImVector<> here, only in ApplyDeletionPostLoop(), but using similar API for consistency and flexibility.
        // - Important: Deletion only works if the underlying ImGuiID for your items are stable: aka not depend on their index, but on e.g. item id/ptr.
        // FIXME-MULTISELECT: Doesn't take account of the possibility focus target will be moved during deletion. Need refocus or scroll offset.
        int ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count)
        {
            if (Size == 0)
                return -1;

            // If focused item is not selected...
            const int focused_idx = (int)ms_io->NavIdItem;  // Index of currently focused item
            if (ms_io->NavIdSelected == false)  // This is merely a shortcut, == Contains(adapter->IndexToStorage(items, focused_idx))
            {
                ms_io->RangeSrcReset = true;    // Request to recover RangeSrc from NavId next frame. Would be ok to reset even when NavIdSelected==true, but it would take an extra frame to recover RangeSrc when deleting a selected item.
                return focused_idx;             // Request to focus same item after deletion.
            }

            // If focused item is selected: land on first unselected item after focused item.
            for (int idx = focused_idx + 1; idx < items_count; idx++)
                if (!Contains(GetStorageIdFromIndex(idx)))
                    return idx;

            // If focused item is selected: otherwise return last unselected item before focused item.
            for (int idx = std::min(focused_idx, items_count) - 1; idx >= 0; idx--)
                if (!Contains(GetStorageIdFromIndex(idx)))
                    return idx;

            return -1;
        }

        // Rewrite item list (delete items) + update selection.
        // - Call after EndMultiSelect()
        // - We cannot provide this logic in core Dear ImGui because we don't have access to your items, nor to selection data.
        template<typename ITEM_TYPE>
        void ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, ImVector<ITEM_TYPE>& items, int item_curr_idx_to_select)
        {
            // Rewrite item list (delete items) + convert old selection index (before deletion) to new selection index (after selection).
            // If NavId was not part of selection, we will stay on same item.
            ImVector<ITEM_TYPE> new_items;
            new_items.reserve(items.Size - Size);
            int item_next_idx_to_select = -1;
            for (int idx = 0; idx < items.Size; idx++)
            {
                if (!Contains(GetStorageIdFromIndex(idx)))
                    new_items.push_back(items[idx]);
                if (item_curr_idx_to_select == idx)
                    item_next_idx_to_select = new_items.Size - 1;
            }
            items.swap(new_items);

            // Update selection
            Clear();
            if (item_next_idx_to_select != -1 && ms_io->NavIdSelected)
                SetItemSelected(GetStorageIdFromIndex(item_next_idx_to_select), true);
        }
    };

    void CHierarchyPanel::OnUIRender(std::unordered_map<std::string, entt::entity>& vNames)
    {
        ImGui::Begin("Hierarchy");
        {
            static ImVector<ImGuiID> items;
            static ExampleSelectionWithDeletion selection;
            selection.UserData = (void*)&items;
            selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self, int idx) { 
                ImVector<ImGuiID>* p_items = (ImVector<ImGuiID>*)self->UserData; 
                return (*p_items)[idx]; 
            }; // Index -> ID

            ImGui::Text("Selection size: %d/%d", selection.Size, items.Size);

            if (selection.Size == 1)
            {
                auto it = vNames.begin();
                for (int i = 0; i < items.Size; i++) {
                    ImGuiID id = items[i];
                    if (selection.Contains(id)) {
                        entt::entity entityId = (*it).second;
                        EventSingleSelection.invoke(entityId);
                    }
                    ++it;
                }
            }

            // Initialize default list with 50 items + button to add/remove items.
            static ImGuiID items_next_id = 0;
            if (items_next_id == 0)
                for (ImGuiID n = 0; n < vNames.size(); n++)
                    items.push_back(items_next_id++);

            // (1) Extra to support deletion: Submit scrolling range to avoid glitches on deletion
            const float items_height = ImGui::GetTextLineHeightWithSpacing();
            ImGui::SetNextWindowContentSize(ImVec2(0.0f, items.Size * items_height));

            if (ImGui::BeginChild("##Basket", ImVec2(-FLT_MIN, ImGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags, selection.Size, items.Size);
                selection.ApplyRequests(ms_io);

                const bool want_delete = ImGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (selection.Size > 0);
                const int item_curr_idx_to_focus = want_delete ? selection.ApplyDeletionPreLoop(ms_io, items.Size) : -1;

                auto it = vNames.begin();
                for (int n = 0; n < items.Size; n++)
                {
                    const ImGuiID item_id = items[n];
                    std::string label = std::format("{}: {}", item_id, (*it++).first.c_str());
                    bool item_is_selected = selection.Contains(item_id);
                    ImGui::SetNextItemSelectionUserData(n);
                    ImGui::Selectable(label.c_str(), item_is_selected);
                    if (item_curr_idx_to_focus == n)
                        ImGui::SetKeyboardFocusHere(-1);
                }

                // Apply multi-select requests
                ms_io = ImGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
                if (want_delete)
                    selection.ApplyDeletionPostLoop(ms_io, items, item_curr_idx_to_focus);
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}