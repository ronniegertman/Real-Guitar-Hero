#include "dsp.h"

class GuitarTab{
    public:
    float note_frequency;
    float LED_INDEX;
};

// GuitarTab guitar_tabs[][] = {
//     { {E2}, {F2}, {F2_SHARP}, {G2}, G2_SHARP, A2, A2_SHARP, B2, C3, C3_SHARP, D3, D3_SHARP, E3 }, // lower E string
//     { A2, A2_SHARP, B2, C3, C3_SHARP, D3, D3_SHARP, E3, F3, F3_SHARP, G3, G3_SHARP, A3 }, // A string
//     { D3, D3_SHARP, E3, F3, F3_SHARP, G3, G3_SHARP, A3, A3_SHARP, B3, C4, C4_SHARP, D4 }, // D string
//     { G3, G3_SHARP, A3, A3_SHARP, B3, C4, C4_SHARP, D4, D4_SHARP, E4, F4, F4_SHARP, G4 }, // G string
//     { B3, C4, C4_SHARP, D4, D4_SHARP, E4, F4, F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4 }, // B string
//     { E4, F4, F4_SHARP, G4, G4_SHARP, A4, A4_SHARP, B4, C5, C5_SHARP, D5, D5_SHARP, E5 }, // high E string
// };