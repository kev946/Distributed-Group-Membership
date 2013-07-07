using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HistVol_StdDev_Calc
{
    public class DataPointCollection : List<DataPoint>
    {
        public DataPointCollection()
        {
            // Nothing
        }
    }

    public class DataPoint
    {
        public DateTime Date { get; set; }

        public double Data { get; set; }

        public DataPoint(double data, DateTime date)
        {
            this.Data = data;
            this.Date = date;
        }
    }
}
