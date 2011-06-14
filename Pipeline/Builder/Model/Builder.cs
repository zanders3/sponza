using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows;
using System.Collections.ObjectModel;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;
using Builder.Model.XML;

namespace Builder.Model
{
    /// <summary>
    /// Tracks BuilderItems and their supported file extensions.
    /// Tracks the root ContentItem nodes and their dependencies.
    /// </summary>
    public class Builder : BasePropertyChanged
    {
        public const string ConfigFile = "Config.txt";
        public const string StateFile = "State.xml";
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

        private int m_progress = 0;
        public int Progress
        {
            get
            {
                return m_progress;
            }
            set
            {
                m_progress = value;
                Changed("Progress");
            }
        }

        public Builder()
        {
            OutputText = new ObservableCollection<string>();

            BuildQueue = new Model.BuildQueue(
                progress =>
                {
                    this.Progress = progress;
                },
                () =>
                {
                    StatusText = "Packaging...";
                    Progress = 0;
                    m_packager.Package(m_outputRoot, PackageFile);
                });
            m_packager = new Packager(
                progress =>
                {
                    this.Progress = progress;
                },
                () =>
                {
                    StatusText = "Build and Package Complete";
                    Progress = 0;
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

            LoadBuilderConfig();
            LoadState();
            TrawlContentItems();
            Build();
        }

        private void LoadState()
        {
            if (File.Exists(StateFile))
            {
                BuilderXML config = BuilderXML.Load(StateFile);

                SetupItems(config.ContentItems.Select(item => item.ToContentItem()));
            }
        }

        private void SetupItems(IEnumerable<ContentItem> items)
        {
            foreach (ContentItem content in items.Where(item => item.Initialize(this)))
            {
                m_contentItems.Add(content.ResourcePath, content);
                SetupItems(content.Dependencies);
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
            try
            {
                new BuilderXML(RootContent).Save(StateFile);

                m_gameConnection.Dispose();
                m_contentFolderListener.Dispose();
                m_builderFolderListener.Dispose();
            }
            catch (Exception e)
            {
                MessageBox.Show("Failed to save clean up properly: " + e.Message);
            }
        }

        public void Validate()
        {
            foreach (ContentItem item in RootContent)
                item.Validate();
        }

        public void Build()
        {
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
            resourcePath = Path.Combine(m_sourceRoot, resourcePath.Trim());
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
