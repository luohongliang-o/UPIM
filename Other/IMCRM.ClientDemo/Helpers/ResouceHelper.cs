using Common;
using Model.Dto;
using Newtonsoft.Json.Linq;
using Security;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace IMCRM.ClientDemo.Helpers
{
    /// <summary>
    /// 用于存储各类资源缓存类信息
    /// </summary>
    public class ResouceHelper
    {
        #region 公众号

        private static List<PublicCodeSYNC_DTO> _publicCodeList;

        public static List<PublicCodeSYNC_DTO> PublicCodeList
        {
            get
            {
                if (_publicCodeList == null || _publicCodeList.Count == 0)
                    Init();
                return ResouceHelper._publicCodeList;
            }
            set { ResouceHelper._publicCodeList = value; }
        }
        /// <summary>
        /// 获取公众号名称
        /// </summary>
        /// <param name="pubCode"></param>
        public static string GetPubName(string pubCode)
        {
            if (string.IsNullOrEmpty(pubCode))
                return "";
            PublicCodeSYNC_DTO entry = PublicCodeList.FirstOrDefault(m => m.PUBLICCODE == pubCode);
            return entry == null ? "" : entry.PUBLICNAME;
        }
        /// <summary>
        /// 初始化公众号列表
        /// </summary>
        private static void Init()
        {

            string url = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.GetList_PublicNumber);
            string clientid = "im_crm";
            string param = "test";
            string key = "content={0}&clientid={1}&sign={2}";
            string jiami = new SecurityHelper().encyptData(clientid, JsonHelper.ObjDivertToJson(param));
            string jiami1 = HttpUtility.UrlEncode(jiami, Encoding.UTF8);
            string jiasuo = new SecurityHelper().signData(clientid, jiami);
            string jiasuo1 = HttpUtility.UrlEncode(jiasuo, Encoding.UTF8);
            key = string.Format(key, jiami1, clientid, jiasuo1);
            url = url + "?" + key;
            string str = Utility.PostData(url, Encoding.UTF8);
            string retmsg = new SecurityHelper().decyptData(clientid, str);
            List<PublicCodeSYNC_DTO> result = JsonHelper.JsonDivertToObj<List<PublicCodeSYNC_DTO>>(JObject.Parse(retmsg)["retmsg"].ToString());
            _publicCodeList = result;
        }
        #endregion

    }

    //public class PublicCodeSYNC_DTO
    //{
    //    public string PUBLICNUM { get; set; }
    //    public string PUBLICCODE { get; set; }
    //    public string PUBLICNAME { get; set; }
    //    public List<AccessCode_DTO> ACCESSARRAY { get; set; }
    //}

    //public class AccessCode_DTO
    //{
    //    public string ACCESSCODE { get; set; }
    //    public string ACCESSNAME { get; set; }
    //}

}
