using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace IMCRM.ClientDemo.Helpers
{
    public class ImgHelper
    {
        public static MemoryStream DownLoadPicToImage(string url)
        {
            MemoryStream ms = new MemoryStream();
            try
            {
                WebClient client = new WebClient();
                Uri uri = new Uri(url);
                byte[] coverByte = client.DownloadData(uri);
                if (coverByte.Length == 0)
                {
                    ms.Close();
                    return ReturnErroImg();
                }
                else
                    ms.Write(coverByte, 0, coverByte.Length);
                return ms;
            }
            catch
            {
                return ReturnErroImg();
            }
        }
        private static MemoryStream ReturnErroImg()
        {
            MemoryStream ms = new MemoryStream();
            Bitmap bmp = new Bitmap(40, 40);
            Graphics g = Graphics.FromImage(bmp);
            g.Clear(Color.FromArgb(212, 228, 238));
            g.DrawString("×", new Font("黑体", 15f, FontStyle.Bold), new SolidBrush(Color.Red), new Point(8, 10));
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            g.Dispose();
            bmp.Dispose();
            return ms;
        }
        /// <summary>
        ///  Base64格式字符串转换成图片
        /// </summary>
        /// <param name="imgStr">"data:image/png;base64"开头的图片字符串</param>
        /// <returns></returns>
        public static Image Base64StringToImg(string imgStr)
        {
            var tmpArr = imgStr.Split(',');
            //tmpArr[1] = tmpArr[1];
            byte[] bytes = Convert.FromBase64String(tmpArr.Count() > 1 ? tmpArr[1] : tmpArr[0]);
            using (MemoryStream ms = new MemoryStream(bytes))
            {
                ms.Write(bytes, 0, bytes.Length);
                return Image.FromStream(ms, true);
            }
        }
    }
}
