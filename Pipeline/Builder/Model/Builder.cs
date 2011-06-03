using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows;
using System.Collections.ObjectModel;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace Builder.Model
{
    /// <summary>
    /// Stores and loads Builder data.
    /// </summary>
    [Serializable]
    public class BuilderConfig
    {
        public List<BuilderItem> BuilderState;
        public List<ContentItem> ContentItems;

        private static IFormatter m_formatter = new BinaryFormatter();

        public void Save(string file)
        {
            using (Stream stream = File.Create(file))
            {
                m_formatter.Serialize(stream, this);
            }
        }

        public static BuilderConfig Load(string file)
        {
            using (Stream stream = File.Open(file, FileMode.Open))
            {
                return (BuilderConfig)m_formatter.Deserialize(stream);
            }
        }
    }

    /// <summary>
    /// Tracks BuilderItems and their supported file extensions.
    /// Tracks the root ContentItem nodes and their dependencies.
    /// </summary>
    public class Builder
    {
        public const string ConfigFile = "Config.txt";
        public const string StateFile = "State.bin";
        public const string OutputPath = "\\Out";
        public const string SourcePath = "\\Source";
        public const string BuilderPath = "\\Builders";

        public static readonly Builder Instance = new Builder();

        private string m_outputRoot;
        private string m_sourceRoot;
        private string m_builderRoot;
        private Dictionary<string, BuilderItem> m_builderExtensionMap = new Dictionary<string, BuilderItem>();
        private Dictionary<string, BuilderItem> m_builderPathMap = new Dictionary<string, BuilderItem>();

        private Dictionary<string, ContentItem> m_contentItems = new Dictionary<string, ContentItem>();

        public ObservableCollection<ContentItem> RootContent
        {
            get;
            private set;
        }

        public Builder()
        {
            m_outputRoot = Environment.CurrentDirectory + OutputPath;
            m_sourceRoot = Environment.CurrentDirectory + SourcePath;
            m_builderRoot = Environment.CurrentDirectory + BuilderPath;

            LoadState();
            LoadBuilderConfig();
            TrawlContentItems();
        }

        private void LoadState()
        {
            if (File.Exists(StateFile))
            {
                BuilderConfig config = BuilderConfig.Load(StateFile);

                foreach (BuilderItem builder in config.BuilderState)
                {
                    m_builderPathMap.Add(builder.Path, builder);
                }

                foreach (ContentItem content in config.ContentItems)
                {
                    m_contentItems.Add(content.ResourcePath, content);
                }
            }
        }

        private void LoadBuilderConfig()
        {
            //Read or create builder configuration file
            if (!File.Exists(ConfigFile))
            {
                File.WriteAllText(ConfigFile, string.Empty);
            }
            else
            {
                //Parse the configuration file
                foreach (string line in File.ReadAllLines(ConfigFile))
                {
                    string[] bits = line.Split(' ');
                    string builderPath = Path.Combine(m_builderRoot, bits.First());

                    if (File.Exists(builderPath))
                    {
                        BuilderItem builderItem;
                        if (m_builderPathMap.TryGetValue(builderPath, out builderItem) == false)
                        {
                            builderItem = new BuilderItem(builderPath);
                            m_builderPathMap.Add(builderPath, builderItem);
                        }

                        foreach (string extension in bits.Skip(1))
                        {
                            m_builderExtensionMap.Add(extension, builderItem);
                        }
                    }
                }
            }
        }

        private void TrawlContentItems()
        {
            RootContent = new ObservableCollection<ContentItem>(
                GetContentItems(
                    Directory.GetFiles(m_sourceRoot, "*.*", SearchOption.TopDirectoryOnly).ToList()));
        }

        public void SaveState()
        {
            BuilderConfig config = new BuilderConfig()
            {
                BuilderState = m_builderPathMap.Values.ToList(),
                ContentItems = m_contentItems.Values.ToList()
            };
            config.Save(StateFile);
        }

        public void Build()
        {
            foreach (ContentItem item in RootContent)
                item.Build();
        }

        public void Clean()
        {
            foreach (ContentItem item in RootContent)
                item.Clean();
        }

        public List<ContentItem> GetContentItems(List<string> resourcePaths)
        {
            return resourcePaths.Select(resourcePath => GetContentItem(resourcePath)).ToList();
        }

        private ContentItem GetContentItem(string resourcePath)
        {
            resourcePath = Path.GetFullPath(resourcePath);
            ContentItem item = null;
            if (m_contentItems.TryGetValue(resourcePath, out item) == false)
            {
                BuilderItem builder;
                if (resourcePath.StartsWith(m_sourceRoot) && m_builderExtensionMap.TryGetValue(Path.GetExtension(resourcePath), out builder))
                {
                    item = new ContentItem(builder, resourcePath, GetOutputPath(resourcePath));
                    m_contentItems.Add(item.ResourcePath, item);
                }
            }

            return item;
        }

        private string GetRelativePath(string path)
        {
            if (path.StartsWith(m_sourceRoot))
            {
                return path.Substring(m_sourceRoot.Length);
            }
            else if (path.StartsWith(m_outputRoot))
            {
                return path.Substring(m_outputRoot.Length);
            }
            else
            {
                return path;
            }
        }

        private string GetOutputPath(string resourcePath)
        {
            return Path.Combine(m_outputRoot, GetRelativePath(resourcePath) + ".dat");
        }

        private string GetResourcePath(string outputPath)
        {
            string relativePath = GetRelativePath(outputPath);
            if (relativePath.EndsWith(".dat"))
            {
                relativePath = relativePath.Substring(relativePath.Length - 4);
            }

            return Path.Combine(m_sourceRoot, relativePath);
        }
    }
}
