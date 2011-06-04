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
        private Action m_onPackageComplete;
        private BackgroundWorker m_backgroundWorker;
        private Exception m_exception;

        private string m_folder;
        private string m_packageFile;

        public Packager(Action onPackageComplete)
        {
            m_onPackageComplete = onPackageComplete;
            m_backgroundWorker = new BackgroundWorker();
            m_backgroundWorker.DoWork += new DoWorkEventHandler(m_backgroundWorker_DoWork);
            m_backgroundWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(m_backgroundWorker_RunWorkerCompleted);
        }

        void m_backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            m_onPackageComplete();
        }

        class FileEntry
        {
            public FileEntry(string file)
            {
                m_file = file;
                m_fileContent = File.ReadAllBytes(file);
            }

            public string m_file;
            public byte[] m_fileContent;
        }

        void m_backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                string[] files = Directory.GetFiles(m_folder, "*.dat", SearchOption.AllDirectories);

                //Read in all file data in parallel.
                FileEntry[] fileData = files.AsParallel().Select(filePath => new FileEntry(filePath)).ToArray();

                //Create the pack file in memory
                MemoryStream packFile = new MemoryStream();
                BinaryWriter writer = new BinaryWriter(packFile);

                //Create the header information
                long fileOffset = 0;
                foreach (FileEntry entry in fileData)
                {
                    writer.Write(entry.m_file.Length);
                    writer.Write(entry.m_file.ToArray());
                    writer.Write(fileOffset);

                    fileOffset += entry.m_fileContent.Length;
                }

                //Append file data
                foreach (FileEntry entry in fileData)
                    writer.Write(entry.m_fileContent);

                packFile.Position = 0;

                using (FileStream stream = File.Create(m_packageFile))
                {
                    using (ZOutputStream zStream = new ZOutputStream(stream, zlibConst.Z_DEFAULT_COMPRESSION))
                    {
                        CopyStream(packFile, zStream);
                    }
                }
            }
            catch (Exception exception)
            {
                m_exception = exception;
            }
        }

        private void CopyStream(Stream input, Stream output)
        {
            byte[] buffer = new byte[2000];
            int len;
            while ((len = input.Read(buffer, 0, 2000)) > 0)
            {
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
