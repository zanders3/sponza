using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ScriptProcessor
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 2 && args[0].EndsWith(".as"))
            {
                try
                {
                    if (File.Exists(args[1]))
                    {
                        File.Delete(args[1]);
                    }

                    File.Copy(args[0], args[1]);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    Environment.ExitCode = 1;
                }
            }
            else
            {
                Console.WriteLine("Usage: [source file].as [destination file]");
                Environment.ExitCode = 1;
            }
        }
    }
}
