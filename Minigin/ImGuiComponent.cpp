#include "ImGuiComponent.h"
#include "imgui_plot.h"
#include <array>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <thread>

using namespace dae;

void ImGuiComponent::RunBenchmarkEx1()
{
    std::thread([this]()
        {
            std::vector<float> timings(11, 0.0f);

            for (int i = 1; i <= m_NumRunsEx1; ++i)
            {
                std::vector<int> arr(10000000, 1);

                for (int index = 0, j = 1; j <= 1024; j *= 2, ++index)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (uint32_t k = 0; k < arr.size(); k += j)
                    {
                        arr[k] *= 2;
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    timings[index] += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }
            }

            for (auto& timing : timings)
            {
                timing /= m_NumRunsEx1;
            }

            std::lock_guard<std::mutex> lock(m_Mutex);
            m_TimingsEx1 = std::move(timings);
        }).detach();
}

void ImGuiComponent::RunBenchmarkEx2()
{
    std::thread([this]()
        {
            std::vector<float> timings(11, 0.0f);

            for (int i = 1; i <= m_NumRunsEx2; ++i)
            {
                std::vector<gameObject3D> arr(10000000);

                for (int index = 0, j = 1; j <= 1024; j *= 2, ++index)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (uint32_t k = 0; k < arr.size(); k += j)
                    {
                        arr[k].id *= 2;
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    timings[index] += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }
            }

            for (auto& timing : timings)
            {
                timing /= m_NumRunsEx2;
            }

            std::lock_guard<std::mutex> lock(m_Mutex);
            m_TimingsEx2 = std::move(timings);
        }).detach();
}

void ImGuiComponent::RunBenchmarkEx2Alt()
{
    std::thread([this]()
        {
            std::vector<float> timings(11, 0.0f);

            for (int i = 1; i <= m_NumRunsEx2; ++i)
            {
                std::vector<gameObject3DAlt> arr(10000000);

                for (int index = 0, j = 1; j <= 1024; j *= 2, ++index)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (uint32_t k = 0; k < arr.size(); k += j)
                    {
                        arr[k].id *= 2;
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    timings[index] += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }
            }

            for (auto& timing : timings)
            {
                timing /= m_NumRunsEx2;
            }

            std::lock_guard<std::mutex> lock(m_Mutex);
            m_TimingsEx2Alt = std::move(timings);
        }).detach();
}


void ImGuiComponent::Render() const
{
    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::Text("# Samples:");
        ImGui::SameLine();
        static int numRuns = m_NumRunsEx2;
        ImGui::InputInt("##Runs", &numRuns);


        if (ImGui::Button("Run Benchmark"))
        {
            const_cast<ImGuiComponent*>(this)->m_NumRunsEx2 = numRuns;
            const_cast<ImGuiComponent*>(this)->RunBenchmarkEx1();
        }

        if (!m_TimingsEx1.empty())
        {
            ImGui::PlotLines("Execution Time (us)", m_TimingsEx1.data(), static_cast<int>(m_TimingsEx1.size()), 0, nullptr, 0.0f, *std::max_element(m_TimingsEx1.begin(), m_TimingsEx1.end()), ImVec2(0, 100));
        }
    }
    ImGui::End();

    if (ImGui::Begin("Exercise 2"))
    {
        ImGui::Text("# Samples:");
        ImGui::SameLine();
        static int numRuns = m_NumRunsEx2;
        ImGui::InputInt("##Runs", &numRuns);


        if (ImGui::Button("Run Benchmark"))
        {
            const_cast<ImGuiComponent*>(this)->m_NumRunsEx2 = numRuns;
            const_cast<ImGuiComponent*>(this)->RunBenchmarkEx2();

        }
        ImGui::SameLine();
        if (ImGui::Button("Run Alt Benchmark"))
        {
            const_cast<ImGuiComponent*>(this)->m_NumRunsEx2 = numRuns;
            const_cast<ImGuiComponent*>(this)->RunBenchmarkEx2Alt();
        }

        if (!m_TimingsEx2.empty())
        {
            ImGui::PlotLines("Time (us)", m_TimingsEx2.data(), static_cast<int>(m_TimingsEx2.size()), 0, nullptr, 0.0f, *std::max_element(m_TimingsEx2.begin(), m_TimingsEx2.end()), ImVec2(0, 80));
        }

        if (!m_TimingsEx2Alt.empty())
        {
            ImGui::PlotLines("Time alt(us)", m_TimingsEx2Alt.data(), static_cast<int>(m_TimingsEx2Alt.size()), 0, nullptr, 0.0f, *std::max_element(m_TimingsEx2Alt.begin(), m_TimingsEx2Alt.end()), ImVec2(0, 80));
        }

        //This Code is made by Claude.AI, I used it to make a fancy graph quickly ;)
        // Combined plot using the provided imgui_plot.h
        if (!m_TimingsEx2.empty() && !m_TimingsEx2Alt.empty())
        {
            ImGui::Separator();
            ImGui::Text("Combined Comparison");

            // Create an array of pointers to the data for multiple plots
            static const float* ys_list[2];
            ys_list[0] = m_TimingsEx2.data();
            ys_list[1] = m_TimingsEx2Alt.data();

            // Set colors for each plot line
            static ImU32 colors[2];
            colors[0] = ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red for standard
            colors[1] = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));  // Blue for alternative

            // Determine y-axis range
            float maxValue = std::max(
                *std::max_element(m_TimingsEx2.begin(), m_TimingsEx2.end()),
                *std::max_element(m_TimingsEx2Alt.begin(), m_TimingsEx2Alt.end())
            );

            // Create stride labels for x-axis (optional - could be shown as overlay)
            static char overlay_text[64];
            snprintf(overlay_text, sizeof(overlay_text), "1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024");

            // Setup the plot configuration
            ImGui::PlotConfig conf;
            conf.values.ys_list = ys_list;
            conf.values.ys_count = 2;
            conf.values.colors = colors;
            conf.values.count = static_cast<int>(m_TimingsEx2.size());

            conf.scale.min = 0;
            conf.scale.max = maxValue * 1.1f;  // Add 10% margin at the top

            conf.grid_x.show = true;
            conf.grid_x.size = 1.0f;  // Grid line every index
            conf.grid_x.subticks = 1;

            conf.grid_y.show = true;
            conf.grid_y.size = maxValue / 5.0f;  // 5 horizontal grid lines
            conf.grid_y.subticks = 1;

            conf.tooltip.show = true;
            conf.tooltip.format = "Index %g: %.2f us";

            conf.frame_size = ImVec2(300, 150);  // Full width, 150px height
            conf.line_thickness = 2.0f;
            conf.overlay_text = overlay_text;

            // Draw the plot
            ImGui::Plot("##CombinedPlot", conf);

            // Add legend manually since PlotConfig doesn't have a built-in legend
            ImGui::Dummy(ImVec2(0, 5));

            const float legendSquareSize = 10.0f;
            const ImVec2 legendSquare(legendSquareSize, legendSquareSize);

            ImGui::Text("Legend:");
            ImGui::SameLine();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();

            // Standard legend item
            draw_list->AddRectFilled(pos, ImVec2(pos.x + legendSquareSize, pos.y + legendSquareSize), colors[0]);
            ImGui::Dummy(legendSquare);
            ImGui::SameLine();
            ImGui::Text("Standard");
            ImGui::SameLine();

            // Alternative legend item
            pos = ImGui::GetCursorScreenPos();
            draw_list->AddRectFilled(pos, ImVec2(pos.x + legendSquareSize, pos.y + legendSquareSize), colors[1]);
            ImGui::Dummy(legendSquare);
            ImGui::SameLine();
            ImGui::Text("Alternative");
        }
    }
    ImGui::End();
}