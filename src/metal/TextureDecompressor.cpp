//
//  TextureDecompressor.cpp
//  Bullet3Common
//
//  Created by nmzik on 19.08.2019.
//

#include "TextureDecompressor.h"
#include "memory"

/*
 ##
 ##  CREDITS https://github.com/bkaradzic/bimg/blob/dab021bc46dabba270123a32cc1c2f8102593e13/src/image.cpp
 ##
 */

namespace Decompressor {
    
    inline uint32_t uint32_sll(uint32_t _a, int32_t _sa)
    {
        return _a << _sa;
    }
    
    inline uint32_t uint32_dec(uint32_t _a)
    {
        return _a - 1;
    }
    
    inline uint32_t uint32_mul(uint32_t _a, uint32_t _b)
    {
        return _a * _b;
    }
    
    inline uint32_t uint32_add(uint32_t _a, uint32_t _b)
    {
        return _a + _b;
    }
    
    inline uint32_t uint32_srl(uint32_t _a, int32_t _sa)
    {
        return _a >> _sa;
    }
    
    uint8_t bitRangeConvert(uint32_t _in, uint32_t _from, uint32_t _to)
    {
        uint32_t tmp0   = uint32_sll(1, _to);
        uint32_t tmp1   = uint32_sll(1, _from);
        uint32_t tmp2   = uint32_dec(tmp0);
        uint32_t tmp3   = uint32_dec(tmp1);
        uint32_t tmp4   = uint32_mul(_in, tmp2);
        uint32_t tmp5   = uint32_add(tmp3, tmp4);
        uint32_t tmp6   = uint32_srl(tmp5, _from);
        uint32_t tmp7   = uint32_add(tmp5, tmp6);
        uint32_t result = uint32_srl(tmp7, _from);
        
        return uint8_t(result);
    }
    
    void decodeBlockDxt(uint8_t _dst[16*4], const uint8_t _src[8])
    {
        uint8_t colors[4*3];
        
        uint32_t c0 = _src[0] | (_src[1] << 8);
        colors[0] = bitRangeConvert( (c0>> 0)&0x1f, 5, 8);
        colors[1] = bitRangeConvert( (c0>> 5)&0x3f, 6, 8);
        colors[2] = bitRangeConvert( (c0>>11)&0x1f, 5, 8);
        
        uint32_t c1 = _src[2] | (_src[3] << 8);
        colors[3] = bitRangeConvert( (c1>> 0)&0x1f, 5, 8);
        colors[4] = bitRangeConvert( (c1>> 5)&0x3f, 6, 8);
        colors[5] = bitRangeConvert( (c1>>11)&0x1f, 5, 8);
        
        colors[6] = (2*colors[0] + colors[3]) / 3;
        colors[7] = (2*colors[1] + colors[4]) / 3;
        colors[8] = (2*colors[2] + colors[5]) / 3;
        
        colors[ 9] = (colors[0] + 2*colors[3]) / 3;
        colors[10] = (colors[1] + 2*colors[4]) / 3;
        colors[11] = (colors[2] + 2*colors[5]) / 3;
        
        for (uint32_t ii = 0, next = 8*4; ii < 16*4; ii += 4, next += 2)
        {
            int idx = ( (_src[next>>3] >> (next & 7) ) & 3) * 3;
            _dst[ii+0] = colors[idx+0];
            _dst[ii+1] = colors[idx+1];
            _dst[ii+2] = colors[idx+2];
        }
    }
    
    void decodeBlockDxt1(uint8_t _dst[16*4], const uint8_t _src[8])
    {
        uint8_t colors[4*4];
        
        uint32_t c0 = _src[0] | (_src[1] << 8);
        colors[0] = bitRangeConvert( (c0>> 0)&0x1f, 5, 8);
        colors[1] = bitRangeConvert( (c0>> 5)&0x3f, 6, 8);
        colors[2] = bitRangeConvert( (c0>>11)&0x1f, 5, 8);
        colors[3] = 255;
        
        uint32_t c1 = _src[2] | (_src[3] << 8);
        colors[4] = bitRangeConvert( (c1>> 0)&0x1f, 5, 8);
        colors[5] = bitRangeConvert( (c1>> 5)&0x3f, 6, 8);
        colors[6] = bitRangeConvert( (c1>>11)&0x1f, 5, 8);
        colors[7] = 255;
        
        if (c0 > c1)
        {
            colors[ 8] = (2*colors[0] + colors[4]) / 3;
            colors[ 9] = (2*colors[1] + colors[5]) / 3;
            colors[10] = (2*colors[2] + colors[6]) / 3;
            colors[11] = 255;
            
            colors[12] = (colors[0] + 2*colors[4]) / 3;
            colors[13] = (colors[1] + 2*colors[5]) / 3;
            colors[14] = (colors[2] + 2*colors[6]) / 3;
            colors[15] = 255;
        }
        else
        {
            colors[ 8] = (colors[0] + colors[4]) / 2;
            colors[ 9] = (colors[1] + colors[5]) / 2;
            colors[10] = (colors[2] + colors[6]) / 2;
            colors[11] = 255;
            
            colors[12] = 0;
            colors[13] = 0;
            colors[14] = 0;
            colors[15] = 0;
        }
        
        for (uint32_t ii = 0, next = 8*4; ii < 16*4; ii += 4, next += 2)
        {
            int idx = ( (_src[next>>3] >> (next & 7) ) & 3) * 4;
            _dst[ii+0] = colors[idx+0];
            _dst[ii+1] = colors[idx+1];
            _dst[ii+2] = colors[idx+2];
            _dst[ii+3] = colors[idx+3];
        }
    }
    
    void decodeBlockDxt23A(uint8_t _dst[16*4], const uint8_t _src[8])
    {
        for (uint32_t ii = 0, next = 0; ii < 16*4; ii += 4, next += 4)
        {
            uint32_t c0 = (_src[next>>3] >> (next&7) ) & 0xf;
            _dst[ii] = bitRangeConvert(c0, 4, 8);
        }
    }
    
    void decodeBlockDxt45A(uint8_t _dst[16*4], const uint8_t _src[8])
    {
        uint8_t alpha[8];
        alpha[0] = _src[0];
        alpha[1] = _src[1];
        
        if (alpha[0] > alpha[1])
        {
            alpha[2] = (6*alpha[0] + 1*alpha[1]) / 7;
            alpha[3] = (5*alpha[0] + 2*alpha[1]) / 7;
            alpha[4] = (4*alpha[0] + 3*alpha[1]) / 7;
            alpha[5] = (3*alpha[0] + 4*alpha[1]) / 7;
            alpha[6] = (2*alpha[0] + 5*alpha[1]) / 7;
            alpha[7] = (1*alpha[0] + 6*alpha[1]) / 7;
        }
        else
        {
            alpha[2] = (4*alpha[0] + 1*alpha[1]) / 5;
            alpha[3] = (3*alpha[0] + 2*alpha[1]) / 5;
            alpha[4] = (2*alpha[0] + 3*alpha[1]) / 5;
            alpha[5] = (1*alpha[0] + 4*alpha[1]) / 5;
            alpha[6] = 0;
            alpha[7] = 255;
        }
        
        uint32_t idx0 = _src[2];
        uint32_t idx1 = _src[5];
        idx0 |= uint32_t(_src[3])<<8;
        idx1 |= uint32_t(_src[6])<<8;
        idx0 |= uint32_t(_src[4])<<16;
        idx1 |= uint32_t(_src[7])<<16;
        for (uint32_t ii = 0; ii < 8*4; ii += 4)
        {
            _dst[ii]    = alpha[idx0&7];
            _dst[ii+32] = alpha[idx1&7];
            idx0 >>= 3;
            idx1 >>= 3;
        }
    }
    
    /*void decodeBlockBc7(uint8_t _dst[16*4], const uint8_t _src[16])
     {
     BitReader bit(_src);
     
     uint8_t mode = 0;
     for (; mode < 8 && 0 == bit.read(1); ++mode)
     {
     }
     
     if (mode == 8)
     {
     bx::memSet(_dst, 0, 16*4);
     return;
     }
     
     const Bc7ModeInfo& mi  = s_bp7ModeInfo[mode];
     const uint8_t modePBits = 0 != mi.endpointPBits
     ? mi.endpointPBits
     : mi.sharedPBits
     ;
     
     const uint8_t partitionSetIdx    = uint8_t(bit.read(mi.partitionBits) );
     const uint8_t rotationMode       = uint8_t(bit.read(mi.rotationBits) );
     const uint8_t indexSelectionMode = uint8_t(bit.read(mi.indexSelectionBits) );
     
     uint8_t epR[6];
     uint8_t epG[6];
     uint8_t epB[6];
     uint8_t epA[6];
     
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epR[ii*2+0] = uint8_t(bit.read(mi.colorBits) << modePBits);
     epR[ii*2+1] = uint8_t(bit.read(mi.colorBits) << modePBits);
     }
     
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epG[ii*2+0] = uint8_t(bit.read(mi.colorBits) << modePBits);
     epG[ii*2+1] = uint8_t(bit.read(mi.colorBits) << modePBits);
     }
     
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epB[ii*2+0] = uint8_t(bit.read(mi.colorBits) << modePBits);
     epB[ii*2+1] = uint8_t(bit.read(mi.colorBits) << modePBits);
     }
     
     if (mi.alphaBits)
     {
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epA[ii*2+0] = uint8_t(bit.read(mi.alphaBits) << modePBits);
     epA[ii*2+1] = uint8_t(bit.read(mi.alphaBits) << modePBits);
     }
     }
     else
     {
     bx::memSet(epA, 0xff, 6);
     }
     
     if (0 != modePBits)
     {
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     const uint8_t pda = uint8_t(                      bit.read(modePBits)      );
     const uint8_t pdb = uint8_t(0 == mi.sharedPBits ? bit.read(modePBits) : pda);
     
     epR[ii*2+0] |= pda;
     epR[ii*2+1] |= pdb;
     epG[ii*2+0] |= pda;
     epG[ii*2+1] |= pdb;
     epB[ii*2+0] |= pda;
     epB[ii*2+1] |= pdb;
     epA[ii*2+0] |= pda;
     epA[ii*2+1] |= pdb;
     }
     }
     
     const uint8_t colorBits = mi.colorBits + modePBits;
     
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epR[ii*2+0] = bitRangeConvert(epR[ii*2+0], colorBits, 8);
     epR[ii*2+1] = bitRangeConvert(epR[ii*2+1], colorBits, 8);
     epG[ii*2+0] = bitRangeConvert(epG[ii*2+0], colorBits, 8);
     epG[ii*2+1] = bitRangeConvert(epG[ii*2+1], colorBits, 8);
     epB[ii*2+0] = bitRangeConvert(epB[ii*2+0], colorBits, 8);
     epB[ii*2+1] = bitRangeConvert(epB[ii*2+1], colorBits, 8);
     }
     
     if (mi.alphaBits)
     {
     const uint8_t alphaBits = mi.alphaBits + modePBits;
     
     for (uint8_t ii = 0; ii < mi.numSubsets; ++ii)
     {
     epA[ii*2+0] = bitRangeConvert(epA[ii*2+0], alphaBits, 8);
     epA[ii*2+1] = bitRangeConvert(epA[ii*2+1], alphaBits, 8);
     }
     }
     
     const bool hasIndexBits1 = 0 != mi.indexBits[1];
     
     const uint8_t* factors[] =
     {
     s_bptcFactors[mi.indexBits[0]-2],
     hasIndexBits1 ? s_bptcFactors[mi.indexBits[1]-2] : factors[0],
     };
     
     uint16_t offset[2] =
     {
     0,
     uint16_t(mi.numSubsets*(16*mi.indexBits[0]-1) ),
     };
     
     for (uint8_t yy = 0; yy < 4; ++yy)
     {
     for (uint8_t xx = 0; xx < 4; ++xx)
     {
     const uint8_t idx = yy*4+xx;
     
     uint8_t subsetIndex = 0;
     uint8_t indexAnchor = 0;
     switch (mi.numSubsets)
     {
     case 2:
     subsetIndex = (s_bptcP2[partitionSetIdx] >> idx) & 1;
     indexAnchor = 0 != subsetIndex ? s_bptcA2[partitionSetIdx] : 0;
     break;
     
     case 3:
     subsetIndex = (s_bptcP3[partitionSetIdx] >> (2*idx) ) & 3;
     indexAnchor = 0 != subsetIndex ? s_bptcA3[subsetIndex-1][partitionSetIdx] : 0;
     break;
     
     default:
     break;
     }
     
     const uint8_t anchor = idx == indexAnchor;
     const uint8_t num[2] =
     {
     uint8_t(                mi.indexBits[0] - anchor    ),
     uint8_t(hasIndexBits1 ? mi.indexBits[1] - anchor : 0),
     };
     
     const uint8_t index[2] =
     {
     (uint8_t)bit.peek(offset[0], num[0]),
     hasIndexBits1 ? (uint8_t)bit.peek(offset[1], num[1]) : index[0],
     };
     
     offset[0] += num[0];
     offset[1] += num[1];
     
     const uint8_t fc = factors[ indexSelectionMode][index[ indexSelectionMode] ];
     const uint8_t fa = factors[!indexSelectionMode][index[!indexSelectionMode] ];
     
     const uint8_t fca = 64 - fc;
     const uint8_t fcb = fc;
     const uint8_t faa = 64 - fa;
     const uint8_t fab = fa;
     
     subsetIndex *= 2;
     uint8_t rr = uint8_t(uint16_t(epR[subsetIndex]*fca + epR[subsetIndex + 1]*fcb + 32) >> 6);
     uint8_t gg = uint8_t(uint16_t(epG[subsetIndex]*fca + epG[subsetIndex + 1]*fcb + 32) >> 6);
     uint8_t bb = uint8_t(uint16_t(epB[subsetIndex]*fca + epB[subsetIndex + 1]*fcb + 32) >> 6);
     uint8_t aa = uint8_t(uint16_t(epA[subsetIndex]*faa + epA[subsetIndex + 1]*fab + 32) >> 6);
     
     switch (rotationMode)
     {
     case 1: bx::swap(aa, rr); break;
     case 2: bx::swap(aa, gg); break;
     case 3: bx::swap(aa, bb); break;
     default:                  break;
     };
     
     uint8_t* bgra = &_dst[idx*4];
     bgra[0] = bb;
     bgra[1] = gg;
     bgra[2] = rr;
     bgra[3] = aa;
     }
     }
     }*/
    
    void imageDecodeToBgra8(void* _dst, const void* _src, uint32_t _width, uint32_t _height, uint32_t _dstPitch, TextureFormat format)
    {
        const uint8_t* src = (const uint8_t*)_src;
        uint8_t* dst = (uint8_t*)_dst;
        
        uint32_t width  = _width/4;
        uint32_t height = _height/4;
        
        uint8_t temp[16*4];
        
        switch (format)
        {
            case TextureFormat::D3DFMT_DXT1:
                for (uint32_t yy = 0; yy < height; ++yy)
                {
                    for (uint32_t xx = 0; xx < width; ++xx)
                    {
                        decodeBlockDxt1(temp, src);
                        src += 8;
                        
                        uint8_t* block = &dst[yy*_dstPitch*4 + xx*16];
                        memcpy(&block[0*_dstPitch], &temp[ 0], 16);
                        memcpy(&block[1*_dstPitch], &temp[16], 16);
                        memcpy(&block[2*_dstPitch], &temp[32], 16);
                        memcpy(&block[3*_dstPitch], &temp[48], 16);
                    }
                }
                break;
                
            case TextureFormat::D3DFMT_DXT3:
                for (uint32_t yy = 0; yy < height; ++yy)
                {
                    for (uint32_t xx = 0; xx < width; ++xx)
                    {
                        decodeBlockDxt23A(temp+3, src);
                        src += 8;
                        decodeBlockDxt(temp, src);
                        src += 8;
                        
                        uint8_t* block = &dst[yy*_dstPitch*4 + xx*16];
                        memcpy(&block[0*_dstPitch], &temp[ 0], 16);
                        memcpy(&block[1*_dstPitch], &temp[16], 16);
                        memcpy(&block[2*_dstPitch], &temp[32], 16);
                        memcpy(&block[3*_dstPitch], &temp[48], 16);
                    }
                }
                break;
                
            case TextureFormat::D3DFMT_DXT5:
                for (uint32_t yy = 0; yy < height; ++yy)
                {
                    for (uint32_t xx = 0; xx < width; ++xx)
                    {
                        decodeBlockDxt45A(temp+3, src);
                        src += 8;
                        decodeBlockDxt(temp, src);
                        src += 8;
                        
                        uint8_t* block = &dst[yy*_dstPitch*4 + xx*16];
                        memcpy(&block[0*_dstPitch], &temp[ 0], 16);
                        memcpy(&block[1*_dstPitch], &temp[16], 16);
                        memcpy(&block[2*_dstPitch], &temp[32], 16);
                        memcpy(&block[3*_dstPitch], &temp[48], 16);
                    }
                }
                break;
        }
        /*case TextureFormat::BC4:
         for (uint32_t yy = 0; yy < height; ++yy)
         {
         for (uint32_t xx = 0; xx < width; ++xx)
         {
         decodeBlockDxt45A(temp, src);
         src += 8;
         
         uint8_t* block = &dst[yy*_dstPitch*4 + xx*16];
         memcpy(&block[0*_dstPitch], &temp[ 0], 16);
         memcpy(&block[1*_dstPitch], &temp[16], 16);
         memcpy(&block[2*_dstPitch], &temp[32], 16);
         memcpy(&block[3*_dstPitch], &temp[48], 16);
         }
         }
         break;*/
        
        /*case TextureFormat::BC7:
         for (uint32_t yy = 0; yy < height; ++yy)
         {
         for (uint32_t xx = 0; xx < width; ++xx)
         {
         decodeBlockBc7(temp, src);
         src += 16;
         
         uint8_t* block = &dst[yy*_dstPitch*4 + xx*16];
         memcpy(&block[0*_dstPitch], &temp[ 0], 16);
         memcpy(&block[1*_dstPitch], &temp[16], 16);
         memcpy(&block[2*_dstPitch], &temp[32], 16);
         memcpy(&block[3*_dstPitch], &temp[48], 16);
         }
         }
         break;*/
        
    }
};
