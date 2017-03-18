using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Configuration;

namespace RIT_FIRST_Serial_Csharp
{
    class ControllerDataContainer
    {
        /// <summary>
        /// Interface for the Serial Port at which an Arduino Board
        /// is connected.
        /// </summary>
        SerialPort arduinoBoard = new SerialPort();

        /// <summary>
        /// Holds a List of <see cref="ControllerDataItem"/> in order
        /// to store input data received from a controller.
        /// </summary>
        List<ControllerDataItem> controllerDataList = new List<ControllerDataItem>();

        /// <summary>
        /// Holds a List of <see cref="ControllerDataItem"/> in order
        /// to store input data received from an arduino.
        /// </summary>
        List<ControllerDataItem> arduinoDataList = new List<ControllerDataItem>();

        /// <summary>
        /// Raised when new controller inputs are added
        /// to the internal controller data list.
        /// </summary>
        public event EventHandler NewControllerDataReceived;

        /// <summary>
        /// Raised when new arduino data is added
        /// to the internal arduino data list.
        /// </summary>
        public event EventHandler NewArduinoDataReceived;

        /// <summary>
        /// Gets a list of <see cref="ControllerDataItem"/> which was
        /// previsously retrieved from an Xbox controller.
        /// </summary>
        internal List<ControllerDataItem> ControllerDataList
        {
            get { return controllerDataList; }
        }

        /// <summary>
        /// Gets a list of <see cref="ControllerDataItem"/> which was
        /// previsously retrieved from an Arduino Board.
        /// </summary>
        internal List<ControllerDataItem> ArduinoDataList
        {
            get { return arduinoDataList; }
        }
        /// <summary>
        /// Closes the connection to an Arduino Board.
        /// </summary>
        public void CloseArduinoConnection()
        {
            arduinoBoard.Close();
        }
        /// <summary>
        /// Opens the connection to an Arduino board
        /// </summary>
        public void OpenArduinoConnection()
        {
            ControllerDataItem item = new ControllerDataItem();
            item.DataArray = new byte[6] { 32, 64, 0, 0, 0, 0 };
            controllerDataList.Add(item);
            if (!arduinoBoard.IsOpen)
            {
                arduinoBoard.DataReceived += arduinoBoard_DataReceived;
                arduinoBoard.PortName = ConfigurationManager.AppSettings["ArduinoPort"];
                arduinoBoard.Open();
            }
            else
            {
                throw new InvalidOperationException("The Serial Port is already open!");
            }
        }
        /// <summary>
        /// Sends the data to the Arduino board which triggers the board
        /// to echo the data it receives.
        /// </summary>
        public void SendDataToArduinoBoard()
        {
            if (arduinoBoard.IsOpen)
            {
                foreach (ControllerDataItem item in controllerDataList)
                {
                    arduinoBoard.Write(item.DataArray, 0, 6);
                }
            }
            else
            {
                throw new InvalidOperationException("Can't get weather data if the serial Port is closed!");
            }
        }
        /// <summary>
        /// Reads data from the arduinoBoard serial port
        /// </summary>
        void arduinoBoard_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = arduinoBoard.ReadTo("\x03");//Read until the EOT code
            //Split into individual text
            string[] dataArray = data.Split(new string[] { "\x02", "$" }, StringSplitOptions.RemoveEmptyEntries);
            //Iterate through the splitted data and parse it into weather data items
            //and add them to the list of received weather data.
            if (dataArray[0] != "READY" && dataArray[0] != "BORED")
            {
                byte[] arduinoData = new byte[6];
                arduinoData[0] = (byte)int.Parse(dataArray[0]);
                arduinoData[1] = (byte)int.Parse(dataArray[1]);
                arduinoData[2] = (byte)int.Parse(dataArray[2]);
                arduinoData[3] = (byte)int.Parse(dataArray[3]);
                arduinoData[4] = (byte)int.Parse(dataArray[4]);
                arduinoData[5] = (byte)int.Parse(dataArray[5]);
                arduinoDataList.Add(new ControllerDataItem() { DataArray = arduinoData });
            }
            if (NewArduinoDataReceived != null)//If there is someone waiting for this event to be fired
            {
                NewArduinoDataReceived(this, new EventArgs()); //Fire the event, indicating that new Controller Data was added to the list.
            }
        }
    }
}
