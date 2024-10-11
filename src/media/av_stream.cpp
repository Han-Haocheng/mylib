#include "av_stream.h"

MYLIB_BEGIN

AVStream::AVStream() {
}

AVStream::~AVStream() {
  if (is_open()) {
    close();
  }
}

int AVStream::open(String url) {
  if (avformat_open_input(&m_fmt_ctx, url.c_str(), NULL, NULL) < 0) {
    fprintf(stderr, "Could not open source file %s\n", url.c_str());
    return false;// 打开失败
  }

  if (avformat_find_stream_info(m_fmt_ctx, NULL) < 0) {
    fprintf(stderr, "Could not find stream information\n");
    avformat_close_input(&m_fmt_ctx);// 关闭输入
    return false;                    // 查找流信息失败
  }

  int32 m_stream_index = -1;
  for (int32 i = 0; i < m_fmt_ctx->nb_streams; i++) {
    m_stream = m_fmt_ctx->streams[i];
    AVCodecParameters *codec_parm = m_stream->codecpar;
    const ::AVCodec *m_codec = avcodec_find_decoder(codec_parm->codec_id);
    if (m_codec == NULL) {
      continue;
    }

    m_stream_index = i;
    m_codec_ctx = avcodec_alloc_context3(m_codec);

    if (m_codec_ctx == NULL) {
      fprintf(stderr, "Failed to allocate the decoder context\n");
      avformat_close_input(&m_fmt_ctx);// 关闭输入
      return AVERROR(ENOMEM);
    }

    if (avcodec_parameters_to_context(m_codec_ctx, codec_parm) < 0) {
      fprintf(stderr, "Failed to copy codec parameters to codec context\n");
      avcodec_free_context(&m_codec_ctx);// 释放解码器上下文
      avformat_close_input(&m_fmt_ctx);  // 关闭输入
      return false;
    }

    if (avcodec_open2(m_codec_ctx, m_codec, NULL) < 0) {
      fprintf(stderr, "Failed to open codec\n");
      avcodec_free_context(&m_codec_ctx);// 释放解码器上下文
      avformat_close_input(&m_fmt_ctx);  // 关闭输入
      return false;
    }
    break;
  }

  if (m_stream_index == -1) {
    fprintf(stderr, "Could not find a stream with a decoder\n");
    avformat_close_input(&m_fmt_ctx);// 关闭输入
    return AVERROR(EINVAL);
  }

  return true;
}

void AVStream::close() {
  avcodec_free_context(&m_codec_ctx);// 释放解码器上下文
  avformat_close_input(&m_fmt_ctx);
}

bool AVStream::is_open() const {
  return false;
}

void AVStream::read() {
}

MYLIB_END
