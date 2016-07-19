using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Common;
using io.rong;
using IMCRM.ClientDemo;
using IMCRM.ClientDemo.Entity;
using IMCRM.ClientDemo.Helpers;
using IMWorkBench.SDK;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using rcsdk_test;
using SecurityHelper = Security.SecurityHelper;

namespace IMCRM.ClientDemo
{
    public partial class Form1 : Form
    {
        SecurityHelper security = new SecurityHelper();
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            AllocConsole();
            TxtUserId.Text = "2";
            TxtName.Text = "李四";
            TxtSoftUser.Text = "beixi0302";
        }
        private string GetToKen(string uid, string name, string portraitUri)
        {
            //请求token URL
            var toKenUrl = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.GetToKen);
            var rongCloudContent = "{\"IMN\":\"" + uid + "\",\"NC\":\"" + name + "\",\"IMR\":\"" + portraitUri + "\"}";
            //加密参数
            var content = security.CreateSignEncodingStr("im_crm", rongCloudContent);
            //请求返回结果，并解密
            var result = security.decyptData("im_crm", Utility.GetData(toKenUrl, content, Encoding.UTF8));
            if ((bool)JObject.Parse(result)["result"])
            {
                //返回结果成功
                var retmsg = JObject.Parse(result)["retmsg"];
                var toKen = JObject.Parse(retmsg.ToString())["Token"];
                Shell.WriteLine("info###融云toKen：" + toKen.ToString());
                return toKen.ToString();
            }
            return null;
        }
        private void OpenChar(string uid, string name, string portraitUri)
        {
            var token = GetToKen(uid, name, portraitUri);
            if (string.IsNullOrEmpty(token))
            {
                MessageBox.Show("获取token失败");
                return;
            }
            User user = new User()
            {
                userId = uid,
                name = name,
                nickname = uid,
                token = token
            };
            FrmMain main = new FrmMain(user);
            main.Show();
        }
        private void BtnLogin_Click(object sender, EventArgs e)
        {
            string uid = TxtUserId.Text;
            string name = TxtName.Text;
            string portraitUri = "http://www.qqw21.com/article/UploadPic/2012-11/201211259378560.jpg";
            OpenChar(uid, name, portraitUri);
        }
        private void BtnSoftLogin_Click(object sender, EventArgs e)
        {
            var softName = TxtSoftUser.Text;
            if (string.IsNullOrWhiteSpace(softName))
            {
                MessageBox.Show("用户名不能为空");
                return;
            }
            var softUserUrl = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.GetSoftUser);
            var imUserUrl = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.GETIMUser);
            var imUserRegUrl = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.ImUserReg);
            var url = softUserUrl + softName;
            //根据软件用户名从SSO获取软件用户信息
            var data = Utility.PostData(url, Encoding.UTF8);
            if (string.IsNullOrWhiteSpace(data))
            {
                MessageBox.Show("软件用户名为空！");
                return;
            }
            var job = JObject.Parse(data);
            var cid = job["cid"].ToString();
            var dv = job["dv"].ToString();
            var platform = "B";
            Shell.WriteLine(string.Format("info:用户卡号（{0}），渠道（{1}），业务编码（{2}）", cid, dv, platform));
            
            var json = "{\"JTCODE\":\"" + platform + "\",\"REGCAMPAIGNID\":\"" + dv + "\",\"USERNO\":\"" + cid + "\"}";
            Shell.WriteLine("查询IM帐号参数：" + json);
            var content = security.CreateSignEncodingStr("im_crm", json);
            //根据软件用户信息查询IM帐号信息
            var result = Utility.GetData(imUserUrl, content, Encoding.UTF8);
            var obj = JObject.Parse(security.decyptData("im_crm", result.Trim(new char[] {'"'})));
            Shell.WriteLine("warn:获取IM帐号信息:" + obj);
            //retmsg: [{ IMN: "BxxxxxxUserId", NC: "NC", IMG: "IMG" }]
            if ((bool) obj["result"])
            {
                //返回成功，直接登录
                var softUser = JObject.Parse(obj["retmsg"].ToString());
                if (softUser != null)
                {
                    var imn = softUser["IMN"].ToString();
                    var nc = string.IsNullOrEmpty(softUser["NC"].ToString()) ? cid : softUser["NC"].ToString();
                    var img = string.IsNullOrEmpty(softUser["IMR"].ToString()) ? "http://7xs3cl.com1.z0.glb.clouddn.com/001.png" : softUser["IMR"].ToString();
                    OpenChar(imn, nc, img);
                }
            }
            else
            {
                //返回失败，先注册再登录
                Shell.WriteLine("注册帐号url：" + imUserRegUrl + "?" + content);
                var regResult = Utility.GetData(imUserRegUrl, content, Encoding.UTF8);
                var regObj = JObject.Parse(security.decyptData("im_crm", regResult));
                Shell.WriteLine("warn:注册信息：" + regObj);
                if ((bool) regObj["result"])
                {
                    var softUser = regObj["retmsg"];
                    var imn = softUser["IMN"].ToString();
                    var nc = string.IsNullOrEmpty(softUser["NC"].ToString()) ? cid : softUser["NC"].ToString();
                    var img = string.IsNullOrEmpty(softUser["IMR"].ToString()) ? "http://7xs3cl.com1.z0.glb.clouddn.com/001.png" : softUser["IMR"].ToString();
                    OpenChar(imn, nc, img);
                }
                else
                {
                    MessageBox.Show("注册失败!");
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //string url = "http://localhost:23187/Home/Index";
            //string para = string.Empty;
            //for (int i = 1; i <= 10; i++)
            //{
            //    para = string.Format("uid={0}&name={1}", "zhanghao" + i, "zhanghao" + i);
            //    string ret = Utility.PostData(url + "?" + para, Encoding.UTF8);

            //}
        }

        [DllImport("kernel32.dll")]
        public static extern Boolean AllocConsole();
        [DllImport("kernel32.dll")]
        public static extern Boolean FreeConsole();
    }

    /// <summary>  
    /// 与控制台交互  
    /// </summary>
    static class Shell
    {
        /// <summary>  
        /// 输出信息  
        /// </summary>  
        /// <param name="format"></param>  
        /// <param name="args"></param>  
        public static void WriteLine(string format, params object[] args)
        {
            WriteLine(string.Format(format, args));
        }

        /// <summary>  
        /// 输出信息  
        /// </summary>  
        /// <param name="output"></param>  
        public static void WriteLine(string output)
        {
            Console.ForegroundColor = GetConsoleColor(output);
            Console.WriteLine(@"[{0}]{1}", DateTimeOffset.Now, output);
        }

        /// <summary>  
        /// 根据输出文本选择控制台文字颜色  
        /// </summary>  
        /// <param name="output"></param>  
        /// <returns></returns>  
        private static ConsoleColor GetConsoleColor(string output)
        {
            if (output.StartsWith("warn")) return ConsoleColor.Yellow;
            if (output.StartsWith("err")) return ConsoleColor.Red;
            if (output.StartsWith("info")) return ConsoleColor.Green;
            return ConsoleColor.Gray;
        }
    }
}
