#include"rdkafka.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OK 0
#define ERROR -1

/**
 * @brief Message delivery report callback.
 *
 * This callback is called exactly once per message, indicating if
 * the message was succesfully delivered
 * (rkmessage->err == RD_KAFKA_RESP_ERR_NO_ERROR) or permanently
 * failed delivery (rkmessage->err != RD_KAFKA_RESP_ERR_NO_ERROR).
 *
 * The callback is triggered from rd_kafka_poll() and executes on
 * the application's thread.
 */
static void dr_msg_cb (rd_kafka_t *rk,const rd_kafka_message_t *rkmessage, void *opaque) {

    if (rkmessage->err)
        printf( "  WARN  Message delivery failed: %s\n",rd_kafka_err2str(rkmessage->err));
    else
        printf("  INFO  Message delivered (%d bytes, partition %d)\n",rkmessage->len, rkmessage->partition);
        /* The rkmessage is destroyed automatically by librdkafka */
}

int main(int argc,char *argv[]){

	char* errstr;//保存错误信息
	errstr = (char *)malloc(512); 
    if(argc != 2){
        printf("\nusage:producer-pc broker_ip:port\n");
        return 0;
    }
    char* broker_ip = argv[1];
	//初始化生产者配置文件
	rd_kafka_conf_t *rd_kafka_conf = rd_kafka_conf_new();
	if (rd_kafka_conf_set(rd_kafka_conf, "bootstrap.servers", broker_ip,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {//设置服务器地址
        fprintf(stderr, "  ERROR  %s\n", errstr);
		return ERROR;
     }
     if (rd_kafka_conf_set(rd_kafka_conf, "queue.buffering.max.ms", "1",
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "  ERROR  %s\n", errstr);
		return ERROR;
     }
     if (rd_kafka_conf_set(rd_kafka_conf, "queue.buffering.max.messages", "1",
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "  ERROR  %s\n", errstr);
		return ERROR;
     }
     /*
    if (rd_kafka_conf_set(rd_kafka_conf, "compression.codec", "snappy",
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {//设置消息压缩格式
        fprintf(stderr, "  ERROR  %s\n", errstr);
        return;
     }
     */
	rd_kafka_conf_set_dr_msg_cb(rd_kafka_conf, dr_msg_cb);//注册回调函数，负责报告消息发送成功与否

	//初始化生产者
	rd_kafka_t* rd_kafka_producer = rd_kafka_new(RD_KAFKA_PRODUCER, rd_kafka_conf, errstr, sizeof(errstr));
    if (!rd_kafka_producer) {
	 	fprintf(stderr,"  ERROR  Failed to create new producer: %s\n", errstr);
		return ERROR;
    }
	free(errstr);
	rd_kafka_topic_t* rd_kafka_topic = rd_kafka_topic_new(rd_kafka_producer,"hello",NULL);
    //int arr[5]={0,1,0,1,1};
    
	char* str = "Nice to meet you.Use builtin partitioner to select partition.Use builtin partitioner to select partitionUse builtin pNice to meet you.Use builtin partitioner to select partition.Use builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partitionUse builtin partitioner to select partition. Make a copy of the payload.  Make a copy of the payload.  Make a copy of the payload.  Make a copy of the payload.  Make a copy of the payload.  Make a copy of the payload. ";
    printf("%s\n",str);
    printf("%d\n",strlen(str));
    long count = 1;
    char* buf = (char *)malloc(strlen(str) + 5);

retry:
    strncpy(buf, str, count % strlen(str));
    buf[count % strlen(str)] = '\0';

	if (rd_kafka_produce(
                        /* Topic object */
                        rd_kafka_topic,
                        /* Use builtin partitioner to select partition*/
                        RD_KAFKA_PARTITION_UA,
                        /* Make a copy of the payload. */
                        RD_KAFKA_MSG_F_COPY,
                        /* Message payload (value) and length */
                        buf, strlen(buf),
                        /* Optional key and its length */
                        NULL, 0,
                        /* Message opaque, provided in
                         * delivery report callback as
                         * msg_opaque. */
                        NULL) == -1) {
        /**
         * Failed to *enqueue* message for producing.
         */
		printf("  WARN  Failed to produce to topic %s: %s",
				rd_kafka_topic_name(rd_kafka_topic),
				rd_kafka_err2str(rd_kafka_last_error()));

		if(rd_kafka_last_error() == RD_KAFKA_RESP_ERR__QUEUE_FULL){
			rd_kafka_poll(rd_kafka_producer, 1000/*block for max 1000ms*/);
            goto retry;
		}
	}else{
		/**
		 *Success to *enqueue* message for producing.
		 */
		//printf("  INFO  Producer success to enqueue message (%zd byte) for topic %s\n",
			   // strlen(buf), rd_kafka_topic_name(rd_kafka_topic));
        rd_kafka_poll(rd_kafka_producer, -1);
        //rd_kafka_flush(rd_kafka_producer, 5*1000 /* wait for max 5 seconds */);
	}
    printf("count : %ld\n",count++);
    goto retry;
    rd_kafka_topic_destroy(rd_kafka_topic);
    printf("  INFO  Flushing final messages..\n");
    rd_kafka_destroy(rd_kafka_producer);
	return OK;
}
