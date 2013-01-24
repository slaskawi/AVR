using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Projekt
{

    /// <summary>
    /// Main frame class. 
    /// Displays menu and handles Serial Port connection.
    /// </summary>
    public partial class Form1 : Form
    {

        #region public variables
        /// <summary>
        /// This variable is used dor debugging.
        /// In release version is should be set to false
        /// </summary>
        public static Boolean DEBUG = false;

        private Console m_consolePanel;
        private PCFDebug m_pcfDebug;
        private Temperature m_temperature;
        private Settings m_settingsPanel;
        private Setup m_setup;
        private Graph m_graph;

        /// <summary>
        /// Waiting window reference.
        /// </summary>
        

        private static VS.Logger.Class1.Logger s_logger;

        /// <summary>
        /// This variable indicates current application state.
        /// iex. Main frame or Console.
        /// It should be used with e_states enum.
        /// </summary>
        public static int s_currentState = -1;
       
        /// <summary>
        /// public Serial port properties.
        /// </summary>
        public static String[] s_portNames = System.IO.Ports.SerialPort.GetPortNames();
        public static int[] s_speeds = { 19200, 5700};
        public static Parity[] s_parity = { Parity.None, Parity.Even, Parity.Odd};
        public static int[] s_dataBits = { 8, 7, 6, 5 };
        public static StopBits[] s_stopBits = { StopBits.One, StopBits.OnePointFive, StopBits.Two };

        /// <summary>
        /// Public serial port reference.
        /// </summary>
        public SerialPort m_serialPort;

        /// <summary>
        /// This variable is set to true is we have serial port connection.
        /// </summary>
        public Boolean m_isConnected = false;

        /// <summary>
        /// Proper application states.
        /// </summary>
        public enum e_states
        {
            None = -1,
            PortMenu = 0,
            Console = 1,
            PCFDebug = 2,
            Temperature = 3,
            Settings = 4,
            Setup = 5,
            Graph = 6
        }

        #endregion

        /// <summary>
        /// Public constructor - initializes all components
        /// </summary>
        public Form1()
        {
            InitializeComponent();
            s_currentState = (int)e_states.PortMenu;

            comboBox1.Items.AddRange(s_portNames);
            
            comboBox1.SelectedIndex = 0;
            foreach (int i in s_speeds)
            {
                comboBox2.Items.Add(i);
            }
            
            comboBox2.SelectedIndex = 0;
            foreach (Parity p in s_parity)
            {
                comboBox3.Items.Add(p);
            }
            
            comboBox3.SelectedIndex = 0;
            foreach (int i in s_dataBits)
            {
                comboBox4.Items.Add(i);
            }
            
            comboBox4.SelectedIndex = 0;
            foreach (StopBits s in s_stopBits)
            {
                comboBox5.Items.Add(s);
            }
            comboBox5.SelectedIndex = 0;

            if (!DEBUG)
            {
                //turn off all buttons until connection
                button2.Enabled = false;
                button3.Enabled = false;
                button4.Enabled = false;
                button5.Enabled = false;
                button6.Enabled = false;
                button7.Enabled = false;
            }
            initLogger();
            log("Application started");
        }

        ~Form1()
        {
            if (m_serialPort.IsOpen)
                m_serialPort.Close();
            m_serialPort = null;
        }

        /// <summary>
        /// Initializes logging engine.
        /// </summary>
        public static void initLogger()
        {
            s_logger = new VS.Logger.Class1.Logger(2, "log.txt");
        }

        /// <summary>
        /// Closes logging engine.
        /// </summary>
        public static void closeLogger()
        {
            s_logger.shutdown();
        }

        /// <summary>
        /// Loggs given string on given level with given tag.
        /// </summary>
        /// <param name="level"></param>
        /// <param name="tag"></param>
        /// <param name="message"></param>
        public static void log(int level, String tag, String message)
        {
            s_logger.log(level, tag, message);
        }

        /// <summary>
        /// Used to log exceptions.
        /// </summary>
        /// <param name="message"></param>
        public static void logException(String message)
        {
            s_logger.log(1, "EXCEPTION", message);
        }

        /// <summary>
        /// Used to log user actions.
        /// </summary>
        /// <param name="message"></param>
        public static void logUserAction(String message)
        {
            s_logger.log(1, "USER", message);
        }

        /// <summary>
        /// Used to log given string.
        /// </summary>
        /// <param name="message"></param>
        public static void log(String message)
        {
            s_logger.log(1, "", message);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Exit choosen from the menu strip - exit application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// About choosen from the menu strip - display about
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new About().Show();
        }

        #region Combo boxes events

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
        
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        
        }

        private void comboBox4_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        
        }

        private void comboBox5_SelectedIndexChanged(object sender, EventArgs e)
        {
           
    }

        #endregion

    /// <summary>
        /// Connect is enabled. Open port and sent information about connection
        /// to avr. Here can be thrown an exception. If it is ask the user to retry
        /// action. It is also used to disconnect.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (m_isConnected == false)
            {
                try
                {
                    m_serialPort = new SerialPort(
                     s_portNames[comboBox1.SelectedIndex],
                     s_speeds[comboBox2.SelectedIndex],
                     s_parity[comboBox3.SelectedIndex],
                     s_dataBits[comboBox4.SelectedIndex],
                     s_stopBits[comboBox5.SelectedIndex]);
                    m_serialPort.ReadTimeout = 100000;
                    m_serialPort.DtrEnable = true;
                    m_serialPort.Open();
                    m_serialPort.DataReceived += new SerialDataReceivedEventHandler(m_serialPort_DataReceived);
                    log("Port opened (" + s_portNames[comboBox1.SelectedIndex] + " " + s_speeds[comboBox2.SelectedIndex] + " " + s_parity[comboBox3.SelectedIndex] + " " + s_dataBits[comboBox4.SelectedIndex] + " " + s_stopBits[comboBox5.SelectedIndex] + ")");
                    m_serialPort.DtrEnable = true;
                    m_serialPort.Encoding = Encoding.ASCII;
                    m_serialPort.Write("connect!");
                    button1.Text = "Disconnect";
                    m_isConnected = true;
                    button1.Enabled = true;
                    button2.Enabled = true;
                    button3.Enabled = true;
                    button4.Enabled = true;
                    button5.Enabled = true;
                    button6.Enabled = true;
                    button7.Enabled = true;
                }
                catch (Exception a)
                {
                    MessageBox.Show("Error!\nPort error - please try again");
                }
            }
            else if (m_isConnected == true)
            {
                try
                {
                    m_serialPort.Write("disconnect!");
                    m_serialPort.Close();
                    log("Port closed");
                    m_serialPort = null;
                    button1.Text = "Connect";
                    m_isConnected = false;
                    button1.Enabled = true;
                    
                    button2.Enabled = false;
                    button3.Enabled = false;
                    button4.Enabled = false;
                    button5.Enabled = false;
                    button6.Enabled = false;
                    button7.Enabled = false;
                }
                catch (Exception a)
                {
                    MessageBox.Show("Error!\nPort error - please try again");
                }
            }
        }

        private void progressBar1_Click(object sender, EventArgs e)
        {
            
        }

        /// <summary>
        /// Sends via opened Serial port message.
        /// If an exception is thrown the user is asked to restart application.
        /// This is a serius error.
        /// </summary>
        /// <param name="message"></param>
        public void sendDataViaPort(String message)
        {
            if (m_isConnected == false)
            {
                log("User tried to send message via not opened port msg = " + message);
                MessageBox.Show("Please connect first....");
                return;
            }
            try
            {
                m_serialPort.Write(message);
                log("message sent = " + message);
            }
            catch (Exception e)
            {
                logException("exception while sending via port msg = " + e.Message);
                MessageBox.Show("Error occured - please restart application");
            }
        }

        /// <summary>
        /// Gets from the Serial port string and passes it into proper form.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void m_serialPort_DataReceived(object sender, EventArgs e)
        {

            try
            {
                System.Threading.Thread.Sleep(100);
            }
            catch (Exception e1)
            {
                logException(e1.Message);
            }

            String txt;

            try
            {
                txt = m_serialPort.ReadExisting();
                log("Data recieved from port = " + txt);
            }
            catch (Exception a)
            {
                logException("while recieving data from port msg = " + a.Message);
                MessageBox.Show("Exception occured while reading prom port - try again");
                return;
            }

            if (s_currentState == (int)e_states.PortMenu)
            {
                return;
            }
            else if (s_currentState == (int)e_states.Console)
            {
                m_consolePanel.addDataToDisplay(txt);
            }
            else if (s_currentState == (int)e_states.PCFDebug)
            {
                m_pcfDebug.addDataToDisplay(txt);
            }
            else if (s_currentState == (int)e_states.Temperature)
            {
                m_temperature.addDataToDisplay(txt);
            }
            else if (s_currentState == (int)e_states.Settings)
            {
                m_settingsPanel.addDataToDisplay(txt);
            }
            else if (s_currentState == (int)e_states.Graph)
            {
                m_graph.addDataToDisplay(txt);
            }
        }

        public void checkConnection()
        {

        }

        /// <summary>
        /// Shows console.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            m_consolePanel =  new Console(this);
            m_consolePanel.Show();
            Form1.state = (int)Form1.e_states.Console;
        }

        /// <summary>
        /// Sets or gets current state of application.
        /// </summary>
        public static int state
        {
            get
            {
                return (int)s_currentState;
            }
            set
            {
                logUserAction("State changed to = " + (int)value);
                s_currentState = (int) value;
            }
        }

        /// <summary>
        /// Shows PCFDebug.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click(object sender, EventArgs e)
        {
            m_pcfDebug = new PCFDebug(this);
            m_pcfDebug.Show();
            Form1.state = (int)Form1.e_states.PCFDebug;
        }

        /// <summary>
        /// Shows temperature debug
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button4_Click(object sender, EventArgs e)
        {
            m_temperature = new Temperature(this);
            m_temperature.Show();
            Form1.state = (int)Form1.e_states.Temperature;
        }

        /// <summary>
        /// Shows Settings panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button5_Click(object sender, EventArgs e)
        {
            m_settingsPanel = new Settings(this);
            m_settingsPanel.Show();
            Form1.state = (int)Form1.e_states.Settings;
        }

        /// <summary>
        /// Sets setup panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button6_Click(object sender, EventArgs e)
        {
            m_setup = new Setup(this);
            m_setup.Show();
            Form1.state = (int)Form1.e_states.Setup;
        }

        /// <summary>
        /// Help was choosen from menu strip - show help chm
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void helpToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            try
            {
                System.Diagnostics.Process.Start("projektowanie komputerowe - projekt.doc");
            }
            catch (Exception a)
            {
                logException("Help file not found = " + a.Message);
                MessageBox.Show("Help file not found");
            }
            
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if (m_isConnected)
            {
                try
                {
                    m_serialPort.Write("connect!");
                }
                catch (Exception q)
                {
                    MessageBox.Show("connecting failed");
                    logException("force connect failed msg = " + q.Message);
                }

            }
        }

        private void button7_Click(object sender, EventArgs e)
        {

            m_graph = new Graph(this);
            m_graph.Show();
            Form1.state = (int)Form1.e_states.Graph;
           //graph
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (m_isConnected)
            {
                try
                {
                    m_serialPort.Write("exit!");
                    m_serialPort.Write("disconnect!");
                }
                catch (Exception q)
                {
                    MessageBox.Show("disconnecting failed");
                    logException("force disconnect failed msg = " + q.Message);
                }
                
            }
        }

    }
}