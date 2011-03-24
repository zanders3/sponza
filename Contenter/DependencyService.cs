using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net.Sockets;
using System.Net;
using System.IO;

namespace Contenter
{
    /// <summary>
    /// Allows content builders to connect and request building of dependent assets.
    /// </summary>
    public class DependencyService
    {
        private volatile bool   running = true;
        private Thread          thread;
        private Builder         builder;
        private List<Client>    clients = new List<Client>();

        public DependencyService(Builder builder)
        {
            this.builder = builder;

            thread = new Thread(new ThreadStart(Run));
            thread.Start();
        }

        ~DependencyService()
        {
            running = false;
            thread.Join();
        }

        private void Run()
        {
            TcpListener listener = new TcpListener(IPAddress.Parse("127.0.0.1"), Ports.DependencyPort);
            listener.Start();

            try
            {
                while (running)
                {
                    if (listener.Pending())
                    {
                        Console.WriteLine("Client Connected");
                        clients.Add(new Client(listener.AcceptTcpClient()));
                    }

                    foreach (Client client in clients)
                    {
                        if (!client.Connected)
                        {
                            Console.WriteLine("Client Disconnected");
                            clients.Remove(client);
                            break;
                        }

                        string filename = client.ReadMessage();
                        if (filename != string.Empty)
                        {
                            int id = GetIDForDependency(filename);

                            client.SendMessage(id.ToString());
                        }
                    }
                }
            }
            finally
            {
                listener.Stop();
            }
        }

        private int GetIDForDependency(string filename)
        {
            foreach (Builder.BuildItem item in builder.AllItems)
            {
                if (filename.EndsWith(item.Input))
                {
                    try
                    {
                        Console.WriteLine(String.Format("Dependency: {0} -> {1} ({2})", item.Input, item.Name, builder.IDMap[item]));
                        item.Build();
                    }
                    catch (BuildException e)
                    {
                        Console.WriteLine(e.Message);
                    }

                    return (int)builder.IDMap[item];
                }
            }

            //Not found!
            Console.WriteLine(String.Format("Dependency not found: {0}", filename));
            return -2;
        }
    }
}
