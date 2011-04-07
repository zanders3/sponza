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
        private FileSystemWatcher fsw;
        private Configuration config;
        private HashSet<string> changedFiles = new HashSet<string>();
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

        private void fileChanged(object sender, FileSystemEventArgs e)
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
            Console.WriteLine("Listening for changes... (Press Enter to Quit)");
            TcpListener listener = new TcpListener(IPAddress.Parse("127.0.0.1"), Ports.ListenerPort);
            listener.Start();

            try
            {
                bool running = true;

                new Thread(new ThreadStart(delegate()
                    {
                        Console.ReadLine();
                        running = false;
                    })).Start();

                while (running)
                {
                    if (listener.Pending())
                    {
                        Console.WriteLine("Client Connected");
                        clients.Add(new Client(listener.AcceptTcpClient()));
                    }

                    bool anyChangedFiles = false;
                    lock (changedFiles)
                    {
                        anyChangedFiles = changedFiles.Count > 0;
                    }

                    if (anyChangedFiles)
                    {
                        lock (changedFiles)
                        {
                            try
                            {
                                Builder builder = new Builder(config);
                                builder.Build();
                                if (builder.BuiltItems.Any())
                                {
                                    StringBuilder message = new StringBuilder();
                                    foreach (Builder.BuildItem item in builder.BuiltItems)
                                    {
                                        Console.WriteLine("LOAD " + Path.GetFileName(item.Input));
                                        message.AppendLine("LOAD " + item.ID);
                                    }

                                    clients.ForEach(client => client.SendMessage(message.ToString()));
                                }
                            }
                            catch (BuildException e)
                            {
                                Console.WriteLine(e.Message);
                            }

                            changedFiles.Clear();
                        }

                        Thread.Sleep(5000);
                    }

                    foreach (Client client in clients.Where(client => !client.Connected))
                        Console.WriteLine("Client Disconnected");

                    clients.RemoveAll(client => !client.Connected);

                    Thread.Sleep(500);
                }
            }
            finally
            {
                clients.Clear();
                listener.Stop();
            }
        }
    }
}
