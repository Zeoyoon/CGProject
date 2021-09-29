#ifndef _OPTION_H
#define _OPTION_H
// 配置文件
#include <string>
struct Option {
	bool LoadBunny = false;				// 是否导入模型，不一定是兔子，取名遗留问题
	std::string light = "SpotLight";	// 光照类型
	bool LoadSphere = false; // 是否导入球体
	bool LoadCube = false;
	struct OpObject {
		bool Sphere = false; // 是否移动球体
		bool Cube = false;
	}opObject;

};


#endif