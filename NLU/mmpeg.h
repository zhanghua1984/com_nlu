
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
	int imgHeight;//ͼ���
	int imgWidth;//ͼ���
	AVCodec *codec;//������
	AVCodecParserContext *parser;//������
	AVCodecContext *c = NULL;//�������ݽṹ����
	AVPacket *pkt;//������
	AVFrame *frame;//֡����
	struct SwsContext *sws_ctx;//��ʽת����
	enum AVPixelFormat src_pix_fmt;//��ʽ����
	enum AVPixelFormat dst_pix_fmt;//��ʽ����
	int ret;//����ֵ����
	uint8_t *imgdata;//ͼ��������
public:
	void Ffmpeg_Decoder_Init(int vWidth, int vHeight)//���ò���,��һ������Ϊ������,�ڶ�������Ϊѹ����������Ƶ�Ŀ�ȣ���������Ƶ��Ϊ��߶�
	{
		//���ͼ�񳤿�
		imgWidth = vWidth;
		imgHeight = vHeight;
		//��ʼ��������
		pkt = av_packet_alloc();
		if (!pkt)
			exit(1);
		//��ʼ��������
		codec = avcodec_find_decoder(AV_CODEC_ID_H264);
		if (!codec) {
			fprintf(stderr, "Codec not found\n");
			exit(1);
		}
		//��ʼ��������
		parser = av_parser_init(codec->id);
		if (!parser) {
			fprintf(stderr, "parser not found\n");
			exit(1);
		}
		//��ʼ���������ݽṹ
		c = avcodec_alloc_context3(codec);
		if (!c) {
			fprintf(stderr, "Could not allocate video codec context\n");
			exit(1);
		}
		//�򿪽�����
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			exit(1);
		}
		//��ʼ��֡����
		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			exit(1);
		}
		//���ͼ���ʽ��Ϣ
		src_pix_fmt = AV_PIX_FMT_YUV420P;
		dst_pix_fmt = AV_PIX_FMT_BGRA;
		//��ʼ����ʽת����
		sws_ctx = sws_getContext(imgWidth, imgHeight, src_pix_fmt, imgWidth, imgHeight, dst_pix_fmt, SWS_POINT, NULL, NULL, NULL);
	}
	/*
	���뺯��
	����һ������������
	�����������������ݳ���
	��������������ɴ�Ŷ���
	�����ģ����ж��У��ṩԤ�ȿ����ڴ�
	������\�ĵ�д�������ڼ��ٳ����ڴ濽��������
	����Ƕ��̳߳�����Ҫ���ڴ����
	*/
	void Ffmpeg_Decoder_Decode(uint8_t *data, int data_size, list<uint8_t*> &saveimglist, list<uint8_t*>&emptyimglist, CCriticalSection *mCriticalSection)
	{
		while (data_size > 0)
		{
			//���������йؼ��֣��������
			ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
				data, data_size, 0, 0, 0);
			if (ret < 0) {
				fprintf(stderr, "Error while parsing\n");
				exit(1);
			}
			data += ret;
			data_size -= ret;
			//�ҵ�֡��Ϣ���ɽ��н���
			if (pkt->size)
			{
				//�����ݷ���������н���
				ret = avcodec_send_packet(c, pkt);

				if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					fprintf(stderr, "Error sending a packet for decoding\n");
					cout << "Error sending a packet for decoding" << endl;
					return;
				}
				//�����ͼ��
				while (ret >= 0)
				{
					//��ȡͼ��
					ret = avcodec_receive_frame(c, frame);
					if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
						break;
					else if (ret < 0)
					{
						fprintf(stderr, "Error during decoding\n");
						cout << "Error during decoding" << endl;
						break;
					}
					//�ӿ����ڴ�����л���ڴ�
					mCriticalSection->Lock();
					if (emptyimglist.size() > 0)
					{
						imgdata = emptyimglist.front();
						emptyimglist.pop_front();
						//ͼ���ʽת������YUV420��BGR24
						int imgwidthlen = imgWidth * 4;
						sws_scale(sws_ctx, frame->data, frame->linesize, 0, c->height, &imgdata, &imgwidthlen);//���ظ�ʽת��
						//�����������ݷ���ͼ��洢���У��ȴ���һ������
						saveimglist.push_back(imgdata);
					}
					mCriticalSection->Unlock();
					Sleep(5);
				}
			}
		}
	}
	void Ffmpeg_Decoder_Close()//�ر�
	{
		av_parser_close(parser);
		avcodec_free_context(&c);
		av_frame_free(&frame);
		av_packet_free(&pkt);
	}
};
