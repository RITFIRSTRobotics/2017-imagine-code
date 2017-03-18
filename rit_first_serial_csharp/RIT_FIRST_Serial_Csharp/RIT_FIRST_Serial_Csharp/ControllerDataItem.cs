using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RIT_FIRST_Serial_Csharp
{
    class ControllerDataItem
    {
        byte[] dataArray = new byte[6];

        public byte[] DataArray
        {
            get { return dataArray; }
            set { dataArray = value; }
        }
    }
}
