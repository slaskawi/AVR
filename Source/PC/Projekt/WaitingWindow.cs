using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace Projekt
{
    public partial class WaitingWindow_ : Form
    {
        private Form1 m_parentForm;
        private Point m_location;
        

        public WaitingWindow_()
        {
            InitializeComponent();
            m_location = new Point();
            
            progressBar1.Step = 1;
            progressBar1.Maximum = 100;
            progressBar1.Minimum = 0;
            Rectangle r = Screen.PrimaryScreen.Bounds;
            int h = r.Height;
            int w = r.Width;

            m_location.X = (w - this.Bounds.Width) / 2;
            m_location.Y = (h - this.Bounds.Height) / 2;

            progressBar1.MarqueeAnimationSpeed = 10;
        }

        public WaitingWindow_(Form1 f)
        {
            

        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }

        public void start()
        {   
            this.Show();
            this.Location = m_location;
        }

        public void stop()
        {
            this.Hide();
        }

        public void run()
        {
           
        }
    }
}