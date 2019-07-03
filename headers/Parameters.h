#pragma once
#include <cmath>
#include <string>

/*全局常量，只能在此修改*/

constexpr double EPS = 1e-7;                //Vec3的计算精度
constexpr int WIDTH = 800;                  //生成的图片的宽度
constexpr int HEIGHT = 600;                 //生成的图片的高度
constexpr int TRIANGLEMESH_MAXN = 200000;    //三角网格最多顶点数
constexpr int TRIANGLEMESH_MAXM = 500000;    //三角网格最多面片数
const std::string TEXTURE_DIR = "images/";  //加载纹理图片的位置
const std::string MODEL_DIR = "models/";    //加载obj文件的位置
const std::string OUT_DIR = "Results/";     //输出图片的位置
constexpr double INF = 1e9;                 //极大值


/*以下常数均为默认值，对应的类提供修改的接口*/


/*照相机*/

constexpr double APERTURE = 0.3;            //光圈大小
constexpr double FOCALDIS = 90;             //焦平面距离
constexpr double SAMPLENUM = 10;              //Distributed的采样数
constexpr double LENS = 2800;               //焦距


/*PPM*/

constexpr int PPM_DEPTH = 10;               //PPM进行Hitpoint追踪时遍历深度
constexpr int PPM_ITER = 100000;            //PPM进行光子映射时最大迭代轮数
constexpr int PPM_PHOTON = 500000;          //每一轮PPM发射出光子的个数
constexpr double PPM_INIT_RADIUS = 2.0;     //PPM每个Hitpoint的起始半径
constexpr double PPM_ALPHA = 0.5;           //PPM中参数alpha
constexpr int PPM_NEWTON_ITER = 50;         //PPM中使用牛顿迭代法的轮数



