using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Projekt
{

    /// <summary>
    /// This class is used for temperature debugging.
    /// </summary>
    public partial class Temperature : Form
    {
        private Form1 m_parentForm;
        private Boolean m_autoCheck = false;
        delegate void SetTextCallBack(String text);

        public Temperature()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Public constructor. Initializes necessary components.
        /// </summary>
        /// <param name="f"></param>
        public Temperature(Form1 f)
        {
            InitializeComponent();
            m_parentForm = f;
            this.Disposed += new EventHandler(Temperature_Disposed);
        }

        private void Temperature_Load(object sender, EventArgs e)
        {
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
                    textBox1.Text = msg;

                    if (m_autoCheck)
                    {
                        m_parentForm.sendDataViaPort("temp!");
                    }

                }
                catch (Exception e)
                {
                    Form1.logException("Exception while adding to Console.textBox1 msg = " + e.Message);
                }
            }

        }

        /// <summary>
        /// Disposing operations.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void Temperature_Disposed(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("exit!");
            Form1.state = (int)Form1.e_states.PortMenu;
        }

        /// <summary>
        /// Turn off autocheck.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            button3.Enabled = false;
            m_autoCheck = true;
            m_parentForm.sendDataViaPort("temp!");
        }

        /// <summary>
        /// Turn on autocheck.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            button3.Enabled = true;
            m_autoCheck = false;

        }

        /// <summary>
        /// Manual check.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("temp!");
        }
    }
}