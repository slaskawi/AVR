using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Projekt
{
    public partial class Settings : Form
    {
        private Form1 m_parentForm;
        delegate void SetTextCallBack(String text);

        public Settings()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Public constructor. Initializes necessary components.
        /// </summary>
        /// <param name="f"></param>
        public Settings(Form1 f)
        {
            InitializeComponent();
            m_parentForm = f;
            this.Disposed += new EventHandler(Settings_Disposed);
        }

        private void Settings_Load(object sender, EventArgs e)
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
                    char[] arr = msg.ToCharArray();
                    int indexer = 0;
                    StringBuilder sb = new StringBuilder();
                    for (int i = 0; i < arr.Length; i++)
                    {
                        if (arr[i] != '!')
                        {
                            sb.Append(arr[i]);
                        }
                        else
                        {
                            switch (indexer)
                            {
                                case 0:
                                    {
                                        textBox1.Text = sb.ToString();
                                        break;
                                    }

                                case 1:
                                    {
                                        textBox2.Text = sb.ToString();
                                        break;
                                    }

                                case 2:
                                    {
                                        textBox3.Text = sb.ToString();
                                        break;
                                    }

                                case 3:
                                    {
                                        textBox4.Text = sb.ToString();
                                        break;
                                    }

                                case 4:
                                    {
                                        textBox5.Text = sb.ToString();
                                        break;
                                    }

                                case 5:
                                    {
                                        if(sb.ToString().Equals("0"))
                                        {
                                            radioButton1.Enabled = true;
                                            radioButton1.Select();
                                            radioButton2.Enabled = false;
                                        }
                                        else
                                        {
                                            radioButton1.Enabled = false;
                                            radioButton2.Enabled = true;
                                            radioButton2.Select();
                                        }
                                        break;
                                    }
                            }

                            sb = null;
                            sb = new StringBuilder();
                            indexer++;
                        }
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
        private void Settings_Disposed(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("exit!");
            Form1.state = (int)Form1.e_states.PortMenu;
        }

        /// <summary>
        /// Check button action.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("settings!");
        }

    }
}