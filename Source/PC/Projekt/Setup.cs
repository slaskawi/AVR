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
    /// This class is used to set up avr properties and write it to chip.
    /// </summary>
    public partial class Setup : Form
    {
        Form1 m_parentForm;
        public WaitingWindow_ m_waitingWindow;

        public Setup()
        {
            InitializeComponent();

            DateTime now = DateTime.Now;
            textBox1.Text = now.Hour.ToString();
            textBox2.Text = now.Minute.ToString();
            textBox3.Text = now.Second.ToString();

            textBox6.Text = now.Day.ToString();
            textBox6.Text = now.Month.ToString();
            textBox6.Text = now.Year.ToString();
        }

        /// <summary>
        /// Public constructor. Initializes necessary components.
        /// </summary>
        /// <param name="f"></param>
        public Setup(Form1 f)
        {
            InitializeComponent();
            m_parentForm = f;
            this.Disposed += new EventHandler(Setup_Disposed);
            m_waitingWindow = new WaitingWindow_();
        }

        /// <summary>
        /// Used to send time settings.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            int h, m, s;
            try
            {
                h = Int32.Parse(textBox1.Text);
                m = Int32.Parse(textBox2.Text);
                s = Int32.Parse(textBox3.Text);

                if (h < 0 || h > 24)
                    throw new Exception("Number of hours is incorrect");
                if (m < 0 || m > 60)
                    throw new Exception("Number of minutes is incorrect");
                if(s < 0 || s > 60)
                    throw new Exception("Number of seconds is incorrect");

                StringBuilder sb = new StringBuilder();
                if (h < 10)
                    sb.Append("0");
                sb.Append(h);

                sb.Append(":");

                if (m < 10)
                    sb.Append("0");
                sb.Append(m);

                sb.Append(":");

                if (s < 10)
                    sb.Append("0");
                sb.Append(s);

                m_parentForm.sendDataViaPort("timeset!");
                m_parentForm.sendDataViaPort(sb.ToString());
                m_parentForm.sendDataViaPort("exit!");

            }
            catch (Exception a)
            {
                paintError("Please enter valid time");
            }

            
        }

        /// <summary>
        /// Used to send date settings.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            int d, m, y;
            try
            {
                d = Int32.Parse(textBox6.Text);
                m = Int32.Parse(textBox5.Text);
                y = Int32.Parse(textBox4.Text);

                if (d < 0 || d > 31)
                    throw new Exception();
                if (m < 0 || m > 12)
                    throw new Exception();
                if (y < 2000 || y > 2030)
                    throw new Exception();

                StringBuilder sb = new StringBuilder();
                if (d < 10)
                    sb.Append("0");
                sb.Append(d);

                sb.Append(":");

                if (m < 10)
                    sb.Append("0");
                sb.Append(m);

                sb.Append(":");

                sb.Append(y);

                m_parentForm.sendDataViaPort("dataset!");
                m_parentForm.sendDataViaPort(sb.ToString());
                m_parentForm.sendDataViaPort("exit!");

            }
            catch (Exception a)
            {
                paintError("Please enter valid date");
            }
        }

        /// <summary>
        /// Used to send temperature settings.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click(object sender, EventArgs e)
        {
            int temp1 = 0;
            int temp2 = 0;
            int offset = 0;

            try
            {
                temp1 = Int32.Parse(textBox7.Text);
                temp2 = Int32.Parse(textBox8.Text);
                offset = Int32.Parse(textBox9.Text);

                if (temp1 < 0 || temp2 < 0)
                    MessageBox.Show("Are you sure it's freezing out there!");

                StringBuilder sb = new StringBuilder();
                
                if (temp1 < 0)
                    sb.Append("0");
                sb.Append(temp1);

                sb.Append(":");

                if (temp2 < 0)
                    sb.Append("0");
                sb.Append(temp2);

                sb.Append(":");

                if (offset < 0)
                    sb.Append("0");
                sb.Append(offset);

                m_parentForm.sendDataViaPort("settemp!");
                m_parentForm.sendDataViaPort(sb.ToString());
                m_parentForm.sendDataViaPort("exit!");
                
            }
            catch (Exception y)
            {
                paintError("Please enter correct Temperature");
            }

        }

        /// <summary>
        /// Send all
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button4_Click(object sender, EventArgs e)
        {
            try
            {
                this.button1_Click(null, null);
                System.Threading.Thread.Sleep(1000);
                this.button2_Click(null, null);
                System.Threading.Thread.Sleep(1000);
                this.button3_Click(null, null);
                System.Threading.Thread.Sleep(1000);
                this.button5_Click(null, null);
                System.Threading.Thread.Sleep(1000);
                this.button6_Click(null, null);   
            }
            catch (Exception a)
            {
                paintError("Something gone wrong - try to do it manually");
            }
        }

        /// <summary>
        /// Loads default values into fields.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Setup_Load(object sender, EventArgs e)
        {
            DateTime currentDate = DateTime.Now;
            textBox1.Text = "" + currentDate.Hour;
            textBox2.Text = "" + currentDate.Minute;
            textBox3.Text = "" + currentDate.Second;
            textBox6.Text = "" + currentDate.Day;
            textBox5.Text = "" + currentDate.Month;
            textBox4.Text = "" + currentDate.Year;
            textBox7.Text = "" + 20;
            textBox8.Text = "" + 18;
            textBox9.Text = "" + 0;
            textBox10.Text = "" + 20;
            textBox11.Text = "" + 12;
        }

        /// <summary>
        /// Disposing operations.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void Setup_Disposed(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("exit!");
            Form1.state = (int)Form1.e_states.PortMenu;
        }

        /// <summary>
        /// Displays error message to user.
        /// </summary>
        /// <param name="message"></param>
        public void paintError(String message)
        {
            MessageBox.Show("ERROR \n" + message);
        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Sends night settings.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button5_Click(object sender, EventArgs e)
        {
            int hour1, hour2;
            try
            {
                hour1 = Int32.Parse(textBox10.Text);
                hour2 = Int32.Parse(textBox11.Text);

                if (hour1 < 0 || hour1 > 24)
                    throw new Exception();
                if(hour2 < 0 || hour2 > 24)
                    throw new Exception();

                StringBuilder sb = new StringBuilder();
                if (hour1 < 10)
                    sb.Append("0");
                sb.Append(hour1);

                sb.Append(":");

                if (hour2 < 10)
                    sb.Append("0");
                sb.Append(hour2);

                m_parentForm.sendDataViaPort("ttsetup!");
                m_parentForm.sendDataViaPort(sb.ToString());
                m_parentForm.sendDataViaPort("exit!");

            }
            catch (Exception a)
            {
                paintError("Please enter correct hours");
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            try
            {
                if (radioButton1.Checked == true)
                {
                    m_parentForm.sendDataViaPort("onwork!");
                }
                else
                {
                    m_parentForm.sendDataViaPort("offwork!");
                }

                m_parentForm.sendDataViaPort("exit!");

            }
            catch (Exception a)
            {
                paintError("Radio buttons sending failed");
            }
        }

    }
}