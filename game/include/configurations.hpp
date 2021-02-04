#ifndef __CONFIGURATIONS_HPP__
#define __CONFIGURATIONS_HPP__

#include <glm/vec4.hpp>

namespace iventory_config {
    struct Config {
        glm::vec4 crosshair_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 inv_cell_color = glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
        glm::vec4 inv_back_color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        glm::vec4 inv_select_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);

        float cell_size = 60;         // Cell size
        float cell_margin = 8;        // Cell margin
        float inv_offset = 65.f;      // Size between sidebar and inventory

        float cross_wsize = 20.f;
        float cross_hsize = 20.f;

        float int_height = (cell_size + cell_margin) * 9 + cell_margin;             // Total height
        float side_width = cell_size + cell_margin + cell_margin;                   // Sidebar total width
        float inv_width = (cell_size + cell_margin) * 3 + cell_margin;              // Inventory total width

        float hor_center;       // Horizontal center
        float vert_center;      // Vertical center

        float inv_left_pos = cell_size + cell_margin + cell_margin + inv_offset;    // Inventory left pos
        float int_bot_pos;                                                          // Bottom pos

        Config(float width, float height) {
            hor_center = width / 2.f;
            vert_center = height / 2.f;
            int_bot_pos = vert_center - int_height / 2.f;
        }

        Config() {
            int_bot_pos = 0;
        }
    };
}

#endif
