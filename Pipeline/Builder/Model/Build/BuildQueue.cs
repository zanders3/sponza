using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Threading;
using System.Windows;

namespace Builder.Model
{
    public delegate void BuildCompleted(bool suceeded, List<string> dependencies, List<string> output);

    public class WorkItem
    {
        public BuilderItem m_builderItem;
        public string m_resourcePath;
        public string m_outputPath;
        public BuildCompleted m_buildCompleted;
        public Exception m_exception;
        public bool m_suceeded;
        public List<string> m_dependencies;
        public List<string> m_output;
    }

    public class BuildQueue : WorkQueue<WorkItem>
    {
        public BuildQueue(Action<int> progress, Action buildCompleted) : base(progress, buildCompleted)
        {
        }

        protected override void Work(WorkItem item)
        {
            try
            {
                item.m_suceeded = item.m_builderItem.Build(item.m_resourcePath, item.m_outputPath, ref item.m_dependencies, ref item.m_output);
            }
            catch (Exception e)
            {
                item.m_exception = e;
            }
        }

        protected override void Completed(WorkItem item)
        {
            if (item.m_exception != null)
            {
                MessageBox.Show(String.Format("Exception whilst building \"{0}\":\n{1}", item.m_resourcePath, item.m_exception.Message), "Build Error");
                item.m_suceeded = false;
            }
            item.m_buildCompleted(item.m_suceeded, item.m_dependencies, item.m_output);
        }

        public void Build(BuilderItem builder, string resourcePath, string outputPath, BuildCompleted buildCompleted)
        {
            WorkItem item = new WorkItem()
            {
                m_builderItem = builder,
                m_resourcePath = resourcePath,
                m_outputPath = outputPath,
                m_buildCompleted = buildCompleted,
                m_exception = null,
                m_suceeded = false,
                m_dependencies = new List<string>(),
                m_output = new List<string>()
            };
            Enqueue(item);
        }
    }
}
