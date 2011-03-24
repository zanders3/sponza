using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;

namespace Contenter
{
    /// <summary>
    /// Wrapper around the TcpClient interface.
    /// </summary>
    public class Client
    {
        private TcpClient client;
        private StreamReader reader;
        private StreamWriter writer;

        public bool Connected { get; private set; }

        public Client(TcpClient client)
        {
            this.client = client;
            writer = new StreamWriter(client.GetStream());
            reader = new StreamReader(client.GetStream());

            Connected = true;
        }

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

        public string ReadMessage()
        {
            if (!Connected) return string.Empty;

            try
            {
                if (client.GetStream().DataAvailable)
                {
                    return reader.ReadLine();
                }
            }
            catch
            {
                Connected = client.Connected;
                if (Connected)
                    throw;
                else
                    Console.WriteLine("Client Disconnected");
            }

            return string.Empty;
        }
    }
}
