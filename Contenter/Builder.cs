using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;

namespace Contenter
{
    public class BuildException : Exception
    {
        public BuildException(string message) : base(message)
        {
        }
    }

    public class Builder
    {
        [DllImport("ContentHasher.dll")]
        static extern uint HashString(String hash);

        public class BuildItem
        {
            public string Processor;
            public string Name;
            public string Input;
            public string Output;
            public uint ID;

            public bool NeedsBuilding()
            {
                return File.GetLastWriteTime(Output) < File.GetLastWriteTime(Input);
            }

            private bool IsBuilt = false;

            public void Build()
            {
                if (IsBuilt) return;
                IsBuilt = true;

                Process process = new Process();
                process.StartInfo.FileName = Processor;
                process.StartInfo.Arguments = String.Format("\"{0}\" \"{1}\"", Input, Output);
                process.StartInfo.ErrorDialog = false;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.UseShellExecute = false;

                try
                {
                    process.Start();
                    process.WaitForExit();
                }
                catch (Exception e)
                {
                    throw new Exception(String.Format("Could not run: \"{0}\"\n{1}", Processor, e.Message));
                }

                if (process.ExitCode != 0)
                {
                    throw new BuildException(String.Format("Item Build Failed: {0} \"{1}\" -> \"{2}\"\n{3}{4}",
                        Processor,
                        Input,
                        Output,
                        process.StandardOutput.ReadToEnd(),
                        process.StandardError.ReadToEnd()));
                }

                Console.WriteLine(String.Format("{0} -> {1}", Path.GetFileName(Input), Path.GetFileName(Output)));
            }
        }

        private Configuration config;
        private List<BuildItem> allItems;
        private List<BuildItem> rebuiltItems;

        public IEnumerable<BuildItem> RebuiltItems
        {
            get { return rebuiltItems; }
        }

        public IEnumerable<BuildItem> AllItems
        {
            get { return allItems; }
        }

        public Builder(Configuration config)
        {
            this.config = config;

            allItems = GenerateBuildItems();
            rebuiltItems = allItems.Where(item => item.NeedsBuilding()).ToList();
        }

        public Builder Build(bool rebuildAll = false)
        {
            List<BuildItem> buildItems = rebuildAll ? allItems : rebuiltItems;

            if (buildItems.Any())
            {
                Console.WriteLine("Building Content...");
                foreach (BuildItem item in buildItems)
                    item.Build();
            }
            return this;
        }

        public Builder GenerateHeader()
        {
            if (!allItems.Any()) return this;

            Console.WriteLine("Generating Header...");
            StringBuilder headerFile = new StringBuilder();
            StringBuilder hashFile = new StringBuilder();

            DateTime now = DateTime.Now;

            headerFile.AppendLine("#pragma once");
            headerFile.AppendLine("//---------------------------------------------------------------------------");
            headerFile.AppendLine("//Auto Generated Contenter Content ID Header");
            headerFile.AppendLine(String.Format("//Created At: {0} {1}", now.ToShortDateString(), now.ToShortTimeString()));
            headerFile.AppendLine("//---------------------------------------------------------------------------");
            headerFile.AppendLine();
            headerFile.AppendLine("struct ContentID");
            headerFile.AppendLine("{");
            headerFile.AppendLine("    enum Type : unsigned int");
            headerFile.AppendLine("    {");

            HashSet<uint> usedHashes = new HashSet<uint>();

            foreach (BuildItem item in allItems.OrderBy(item => item.Name))
            {
                // Create the header
                headerFile.AppendLine(String.Format("        {0} = {1},", item.Name, item.ID));
                hashFile.AppendLine(String.Format("{0},{1}", item.Output.Replace(config.OutputPath, String.Empty), item.ID));
            }
            headerFile.AppendLine("    };");
            headerFile.AppendLine("};");

            File.WriteAllText(Path.Combine(config.HeaderPath, "ContentIDs.h"), headerFile.ToString());
            File.WriteAllText(Path.Combine(config.OutputPath, "Manifest.txt"), hashFile.ToString());

            return this;
        }

        private List<BuildItem> GenerateBuildItems()
        {
            string[] files = Directory.GetFiles(config.ContentPath);
            List<BuildItem> buildItems = new List<BuildItem>();

            foreach (BuildTarget target in config.BuildTargets)
            {
                Regex regex = WildcardToRegex(target.FileType);

                foreach (string file in files.Where(file => regex.IsMatch(file)))
                {
                    buildItems.Add(new BuildItem()
                    {
                        Input = file,
                        Output = Path.Combine(config.OutputPath, Path.GetFileNameWithoutExtension(file) + ".blob"),
                        Name = Path.GetFileNameWithoutExtension(file).ToUpper(),
                        Processor = target.Program,
                        ID = HashString(Path.GetFileNameWithoutExtension(file))
                    });
                }
            }

            buildItems = buildItems.OrderBy(item => item.Processor).ToList();

            return buildItems;
        }

        private static Regex WildcardToRegex(string pattern)
        {
            return new Regex("^" + Regex.Escape(pattern).
            Replace("\\*", ".*").
            Replace("\\?", ".") + "$");
        }
    }
}
