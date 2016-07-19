using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IMCRM.ClientDemo.Helpers
{
    public class ConfigHelper
    {
        public static readonly string _serverIp = Utility.GetSettingByKey("ServerIP");
        public static readonly string _serverPost = Utility.GetSettingByKey("ServerPort");
        public static readonly string _appKey = Utility.GetSettingByKey("AppKey");
        public static readonly string _appSecret = Utility.GetSettingByKey("AppSecret");
        public static readonly string _qiniuAppKey = Utility.GetSettingByKey("ACCESS_KEY");
        public static readonly string _qiniuSecret = Utility.GetSettingByKey("SECRET_KEY");
        public static readonly string _qiniuUserAgent = Utility.GetSettingByKey("USER_AGENT");
    }
}
