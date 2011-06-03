using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.IO;
using System.ComponentModel;

namespace Builder.Model
{
    /// <summary>
    /// Represents a single build stage which outputs a file and it's dependencies.
    /// </summary>
    [Serializable]
    public class ContentItem : BasePropertyChanged
    {
        public enum State
        {
            Building,
            Succeeded,
            Failed
        }

        private State m_state;
        [NonSerialized]
        private BuilderItem m_builder;
        private string m_resourcePath;
        private string m_outputPath;

        #region Properties
        public State ContentState
        {
            get { return m_state; }
            set { m_state = value; Changed("ContentState"); }
        }

        public BuilderItem BuilderItem
        {
            get { return m_builder; }
            set { m_builder = value; }
        }

        public string ResourcePath
        {
            get { return m_resourcePath; }
        }

        public string OutputPath
        {
            get { return m_outputPath; }
        }

        public ObservableCollection<ContentItem> Dependencies
        {
            get;
            private set;
        }
        #endregion

        public ContentItem(BuilderItem builder, string resourcePath, string outputPath)
        {
            m_builder = builder;
            m_resourcePath = resourcePath;
            m_outputPath = outputPath;
            Dependencies = new ObservableCollection<ContentItem>();
        }

        public void Validate()
        {
            if (ContentState != State.Building)
            {
                if (m_builder.UpToDate() &&
                    File.Exists(m_resourcePath) &&
                    File.Exists(m_outputPath) &&
                    File.GetLastWriteTime(m_resourcePath) == File.GetLastWriteTime(m_outputPath))
                {
                    ContentState = State.Succeeded;
                }
                else
                {
                    ContentState = State.Failed;
                }

                foreach (ContentItem child in Dependencies)
                    child.Validate();
            }
        }

        public void Build()
        {
            if (ContentState == State.Failed)
            {
                ContentState = State.Building;

                m_builder.Build(m_resourcePath, m_outputPath,
                    (succeeded, dependencies) =>
                    {
                        if (succeeded)
                        {
                            ContentState = State.Succeeded;

                            Dependencies.Clear();
                            foreach (ContentItem item in Builder.Instance.GetContentItems(dependencies))
                            {
                                Dependencies.Add(item);
                            }
                        }
                        else
                        {
                            ContentState = State.Failed;
                        }
                    });
            }
        }

        public void Clean()
        {
            if (File.Exists(m_outputPath))
            {
                File.Delete(m_outputPath);
            }
        }

        public override bool Equals(object obj)
        {
            if (obj is ContentItem)
            {
                ContentItem other = (ContentItem)obj;
                return other.m_resourcePath == m_resourcePath;
            }
            else return false;
        }

        public override int GetHashCode()
        {
            return m_resourcePath.GetHashCode();
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
