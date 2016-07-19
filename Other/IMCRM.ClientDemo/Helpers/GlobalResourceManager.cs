

using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace IMCRM.ClientDemo.Helpers
{
    public class GlobalResourceManager
    {
        #region 初始化表情
        public static void Initialize()
        {
            try
            {
                #region Emotion
                List<string> tempList = FileHelper.GetOffspringFiles(AppDomain.CurrentDomain.BaseDirectory + "Expression\\");
                List<string> emotionFileList = new List<string>();
                //List<string> listFace = new List<string>();
                //string file = Path.Combine(Application.StartupPath, "Expression\\");
                List<Image> emotionList = new List<Image>();
                string picName = "";
                foreach (var item in FaceHelper.FaceDic)
                {
                    //listFace.Add(file + item.Key + ".png");
                    picName = item.Key + ".png";
                    emotionFileList.Add(picName);
                    emotionList.Add(Image.FromFile(AppDomain.CurrentDomain.BaseDirectory + "Expression\\" + picName));
                }

                //foreach (string file in tempList)
                //{
                //    string name = file.ToLower();
                //    if (name.EndsWith(".bmp") || name.EndsWith(".jpg") || name.EndsWith(".jpeg") || name.EndsWith(".png") || name.EndsWith(".gif"))
                //    {
                //        emotionFileList.Add(name);
                //    }
                //}
                //emotionFileList.Sort(new Comparison<string>(CompareEmotionName));

                for (int i = 0; i < emotionFileList.Count; i++)
                {
                    emotionList.Add(Image.FromFile(AppDomain.CurrentDomain.BaseDirectory + "Expression\\" + emotionFileList[i]));
                }
                GlobalResourceManager.emotionList = emotionList;
                #endregion
            }
            catch (Exception ee)
            {
                MessageBox.Show("加载系统资源时，出现错误。" + ee.Message);
            }
        }
        public static int CompareEmotionName(string a, string b)
        {
            if (a.Length != b.Length)
            {
                return a.Length - b.Length;
            }
            return a.CompareTo(b);
        }
        #endregion

        #region EmotionList、EmotionDictionary
        private static List<Image> emotionList;
        public static List<Image> EmotionList
        {
            get { return emotionList; }
        }
        private static Dictionary<uint, Image> emotionDictionary;
        public static Dictionary<uint, Image> EmotionDictionary
        {
            get
            {
                if (emotionDictionary == null)
                {
                    emotionDictionary = new Dictionary<uint, Image>();
                    for (uint i = 0; i < emotionList.Count; i++)
                    {
                        emotionDictionary.Add(i, emotionList[(int)i]);
                    }
                }
                return emotionDictionary;
            }
        }
        #endregion
    }
}
