using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Research.DynamicDataDisplay;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using Microsoft.Research.DynamicDataDisplay.Charts;


namespace HistVol_StdDev_Calc
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        private LineGraph _lineGraph1;
        private LineGraph _lineGraph2;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void StartProgram(string symbol, int period, int observations)
        {
            // Check if chart already present
            ClearChart();

            // Singleton
            Program program = Program.Instance;
            program.GetData(symbol, System.Convert.ToInt32(period), System.Convert.ToInt32(observations));

            // Std Dev dataset
            var ds1 = new EnumerableDataSource<DataPoint>(program.GetVolStdDev());
            ds1.SetYMapping(y => y.Data);
            ds1.SetXMapping(x => dateAxis.ConvertToDouble(x.Date));
            
            // Vols dataset
            var ds2 = new EnumerableDataSource<DataPoint>(program.GetVols());
            ds2.SetYMapping(y => y.Data);
            ds2.SetXMapping(x => dateAxis.ConvertToDouble(x.Date));

            // Add data
            _lineGraph1 = Plotter.AddLineGraph(ds2, Colors.Red, 2, "volatility");
            _lineGraph2 = Plotter.AddLineGraph(ds1, Colors.Green, 1, "σ of volatility");
            ClearWindow();
        }

        private void ClearChart()
        {
            // Clear data in program
            Program program = Program.Instance;
            program.ClearData();

            // Clear data on chart
            if(_lineGraph1 != null)
                Plotter.Children.Remove(_lineGraph1);
            if(_lineGraph2 != null)
                Plotter.Children.Remove(_lineGraph2);
        }

        private void ClearWindow()
        {
            txtSymbol.Text = "";
            txtPeriod.Text = "";
            txtObservations.Text = "";
        }

        private void btnGetData_Click(object sender, RoutedEventArgs e)
        {
            string symbol = txtSymbol.Text.ToUpper();
            string period = txtPeriod.Text;
            string observations = txtObservations.Text;

            if (symbol == "" || period == "" || observations == "")
            {
                ClearWindow();
                MessageBox.Show("Invalid arguments...");
                return;
            }

            StartProgram(symbol, Convert.ToInt32(period), Convert.ToInt32(observations));
        }
    }
}
