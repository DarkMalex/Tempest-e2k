#pragma once

#include <Tempest/AbstractGraphicsApi>
#include <Metal/MTLTexture.h>

#include "utility/spinlock.h"

namespace Tempest {
namespace Detail {

class MtDevice;

class MtTexture : public Tempest::AbstractGraphicsApi::Texture {
  public:
    MtTexture(MtDevice &d,
              const uint32_t w, const uint32_t h, uint32_t mips, TextureFormat frm, bool storageTex);
    MtTexture(MtDevice &d, const Pixmap& pm, uint32_t mips, TextureFormat frm);
    ~MtTexture();

    uint32_t mipCount() const override;
    void     readPixels(Pixmap& out, TextureFormat frm,
                        const uint32_t w, const uint32_t h, uint32_t mip);

    uint32_t bitCount() const;

    id<MTLTexture>  view(ComponentMapping m, uint32_t mipLevel);

    MtDevice&       dev;
    id<MTLTexture>  impl;
    const uint32_t  mipCnt = 0;

  private:
    void createCompressedTexture(id<MTLTexture> val, const Pixmap& p, TextureFormat frm, uint32_t mipCnt);
    void createRegularTexture(id<MTLTexture> val, const Pixmap& p);

    id<MTLTexture> alloc(TextureFormat frm, const uint32_t w, const uint32_t h, const uint32 mips,
                         MTLStorageMode smode, MTLTextureUsage umode);

    struct View {
      ComponentMapping m;
      uint32_t         mip = uint32_t(0);
      id<MTLTexture>   v;
      };
    Detail::SpinLock  syncViews;
    std::vector<View> extViews;
  };

}
}
