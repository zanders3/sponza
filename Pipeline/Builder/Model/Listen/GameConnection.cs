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
            m_worker.RunWorkerAsync();
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
            m_worker.ReportProgress(0);
        }

        struct Client
        {
            public TcpClient client;
            public BinaryReader reader;
            public BinaryWriter writer;
        }

        void m_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                TcpListener listener = new TcpListener(IPAddress.Loopback, 1234);
                List<Client> clients = new List<Client>();

                listener.Start();

                while (!m_worker.CancellationPending)
                {
                    if (listener.Pending())
                    {
                        Client client = new Client();
                        client.client = listener.AcceptTcpClient();
                        client.reader = new BinaryReader(client.client.GetStream());
                        client.writer = new BinaryWriter(client.client.GetStream());
                        clients.Add(client);

                        StatusMessage("Game Connected");
                    }

                    string pendingMessage = null;
                    lock (m_pendingMessages)
                    {
                        if (m_pendingMessages.Count > 0)
                            pendingMessage = m_pendingMessages.Dequeue();
                    }

                    foreach (Client client in clients)
                    {
                        if (!client.client.Connected)
                        {
                            StatusMessage("Game Disconnected");
                            clients.Remove(client);
                            break;
                        }

                        if (client.client.GetStream().DataAvailable)
                        {
                            string recievedMessage = client.reader.ReadString();
                            StatusMessage(recievedMessage);
                            if (recievedMessage == "Quit")
                            {
                                client.client.Close();
                                continue;
                            }
                        }

                        if (pendingMessage != null)
                        {
                            try
                            {
                                char[] message = pendingMessage.ToArray();
                                client.writer.Write(message);
                                client.writer.Flush();
                            }
                            catch
                            {
                                client.client.Close();
                            }
                        }
                    }

                    if (pendingMessage == null)
                    {
                        Thread.Sleep(200);
                    }
                    else
                    {
                        StatusMessage("[" + clients.Count + " Notified]: " + pendingMessage);
                    }
                }

                listener.Stop();
            }
            catch (Exception ex)
            {
                StatusMessage("GAME CONNECTION EXCEPTION: " + ex.Message);
            }
        }
    }
}
