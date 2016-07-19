using Common;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace IMCRM.ClientDemo.Helpers
{
    /// <summary>
    /// 表情数据加载
    /// </summary>
    public class FaceHelper
    {
        private static Dictionary<string, string> _faceDic;
        private static Dictionary<string, string> _imgDic;

        /// <summary>
        /// 表情列表<图片名称,图片符号>
        /// </summary>
        public static Dictionary<string, string> FaceDic
        {
            get
            {
                if (_faceDic == null || _faceDic.Count == 0)
                {
                    InitFaceDic();
                    return _faceDic;
                }
                else
                    return _faceDic;
            }
            set { _faceDic = value; }
        }
        /// <summary>
        /// 表情图片列表<图片符号,图片名称>
        /// </summary>
        public static Dictionary<string, string> ImgDic
        {
            get
            {
                if (_imgDic == null || _imgDic.Count == 0)
                {
                    InitFaceDic();
                    return _imgDic;
                }
                else
                    return _imgDic;
            }
            set { _imgDic = value; }
        }

        /// <summary>
        /// 初始化表情
        /// </summary>
        public static void InitFaceDic()
        {
            try
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(Path.Combine(Application.StartupPath, "Xml\\rc_emoji.xml"));
                int imgCount = doc.DocumentElement.ChildNodes[0].ChildNodes.Count;
                int uniCodeCount = doc.DocumentElement.ChildNodes[1].ChildNodes.Count;
                int forCount = imgCount <= uniCodeCount ? imgCount : uniCodeCount;
                _faceDic = new Dictionary<string, string>();
                _imgDic = new Dictionary<string, string>();
                for (int i = 0; i < forCount; i++)
                {
                    _faceDic.Add(doc.DocumentElement.ChildNodes[0].ChildNodes[i].InnerText, doc.DocumentElement.ChildNodes[1].ChildNodes[i].InnerText);
                    int value = Convert.ToInt32(doc.DocumentElement.ChildNodes[1].ChildNodes[i].InnerText, 16);
                    string stringValue = Char.ConvertFromUtf32(value);
                    //string charValue = ((char)value)+"";
                    //if (stringValue.Length == 1)
                    //{
                    //    stringValue = ((char)value) + "";
                    //}
                    //if (!_imgDic.ContainsKey(stringValue))
                    _imgDic.Add(stringValue, doc.DocumentElement.ChildNodes[0].ChildNodes[i].InnerText);
                    //if (stringValue.Length == 1)
                    //    ZY1dic.Add(stringValue, doc.DocumentElement.ChildNodes[0].ChildNodes[i].InnerText);
                }
            }
            catch (Exception ex) { LogHelper.Error(ex.ToString()); }
        }
        public static Dictionary<string, string> ZY1dic { get; set; }
        public static string Get_uft8(string unicodeString)
        {
            UTF8Encoding utf8 = new UTF8Encoding();
            Byte[] encodedBytes = utf8.GetBytes(unicodeString);
            String decodedString = utf8.GetString(encodedBytes);
            return decodedString;
        }
        


    }
    /// <summary>
    /// 表情信息
    /// </summary>
    public class FaceMsg
    {
        /// <summary>
        /// 索引
        /// </summary>
        public int index { get; set; }
        /// <summary>
        /// 图片
        /// </summary>
        public Image image { get; set; }
        /// <summary>
        /// 占位符长度-1
        /// </summary>
        public int length { get; set; }
        /// <summary>
        /// 图片名称
        /// </summary>
        public string imgeName { get;set;}
    }
}
