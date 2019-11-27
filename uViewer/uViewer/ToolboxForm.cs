﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace uViewer
{
    public partial class ToolboxForm : Form
    {
        private ViewerMainForm main;

        public ToolboxForm(ViewerMainForm Main)
        {
            InitializeComponent();
            main = Main;
            IncrementNumeric.Increment = 0.1m;
            IncrementNumeric.Minimum = 0.1m;
            IncrementNumeric.Value = 1.0m;
        }

        private void ScreenshotButton_Click(object sender, EventArgs e)
        {
            new ScreenshotForm(main).ShowDialog();
        }

        private void ResizeButton_Click(object sender, EventArgs e)
        {
            var factor = IncrementNumeric.Value;
            int w = (int)(1296 * factor);
            int h = (int)(759 * factor);
            main.Width = w;
            main.Height = h;
        }
    }
}
