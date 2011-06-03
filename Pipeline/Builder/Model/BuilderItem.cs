using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Builder.Model
{
    /// <summary>
    /// Represents a Builder Plugin.
    /// Builds ContentItems and tracks plugin write changes.
    /// </summary>
    [Serializable]
    public class BuilderItem
    {
        private string m_builderPath;
        private DateTime m_builderBuiltTime;

        public string Path { get { return m_builderPath; } }

        public BuilderItem()
        {
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

        public void Build(string resourcePath, string outputPath, Action<bool, List<string>> buildCompleted)
        {
            //TODO!
        }

        
    }
}
