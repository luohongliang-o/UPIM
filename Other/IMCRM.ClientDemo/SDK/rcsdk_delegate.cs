using Newtonsoft.Json.Linq;
using System.Runtime.InteropServices;
using System.Text;
using IMCRM.ClientDemo;

namespace rcsdk_test
{
    //rcsdk callbacks
    //public delegate void ConnectAckEventHandler(string json_str);
    class connect_ack_callback_delegate
    {
        public static void connect_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            //JObject jobj = JObject.Parse(json_str);
            //string result = jobj["result"].ToObject<string>();
            //string userId = jobj["userId"].ToString();
            //int err_code = jobj["err_code"].ToObject<int>();
            FrmMain.frm.ConnectOpenCallBack(json_str);
        }
    }

    //public delegate void BlacklistInfoListenerEventHandler(string json_str);
    class black_list_callback_delegate
    {
        public static void black_list_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void public delegate void PublishAckListenerEventHandler(string json_str);
    class remove_member_from_discussion_callback_delegate
    {
        public static void remove_member_from_discussion_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void public delegate void PublishAckListenerEventHandler(string json_str);
    class send_message_callback_delegate
    {
        public static void send_message_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            //JObject jobj = JObject.Parse(json_str);
            FrmMain.frm.sendmessage_call_dealwith(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class sync_groups_callback_delegate
    {
        public static void sync_groups_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class join_group_callback_delegate
    {
        public static void join_group_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class quit_group_callback_delegate
    {
        public static void quit_group_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class set_user_data_callback_delegate
    {
        public static void set_user_data_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class remove_push_setting_callback_delegate
    {
        public static void remove_push_setting_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class quit_discussion_callback_delegate
    {
        public static void quit_discussion_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void BlacklistInfoListenerEventHandler(string json_str);
    class set_block_push_callback_delegate
    {
        public static void set_block_push_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void BlacklistInfoListenerEventHandler(string json_str);
    class get_block_push_callback_delegate
    {
        public static void get_block_push_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void UserInfoListeneEventHandler(string json_str);
    class get_user_info_callback_delegate
    {
        public static void get_userinfo_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
        public static void get_whait_userinfo_call_back(string json_str)
        {
            FrmMain.frm.whait_userinfo_dealwith(json_str);
        }
        public static void Get_Session_UserInfo_CallBack(string json_str)
        {
            FrmMain.frm.Session_UserInfo_DealWith(json_str);
        }
    }

    //public delegate void CreateDiscussionListenerEventHandler(string json_str);
    class create_invite_discussion_callback_delegate
    {
        public static void create_invite_discussion_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
            string discussion_id = jobj["discussionId"].ToObject<string>();
        }
    }
    //public delegate void PublishAckListenerEventHandler(string json_str);
    class invite_member_todiscussion_callback_delegate
    {
        public static void invite_to_discussion_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class sub_scribe_account_callback_delegate
    {
        public static void sub_scribe_account_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class join_chatroom_callback_delegate
    {
        public static void join_chatroom_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PublishAckListenerEventHandler(string json_str);
    class quit_chatroom_callback_delegate
    {
        public static void quit_chatroom_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PublishAckListenerEventHandler(string json_str);
    class add_to_black_callback_delegate
    {
        public static void add_to_black_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PublishAckListenerEventHandler(string json_str);
    class remove_from_black_callback_delegate
    {
        public static void remove_from_black_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PublishAckListenerEventHandler(string json_str);
    class set_invite_status_callback_delegate
    {
        public static void set_invite_status_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class rename_discussion_callback_delegate
    {
        public static void rename_discussion_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void CreateDiscussionListenerEventHandler(string json_str);
    class get_user_data_callback_delegate
    {
        public static void get_user_data_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void ImageListenerEventHandler(string json_str);
    class send_file_with_url_callback_delegate
    {
        public static void send_file_with_url_process_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
        public static void send_file_with_url_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
            string result = jobj["result"].ToObject<string>();
            if(result == "failed")
            {
                return;
            }
            string url = jobj["url"].ToObject<string>();
        }
    }

    //public delegate void ImageListenerEventHandler(string json_str);
    class down_file_with_url_callback_delegate
    {
        public static void down_file_with_url_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }

        public static void down_file_with_url_process_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PublishAckListenerEventHandler(string json_str);
    class add_push_setting_callback_delegate
    {
        public static void add_push_setting_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void PushSettingListenerEventHandler(string json_str);
    class query_push_setting_callback_delegate
    {
        public static void query_push_setting_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    // public delegate void EnvirtmentCallbackEventHandler(string json_str);
    class envirtment_change_callback_delegate
    {
        public static void envirtment_change_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageInfoEventHandler(string json_str);
    class get_paged_messageex_callback_delegate
    {
        public static unsafe void get_paged_messagex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageInfoEventHandler(string json_str);
    class get_message_by_id_callback_delegate
    {
        public static unsafe void get_message_by_id_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageInfoEventHandler(string json_str);
    class search_messageex_callback_delegate
    {
        public static unsafe void search_messagex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageListenerEventHandler(string json_str);
    class set_message_listener_callback_delegate
    {
        public static void set_message_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            FrmMain.frm.message_dealwith(json_str);
            //JObject jobj = JObject.Parse(json_str);
//             string msg_json_str = jobj["m_Message"].ToString();
//             JObject msg_jobj = JObject.Parse(msg_json_str);
//             string content_str = msg_jobj["content"].ToString();
        }
    }

    //public delegate void MessageListenerEventHandler(string json_str);
    //GetRemoteHistoryMessage
    class get_remote_history_message_listener_callback_delegate
    {
        public static void get_remote_history_messag_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
           
        }
    }

    //public delegate void ExceptionListenerEventHandler(string json_str);
    class exception_listener_callback_delegate
    {
        public static void exception_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageInfoEventHandler(string json_str);
    class get_his_msg_callback_delegate
    {
        public static unsafe void get_his_msg_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void DataBufferEventHander(string json_str);
    class get_data_buffer_delegate
    {
        public static unsafe void get_data_buffer_delegate_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void AccountInfoEventHandler(string json_str);
    class load_account_info_callback_delegate
    {
        public static unsafe void load_account_info_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }
    //public delegate void DiscussionInfoListenerEventHandler(string json_str);
    class discussion_info_callback_delegate
    {
        public static void discussion_info_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void DisInfoSyncEventHandler(string json_str);
    class get_dis_info_sync_callback_delegate
    {
        public static void get_dis_info_sync_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void PublishAckListenerEventHandler(string json_str);
    class account_listener_callback_delegate
    {
        public static void account_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void UserInfoEventHandler(string json_str);
    class user_info_ex_sync_callback_delegate
    {
        public static void user_info_ex_sync_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void ConversationInfoEventHandler(string json_str);
    class get_con_ex_callback_delegate
    {
        public static void get_con_ex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }
    //public delegate void ConversationInfoEventHandler(string json_str);
    class get_con_listex_callback_delegate
    {
        public static unsafe void get_con_listex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            //JObject jobj = JObject.Parse(json_str);
            FrmMain.frm.GetConversationList(json_str);
        }
    }
    //public delegate void ConversationInfoEventHandler(string json_str);
    class get_topcon_listex_callback_delegate
    {
        public static unsafe void get_topcon_listex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void MessageListenerEventHandler(string json_str);
    class load_history_message_callback_delegate
    {
        public static void load_his_msg_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void NaviDataListenerEventHandler(string json_str);
    class navi_data_listener_callback_delegate
    {
        public static void navi_data_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //public delegate void TokenListenerEventHandler(string json_str);

    class token_listener_callback_delegate
    {
        public static void token_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //ChatroomInfoListenerEventHandler
    class chat_room_info_listener_callback_delegate
    {
        public static void chat_room_info_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }

    //BizAckListenerEventHandler
    class set_block_push
    {
        public static void set_block_push_listener_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
    }
}

