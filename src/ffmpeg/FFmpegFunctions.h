/**********************************************************************

  Audacity: A Digital Audio Editor

  FFmpegFunctions.h

  Dmitry Vedenko

**********************************************************************/

#pragma once

#include <memory>
#include <vector>

#include <wx/string.h>

#include "AVCodecFunctions.h"
#include "AVFormatFunctions.h"
#include "AVUtilFunctions.h"
#include "AVCodecID.h"

#include "wrappers/AVDictionaryWrapper.h"
#include "wrappers/AVIOContextWrapper.h"
#include "wrappers/AVFormatContextWrapper.h"
#include "wrappers/AVStreamWrapper.h"
#include "wrappers/AVPacketWrapper.h"
#include "wrappers/AVFrameWrapper.h"
#include "wrappers/AVInputFormatWrapper.h"
#include "wrappers/AVOutputFormatWrapper.h"
#include "wrappers/AVCodecWrapper.h"
#include "wrappers/AVCodecContextWrapper.h"
#include "wrappers/AVFifoBufferWrapper.h"

class StringSetting;

extern StringSetting AVFormatPath;

class FFmpegFunctions;
template <typename T>
class AVAllocator : public std::allocator<T>
{
public:
   typedef size_t size_type;
   typedef T* pointer;
   typedef const T* const_pointer;

   template <typename _Tp1> struct rebind
   {
      typedef AVAllocator<_Tp1> other;
   };

   pointer allocate(size_type n) noexcept;

   void deallocate(pointer p, size_type ) noexcept;

   AVAllocator();

   AVAllocator(const AVAllocator& a)
       : std::allocator<T>(a)
       , mFFmpeg(a.mFFmpeg)
   {
   }

   template <class U>
   AVAllocator(const AVAllocator<U>& a)
       : std::allocator<T>(a)
       , mFFmpeg(a.mFFmpeg)
   {
   }

private:
   template <class U> friend class AVAllocator;

   std::shared_ptr<FFmpegFunctions> mFFmpeg;
};

template<typename T>
using AVDataBuffer = std::vector<T, AVAllocator<T>>;

struct FFmpegFunctions :
   AVCodecFunctions,
   AVFormatFunctions,
   AVUtilFunctions
{
   FFmpegFunctions();
   ~FFmpegFunctions();

   static std::shared_ptr<FFmpegFunctions> Load();

   AVCodecIDFwd (*GetAVCodecID)(AudacityAVCodecID) = nullptr;
   AudacityAVCodecID (*GetAudacityCodecID)(AVCodecIDFwd) = nullptr;

   static std::vector<wxString> GetSearchPaths();

   std::unique_ptr<AVIOContextWrapper> CreateAVIOContext() const;
   std::unique_ptr<AVFormatContextWrapper> CreateAVFormatContext() const;

   std::unique_ptr<AVStreamWrapper> CreateAVStreamWrapper(AVStream* stream) const;

   //! @post return value is not null
   std::unique_ptr<AVPacketWrapper> CreateAVPacketWrapper() const;

   //! @post return value is not null
   std::unique_ptr<AVFrameWrapper> CreateAVFrameWrapper() const;

   std::unique_ptr<AVInputFormatWrapper> CreateAVInputFormatWrapper(AVInputFormat* inputFormat) const;
   std::unique_ptr<AVOutputFormatWrapper> CreateAVOutputFormatWrapper(AVOutputFormat* outputFormat) const;

   std::unique_ptr<AVCodecWrapper> CreateDecoder(AVCodecIDFwd codecID) const;
   std::unique_ptr<AVCodecWrapper> CreateEncoder(AVCodecIDFwd codecID) const;
   std::unique_ptr<AVCodecWrapper> CreateEncoder(const char* codecName) const;

   std::unique_ptr<AVCodecContextWrapper> CreateAVCodecContextWrapper(AVCodecContext* context) const;
   std::unique_ptr<AVCodecContextWrapper> CreateAVCodecContextWrapperFromCodec(std::unique_ptr<AVCodecWrapper> codec) const;

   std::unique_ptr<AVOutputFormatWrapper> GuessOutputFormat(const char* short_name, const char* filename, const char* mime_type);
   std::unique_ptr<AVOutputFormatWrapper> GetNextOutputFormat(const AVOutputFormatWrapper* fmt) const;

   std::unique_ptr<AVCodecWrapper> GetNextCodec(const AVCodecWrapper* codec) const;

   std::unique_ptr<AVFifoBufferWrapper> CreateFifoBuffer(int size) const;

   template<typename T>
   AVDataBuffer<T> CreateMemoryBuffer(int preallocatedSize) const
   {
      return AVDataBuffer<T>(preallocatedSize, T {}, AVAllocator<T>());
   }

private:
   struct Private;
   std::unique_ptr<Private> mPrivate;
};

template<typename T>
typename AVAllocator<T>::pointer
AVAllocator<T>::allocate(typename AVAllocator<T>::size_type n) noexcept
{
   if (mFFmpeg)
      return static_cast<pointer>(mFFmpeg->av_malloc(n * sizeof(T)));
   else
      return static_cast<pointer>(::malloc(n * sizeof(T)));
}

template<typename T>
void AVAllocator<T>::deallocate(
   typename AVAllocator<T>::pointer p,
   typename AVAllocator<T>::size_type ) noexcept
{
   if (mFFmpeg)
      mFFmpeg->av_free(p);
   else
      ::free(p);
}

template<typename T>
AVAllocator<T>::AVAllocator()
   : mFFmpeg(FFmpegFunctions::Load())
{
}
