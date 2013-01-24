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
    /// This is Console class. Allows user to manually send and recieve data from port.
    /// </summary>
    public partial class Console : Form
    {
        private Form1 m_parentForm;

        delegate void SetTextCallBack(String text);

        public Console()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Public constructor. Initializes necessary components.
        /// </summary>
        /// <param name="f"></param>
        public Console(Form1 f)
        {
            InitializeComponent();
            m_parentForm = f;
            this.Disposed += new EventHandler(Console_Disposed);
            System.Threading.Thread.Sleep(200);
        }

        /// <summary>
        /// Sends via port given string.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox2.Text != null && textBox2.Text != "")
            {
                //send data via port
                m_parentForm.sendDataViaPort(textBox2.Text);
            }
        }

        /// <summary>
        /// This method is responsible for displaying recieved data in textBox.
        /// (ThreadSafety!)
        /// </summary>
        /// <param name="msg"></param>
        public void addDataToDisplay(String msg)
        {
            if (this.textBox1.InvokeRequired)
            {
                SetTextCallBack d = new SetTextCallBack(addDataToDisplay);
                try
                {
                    this.Invoke(d, new object[] { msg });
                }
                catch (Exception e)
                {
                    Form1.logException("Exception while adding to Console.textBox1 msg = " + e.Message);
                }
            }
            else
            {
                try
                {
                    textBox1.AppendText(msg+"\n");
                }
                catch (Exception e)
                {
                    Form1.logException("Exception while adding to Console.textBox1 msg = " + e.Message);
                }
            }
            
        }

        private void Console_Load(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Clears textBox.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }

        /// <summary>
        /// Disposing operations.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void Console_Disposed(object sender, EventArgs e)
        {
            Form1.state = (int)Form1.e_states.PortMenu;
        }
    }
}