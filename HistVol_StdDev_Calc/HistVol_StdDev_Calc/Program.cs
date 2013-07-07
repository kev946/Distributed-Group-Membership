using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Windows;
using System.IO;

namespace HistVol_StdDev_Calc
{
    class Program
    {
        private static Program _instance;
        private static List<DataPoint> _vols;
        private static DataPointCollection _volStdDev;

        private Program() 
        { 
            _volStdDev = new DataPointCollection();
            _vols = new List<DataPoint>();
        }

        public static Program Instance
        {
            get
            {
                if(_instance == null)
                {
                    _instance = new Program();
                }
                return _instance;
            }
        }

        public DataPointCollection GetVolStdDev()
        {
            return _volStdDev;
        }

        public List<DataPoint> GetVols()
        {
            return _vols;
        }

        public void ClearData()
        {
            if (_volStdDev.Count > 0)
                _volStdDev.Clear();
            if (_vols.Count > 0)
                _vols.Clear();
        }

        // Retrieve end of day price data from yahoo finance
        public void GetData(string symbol, int period, int observations)
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create("http://ichart.yahoo.com/table.csv?s=" + symbol);
            HttpWebResponse resp = (HttpWebResponse)req.GetResponse();
            StreamReader sr = new StreamReader(resp.GetResponseStream());

            DataPointCollection logReturns = new DataPointCollection();

            int i = 0;
            string line = sr.ReadLine();// skip first line (header)
            string[] m = sr.ReadLine().Split(',');

            // The # of items we need to retrieve (make sure right amount)
            int j = period*4 + 1;
            if (j <= observations*4+1) 
            {
                j = ((observations * 4) + 1);
            }
            
            while (i < j)
            {
                string[] n = sr.ReadLine().Split(',');
                double l = Convert.ToDouble(m[4]);
                double k = Convert.ToDouble(n[4]);
                DataPoint t = new DataPoint(Math.Log(l / k), Convert.ToDateTime(m[0]));
                logReturns.Add(t);
                m = n;
                i++;
            }

            // Calculate volatilities
            double annualFactor = Math.Sqrt(252);
            for (i = 0; i < j / 2; ++i)
            {
                double vol = StandardDeviation(logReturns.GetRange(i, period)) * annualFactor;
                DataPoint t = new DataPoint(vol, logReturns[i].Date);
                _vols.Add(t);
            }

            // Calculate std-dev of all volatilities
            for(i = 0; i < observations; ++i)
            {
                double stdDev = StandardDeviation(_vols.GetRange(i, period));
                DataPoint t = new DataPoint(stdDev, _vols[i].Date);
                _volStdDev.Add(t);
            }

            // Take subset so we can plot on graph 
            _vols = _vols.GetRange(0, observations);
        }

        private double StandardDeviation(List<DataPoint> valueList)
        {
            double M = 0.0;
            double S = 0.0;
            int k = 1;
            foreach (DataPoint t in valueList)
            {
                double tmpM = M;
                M += (t.Data - tmpM) / k;
                S += (t.Data - tmpM) * (t.Data - M);
                k++;
            }
            return Math.Sqrt(S / (k - 2));
        }
    }
}
