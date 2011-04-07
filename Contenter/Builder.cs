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

    public partial class Builder
    {
        private Configuration config;
        private List<BuildItem> buildItems;

        private HashSet<BuildItem> builtItems = new HashSet<BuildItem>();
        private Queue<BuildItem> buildQueue;

        public IEnumerable<BuildItem> BuiltItems
        {
            get { return builtItems; }
        }

        public Builder(Configuration config)
        {
            this.config = config;

            buildItems = GenerateBuildItems().ToList();
        }

        public Builder Build(bool rebuildAll = false)
        {
            builtItems.Clear();
            buildQueue = new Queue<BuildItem>(buildItems);

            List<BuildException> exceptions = new List<BuildException>();

            Console.WriteLine("Building Content...");
            while (buildQueue.Count > 0)
            {
                BuildItem item = buildQueue.Dequeue();
                if (rebuildAll || item.NeedsBuilding())
                {
                    try
                    {
                        item.Build(this);
                        builtItems.Add(item);
                    }
                    catch (BuildException e)
                    {
                        exceptions.Add(e);
                        File.Delete(item.Output);
                    }
                }
            }

            if (exceptions.Any())
            {
                StringBuilder messages = new StringBuilder();
                foreach (BuildException e in exceptions)
                    messages.AppendLine(e.Message);

                throw new BuildException(messages.ToString());
            }

            return this;
        }

        public Builder GenerateHeader()
        {
            string manifestFilePath = Path.Combine(config.OutputPath, "Manifest.txt");

            if (!builtItems.Any() && File.Exists(manifestFilePath)) return this;

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
            string outputPath = Path.GetFullPath(config.OutputPath);

            foreach (BuildItem item in Directory.GetFiles(config.OutputPath, "*.blob").Select(file => new BuildItem(file, config, null)))
            {
                // Create the header
                headerFile.AppendLine(String.Format("        {0} = {1},", item.Name, item.ID));
                hashFile.AppendLine(String.Format("{0},{1}", item.Output.Replace(outputPath, String.Empty), item.ID));
            }
            headerFile.AppendLine("    };");
            headerFile.AppendLine("};");

            File.WriteAllText(Path.Combine(config.HeaderPath, "ContentIDs.h"), headerFile.ToString());
            File.WriteAllText(manifestFilePath, hashFile.ToString());

            return this;
        }

        private BuildTarget BuildTargetFromFile(string file)
        {
            string fileType = Path.GetExtension(file);
            return config.BuildTargets.FirstOrDefault(target => target.FileType.EndsWith(fileType));
        }

        private IEnumerable<BuildItem> BuildTargetsFromTarget(BuildTarget target)
        {
            return
                Directory.GetFiles(config.ContentPath, target.FileType, SearchOption.TopDirectoryOnly)
                .Select(file => new BuildItem(file, config, target));
        }

        private IEnumerable<BuildItem> GenerateBuildItems()
        {
            return config.BuildTargets
                .SelectMany(target => BuildTargetsFromTarget(target)).ToList()
                .OrderBy(item => item.Processor);
        }

        private void AddDependencies(List<string> dependencies)
        {
            //Root all dependency paths realative to the content path.
            dependencies = dependencies.Select(dep => Path.IsPathRooted(dep) ? dep : Path.GetFullPath(Path.Combine(config.ContentPath, dep))).ToList();

            StringBuilder errorList = new StringBuilder(String.Empty);

            foreach (string dependency in dependencies)
            {
                BuildTarget target = BuildTargetFromFile(dependency);
                if (target == null)
                    errorList.AppendLine(String.Format("Failed to find content builder for type: \"{0}\"", dependency));
                else if (!File.Exists(dependency))
                    errorList.AppendLine(String.Format("Dependency doesn't exist: \"{0}\"", dependency));
                else
                {
                    buildQueue.Enqueue(new BuildItem(dependency, config, target));
                }
            }

            if (errorList.Length > 0)
            {
                throw new BuildException(errorList.ToString());
            }
        }
    }
}
