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

namespace Contenter
{
    public class Builder
    {
        public class BuildItem
        {
            public string Processor;
            public string Name;
            public string Input;
            public string Output;

            public bool NeedsBuilding()
            {
                return File.GetLastWriteTime(Output) < File.GetLastWriteTime(Input);
            }

            public void Build()
            {
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
                    throw new Exception(String.Format("Item Build Failed: {0} \"{1}\" -> \"{2}\"\n{3}{4}",
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
        private Dictionary<BuildItem, uint> idMap = null;

        public Dictionary<BuildItem, uint> IDMap
        {
            get
            {
                if (idMap == null)
                {
                    idMap = new Dictionary<BuildItem, uint>();

                    uint index = 0;
                    foreach (BuildItem item in allItems.OrderBy(item => item.Name))
                    {
                        idMap.Add(item, index);
                        ++index;
                    }
                }
                return idMap;
            }
        }

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
                headerFile.AppendLine(String.Format("        {0},", item.Name));
                hashFile.AppendLine(item.Output.Replace(config.OutputPath, String.Empty));
            }

            headerFile.AppendLine("        CONTENT_MAX");
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
                        Processor = target.Program
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
