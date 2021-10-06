using greeterWithTLS;
using Grpc.Core;
using Grpc.Reflection;
using Grpc.Reflection.V1Alpha;
using System;
using System.IO;
using System.Threading.Tasks;

namespace server
{
    class Program
    {
        private const int PORT = 8085;
        private const string KEYS_PATH = @"C:\git\TlsTest\Keys\";

        static void Main(string[] args)
        {
            //server-side Tls
            var keyCertPair = new KeyCertificatePair(
                File.ReadAllText(Path.Combine(KEYS_PATH, "localhost.crt")),
                File.ReadAllText(Path.Combine(KEYS_PATH, "localhost.key")));
            ServerCredentials serverCredentials = new SslServerCredentials(new[] { keyCertPair });//, caRoots, SslClientCertificateRequestType.r

            // the reflection service will be aware of "Greeter" and "ServerReflection" services.
            // Used for grpcui - can be removed
            var reflectionServiceImpl = new ReflectionServiceImpl(Greeter.Descriptor, ServerReflection.Descriptor);

            Server server = new Server()
            {
                Services = {
                    Greeter.BindService(new GreeterService()),
                    ServerReflection.BindService(reflectionServiceImpl)
                },
                Ports = { new ServerPort("127.0.0.1", PORT, serverCredentials) }
            };
            
            try
            {
                server.Start();
            }
            catch (Exception e)
            {
                Console.WriteLine("exception: " + e);
            }

            Console.WriteLine("Started server on port " + PORT);

            server.ShutdownTask.Wait();
        }
    }

    public class GreeterService : Greeter.GreeterBase
    {
        public GreeterService()
        {
        }

        public override Task<HelloReply> SayHello(HelloRequest request, ServerCallContext context)
        {
            Console.WriteLine("Received say hello from " + request.Name);

            return Task.FromResult(new HelloReply
            {
                Message = "Hello " + request.Name
            });
        }
    }
}
