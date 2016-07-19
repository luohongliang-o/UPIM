#ifndef RC_WriteLog_H
#define RC_WriteLog_H

#pragma once
#include <string>
#ifdef _WIN32
//#include <boost/lexical_cast.hpp>
//#define TOSTR(a) boost::lexical_cast<std::string>((a))
//glog
#define GOOGLE_GLOG_DLL_DECL           // 使用静态glog库用这个
#define GLOG_NO_ABBREVIATED_SEVERITIES // 没这个编译会出错,传说因为和Windows.h冲突
#include <glog/logging.h>
//glog
#endif

#endif

