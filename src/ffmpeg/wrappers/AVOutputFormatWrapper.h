/**********************************************************************

  Audacity: A Digital Audio Editor

  AVOutputFormatWrapper.h

  Dmitry Vedenko

**********************************************************************/

#pragma once

#include "../FFmpegTypes.h"

struct FFmpegFunctions;
typedef struct AVOutputFormat AVOutputFormat;

class AVOutputFormatWrapper
{
public:
   AVOutputFormatWrapper(const AVOutputFormatWrapper&) = delete;
   AVOutputFormatWrapper& operator=(AVOutputFormatWrapper&) = delete;

   AVOutputFormatWrapper(AVOutputFormatWrapper&&) = delete;
   AVOutputFormatWrapper& operator=(AVOutputFormatWrapper&&) = delete;

   explicit AVOutputFormatWrapper(AVOutputFormat* wrapped) noexcept;

   AVOutputFormat* GetWrappedValue() noexcept;
   const AVOutputFormat* GetWrappedValue() const noexcept;

   //! This class is move-only, although it doesn't manage a resource
   virtual ~AVOutputFormatWrapper() = default;

   virtual const char* GetName() const noexcept = 0;
   virtual const char* GetLongName() const noexcept = 0;
   virtual const char* GetMimeType() const noexcept = 0;
   virtual const char* GetExtensions() const noexcept = 0;
   virtual AVCodecIDFwd GetAudioCodec() const noexcept = 0;
   virtual int GetFlags() const noexcept = 0;
   virtual const struct AVCodecTag* const* GetCodecTag() const noexcept = 0;
protected:
   AVOutputFormat* mAVOutputFormat { nullptr };
};
