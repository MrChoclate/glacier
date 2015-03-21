#ifndef _BASE_PNG_H_
#define _BASE_PNG_H_

/* *
 * Rémi Vernay
 * 21 octobre 2012
 */

 #include <vector>
 #include <string>

typedef enum LodePNGColorType {
  LCT_GREY = 0,
  LCT_RGB = 2,
  LCT_PALETTE = 3,
  LCT_GREY_ALPHA = 4,
  LCT_RGBA = 6
} LodePNGColorType;

namespace lodepng {
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const std::string& filename,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
unsigned encode(const std::string& filename,
                const std::vector<unsigned char>& in, unsigned w, unsigned h,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
}

#endif 
