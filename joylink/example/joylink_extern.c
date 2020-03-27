/* --------------------------------------------------
 * @brief: 
 *
 * @version: 1.0
 *
 * @date: 08/01/2018
 *
 * --------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "joylink.h"
#include "joylink_packets.h"
#include "joylink_json.h"
#include "joylink_extern_json.h"
#include "joylink_auth_crc.h"

#include <fcntl.h> 

#include "joylink_extern.h"
#include "joylink_extern_ota.h"

JLPInfo_t user_jlp = 
{
	.version = JLP_VERSION,
	.uuid = JLP_UUID,

	.devtype = JLP_DEV_TYPE,
	.lancon = JLP_LAN_CTRL,
	.cmd_tran_type = JLP_CMD_TYPE,

	.joySdkVersion = _VERSION_,
	.noSnapshot = JLP_SNAPSHOT,
};
jl2_d_idt_t user_idt = 
{
	.type = 0,
	.cloud_pub_key = IDT_CLOUD_PUB_KEY,

	.sig = "01234567890123456789012345678901",
	.pub_key = "01234567890123456789012345678901",

	.f_sig = "01234567890123456789012345678901",
	.f_pub_key = "01234567890123456789012345678901",
};

user_dev_status_t user_dev;

/*E_JLDEV_TYPE_GW*/
#ifdef _SAVE_FILE_
char  *file = "joylink_info.txt";
#endif

extern int joylink_parse_jlp(JLPInfo_t *jlp, char * pMsg);
extern int joylink_util_cut_ip_port(const char *ipport, char *out_ip, int *out_port);

/**
 * brief: 
 *
 * @Param:
 *
 * @Returns: 
 */
int
joylink_dev_get_random()
{
    /**
     *FIXME:must to do
     */
    return random();
}

/**
 * brief: 
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_is_net_ok()
{
    /**
     *FIXME:must to do
     */
    return E_RET_TRUE;
}

/**
 * brief: 
 *
 * @Param: st
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_set_connect_st(int st)
{
	/**
	*FIXME:must to do
	*/
	char buff[64] = {0};
	int ret = 0;

	sprintf(buff, "{\"conn_status\":\"%d\"}", st);
	log_info("--set_connect_st:%s\n", buff);

	return ret;
}

/**
 * brief: 
 *
 * @Param: jlp
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_set_attr_jlp(JLPInfo_t *jlp)
{
	if(NULL == jlp){
		return E_RET_ERROR;
	}
	/**
	*FIXME:must to do
	*Must save jlp info to flash 
	*/
	int ret = E_RET_ERROR;

	memcpy(&user_jlp, jlp, sizeof(JLPInfo_t));
#ifdef _SAVE_FILE_
	FILE *outfile;
	outfile = fopen(file, "wb+" );
	fwrite(&user_jlp, sizeof(JLPInfo_t), 1, outfile );
	fclose(outfile);
#endif
	ret = E_RET_OK;

	return ret;
}

/**
 * brief: 
 *
 * @Param: jlp
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_get_idt(jl2_d_idt_t *pidt)
{
	if(NULL == pidt){
		return E_RET_ERROR; 
	}
	pidt->type = 0;
	/**
	*FIXME:must to do
	*/
	strcpy(pidt->sig, user_idt.sig);
	strcpy(pidt->pub_key, user_idt.pub_key);
	//strcpy(pidt->rand, user_idt.rand);
	strcpy(pidt->f_sig, user_idt.f_sig);
	strcpy(pidt->f_pub_key, user_idt.f_pub_key);
	strcpy(pidt->cloud_pub_key, user_idt.cloud_pub_key);

	return E_RET_OK;
}

/**
 * brief: 
 *
 * @Returns: 
 */
int
joylink_dev_get_user_mac(char *out)
{
	/**
	*FIXME:must to do
	*/
	return -1;
}

/**
 * brief: 
 *
 * @Returns: 
 */
int
joylink_dev_get_private_key(char *out)
{
	/**
	*FIXME:must to do
	*/
	return -1;
}

/**
 * brief: 
 *
 * @Param: jlp
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_get_jlp_info(JLPInfo_t *jlp)
{
	if(NULL == jlp){
		return -1;
	}
	/**
	*FIXME:must to do
 	*Must get jlp info from flash 
	*/
	int ret = E_RET_OK;

	JLPInfo_t fjlp;
	memset(&fjlp, 0, sizeof(JLPInfo_t));

#ifdef _SAVE_FILE_
	FILE *infile;
	infile = fopen(file, "rb+");
	if(infile > 0)
	{
		fread(&fjlp, sizeof(fjlp), 1, infile);
		fclose(infile);

		strcpy(user_jlp.feedid, fjlp.feedid);
		strcpy(user_jlp.accesskey, fjlp.accesskey);
		strcpy(user_jlp.localkey, fjlp.localkey);
		strcpy(user_jlp.joylink_server, fjlp.joylink_server);
		user_jlp.server_port = fjlp.server_port;
	}
#endif
	strcpy(jlp->feedid, user_jlp.feedid);
	strcpy(jlp->accesskey, user_jlp.accesskey);
	strcpy(jlp->localkey, user_jlp.localkey);
	strcpy(jlp->joylink_server, user_jlp.joylink_server);
	jlp->server_port = user_jlp.server_port;
	if(strlen(jlp->feedid) > 0){
		_g_pdev->jlp.is_actived = 1;
	}
	/**
	*MUST TODO
	*This jlp->mac must return the device real mac.
	*/
	if(joylink_dev_get_user_mac(jlp->mac) < 0){
		strcpy(jlp->mac, user_jlp.mac);
	}

	if(joylink_dev_get_private_key(jlp->prikey) < 0){
		strcpy(jlp->prikey, user_jlp.prikey);
	}

	jlp->version = user_jlp.version;
	strcpy(jlp->uuid, user_jlp.uuid);

	jlp->devtype = user_jlp.devtype;
	jlp->lancon = user_jlp.lancon;
	jlp->cmd_tran_type = user_jlp.cmd_tran_type;
	
	strcpy(jlp->joySdkVersion, user_jlp.joySdkVersion);
	jlp->noSnapshot = user_jlp.noSnapshot;

	return ret;
}

/**
 * brief: 
 *
 * @Param: out_modelcode
 * @Param: out_max
 *
 * @Returns: 
 */
int
joylink_dev_get_modelcode(char *out_modelcode, int32_t out_max)
{
	if(NULL == out_modelcode || out_max < 0){
		return 0;
	}
	/**
	*FIXME:must to do
	*/
	int len = 0;

	char *packet_data =  joylink_dev_modelcode_info(0, &_g_pdev->jlp);
	if(NULL !=  packet_data){
		len = strlen(packet_data);
		log_info("------>%s:len:%d\n", packet_data, len);
		if(len < out_max){
			memcpy(out_modelcode, packet_data, len); 
		}else{
			len = 0;
		}
	}

	if(NULL !=  packet_data){
		free(packet_data);
	}

	return len;
}

/**
 * brief: 
 *
 * @Returns: 
 */
int
joylink_dev_user_data_get(user_dev_status_t *user_data)
{
	/**
	*FIXME:must to do
	*/
	return 0;
}

/**
 * brief: 
 *
 * @Param: out_snap
 * @Param: out_max
 *
 * @Returns: 
 */
int
joylink_dev_get_snap_shot_with_retcode(int32_t ret_code, char *out_snap, int32_t out_max)
{
	if(NULL == out_snap || out_max < 0){
		return 0;
	}
	/**
	*FIXME:must to do
	*/
	int len = 0;

	joylink_dev_user_data_get(&user_dev);

	char *packet_data =  joylink_dev_package_info(ret_code, &user_dev);
	if(NULL !=  packet_data){
		len = strlen(packet_data);
		log_info("------>%s:len:%d\n", packet_data, len);
		if(len < out_max){
			memcpy(out_snap, packet_data, len); 
		}else{
			len = 0;
		}
	}

	if(NULL !=  packet_data){
		free(packet_data);
	}
	return len;
}

/**
 * brief: 
 *
 * @Param: out_snap
 * @Param: out_max
 *
 * @Returns: 
 */
int
joylink_dev_get_snap_shot(char *out_snap, int32_t out_max)
{
	return joylink_dev_get_snap_shot_with_retcode(0, out_snap, out_max); 
}

/**
 * brief: 
 *
 * @Param: out_snap
 * @Param: out_max
 * @Param: code
 * @Param: feedid
 *
 * @Returns: 
 */
int
joylink_dev_get_json_snap_shot(char *out_snap, int32_t out_max, int code, char *feedid)
{
    /**
     *FIXME:must to do
     */
    sprintf(out_snap, "{\"code\":%d, \"feedid\":\"%s\"}", code, feedid);

    return strlen(out_snap);
}

/**
 * brief: 
 *
 * @Param: json_cmd
 *
 * @Returns: 
 */
E_JLRetCode_t 
joylink_dev_lan_json_ctrl(const char *json_cmd)
{
    /**
     *FIXME:must to do
     */
    log_debug("json ctrl:%s", json_cmd);

    return E_RET_OK;
}

/**
 * brief: 
 *
 * @Returns: 
 */
int
joylink_dev_user_data_set(char *cmd, user_dev_status_t *user_data)
{
	/**
	*FIXME:must to do
	*/
	return 0;
}

/**
 * brief: 
 *
 * @Param: src
 * @Param: src_len
 * @Param: ctr
 * @Param: from_server
 *
 * @Returns: 
 */
E_JLRetCode_t 
joylink_dev_script_ctrl(const char *src, int src_len, JLContrl_t *ctr, int from_server)
{
	if(NULL == src || NULL == ctr){
		return -1;
	}
	/**
	*FIXME:must to do
	*/
	int ret = -1;
	ctr->biz_code = (int)(*((int *)(src + 4)));
	ctr->serial = (int)(*((int *)(src +8)));

	time_t tt = time(NULL);
	log_info("bcode:%d:server:%d:time:%ld", ctr->biz_code, from_server,(long)tt);

	if(ctr->biz_code == JL_BZCODE_GET_SNAPSHOT){
		/*
		*Nothing to do!
		*/
		ret = 0;
	}else if(ctr->biz_code == JL_BZCODE_CTRL){
		joylink_dev_parse_ctrl(src + 12, &user_dev);
#ifdef __MTK_7687__
		gpio_ligt_ctrl(user_dev.power);
#endif
		return 0;
	}else{
		log_error("unKown biz_code:%d", ctr->biz_code);
	}
	return ret;
}

/**
 * brief: 
 *
 * @Param: otaOrder
 *
 * @Returns: 
 */
E_JLRetCode_t
joylink_dev_ota(JLOtaOrder_t *otaOrder)
{
	if(NULL == otaOrder){
		return -1;
	}
#ifdef __MTK_7687__
	log_debug("serial:%d | feedid:%s | productuuid:%s | version:%d | versionname:%s | crc32:%d | url:%s\n",
		otaOrder->serial, otaOrder->feedid, otaOrder->productuuid, otaOrder->version, 
		otaOrder->versionname, otaOrder->crc32, otaOrder->url);

	strcpy(_g_fota_ctx.download_url, (const char*)otaOrder->url);
	//strcpy(_g_fota_ctx.download_url, (const char*)"http://192.168.1.96/mt7687_iot_sdk.bin");
	_g_fota_ctx.crc32 = otaOrder->crc32;
	joylink_fota_download_package();
#else
	pthread_t tidp;
	int ret = pthread_create(&tidp, NULL, (void *)joylink_ota_task, (void *)otaOrder);
	if(ret < 0){
		log_info("ota progress pthread create failed!");
	}
#endif
	return E_RET_OK;
}


/**
 * brief: 
 */
void
joylink_dev_ota_status_upload()
{
	return;	
}

int joylink_dev_https_post( char* host, char* query ,char *revbuf,int buflen)
{
	int ret = -1;
	/**
		 *FIXME:must to do
	*/
	return ret;
}

int joylink_dev_run_status(JLRunStatus_t status)
{
	int ret = -1;
	/**
		 *FIXME:must to do
	*/
	return ret;
}

