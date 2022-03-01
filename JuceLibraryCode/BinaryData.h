/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Cosine_Wave_png;
    const int            Cosine_Wave_pngSize = 5628;

    extern const char*   play_png;
    const int            play_pngSize = 4111;

    extern const char*   Sine_Wave_png;
    const int            Sine_Wave_pngSize = 5895;

    extern const char*   Slider_png;
    const int            Slider_pngSize = 4296;

    extern const char*   stop_png;
    const int            stop_pngSize = 2542;

    extern const char*   White_noise_png;
    const int            White_noise_pngSize = 13634;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
