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

            public void Build(Builder builder)
            {
                if (IsBuilt) return;

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

                builder.ValidateDependencies(process.StandardOutput.ReadToEnd()
                    .Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries)
                    .Select(line =>
                        line.Replace("DEPENDENCY ", string.Empty).Trim()).ToList());

                IsBuilt = true;
            }            
        }

        private Configuration config;
        private List<BuildItem> allItems;
        private List<BuildItem> rebuildItems;

        public IEnumerable<BuildItem> RebuiltItems
        {
            get { return rebuildItems; }
        }

        public IEnumerable<BuildItem> AllItems
        {
            get { return allItems; }
        }

        public Builder(Configuration config)
        {
            this.config = config;

            allItems = GenerateBuildItems();
            rebuildItems = allItems.Where(item => item.NeedsBuilding()).ToList();
        }

        public Builder Build(bool rebuildAll = false)
        {
            List<BuildItem> buildItems = rebuildAll ? allItems : rebuildItems;

            if (buildItems.Any())
            {
                Console.WriteLine("Building Content...");
                foreach (BuildItem item in buildItems)
                    item.Build(this);
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

        private IEnumerable<BuildItem> BuildTargetsFromTarget(BuildTarget target)
        {
            return
                Directory.GetFiles(config.ContentPath, target.FileType, SearchOption.AllDirectories)
                .Select(file => new BuildItem()
                {
                    Input = file,
                    Output = Path.Combine(config.OutputPath, Path.GetFileNameWithoutExtension(file) + ".blob"),
                    Name = Path.GetFileNameWithoutExtension(file).ToUpper(),
                    Processor = target.Program,
                    ID = HashString(Path.GetFileNameWithoutExtension(file))
                });
        }

        private List<BuildItem> GenerateBuildItems()
        {
            return config.BuildTargets
                .SelectMany(target => BuildTargetsFromTarget(target)).ToList()
                .OrderBy(item => item.Processor).ToList();
        }

        private void ValidateDependencies(List<string> dependencies)
        {
            HashSet<string> itemList = new HashSet<string>(allItems.Select(item => Path.GetFullPath(item.Input)));

            var list = dependencies.Where(dep => 
                !itemList.Contains(dep));
            if (list.Any())
            {
                string rootedContentPath = Path.GetFullPath(config.ContentPath);

                StringBuilder errorMessage = new StringBuilder("Build Failed due to missing dependencies:\n");
                foreach (string item in list)
                {
                    errorMessage.AppendLine(item.Replace(rootedContentPath, string.Empty));
                }

                throw new BuildException(errorMessage.ToString());
            }
        }
    }
}
