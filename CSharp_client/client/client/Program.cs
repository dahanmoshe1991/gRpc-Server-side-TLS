using Grpc.Core;
using System;
using System.IO;

namespace client
{
    class Program
    {
        private const int PORT = 8085;
        private const string KEYS_PATH = @"C:\git\TlsTest\Keys";
        static void Main(string[] args)
        {
            var caRoots = File.ReadAllText(Path.Combine(KEYS_PATH, "RootCA.crt"));
            var channelCredentials = new SslCredentials(caRoots);
            Channel channel = new Channel("localhost", PORT, channelCredentials);
            var client = new Greeter.GreeterClient(channel);

            Console.WriteLine("channel.State: " + channel.State);
            if (channel.State == ChannelState.Idle)
                while (!Console.ReadLine().Equals('q'))
                {
                    try
                    {
                        var reply = client.SayHello(new HelloRequest { Name = "cs_Client" });
                        Console.WriteLine("Greeting: " + reply.Message);
                    }
                    catch (RpcException e)
                    {
                        Console.WriteLine("Error invoking greeting: " + e.Status);
                    }
                }

            channel.ShutdownAsync().Wait();
        }
    }
}
