using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace meteo
{
    public partial class splashScreenForm : Form
    {
        public splashScreenForm()
        {
            InitializeComponent();

        }

        private void splashScreenForm_Load(object sender, EventArgs e)
        {

        }
      

        private void splashScreenStartButton_Click(object sender, EventArgs e)
        {
            mainForm mainForm = new mainForm();
            mainForm.Show();

            mainForm.FormClosed += (s, args) => { Application.Exit(); };
            mainForm.Show();
            this.Hide();
        }


    

        private void splashScreenExitButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            mainForm mainForm = new mainForm();
            mainForm.FormClosed += (s, args) => { Application.Exit(); };
            mainForm.Show();
            this.Hide();
        }

        private void buttonExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
