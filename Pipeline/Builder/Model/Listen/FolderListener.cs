using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Builder.Model
{
    /// <summary>
    /// Listens for file changes on a folder.
    /// </summary>
    public class FolderListener
    {
        private FileSystemWatcher m_watcher;
        private Action<string> m_folderChanged;
        private HashSet<string> m_changedFiles = new HashSet<string>();
        private DateTime m_lastNotification = DateTime.Now;

        public FolderListener(string directory, Action<string> folderChanged)
        {
            m_folderChanged = folderChanged;

            m_watcher = new FileSystemWatcher(directory);
            m_watcher.EnableRaisingEvents = true;
            m_watcher.IncludeSubdirectories = true;
            m_watcher.Changed += new FileSystemEventHandler(m_watcher_Changed);
            m_watcher.Created += new FileSystemEventHandler(m_watcher_Changed);
            m_watcher.Deleted += new FileSystemEventHandler(m_watcher_Changed);
        }

        void m_watcher_Changed(object sender, FileSystemEventArgs e)
        {
            m_changedFiles.Add(e.FullPath);

            DateTime now = DateTime.Now;
            if (now > new DateTimeOffset(m_lastNotification, new TimeSpan(0, 0, 5)))
            {
                m_lastNotification = now;

                foreach (string file in m_changedFiles)
                    m_folderChanged(file);
            }
        }
    }
}
