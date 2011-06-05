using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.IO;

namespace Builder.Model
{
    /// <summary>
    /// Manages the connection to the game and sends string data.
    /// </summary>
    public class GameConnection : IDisposable
    {
        private Action<string> m_statusChanged;
        private BackgroundWorker m_worker;
        private Queue<string> m_statusMessages = new Queue<string>();
        private Queue<string> m_pendingMessages = new Queue<string>();

        public GameConnection(Action<string> statusChanged)
        {
            m_statusChanged = statusChanged;

            m_worker = new BackgroundWorker();
            m_worker.DoWork += new DoWorkEventHandler(m_worker_DoWork);
            m_worker.ProgressChanged += new ProgressChangedEventHandler(m_worker_ProgressChanged);
            m_worker.WorkerSupportsCancellation = true;
            m_worker.WorkerReportsProgress = true;
        }

        public void Dispose()
        {
            m_worker.CancelAsync();
        }

        public void SendMessage(string message)
        {
            lock (m_pendingMessages)
            {
                m_pendingMessages.Enqueue(message);
            }
        }

        void m_worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            lock (m_statusMessages)
            {
                while (m_statusMessages.Count > 0)
                    m_statusChanged(m_statusMessages.Dequeue());
            }
        }

        private void StatusMessage(string message)
        {
            lock (m_statusMessages)
            {
                m_statusMessages.Enqueue(message);
            }
        }

        void m_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            TcpListener listener = new TcpListener(IPAddress.Loopback, 1234);
            NetworkStream stream = new NetworkStream(listener.Server);
            BinaryWriter writer = new BinaryWriter(stream);
            listener.Start();

            while (!m_worker.CancellationPending)
            {
                if (listener.Pending())
                {
                    listener.AcceptTcpClient();
                    StatusMessage("Game Connected");
                }

                string message = null;
                lock (m_pendingMessages)
                {
                    if (m_pendingMessages.Count > 0)
                        message = m_pendingMessages.Dequeue();
                }

                if (message != null)
                {
                    writer.Write(message + Environment.NewLine);
                    StatusMessage("Sent: " + message);
                }
                else
                {
                    Thread.Sleep(200);
                }
            }

            listener.Stop();
        }
    }
}
