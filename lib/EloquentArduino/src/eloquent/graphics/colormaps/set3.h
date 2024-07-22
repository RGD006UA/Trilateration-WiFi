//
// Created by Simone on 18/05/2022.
//

#pragma once
#include "../../macros.h"


namespace Eloquent {
    namespace Graphics {
        namespace Colormaps {
            class Set3 {
            public:

                /**
                 * Convert single byte to RGB color
                 * @param x
                 * @param r
                 * @param g
                 * @param b
                 */
                void convert(uint8_t x, uint8_t *r, uint8_t *g, uint8_t *b) {
                    *r = red[x << 2];
                    *g = green[x << 2];
                    *b = blue[x << 2];
                }

            protected:
                uint8_t red[64] = {141, 141, 141, 141, 141, 141, 255, 255, 255, 255, 255, 190, 190, 190, 190, 190, 251, 251, 251, 251, 251, 128, 128, 128, 128, 128, 128, 253, 253, 253, 253, 253, 179, 179, 179, 179, 179, 252, 252, 252, 252, 252, 217, 217, 217, 217, 217, 217, 188, 188, 188, 188, 188, 204, 204, 204, 204, 204, 255, 255, 255, 255, 255, 255};
                uint8_t green[64] = {211, 211, 211, 211, 211, 211, 255, 255, 255, 255, 255, 186, 186, 186, 186, 186, 128, 128, 128, 128, 128, 177, 177, 177, 177, 177, 177, 180, 180, 180, 180, 180, 222, 222, 222, 222, 222, 205, 205, 205, 205, 205, 217, 217, 217, 217, 217, 217, 128, 128, 128, 128, 128, 235, 235, 235, 235, 235, 237, 237, 237, 237, 237, 237};
                uint8_t blue[64] = {199, 199, 199, 199, 199, 199, 179, 179, 179, 179, 179, 218, 218, 218, 218, 218, 114, 114, 114, 114, 114, 211, 211, 211, 211, 211, 211, 98, 98, 98, 98, 98, 105, 105, 105, 105, 105, 229, 229, 229, 229, 229, 217, 217, 217, 217, 217, 217, 189, 189, 189, 189, 189, 197, 197, 197, 197, 197, 111, 111, 111, 111, 111, 111};
            };
        }
    }
}

ELOQUENT_SINGLETON(Eloquent::Graphics::Colormaps::Set3 set3);