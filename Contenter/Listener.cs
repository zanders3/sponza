using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace Contenter
{
    public class Listener
    {
        class Client
        {
            public Client(TcpClient client)
            {
                this.client = client;
                writer = new StreamWriter(client.GetStream());
                Connected = client.Connected;
            }

            public bool Connected { get; private set; }

            private TcpClient client;
            private StreamWriter writer;

            public void SendMessage(string text)
            {
                if (!Connected) return;

                try
                {
                    writer.WriteLine(text);
                    writer.Flush();
                }
                catch
                {
                    Connected = client.Connected;
                    if (Connected) 
                        throw;
                    else
                        Console.WriteLine("Client Disconnected");
                }
            }
        }

        private FileSystemWatcher fsw;
        private Configuration config;
        private List<string> changedFiles = new List<string>();
        private List<Client> clients = new List<Client>();

        public Listener(Configuration config)
        {
            fsw = new FileSystemWatcher(config.ContentPath);
            fsw.EnableRaisingEvents = true;
            fsw.NotifyFilter = NotifyFilters.LastWrite;
            fsw.IncludeSubdirectories = true;
            fsw.Changed += new FileSystemEventHandler(fileChanged);

            this.config = config;
        }

        void fileChanged(object sender, FileSystemEventArgs e)
        {
            if (e.ChangeType == WatcherChangeTypes.Changed && !changedFiles.Contains(e.FullPath))
            {
                lock (changedFiles)
                {
                    changedFiles.Add(e.FullPath);
                }
            }
        }

        public void Listen()
        {
            Console.WriteLine("Listening for content changes.. (Press Q to Quit)");
            TcpListener listener = new TcpListener(IPAddress.Parse("127.0.0.1"), 4567);
            listener.Start();

            try
            {
                while (true)
                {
                    if (listener.Pending())
                    {
                        Console.WriteLine("Client Connected");
                        clients.Add(new Client(listener.AcceptTcpClient()));
                    }

                    if (changedFiles.Any())
                    {
                        IEnumerable<Builder.BuildItem> builtItems = Builder.Build(config);
                        if (builtItems.Any())
                        {
                            Console.WriteLine("Notifying Content Changes..");
                            StringBuilder message = new StringBuilder();
                            foreach (Builder.BuildItem item in builtItems)
                            {
                                message.AppendLine("BUILD " + item.Name);
                            }

                            clients.ForEach(client => client.SendMessage(message.ToString()));
                        }

                        Thread.Sleep(1000);
                        changedFiles.Clear();

                        Console.WriteLine("Listening for Console Changes..");
                    }

                    clients.RemoveAll(client => !client.Connected);

                    if (Console.KeyAvailable && Console.ReadKey().Key == ConsoleKey.Q)
                        break;
                }
            }
            finally
            {
                listener.Stop();
            }
        }
    }
}
