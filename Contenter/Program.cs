using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace Contenter
{
    public static class Ports
    {
        public const int ListenerPort = 4567;
        public const int DependencyPort = 5678;
    }

    class Program
    {
        static void Main(string[] args)
        {
            string currentDirectory = Environment.CurrentDirectory;

            try
            {
                string startupPath = Environment.CommandLine.Split(' ').First().Trim('"', ' ', '\\');
                if (File.Exists(startupPath))
                    Environment.CurrentDirectory = Path.GetDirectoryName(startupPath);

                if (args.Length == 0)
                {
                    PrintHelp();
                    return;
                }

                // Load the configuration file
                Configuration config = null;
                try
                {
                    config = Configuration.LoadConfiguration();
                    config.ValidateConfiguration();
                }
                catch (Exception e)
                {
                    Console.WriteLine(String.Format("Configuration Error:\n{0}", Path.GetFullPath(Configuration.ConfigurationFilePath)));
                    Console.WriteLine();
                    Console.WriteLine(e.Message);

                    Environment.Exit(1);
                }

                // Parse the command line
                switch (args[0].ToLower())
                {
                    case "build":
                        new Builder(config).Build().GenerateHeader();
                        break;
                    case "rebuild":
                        new Builder(config).Build(true).GenerateHeader();
                        break;
                    case "listen":
                        new Listener(config).Listen();
                        break;
                    case "clean":
                        string[] files = Directory.GetFiles(config.OutputPath, "*.blob");
                        foreach (string file in files)
                            File.Delete(file);

                        File.Delete(Path.Combine(config.OutputPath, "Manifest.txt"));
                        break;
                    default:
                        PrintHelp();
                        break;
                }
            }
            catch (BuildException e)
            {
                Console.WriteLine(e.Message);

                Environment.Exit(1);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unhandled Error:");
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);

                Environment.Exit(1);
            }
            finally
            {
                Environment.CurrentDirectory = currentDirectory;
            }
        }

        private static void PrintHelp()
        {
            Console.WriteLine("Contenter");
            Console.WriteLine();
            Console.WriteLine("Commands:");
            Console.WriteLine("     Build   - Builds content");
            Console.WriteLine("     Rebuild - Forces a full content rebuild");
            Console.WriteLine("     Clean   - Cleans the content output directory");
            Console.WriteLine("     Listen  - Listens for content changes then builds and notifies the game.");
        }
    }
}
