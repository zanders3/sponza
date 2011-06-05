using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Threading;

namespace Builder.Model
{
    /// <summary>
    /// Listens for file changes on a folder.
    /// </summary>
    public class FolderListener : IDisposable
    {
        private BackgroundWorker m_worker;
        private FileSystemWatcher m_watcher;
        private Action<string> m_fileChanged;
        private Action m_folderChanged;
        private HashSet<string> m_changedFiles = new HashSet<string>();

        public FolderListener(string directory, Action<string> fileChanged)
        {
            m_fileChanged = fileChanged;
            Init(directory);
        }

        public FolderListener(string directory, Action folderChanged)
        {
            m_folderChanged = folderChanged;
            Init(directory);
        }

        public FolderListener(string directory, Action folderChanged, Action<string> fileChanged)
        {
            m_fileChanged = fileChanged;
            m_folderChanged = folderChanged;
            Init(directory);
        }

        private void Init(string directory)
        {
            m_watcher = new FileSystemWatcher(directory);
            m_watcher.EnableRaisingEvents = true;
            m_watcher.IncludeSubdirectories = true;
            m_watcher.Changed += new FileSystemEventHandler(m_watcher_Changed);
            m_watcher.Created += new FileSystemEventHandler(m_watcher_Changed);
            m_watcher.Deleted += new FileSystemEventHandler(m_watcher_Changed);

            m_worker = new BackgroundWorker();
            m_worker.WorkerSupportsCancellation = true;
            m_worker.WorkerReportsProgress = true;
            m_worker.DoWork += new DoWorkEventHandler(m_worker_DoWork);
            m_worker.ProgressChanged += new ProgressChangedEventHandler(m_worker_ProgressChanged);
            m_worker.RunWorkerAsync();
        }

        public void Dispose()
        {
            m_worker.CancelAsync();
        }

        void m_worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (m_folderChanged != null)
            {
                m_folderChanged();
            }
            if (m_fileChanged != null)
            {
                lock (m_changedFiles)
                {
                    foreach (string file in m_changedFiles)
                        m_fileChanged(file);

                    m_changedFiles.Clear();
                }
            }
        }

        void m_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            int timeSlept = 0;
            while (!m_worker.CancellationPending)
            {
                if (timeSlept > 2000)
                {
                    timeSlept = 0;
                    bool anyChangedFiles;
                    lock (m_changedFiles)
                    {
                        anyChangedFiles = m_changedFiles.Count > 0;
                    }

                    if (anyChangedFiles)
                        m_worker.ReportProgress(0);
                }

                Thread.Sleep(200);
                timeSlept += 200;
            }
        }

        void m_watcher_Changed(object sender, FileSystemEventArgs e)
        {
            lock (m_changedFiles)
            {
                m_changedFiles.Add(e.FullPath);
            }
        }
    }
}
