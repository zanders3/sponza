﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading;
using System.Xml.Serialization;

namespace Builder.Model
{
    /// <summary>
    /// Represents a Builder Plugin.
    /// Builds ContentItems and tracks plugin write changes.
    /// </summary>
    public class BuilderItem
    {
        private string m_builderPath;
        private DateTime m_builderBuiltTime;

        public string Path { get { return m_builderPath; } }

        public BuilderItem()
        {
        }

        [XmlAttribute("Path")]
        public string BuilderPath
        {
            get { return m_builderPath; }
            set { m_builderPath = value; }
        }

        [XmlAttribute("LastBuiltTime")]
        public DateTime LastBuildTime
        {
            get { return m_builderBuiltTime; }
            set { m_builderBuiltTime = value; }
        }

        public BuilderItem(string builderPath)
        {
            m_builderPath = builderPath;
            m_builderBuiltTime = File.GetLastWriteTime(builderPath);
        }

        public bool UpToDate()
        {
            return m_builderBuiltTime == File.GetLastWriteTime(m_builderPath);
        }

        /// <summary>
        /// Builds content synchronously using the resource path and output path.
        /// </summary>
        public bool Build(string resourcePath, string outputPath, ref List<string> dependencies, ref List<string> output)
        {
            m_builderBuiltTime = File.GetLastWriteTime(m_builderPath);

            string outputDirectory = System.IO.Path.GetDirectoryName(outputPath);
            if (!Directory.Exists(outputDirectory))
            {
                Directory.CreateDirectory(outputDirectory);
            }

            using (Process process = new Process())
            {
                process.StartInfo.FileName = m_builderPath;
                process.StartInfo.Arguments = resourcePath + " " + outputPath;
                process.StartInfo.ErrorDialog = false;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardInput = true;
                process.StartInfo.UseShellExecute = false;

                process.Start();
                process.WaitForExit();

                output.AddRange(process.StandardOutput.ReadToEnd().Split('\n'));
                output.AddRange(process.StandardError.ReadToEnd().Split('\n'));

                dependencies.AddRange(output.Where(line => line.StartsWith("DEPENDENCY ")).Select(line => line.Substring("DEPENDENCY ".Length)));

                return process.ExitCode == 0;
            }
        }
    }
}
