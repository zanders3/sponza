using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;

namespace ContenterUI
{
    public partial class OutputWindow : Form
    {
        private NotifyIcon notify;
        private Action<string> menuItemClicked;
        private string firstMenuItem;

        public string Title
        {
            get { return base.Text; }
            set
            {
                base.Text = value;
                notify.Text = value;
            }
        }

        public OutputWindow(string[] menuItems, string title, Action<string> menuItemClicked)
        {
            InitializeComponent();

            TopMost = true;
            ShowInTaskbar = false;
            this.Left = Screen.PrimaryScreen.WorkingArea.Right - this.Width;
            this.Top = Screen.PrimaryScreen.WorkingArea.Bottom - this.Height;

            //Create Notification Menu
            notify = new NotifyIcon();
            notify.Visible = true;
            notify.Text = "Contenter - Idle";
            notify.Icon = Properties.Resources.Contenter;

            notify.DoubleClick += new EventHandler(notify_Click);
            notify.ContextMenu = new ContextMenu();

            foreach (string item in menuItems)
            {
                notify.ContextMenu.MenuItems.Add(new MenuItem(item, toolstripCommandClicked));
            }
            firstMenuItem = menuItems.First();
            notify.ContextMenu.MenuItems[0].DefaultItem = true;

            this.menuItemClicked = menuItemClicked;
            this.Text = title;
        }

        void notify_Click(object sender, EventArgs e)
        {
            menuItemClicked(firstMenuItem);
        }

        public void AppendOutput(string output)
        {
            if (output == null || output.Length == 0) return;

            lstOutput.Text += (output + "\n");

            lstOutput.SelectionStart = lstOutput.Text.Length;
        }

        public void Notify(string title, string text)
        {
            notify.ShowBalloonTip(100, title, text, ToolTipIcon.Info);
        }

        private void toolstripCommandClicked(object sender, EventArgs e)
        {
            MenuItem item = sender as MenuItem;
            if (item != null && item.Text != "-")
            {
                menuItemClicked(item.Text);
            }
        }

        private void OutputWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                Visible = false;
                e.Cancel = true;
            }
        }
    }
}
