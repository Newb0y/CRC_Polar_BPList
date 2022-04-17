/**
@Date 2020/12/25
@Author 李秉豪
@Description 定义涉及到的所有文件的文件名
*/

/**
	文件命名的方式：各文件的前缀 + 码长 + 信息长度 + 构造方式
	目录命名的方式：input/(码长 + 信息长度) 或 output/(码长 + 信息长度)
*/
#ifndef FILENAME_H
#define FILENAME_H

#define SNR_FILE		"../input/parameter/SNR.txt" //
#define CONFIG_FILE "../input/parameter/config.txt"
#define POLAR_FILE	"../input/parameter/polar.txt"
#define PFG_CONFIG_FILE	"../input/parameter/PFGConfig.txt"
#define BPL_FILE				"../input/parameter/BPL.txt"
#define SEQUENCE_FILE	"../input/ConstructorSequence.txt"
#define METRIC_FILE		"../input/ConstructorMetric.txt"
#define MetricPAI_FILE	"../input/FGMetricPAI.txt"
#define PAI_FILE	"../input/FGPAI.txt"
#define CRC_length		"../input/parameter/CRC.txt"

#define RESULT_FILE	"../output/result.txt"//参数文件
#define BLER_FILE	    "../output/BLER.txt"//参数文件
#define BER_FILE			"../output/BER.txt"//参数文件
#define ERROR "../output/error_path.txt"
#define BED_FILE		"../output/BPL/1024_512/BED.txt"//参数文件
#endif