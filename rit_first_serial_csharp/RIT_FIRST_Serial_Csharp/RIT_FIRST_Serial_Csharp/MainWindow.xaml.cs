using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace RIT_FIRST_Serial_Csharp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Contains a collection of <see cref="ControllerDataItem"/>
        /// and control methods to send commands to the Arduino Board
        /// </summary>
        ControllerDataContainer dataContainer = new ControllerDataContainer();

        /// <summary>
        /// Constructor
        /// </summary>
        public MainWindow()
        {
            dataContainer.NewArduinoDataReceived += arduinoData_NewArduinoDataReceived;
            InitializeComponent();
            try
            {
                dataContainer.OpenArduinoConnection();
            }
            catch (Exception)
            {
                MessageBox.Show("Error: Can not connect to the Arduino Board - Configure the COM Port in the app.config file and check whether an Arduino Board is connected to your computer.");
            }

            var data = dataContainer.ControllerDataList;
            for (int i = 0; i < 6; i++)
            {
                var col = new DataGridTextColumn();
                var binding = new Binding("DataArray[" + i + "]");
                col.Binding = binding;
                controllerDataGrid.Columns.Add(col);
            }
            controllerDataGrid.ItemsSource = data;
        }
        /// <summary>
        /// OnArduinoDataReceived event is catched in
        /// order to update the arduino data display on the form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void arduinoData_NewArduinoDataReceived(object sender, EventArgs e)
        {
            Dispatcher.BeginInvoke(new ThreadStart(PopulateGrids));
        }
        /// <summary>
        /// OnWeatherDataReceived event is catched in
        /// order to update the weather data display on the form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void controllerData_NewControllerDataReceived(object sender, EventArgs e)
        {
            // IDK how to handle this yet.
        }
        /// <summary>
        /// Click-Event for the Button to send controller data
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void sendControllerData_Click(object sender, RoutedEventArgs e)
        {
            dataContainer.SendDataToArduinoBoard();
        }

        private void PopulateGrids()
        {
            var controllerData = dataContainer.ControllerDataList;
            for (int i = 0; i < 6; i++)
            {
                var col = new DataGridTextColumn();
                var binding = new Binding("DataArray[" + i + "]");
                col.Binding = binding;
                controllerDataGrid.Columns.Add(col);
            }
            controllerDataGrid.ItemsSource = controllerData;

            var arduinoData = dataContainer.ArduinoDataList;
            for (int i = 0; i < 6; i++)
            {
                var col = new DataGridTextColumn();
                var binding = new Binding("DataArray[" + i + "]");
                col.Binding = binding;
                arduinoDataGrid.Columns.Add(col);
            }
            arduinoDataGrid.ItemsSource = arduinoData;
        }
    }
}
