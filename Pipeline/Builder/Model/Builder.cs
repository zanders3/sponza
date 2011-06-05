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
    public class Builder : BasePropertyChanged
    {
        public const string ConfigFile = "Config.txt";
        public const string StateFile = "State.bin";
        public const string PackageFile = "Content.pack";
        public const string OutputPath = "\\Out";
        public const string SourcePath = "\\Source";
        public const string BuilderPath = "\\Builders";

        private string m_statusText = "Idle";

        private string m_outputRoot;
        private string m_sourceRoot;
        private string m_builderRoot;
        private Dictionary<string, BuilderItem> m_builderExtensionMap = new Dictionary<string, BuilderItem>();
        private Dictionary<string, BuilderItem> m_builderPathMap = new Dictionary<string, BuilderItem>();

        private Dictionary<string, ContentItem> m_contentItems = new Dictionary<string, ContentItem>();

        private Packager m_packager;
        private ObservableCollection<ContentItem> m_rootContent = new ObservableCollection<ContentItem>();

        private FolderListener m_contentFolderListener;
        private FolderListener m_builderFolderListener;
        private GameConnection m_gameConnection;

        public ObservableCollection<ContentItem> RootContent
        {
            get { return m_rootContent; }
            private set { m_rootContent = value; Changed("RootContent"); }
        }

        public BuildQueue BuildQueue
        {
            get;
            private set;
        }

        public string StatusText
        {
            get { return m_statusText; }
            set { m_statusText = value; Changed("StatusText"); OutputText.Add(value); }
        }

        public ObservableCollection<string> OutputText
        {
            get;
            private set;
        }

        private bool m_isPackaging = false;
        public bool IsPackaging
        {
            get { return m_isPackaging; }
            set { m_isPackaging = value; Changed("IsPackaging"); }
        }

        public Builder()
        {
            OutputText = new ObservableCollection<string>();

            BuildQueue = new Model.BuildQueue(
                () =>
                {
                    StatusText = "Packaging...";
                    IsPackaging = true;
                    m_packager.Package(m_outputRoot, PackageFile);
                });
            m_packager = new Packager(
                () =>
                {
                    StatusText = "Build and Package Complete";
                    IsPackaging = false;
                });

            m_gameConnection = new GameConnection(
                statusChanged =>
                {
                    StatusText = statusChanged;
                });

            m_outputRoot = Environment.CurrentDirectory + OutputPath;
            m_sourceRoot = Environment.CurrentDirectory + SourcePath;
            m_builderRoot = Environment.CurrentDirectory + BuilderPath;

            m_contentFolderListener = new FolderListener(
                m_sourceRoot,
                content =>
                {
                    ContentItem item = GetContentItem(content);
                    if (item != null)
                    {
                        item.Validate();
                        item.Build();
                    }
                });
            m_builderFolderListener = new FolderListener(
                m_builderRoot,
                () =>
                {
                    TrawlContentItems();
                    Build();
                });

            LoadState();
            LoadBuilderConfig();
            TrawlContentItems();
            Build();
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

                foreach (ContentItem content in config.ContentItems.Where(item => item.Initialize(this)))
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

        public void Dispose()
        {
            BuilderConfig config = new BuilderConfig()
            {
                BuilderState = m_builderPathMap.Values.ToList(),
                ContentItems = m_contentItems.Values.ToList()
            };
            config.Save(StateFile);

            m_gameConnection.Dispose();
            m_contentFolderListener.Dispose();
            m_builderFolderListener.Dispose();
        }

        public void Validate()
        {
            foreach (ContentItem item in RootContent)
                item.Validate();
        }

        public void Build()
        {
            StatusText = "Building all...";

            Validate();
            foreach (ContentItem item in RootContent)
                item.Build();
        }

        public void Clean()
        {
            Validate();
            foreach (ContentItem item in RootContent)
                item.Clean();

            StatusText = "Clean Completed";
        }

        public List<ContentItem> GetContentItems(List<string> resourcePaths)
        {
            return resourcePaths.Select(resourcePath => GetContentItem(resourcePath)).Where(item => item != null).ToList();
        }

        public BuilderItem GetBuilder(string resourcePath)
        {
            BuilderItem builder = null;
            m_builderExtensionMap.TryGetValue(Path.GetExtension(resourcePath), out builder);
            return builder;
        }

        public ContentItem GetContentItem(string resourcePath)
        {
            resourcePath = Path.GetFullPath(resourcePath);
            ContentItem item = null;
            if (m_contentItems.TryGetValue(resourcePath, out item) == false)
            {
                item = new ContentItem(resourcePath, GetOutputPath(resourcePath));
                if (item.Initialize(this))
                {
                    m_contentItems.Add(item.ResourcePath, item);
                    return item;
                }
                else
                {
                    return null;
                }
            }

            return item;
        }

        public string GetRelativePath(string path)
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
            return m_outputRoot + GetRelativePath(resourcePath) + ".dat";
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
