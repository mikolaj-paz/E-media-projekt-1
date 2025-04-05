#include <png.hpp>

#ifndef PNG_CHUNK_PRESENCE
    #define PNG_CHUNK_PRESENCE
    #define UNKNOWN_MASK 0b0000'0000'0000'0001
    #define tRNS_MASK 0b0000'0000'0000'0010
    #define gAMA_MASK 0b0000'0000'0000'0100
    #define cHRM_MASK 0b0000'0000'0000'1000
    #define sRGB_MASK 0b0000'0000'0001'0000
    #define iCCP_MASK 0b0000'0000'0010'0000
    #define tEXt_MASK 0b0000'0000'0100'0000
    #define zTXt_MASK 0b0000'0000'1000'0000
    #define iTXt_MASK 0b0000'0001'0000'0000
    #define bKGD_MASK 0b0000'0010'0000'0000
    #define pHYs_MASK 0b0000'0100'0000'0000
    #define sBIT_MASK 0b0000'1000'0000'0000
    #define sPLT_MASK 0b0001'0000'0000'0000
    #define hIST_MASK 0b0010'0000'0000'0000
    #define tIME_MASK 0b0100'0000'0000'0000
    #define eXIf_MASK 0b1000'0000'0000'0000
#endif

void PNGfile::anonymize() {
    int presence = 0;

    for (auto& chunk : ancillaryChunks) {
        std::string chunkType = chunk->type;
        if (chunkType == "tRNS") presence |= tRNS_MASK;
        else if (chunkType == "gAMA") presence |= gAMA_MASK;
        else if (chunkType == "cHRM") presence |= cHRM_MASK;
        else if (chunkType == "sRGB") presence |= sRGB_MASK;
        else if (chunkType == "iCCP") presence |= iCCP_MASK;
        else if (chunkType == "tEXt") presence |= tEXt_MASK;
        else if (chunkType == "zTXt") presence |= zTXt_MASK;
        else if (chunkType == "iTXt") presence |= iTXt_MASK;
        else if (chunkType == "bKGD") presence |= bKGD_MASK;
        else if (chunkType == "pHYs") presence |= pHYs_MASK;
        else if (chunkType == "sBIT") presence |= sBIT_MASK;
        else if (chunkType == "sPLT") presence |= sPLT_MASK;
        else if (chunkType == "hIST") presence |= hIST_MASK;
        else if (chunkType == "tIME") presence |= tIME_MASK;
        else if (chunkType == "eXIf") presence |= eXIf_MASK;
        else presence |= UNKNOWN_MASK;
    }

    for (std::vector<base_chunk*>::iterator it = ancillaryChunks.begin(); it != ancillaryChunks.end(); ++it) {

        // "tRNS is prohibited for color types 4 and 6, since a full alpha channel is already present in those cases."
        if ((*it)->type == "tRNS" && (header->colorType == 4 || header->colorType == 6))
            ancillaryChunks.erase(it);
        
        // "An sRGB chunk or iCCP chunk (...) overrides the gAMA chunk."
        else if ((*it)->type == "gAMA" && ((presence & sRGB_MASK) || (presence & iCCP_MASK)))
            ancillaryChunks.erase(it);

        // "An sRGB chunk or iCCP chunk (...) overrides the cHRM chunk."
        else if ((*it)->type == "cHRM" && ((presence & sRGB_MASK) || (presence & iCCP_MASK)))
            ancillaryChunks.erase(it);

        // "The sRGB and iCCP chunks should not both appear."
        else if ((*it)->type == "sRGB" && (presence & iCCP_MASK))
            ancillaryChunks.erase(it);

        else if ((*it)->type == "bKGD")
            ancillaryChunks.erase(it);

        else if ((*it)->type == "sBIT")
            ancillaryChunks.erase(it);

        else if ((*it)->type == "hIST")
            ancillaryChunks.erase(it);
    }

    switch (header->colorType) {
        case 3: // to do, mandatory
            break;
        case 2:
        case 6:
            if (palette) {
                delete palette;
                palette = nullptr;
            }
            break;

        default:
            break;
    }
}