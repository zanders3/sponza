using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;

namespace TextureProcessor
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine(Path.GetFullPath("Builders\\nvdxt.exe"));

                Process process = Process.Start("Builders\\nvdxt.exe", " -file " + args[0]);
                process.WaitForExit();


                string sourcePath = Path.GetFullPath(Path.GetFileNameWithoutExtension(args[0]) + ".dds");
                string destinationPath = Path.GetFullPath(args[1]);
                Console.WriteLine(sourcePath);

                if (File.Exists(destinationPath)) 
                    File.Delete(destinationPath);

                File.Move(sourcePath, args[1]);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                Environment.ExitCode = 1;
            }
        }
    }
}
