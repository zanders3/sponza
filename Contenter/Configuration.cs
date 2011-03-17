using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.IO;

namespace Contenter
{
    public class BuildTarget
    {
        public string Program;
        public string FileType;
    }

    [Serializable]
    public class Configuration
    {
        public const string ConfigurationFilePath = "ContenterConfig.xml";

        public string ContentPath = " ";
        public string OutputPath  = " ";
        public string HeaderPath  = " ";

        public List<BuildTarget> BuildTargets = new List<BuildTarget>();

        public static Configuration LoadConfiguration()
        {
            XmlSerializer xmlSer = new XmlSerializer(typeof(Configuration));

            if (File.Exists(ConfigurationFilePath))
            {
                using (FileStream fs = new FileStream(ConfigurationFilePath, FileMode.Open))
                {
                    try
                    {
                        return (Configuration)xmlSer.Deserialize(fs);
                    }
                    catch (Exception)
                    {
                        throw new Exception("XML invalid: Delete the file to regenerate a correctly formatted blank version.");
                    }
                }
            }
            else
            {
                using (FileStream fs = new FileStream(ConfigurationFilePath, FileMode.Create))
                {
                    xmlSer.Serialize(fs, new Configuration() { BuildTargets = new List<BuildTarget>(new BuildTarget[] { new BuildTarget() { FileType = " ", Program = " " } }) });
                }
                throw new Exception("You need to add settings to the Configuration File.");
            }
        }

        public void ValidateConfiguration()
        {
            if (!Directory.Exists(ContentPath)) throw new Exception("Content Path doesn't exist!");
            if (!Directory.Exists(OutputPath)) throw new Exception("Output Path doesn't exist!");
            if (!Directory.Exists(HeaderPath)) throw new Exception("Header Path doesn't exist!");

            foreach (BuildTarget target in BuildTargets)
            {
                if (!File.Exists(target.Program)) throw new Exception(String.Format("Builder not found: \"{0}\"", target.Program));
            }
        }
    }
}
