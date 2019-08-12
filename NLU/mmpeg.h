
#ifndef INT64_C 
#define INT64_C(c) (c ## LL) 
#define UINT64_C(c) (c ## ULL) 
#endif 

#ifdef __cplusplus 
extern "C"
{
#endif 
	/*Include ffmpeg header file*/
#include <libavformat/avformat.h> 
#include <libavcodec/avcodec.h> 
#include <libswscale/swscale.h> 

#include <libavutil/imgutils.h>  
#include <libavutil/opt.h>     
#include <libavutil/mathematics.h>   
#include <libavutil/samplefmt.h>

#ifdef __cplusplus 
}
#endif 

class Ffmpeg_Decoder
{
public:
	int imgHeight;//图像高
	int imgWidth;//图像宽
	AVCodec *codec;//解码器
	AVCodecParserContext *parser;//查找器
	AVCodecContext *c = NULL;//解码数据结构对象
	AVPacket *pkt;//包对象
	AVFrame *frame;//帧对象
	struct SwsContext *sws_ctx;//格式转换器
	enum AVPixelFormat src_pix_fmt;//格式对象
	enum AVPixelFormat dst_pix_fmt;//格式对象
	int ret;//返回值对象
	uint8_t *imgdata;//图像数据区
public:
	void Ffmpeg_Decoder_Init(int vWidth, int vHeight)//设置参数,第一个参数为编码器,第二个参数为压缩出来的视频的宽度，第三个视频则为其高度
	{
		//获得图像长宽
		imgWidth = vWidth;
		imgHeight = vHeight;
		//初始化包对象
		pkt = av_packet_alloc();
		if (!pkt)
			exit(1);
		//初始化解码器
		codec = avcodec_find_decoder(AV_CODEC_ID_H264);
		if (!codec) {
			fprintf(stderr, "Codec not found\n");
			exit(1);
		}
		//初始化查找器
		parser = av_parser_init(codec->id);
		if (!parser) {
			fprintf(stderr, "parser not found\n");
			exit(1);
		}
		//初始化解码数据结构
		c = avcodec_alloc_context3(codec);
		if (!c) {
			fprintf(stderr, "Could not allocate video codec context\n");
			exit(1);
		}
		//打开解码器
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			exit(1);
		}
		//初始化帧对象
		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			exit(1);
		}
		//获得图像格式信息
		src_pix_fmt = AV_PIX_FMT_YUV420P;
		dst_pix_fmt = AV_PIX_FMT_BGRA;
		//初始化格式转换器
		sws_ctx = sws_getContext(imgWidth, imgHeight, src_pix_fmt, imgWidth, imgHeight, dst_pix_fmt, SWS_POINT, NULL, NULL, NULL);
	}
	/*
	解码函数
	参数一：待解码数据
	参数二：待解码数据长度
	参数三：解码完成存放队列
	参数四：空闲队列，提供预先开辟内存
	参数三\四的写法有助于减少程序内存拷贝的消耗
	如果是多线程程序，需要做内存防护
	*/
	void Ffmpeg_Decoder_Decode(uint8_t *data, int data_size, list<uint8_t*> &saveimglist, list<uint8_t*>&emptyimglist, CCriticalSection *mCriticalSection)
	{
		while (data_size > 0)
		{
			//查找数据中关键字，按包存放
			ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
				data, data_size, 0, 0, 0);
			if (ret < 0) {
				fprintf(stderr, "Error while parsing\n");
				exit(1);
			}
			data += ret;
			data_size -= ret;
			//找到帧信息，可进行解码
			if (pkt->size)
			{
				//把数据放入解码器中解码
				ret = avcodec_send_packet(c, pkt);

				if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					fprintf(stderr, "Error sending a packet for decoding\n");
					cout << "Error sending a packet for decoding" << endl;
					return;
				}
				//解码出图像
				while (ret >= 0)
				{
					//获取图像
					ret = avcodec_receive_frame(c, frame);
					if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
						break;
					else if (ret < 0)
					{
						fprintf(stderr, "Error during decoding\n");
						cout << "Error during decoding" << endl;
						break;
					}
					//从空闲内存队列中获得内存
					mCriticalSection->Lock();
					if (emptyimglist.size() > 0)
					{
						imgdata = emptyimglist.front();
						emptyimglist.pop_front();
						//图像格式转换，从YUV420到BGR24
						int imgwidthlen = imgWidth * 4;
						sws_scale(sws_ctx, frame->data, frame->linesize, 0, c->height, &imgdata, &imgwidthlen);//像素格式转换
						//将解码完数据放入图像存储队列，等待进一步处理
						saveimglist.push_back(imgdata);
					}
					mCriticalSection->Unlock();
					Sleep(5);
				}
			}
		}
	}
	void Ffmpeg_Decoder_Close()//关闭
	{
		av_parser_close(parser);
		avcodec_free_context(&c);
		av_frame_free(&frame);
		av_packet_free(&pkt);
	}
};
