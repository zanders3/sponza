using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.IO;
using zlib;

namespace Builder.Model
{
    /// <summary>
    /// Packages all content in the output folder into a compressed package file.
    /// </summary>
    public class Packager
    {
        private Action<int> m_progress;
        private Action m_onPackageComplete;
        private BackgroundWorker m_backgroundWorker;
        private Exception m_exception;

        private string m_folder;
        private string m_packageFile;

        public Packager(Action<int> progress, Action onPackageComplete)
        {
            m_progress = progress;
            m_onPackageComplete = onPackageComplete;

            m_backgroundWorker = new BackgroundWorker();
            m_backgroundWorker.DoWork += new DoWorkEventHandler(m_backgroundWorker_DoWork);
            m_backgroundWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(m_backgroundWorker_RunWorkerCompleted);
            m_backgroundWorker.WorkerReportsProgress = true;
            m_backgroundWorker.ProgressChanged += new ProgressChangedEventHandler(m_backgroundWorker_ProgressChanged);
        }

        void m_backgroundWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            m_progress(e.ProgressPercentage);
        }

        void m_backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            m_onPackageComplete();
        }

        class FileEntry
        {
            public FileEntry(string file)
            {
                FilePath = file;
                FileSize = new FileInfo(file).Length;
            }

            public string FilePath;
            public long FileSize;
        }

        void m_backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                string[] files = Directory.GetFiles(m_folder, "*.dat", SearchOption.AllDirectories);

                //Read in all file data in parallel.
                FileEntry[] fileData = files.AsParallel().Select(filePath => new FileEntry(filePath)).ToArray();

                //Create the pack file in memory
                using (Stream packFile = new FileStream(m_packageFile, FileMode.Create))
                {
                    BinaryWriter writer = new BinaryWriter(packFile);

                    //Create the header information
                    long fileOffset = 0;
                    foreach (FileEntry entry in fileData)
                    {
                        writer.Write(entry.FilePath.Length);
                        writer.Write(entry.FilePath.ToArray());
                        writer.Write(fileOffset);

                        fileOffset += entry.FileSize;
                    }

                    //Append file data
                    foreach (FileEntry entry in fileData)
                    {
                        using (FileStream fileStream = new FileStream(entry.FilePath, FileMode.Open))
                        {
                            CopyStream(fileStream, packFile);
                        }
                    }
                }
            }
            catch (Exception exception)
            {
                m_exception = exception;
            }
        }

        private byte[] buffer = new byte[2000];

        private void CopyStream(Stream input, Stream output)
        {
            int len;
            while ((len = input.Read(buffer, 0, 2000)) > 0)
            {
                //m_backgroundWorker.ReportProgress((int)(input.Position * 100 / input.Length));
                output.Write(buffer, 0, len);
            }
            output.Flush();
        }

        public void Package(string folder, string packageFile)
        {
            m_folder = folder;
            m_packageFile = packageFile;

            m_backgroundWorker.RunWorkerAsync();
        }
    }
}
