using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace IMCRM.ClientDemo.Helpers
{
    public class InterfaceXmlHelper
    {
        private static Dictionary<string, string> _interFaceDic;
        /// <summary>
        /// 获取InterfaceConfig.xml配置信息
        /// </summary>
        public static Dictionary<string, string> InterFaceDic
        {

            get
            {
                if (_interFaceDic == null || _interFaceDic.Count == 0)
                {
                    InitInterFaceDic();
                    return _interFaceDic;
                }
                else
                    return _interFaceDic;
            }
            set { _interFaceDic = value; }
        }
        /// <summary>
        /// 数据初始化
        /// </summary>
        private static void InitInterFaceDic()
        {
            _interFaceDic = new Dictionary<string, string>();
            XmlDocument doc = new XmlDocument();
            doc.Load(Path.Combine(Application.StartupPath, "Xml\\InterfaceConfig.xml"));
            string key = string.Empty;
            string url = string.Empty;
            foreach (XmlNode item in doc.DocumentElement.ChildNodes)
            {
                key = item.Attributes["key"].Value.ToString();
                url = item.Attributes["url"].Value.ToString();
                if (!_interFaceDic.ContainsKey(key))
                {
                    _interFaceDic.Add(key, url);
                }
            }
        }
        /// <summary>
        /// 获取 接口Url地址
        /// </summary>
        /// <param name="key">接口key</param>
        /// <returns></returns>
        public static string GetInterFaceUrlFromXmlConfig(InterFaceConfig key)
        {
            return InterFaceDic[key.ToString()];
        }
        public enum InterFaceConfig
        {
            CustomerDetialUrl,//客户信息地址
                /// <summary>
                /// 获取公众号列表
                /// </summary>
            GetList_PublicNumber,
            GetSoftUser,
            GETIMUser,
            ImUserReg,
            GetToKen
        }
    }
}
