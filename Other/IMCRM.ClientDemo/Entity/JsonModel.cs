
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IMCRM.ClientDemo.Entity
{
    public class JsonModel
    {
    }
    public class ResultModel
    {
        public bool IsSucc { get; set; }

        public string ErrMessage { get; set; }

        public string Data { get; set; }
    }
}
