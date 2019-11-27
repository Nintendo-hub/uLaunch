﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace uViewer
{
    public partial class ScreenshotForm : Form
    {
        private ViewerMainForm Main;

        public byte[][] CaptureBackups = new byte[][]
        {
            new byte[ViewerMainForm.RawRGBAScreenBufferSize], // Backups (5) so that new captures made by the main form don't replace old ones
            new byte[ViewerMainForm.RawRGBAScreenBufferSize],
            new byte[ViewerMainForm.RawRGBAScreenBufferSize],
            new byte[ViewerMainForm.RawRGBAScreenBufferSize],
            new byte[ViewerMainForm.RawRGBAScreenBufferSize],
        };

        public ScreenshotForm(ViewerMainForm main)
        {
            InitializeComponent();
            ViewerMainForm.InitializePictureBox(ScreenshotBox);
            Main = main;

            RefreshBackups();

            ScreenshotList.SelectedIndex = 0;
            FormatList.SelectedIndex = 0;
        }

        public void RefreshBackups()
        {
            Array.Copy(Main.CaptureBlocks[5], CaptureBackups[4], CaptureBackups[4].Length);
            Array.Copy(Main.CaptureBlocks[4], CaptureBackups[3], CaptureBackups[3].Length);
            Array.Copy(Main.CaptureBlocks[3], CaptureBackups[2], CaptureBackups[2].Length);
            Array.Copy(Main.CaptureBlocks[2], CaptureBackups[1], CaptureBackups[1].Length);
            Array.Copy(Main.CaptureBlocks[1], CaptureBackups[0], CaptureBackups[0].Length);
        }

        private void ScreenshotList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ScreenshotList.SelectedIndex >= 0)
            {
                ViewerMainForm.ApplyRGBAToPictureBox(ScreenshotBox, CaptureBackups[ScreenshotList.SelectedIndex]);
                Refresh();
            }
        }

        private void RefreshButton_Click(object sender, EventArgs e)
        {
            RefreshBackups();
            ScreenshotList.SelectedIndex = 0;
            ViewerMainForm.ApplyRGBAToPictureBox(ScreenshotBox, CaptureBackups[ScreenshotList.SelectedIndex]);
            Refresh();
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            ImageFormat fmt;
            string filter;
            switch(FormatList.SelectedIndex)
            {
                case 0:
                    fmt = ImageFormat.Png;
                    filter = "PNG image (*.png)|*.png";
                    break;
                case 1:
                    fmt = ImageFormat.Jpeg;
                    filter = "JPEG image (*.jpg)|*.jpg";
                    break;
                case 2:
                    fmt = ImageFormat.Bmp;
                    filter = "Bitmap image (*.bmp)|*.bmp";
                    break;
                default:
                    MessageBox.Show("No valid image format was selected.");
                    return;
            }
            var sfd = new SaveFileDialog
            {
                Filter = filter,
                Title = "Save foreground display screenshot",
            };
            if(sfd.ShowDialog() == DialogResult.OK)
            {
                ScreenshotBox.Image.Save(sfd.FileName, fmt);
                MessageBox.Show("Screenshot successfully saved!");
            }
        }

        private void ClipboardButton_Click(object sender, EventArgs e)
        {
            Clipboard.SetImage(ScreenshotBox.Image);
        }
    }
}
