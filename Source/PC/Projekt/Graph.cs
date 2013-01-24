using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ZedGraph;
using System.Xml;

namespace Projekt
{

    /// <summary>
    /// This class is responsible for drawing temperature = f (time) chart.
    /// </summary>
    public partial class Graph : Form
    {
        public const String X_AXIS_NAME = "Date";
        public const String Y_AXIS_NAME = "Temperature [C]";
        public const String TITLE = "Temperature ( Date )";
        public const String CURVE_NAME = "TEMPERATURE";

        public const String ROOT = "TEMPERATURE";
        public const String ELEMENT = "TEMP";
        public const String ATTRIBUTE = "Date";

        private GraphPane m_pane;
        private LineItem m_tempLine;
        private PointPairList m_pairList;

        private Form1 m_parentForm;
        delegate void SetTextCallBack(String text);

        private bool m_check = false;
       

        /// <summary>
        /// Public constructor.
        /// Initializes form and graph.
        /// </summary>
        /// <param name="f"></param>
        public Graph(Form1 f)
        {
            InitializeComponent();
            m_parentForm = f;
            this.Disposed += new EventHandler(Graph_Disposed);

            m_pane = zedGraphControl1.GraphPane;

            m_pane.Title.Text = TITLE;
            m_pane.XAxis.Title.Text = X_AXIS_NAME;
            m_pane.YAxis.Title.Text = Y_AXIS_NAME;
            m_pane.XAxis.Type = AxisType.Date;

            m_pairList = new PointPairList();

            m_tempLine = m_pane.AddCurve(CURVE_NAME,
                  m_pairList, Color.Red, SymbolType.None);


            zedGraphControl1.AxisChange();            
        }


        private void zedGraphControl1_Load(object sender, EventArgs e)
        {

        }

        
        /// <summary>
        /// Autocheck button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            m_check = true;
            m_parentForm.sendDataViaPort("temp!");
        }


        /// <summary>
        /// Stop checking.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            m_check = false;
        }

        /// <summary>
        /// Generate XML.
        /// This method uses DOM to fill and save XML file.
        /// The chart date is converted to narmal date while saving.
        /// Below is used schema:
        /// [year,month,day,hour,minute,second]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "XML Files (*.xml)|*.xml";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                System.IO.FileStream fs = null;
                XmlTextWriter writer = null;

                try
                {
                    XmlDocument doc = new XmlDocument();
                    doc.AppendChild(doc.CreateXmlDeclaration("1.0", "UTF-8", null));
                    XmlElement element = doc.CreateElement(ROOT);
                    doc.AppendChild(element);

                    int day, month, year, hour, minute, second;
                    double x, y;
                    foreach (PointPair p in m_pairList)
                    {
                        x = p.X;
                        XDate.XLDateToCalendarDate(x, out year, out month, out day, out hour, out minute, out second);
                        y = p.Y;

                        XmlElement node = doc.CreateElement(ELEMENT);
                        node.SetAttribute(ATTRIBUTE, "[" + year + "," + month + "," + day + "," + hour + "," + minute + "," + second + "]");
                        node.InnerText = "" + y;
                        element.AppendChild((XmlNode)node);

                    }

                    fs = new System.IO.FileStream(sfd.FileName, System.IO.FileMode.Create);
                    writer = new XmlTextWriter(fs, new UTF8Encoding());
                    writer.Formatting = Formatting.Indented;
                    doc.WriteTo(writer);
                }
                catch (Exception a)
                {
                    Form1.logException("Exception while writing xml file = " + a.Message);
                    MessageBox.Show("Generation of XML file failed!");
                }
                finally
                {
                    writer.Flush();
                    writer.Close();
                    fs.Close();
                }

            }
        }

        /// <summary>
        /// Clean up!
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void Graph_Disposed(object sender, EventArgs e)
        {
            m_parentForm.sendDataViaPort("exit!");
            Form1.state = (int)Form1.e_states.PortMenu;
        }

        /// <summary>
        /// Adds text to text box and to graph.
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
                    double x, y1;
                    DateTime dt = DateTime.Now;
                    x = new XDate(dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);

                    msg = msg.Trim();

                    //parsing to double bug fix
                    int firstPart = 0;
                    int lastPart = 0;
                    if (msg.Length == 4)
                    {
                        firstPart = Int32.Parse(msg.Substring(0, 2));
                        lastPart = Int32.Parse(msg.Substring(3, 1));
                    }
                    else if (msg.Length == 3)
                    {
                        firstPart = Int32.Parse(msg.Substring(0, 1));
                        lastPart = Int32.Parse(msg.Substring(2, 1));
                    }
                    
                    y1 = firstPart + (0.1 * lastPart);


                    m_pairList.Add(x, y1);

                    m_pane.AxisChange();
                    zedGraphControl1.AxisChange();
                    zedGraphControl1.Refresh();


                    if (m_check)
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

    }
}