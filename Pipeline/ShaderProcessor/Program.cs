using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace ShaderProcessor
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 2 && (args[0].EndsWith(".fx") || args[0].EndsWith(".fxh")))
            {
                string resourcePath = args[0];
                string destinationPath = args[1];
                string parameters = string.Empty; //args[0].EndsWith(".fx") ? "/Gch" : string.Empty;

                Process process = new Process();
                process.StartInfo.FileName = "fxc";
                process.StartInfo.Arguments = string.Format("{0} /T fx_4_0 \"{1}\" /Fo \"{2}\"", parameters, resourcePath, destinationPath);
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.ErrorDialog = false;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.UseShellExecute = false;
                process.ErrorDataReceived += new DataReceivedEventHandler(process_ErrorDataReceived);
                process.OutputDataReceived += new DataReceivedEventHandler(process_OutputDataReceived);

                process.Start();
                process.BeginErrorReadLine();
                process.BeginOutputReadLine();

                process.WaitForExit();
                Environment.ExitCode = process.ExitCode;

                process.Close();
            }
            else
            {
                Console.WriteLine("Usage: ShaderProcessor.exe input.fx output.dat");
            }
        }

        static void process_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.WriteLine(e.Data);
        }

        static void process_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.WriteLine(e.Data);
        }
    }
}
