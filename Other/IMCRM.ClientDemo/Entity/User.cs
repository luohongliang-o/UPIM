using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IMCRM.ClientDemo.Entity
{
    public class User
    {
        public User()
        {
            isNewAccessUser = false;//默认为false
        }
        public string userId { get; set; }
        public string mobile { get; set; }
        public string name { get; set; }
        public string nickname { get; set; }
        public string portraitUri { get; set; }
        public string token { get; set; }
        public string extra { get; set; }
        /// <summary>
        /// 未读消息记录
        /// </summary>
        public int msgCount { get; set; }
        /// <summary>
        /// 未读消息JSON(只能存储从公众号推送过来的消息)
        /// </summary>
        public List<string> hisMsg { get; set; }
        /// <summary>
        /// 底部提示信息（可以存公众号）（可以最新聊天内容）
        /// </summary>
        public string personalMsg { get; set; }
        /// <summary>
        /// 接入码（等待用户专用）
        /// </summary>
        public string accessCode { get; set; }
        /// <summary>
        /// 公众号（等待用户专用）
        /// </summary>
        public string pubNumber { get; set; }
        /// <summary>
        /// 是否为最新分配的等待用户
        /// </summary>
        public bool isNewAccessUser { get; set; }
    }

    public class PublicUser
    {
        public string PUBLICNUM { get; set; }
        public string PUBLICCODE { get; set; }
        public string PUBLICNAME { get; set; }
        public IList<AccessArray> ACCESSARRAY { get; set; }
    }

    public class AccessArray
    {
        public string AccessCode { get; set; }
        public string AccessName { get; set; }
    }
}

//new User() {id = 1, mobile = "13411111111", name = "zhansan", nickname = "张三", portraitUri = "http://www.qqw21.com/article/UploadPic/2012-11/201211259378560.jpg", token = "yA0oZYXkKEUKfCEdLzpKHDX059J6AZRHydNrvYHOyosIjhx74OSh04NOSqTjmqKhqNcc+Qqb7nOuOlEt7l0OqA=="},
