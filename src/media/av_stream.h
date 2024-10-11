#ifndef MEDIA_AV_STREAM_H_
#define MEDIA_AV_STREAM_H_

#include "../core/core.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
}

MYLIB_BEGIN

class AVStream {
public:
  AVStream();
  ~AVStream();

  int open(String url);
  void close();
  bool is_open() const;
  void read();

private:
  ::AVStream *m_stream;
  AVCodecContext *m_codec_ctx;
  ::AVFormatContext *m_fmt_ctx;
};

MYLIB_END

#endif// !MEDIA_AV_STREAM_H_
