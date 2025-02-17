#include "imgui/imgui.h"
#include "logger_sink.h"

namespace widgets
{
    void LoggerSinkWidget::receive(slog::LogMsg log)
    {
        if (log.lvl >= sink_lvl)
        {
            mtx.lock();
            new_item = true;
            all_lines.push_back({ log.lvl, format_log(log, false) });
            if (all_lines.size() == max_lines)
                all_lines.pop_front();
            mtx.unlock();
        }
    }

    void LoggerSinkWidget::draw()
    {
        mtx.lock();
        for (LogLine& ll : all_lines)
        {
            std::string timestamp = ll.str.substr(0, 24);
            std::string text = ll.str.substr(24, ll.str.size());

            ImGui::Text("%s", timestamp.c_str());
            ImGui::SameLine();

            if (ll.lvl == slog::LOG_TRACE)
                ImGui::TextColored(ImColor(255, 255, 255), "%s", text.c_str());
            else if (ll.lvl == slog::LOG_DEBUG)
                ImGui::TextColored(ImColor(0, 255, 255), "%s", text.c_str());
            else if (ll.lvl == slog::LOG_INFO)
                ImGui::TextColored(ImColor(0, 255, 0), "%s", text.c_str());
            else if (ll.lvl == slog::LOG_WARN)
                ImGui::TextColored(ImColor(255, 255, 0), "%s", text.c_str());
            else if (ll.lvl == slog::LOG_ERROR)
                ImGui::TextColored(ImColor(255, 0, 0), "%s", text.c_str());
            else if (ll.lvl == slog::LOG_CRIT)
                ImGui::TextColored(ImColor(255, 0, 255), "%s", text.c_str());
        }
        if (new_item)
        {
            ImGui::SetScrollHereY(1.0f);
            new_item = false;
        }
        if(ImGui::IsWindowAppearing())
            new_item = true;
        mtx.unlock();
    }
}