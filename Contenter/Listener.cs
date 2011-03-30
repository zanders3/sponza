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
            Console.WriteLine("Listening for content changes.. (Press Q to Quit)");
            TcpListener listener = new TcpListener(IPAddress.Parse("127.0.0.1"), Ports.ListenerPort);
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
                        try
                        {
                            Builder builder = new Builder(config);
                            builder.Build();
                            if (builder.RebuiltItems.Any())
                            {
                                Console.WriteLine("Notifying Content Changes..");
                                StringBuilder message = new StringBuilder();
                                foreach (Builder.BuildItem item in builder.RebuiltItems)
                                {
                                    message.AppendLine("LOAD " + item.ID);
                                }

                                clients.ForEach(client => client.SendMessage(message.ToString()));
                            }

                            Thread.Sleep(1000);
                            changedFiles.Clear();
                        }
                        catch (BuildException e)
                        {
                            Console.WriteLine(e.Message);
                        }
                        
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
