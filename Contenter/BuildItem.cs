using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Contenter
{
    public partial class Builder
    {
        public class BuildItem
        {
            [DllImport("ContentHasher.dll")]
            private static extern uint HashString(String hash);

            public string Processor;
            public string Name;
            public string Input;
            public string Output;

            public string HashID;
            public uint ID { get { return HashString(HashID); } }

            public bool NeedsBuilding()
            {
                return File.GetLastWriteTime(Output) < File.GetLastWriteTime(Input);
            }

            private bool IsBuilt = false;

            public BuildItem(string file, Configuration config, BuildTarget target)
            {
                Input = Path.GetFullPath(file);
                Output = Path.GetFullPath(Path.Combine(config.OutputPath, Path.GetFileNameWithoutExtension(file) + ".blob"));
                Name = Path.GetFileNameWithoutExtension(file).ToUpper();
                Processor = target != null ? target.Program : string.Empty;

                HashID = Path.GetFileNameWithoutExtension(file);
            }

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

                builder.AddDependencies(process.StandardOutput.ReadToEnd()
                    .Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries)
                    .Select(line =>
                        line.Replace("DEPENDENCY ", string.Empty).Trim()).ToList());

                IsBuilt = true;
            }

            public override string ToString()
            {
                return Name;
            }
        }
    }
}
