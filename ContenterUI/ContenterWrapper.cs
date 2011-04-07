using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using System.ComponentModel;

namespace ContenterUI
{
    public class ContenterWrapper
    {
        private Action<string, bool> consoleOutput;

        private Process process = null;
        private BackgroundWorker worker;
        private bool hadError = false;

        public bool HadError
        {
            get { return hadError; }
        }

        public ContenterWrapper(Action<string, bool> consoleOutput)
        {
            this.consoleOutput = consoleOutput;
            this.worker = new BackgroundWorker();
            worker.DoWork += new DoWorkEventHandler(worker_DoWork);
            worker.WorkerReportsProgress = true;
            worker.ProgressChanged += new ProgressChangedEventHandler(worker_ProgressChanged);
            worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(worker_RunWorkerCompleted);
        }

        void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            consoleOutput(string.Empty, true);
            process = null;
        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            string args = (string)e.Argument;

            if (process == null)
            {
                hadError = false;

                process = new Process();
                process.StartInfo.FileName = Path.GetFullPath("Contenter.exe");
                process.StartInfo.Arguments = (string)args;
                process.StartInfo.ErrorDialog = false;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardInput = true;
                process.StartInfo.UseShellExecute = false;
                process.OutputDataReceived += new DataReceivedEventHandler(process_OutputDataReceived);
                process.ErrorDataReceived += new DataReceivedEventHandler(process_ErrorDataReceived);

                try
                {
                    process.Start();
                    process.BeginOutputReadLine();
                    process.BeginErrorReadLine();

                    process.WaitForExit();
                }
                catch (Exception ex)
                {
                    worker.ReportProgress(0, "Exception: " + ex.Message);
                }
            }
        }

        void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            consoleOutput((string)e.UserState, false);
        }

        public void Close()
        {
            if (process != null)
            {
                try
                {
                    process.StandardInput.WriteLine();
                }
                catch
                {
                }
                process.WaitForExit(100);

                if (!process.HasExited)
                {
                    try
                    {
                        process.Kill();
                    }
                    catch
                    {
                    }
                }
            }
        }

        void process_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            worker.ReportProgress(0, e.Data);
        }

        void process_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            worker.ReportProgress(0, e.Data);
        }

        public void Run(string args)
        {
            if (process == null)
            {
                worker.RunWorkerAsync(args);
            }
        }
    }
}
