using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ContenterUI
{
    public class UIController
    {
        private OutputWindow window;
        private ContenterWrapper listen;
        private ContenterWrapper build;
        private bool wasVisible = false;

        public UIController()
        {
            window = new OutputWindow(new string[]
            {
                "Show Output Window",
                "-",
                "Build",
                "Rebuild",
                "Clean",
                "-",
                "Exit"
            }, 
            "Contenter - Idle",
            menuItemClicked);

            listen = new ContenterWrapper(listenerOutput);
            listen.Run("Listen");

            build = new ContenterWrapper(consoleOutput);
        }

        private void menuItemClicked(string item)
        {
            wasVisible = window.Visible;

            switch (item)
            {
                case "Show Output Window":
                    window.Visible = true;
                    break;

                case "Exit":
                    listen.Close();
                    build.Close();

                    Application.Exit();
                    break;
                default:
                    window.Text = "Contenter - " + item + "ing...";
                    window.Visible = true;
                    consoleOutput(item + "ing...", false);
                    build.Run(item);
                    break;
            }
        }

        private void listenerOutput(string output, bool completed)
        {
            window.AppendOutput(output);
            if (output.StartsWith("LOAD"))
            {
                window.Notify("Reloaded Content", output.Substring(5));
            }
            if (output.StartsWith("Client"))
            {
                window.Notify("Contenter", output);
            }
        }

        private void consoleOutput(string output, bool completed)
        {
            window.AppendOutput(output);

            if (completed)
            {
                window.Visible = build.HadError || wasVisible;
                window.Text = "Contenter - Idle";
                consoleOutput("Completed", false);
            }
        }
    }
}
