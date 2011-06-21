﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.IO;
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Xml.Serialization;

namespace Builder.Model
{
    /// <summary>
    /// Represents a single build stage which outputs a file and it's dependencies.
    /// </summary>
    public class ContentItem : BasePropertyChanged
    {
        public enum State
        {
            Building,
            Succeeded,
            Failed
        }

        private State m_state = State.Failed;
        private Builder m_builder;
        private BuilderItem m_builderItem;
        private string m_resourcePath;
        private string m_outputPath;

        private ObservableCollection<ContentItem> m_dependencies;

        #region Properties
        public State ContentState
        {
            get { return m_state; }
            set { m_state = value; Changed("ContentState"); }
        }

        public BuilderItem BuilderItem
        {
            get { return m_builderItem; }
            set { m_builderItem = value; }
        }

        public string ResourcePath
        {
            get { return m_resourcePath; }
            set { m_resourcePath = value; }
        }

        public string ResourceName
        {
            get { return m_builder.GetRelativePath(m_resourcePath); }
        }

        public string OutputPath
        {
            get { return m_outputPath; }
            set { m_outputPath = value; }
        }

        public ObservableCollection<ContentItem> Dependencies
        {
            get { return m_dependencies; }
        }

        #endregion

        public ContentItem(string resourcePath, string outputPath, IEnumerable<ContentItem> dependencies = null)
        {
            m_resourcePath = resourcePath;
            m_outputPath = outputPath;

            if (dependencies != null)
                m_dependencies = new ObservableCollection<ContentItem>(dependencies);
            else
                m_dependencies = new ObservableCollection<ContentItem>();
        }

        public bool Initialize(Builder builder)
        {
            m_builder = builder;
            m_builderItem = builder.GetBuilder(m_resourcePath);

            return m_builderItem != null && File.Exists(m_resourcePath);
        }

        public ContentItem Validate()
        {
            if (ContentState != State.Building)
            {
                if (m_builderItem.UpToDate() &&
                    File.Exists(m_resourcePath) &&
                    File.Exists(m_outputPath) &&
                    File.GetLastWriteTime(m_outputPath) >= File.GetLastWriteTime(m_resourcePath))
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
            return this;
        }

        public void Build()
        {
            if (ContentState == State.Failed)
            {
                ContentState = State.Building;
                m_builder.StatusText = "Building: " + Path.GetFileName(m_resourcePath);

                m_builder.BuildQueue.Build(m_builderItem, m_resourcePath, m_outputPath,
                    (succeeded, dependencies, output) =>
                    {
                        foreach (string outputText in output)
                            if (outputText != null && outputText.Length > 0)
                                m_builder.StatusText = outputText;

                        if (succeeded)
                        {
                            ContentState = State.Succeeded;
                            m_builder.StatusText = "Built: " + Path.GetFileName(m_resourcePath);

                            Dependencies.Clear();
                            foreach (ContentItem item in m_builder.GetContentItems(dependencies))
                            {
                                Dependencies.Add(item);
                                item.Build();
                            }
                        }
                        else
                        {
                            m_builder.StatusText = "Failed: " + Path.GetFileName(m_resourcePath);

                            ContentState = State.Failed;
                        }
                    });
            }

            foreach (ContentItem item in Dependencies)
                item.Build();
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
    }
}
